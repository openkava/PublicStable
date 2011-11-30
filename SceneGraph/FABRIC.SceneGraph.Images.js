
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//


FABRIC.SceneGraph.registerNodeType('Texture', {
  briefDesc: 'The Texture node is an Image node which can be used for texturing of a 3D object.',
  detailedDesc: 'The Texture node is an Image node which can be used for texturing of a 3D object.',
  parentNodeDesc: 'SceneGraphNode',
  factoryFn: function(options, scene) {
    var textureNode = scene.constructNode('SceneGraphNode', options);
    return textureNode;
  }});

  // TODO: The texture image type needs to be broken into LDR and HDR.
  // here we have HDR image loading mixed in with LDR image loading,
  // but it should be a separate type.
FABRIC.SceneGraph.registerNodeType('Image', {
  briefDesc: 'The Image node holds image data, and optionally creates an URL image loader and an OpenGL texture.',
  detailedDesc: 'The Image node holds generic image data (members: pixels, width, height), which might be HDR (member: hdr). ' +
                'If \'options.createResourceLoadNode\', an URL-based image loader will be incorporated, and currently supports ' +
                '.png, .tga, .exr and .hdr image formats. If \'options.createLoadTextureEventHandler\', an OpenGL texture ' +
                'will be created from the image.',
  parentNodeDesc: 'Texture',
  optionsDesc: {
    wantHDR: 'Set to true this flags enables the Image node to use float values instead if bytes.',
    wantRGBA: 'Set to true this flag enables the Image node to store the A channel.',
    createDgNode: 'If this is set to true the Image node will contain a dgnode to store the pixel data.',
    createResourceLoadNode: 'Set to true this flag will enable the Image node to load a texture off a resource load node.',
    createLoadTextureEventHandler: 'If the image uses a ResouceLoadNode and this flag is set, it will create an EventHandler for the Image being loaded.',
    width: 'The width of the empty Image',
    height: 'The height of the empty Image',
    color: 'The standard color for the empty Image',
    url: 'The URL to load the Image from',
    forceRefresh: 'If this is set, the Image will always be re-loaded onto the GPU. This is useful for animated Images.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      wantHDR: false,
      wantRGBA: true,
      createDgNode: false,
      createResourceLoadNode: true,
      createLoadTextureEventHandler: true,
      initImage: true,
      width: 128,
      height: 128,
      color: FABRIC.RT.rgba(0.0,0.0,0.0,0.0),
      url: undefined,
      forceRefresh: false,
      glRepeat: true
    });
    
    var resourceLoadNode, resourceloaddgnode;

    var imageNode = scene.constructNode('Texture', options);
    if(options.createDgNode){
      var dgnode = imageNode.constructDGNode('DGNode')
      dgnode.addMember('hdr', 'Boolean', options.wantHDR);
      dgnode.addMember('width', 'Size', options.createResourceLoadNode ? undefined : options.width);
      dgnode.addMember('height', 'Size', options.createResourceLoadNode ? undefined : options.height);
      dgnode.addMember('pixels', (options.wantHDR ? 'Color' : (options.wantRGBA ? 'RGBA' : 'RGB')) + '[]');
  
      imageNode.addMemberInterface(dgnode, 'width');
      imageNode.addMemberInterface(dgnode, 'height');
    }

    if (options.createResourceLoadNode) {
      resourceLoadNode = scene.constructNode('ResourceLoad', options);
      resourceloaddgnode = resourceLoadNode.getDGLoadNode();
      if(options.createDgNode){
        dgnode.setDependency(resourceloaddgnode, 'resource');
        dgnode.bindings.append(scene.constructOperator({
          operatorName: (options.wantHDR ? 'loadImageHDR' : 'loadImageLDR'),
          parameterLayout: [
            'resource.resource',
            'self.width',
            'self.height',
            'self.pixels'
          ],
          entryFunctionName: (options.wantHDR ? 'loadImageHDR' : 'loadImageLDR'),
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadTexture.kl'
        }));
      };

      imageNode.pub.getResourceLoadNode = function() {
        return resourceLoadNode.pub;
      };

      imageNode.pub.isImageLoaded = function() {
        return resourceLoadNode ? resourceLoadNode.pub.isLoaded() : false;
      };
    } else {
      if(options.createDgNode && options.initImage && options.width && options.height){
        dgnode.addMember('color', 'RGBA', options.color);
        dgnode.addMember('initiated', 'Boolean', false);
        dgnode.bindings.append(scene.constructOperator({
          operatorName: 'initImageFromColor',
          parameterLayout: [
            'self.width',
            'self.height',
            'self.color',
            'self.pixels',
            'self.initiated'
          ],
          entryFunctionName: 'initImageFromColor',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadTexture.kl'
        }));

        imageNode.pub.setColor = function(color) {
          dgnode.setData('initiated', 0, false);
          if( color.getType() === 'FABRIC.RT.Color' ) {
            var byteColor = new FABRIC.RT.RGBA();
            byteColor.setFromScalarColor(color);
            color = byteColor;
          }
          dgnode.setData('color', 0, color );
        }
      };
    }

    if (options.createLoadTextureEventHandler) {
      // Construct the handler for loading the image into texture memory.
      var redrawEventHandler = imageNode.constructEventHandlerNode('Redraw');
      var oglTexture = options.wantHDR ? FABRIC.RT.oglTexture2D_Color() : FABRIC.RT.oglTexture2D();
      if(!options.glRepeat) {
        oglTexture.wrapS = FABRIC.SceneGraph.OpenGLConstants.GL_CLAMP;
        oglTexture.wrapT = FABRIC.SceneGraph.OpenGLConstants.GL_CLAMP;
      }
      oglTexture.forceRefresh = options.forceRefresh;
      redrawEventHandler.addMember('oglTexture2D', 'OGLTexture2D', oglTexture);
      if(options.createDgNode){
        redrawEventHandler.setScope('image', dgnode);
        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'bindTexture' + (options.wantHDR ? 'HDR' : 'LDR'),
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadTexture.kl',
          entryFunctionName: 'bindTexture' + (options.wantHDR ? 'HDR' : 'LDR'),
          parameterLayout: [
            'image.width',
            'image.height',
            'image.pixels',
            'self.oglTexture2D',
            'textureStub.textureUnit'
          ]
        }));
      }
      else if(options.createResourceLoadNode){
        redrawEventHandler.setScope('resource', resourceloaddgnode);
        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'loadAndBindTextureLDR',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadTexture.kl',
          entryFunctionName: 'loadAndBindTextureLDR',
          parameterLayout: [
            'resource.resource',
            'self.oglTexture2D',
            'textureStub.textureUnit'
          ]
        }));
      }
    }

    imageNode.pub.getURL = function() {
      return resourceLoadNode ? resourceLoadNode.pub.getUrl() : '';
    };

    return imageNode;
  }});

FABRIC.SceneGraph.registerNodeType('Image3D', {
  briefDesc: 'The Image node holds 3D image data, and optionally creates an URL image loader and an OpenGL texture.',
  detailedDesc: 'The Image node holds generic image data (members: pixels, width, height, depth), which might be color or grayscale, LDR or HDR. ' +
                'If \'options.createResourceLoadNode\', an URL-based image loader will be incorporated, and currently supports ' +
                '(todo) image formats. If \'options.createLoadTextureEventHandler\', an OpenGL 3D texture ' +
                'will be created from the image.',
  parentNodeDesc: 'Texture',
  optionsDesc: {
    format: 'Pixel format. Currently supported: RGBA, UShort, Byte, Color, Scalar.',
    createDgNode: 'If this is set to true the Image node will contain a dgnode to store the pixel data.',
    createResourceLoadNode: 'Set to true this flag will enable the Image node to load a texture off a resource load node.',
    createLoadTextureEventHandler: 'If the image uses a ResouceLoadNode and this flag is set, it will create an EventHandler for the Image being loaded.',
    width: 'The width of the empty Image',
    height: 'The height of the empty Image',
    depth: 'The depth of the empty Image',
    color: 'The standard color for the empty Image',
    url: 'The URL to load the Image from'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      format: 'UShort',
      createDgNode: true,
      createResourceLoadNode: true,
      createLoadTextureEventHandler: true,
      initImage: true,
      width: 0,
      height: 0,
      depth: 0,
      url: undefined,
      glRepeat: true
    });
    if(options.color === undefined) {
      if(options.format === 'RGBA')
        options.color = new FABRIC.RT.RGBA(0,0,0,0);
      else if(options.format === 'Color')
        options.color = new FABRIC.RT.Color(0,0,0,0);
      else
        options.color = 0;
    }
    var resourceLoadNode, resourceloaddgnode;
    
    var imageNode = scene.constructNode('Texture', options);
    if(options.createDgNode){
      var dgnode = imageNode.constructDGNode('DGNode')
      dgnode.addMember('width', 'Size', options.width);
      dgnode.addMember('height', 'Size', options.height);
      dgnode.addMember('depth', 'Size', options.depth);
      if(options.format === 'UShort')
        dgnode.addMember('pixels', 'Byte[]');
      else
        dgnode.addMember('pixels', options.format + '[]');
  
      imageNode.addMemberInterface(dgnode, 'width');
      imageNode.addMemberInterface(dgnode, 'height');
      imageNode.addMemberInterface(dgnode, 'depth');
    }

    if (options.createResourceLoadNode) {
      if(options.format !== 'UShort')
        throw ('Only UShort 3D textures are currently supported');

      resourceLoadNode = scene.constructNode('ResourceLoad', options);
      resourceloaddgnode = resourceLoadNode.getDGLoadNode();
      if(options.createDgNode){
        dgnode.addMember('xfoMat', 'Mat44');
        dgnode.setDependency(resourceloaddgnode, 'resource');
        dgnode.bindings.append(scene.constructOperator({
          operatorName: 'load3DImageUShortData',
          parameterLayout: [
            'resource.resource',
            'self.width',
            'self.height',
            'self.depth',
            'self.pixels',
            'self.xfoMat'
          ],
          entryFunctionName: 'load3DImageUShortData',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/load3DTexture.kl'
        }));
      };

      imageNode.pub.getResourceLoadNode = function() {
        return resourceLoadNode.pub;
      };

      imageNode.pub.isImageLoaded = function() {
        return resourceLoadNode ? resourceLoadNode.pub.isLoaded() : false;
      };
    } else {
      if(options.createDgNode && options.initImage && options.width && options.height && options.depth){
        if(options.format === 'UShort')
          dgnode.addMember('color', 'Byte', options.color);
        else
          dgnode.addMember('color', options.format, options.color);
        dgnode.addMember('initiated', 'Boolean', false);
        dgnode.bindings.append(scene.constructOperator({
          operatorName: 'initImageFrom' + options.format,
          parameterLayout: [
            'self.width',
            'self.height',
            'self.depth',
            'self.color',
            'self.pixels',
            'self.initiated'
          ],
          entryFunctionName: 'initImageFrom' + options.format,
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/load3DTexture.kl'
        }));
      };
    }

    var redrawEventHandler;
    if (options.createLoadTextureEventHandler) {
      // Construct the handler for loading the image into texture memory.
      redrawEventHandler = imageNode.constructEventHandlerNode('Redraw');
      var oglTexture;

      if(options.format === 'Byte')
         oglTexture = FABRIC.RT.oglTexture3D_Byte();
      else if(options.format === 'Color')
         oglTexture = FABRIC.RT.oglTexture3D_Color();
      else if(options.format === 'Scalar')
         oglTexture = FABRIC.RT.oglTexture3D_Scalar();
      else if(options.format === 'UShort')
         oglTexture = FABRIC.RT.oglTexture3D_UShort();
      else
         oglTexture = FABRIC.RT.oglTexture3D_RGBA();

      if(!options.glRepeat) {
        oglTexture.wrapS = FABRIC.SceneGraph.OpenGLConstants.GL_CLAMP;
        oglTexture.wrapT = FABRIC.SceneGraph.OpenGLConstants.GL_CLAMP;
        oglTexture.wrapR = FABRIC.SceneGraph.OpenGLConstants.GL_CLAMP;
      }
      redrawEventHandler.addMember('oglTexture3D', 'OGLTexture3D', oglTexture);
      redrawEventHandler.addMember('forceSingleRefresh', 'Boolean', false);

      if(options.createDgNode){

        if(resourceLoadNode) {
          redrawEventHandler.addMember('currUrl', 'String');
          redrawEventHandler.setScope('resource', resourceloaddgnode);
          redrawEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: 'detectResourceChange',
            srcCode: 'operator detectResourceChange(io FabricResource resource, io String prevUrl, io Boolean refresh){\n' + 
                          '  if(prevUrl != resource.url){' +
                          '    prevUrl = resource.url;\n' +
                          '    refresh = true;} }',
            entryFunctionName: 'detectResourceChange',
            parameterLayout: [
              'resource.resource',
              'self.currUrl',
              'self.forceSingleRefresh'
            ]
          }));
        }

        redrawEventHandler.setScope('image', dgnode);
        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'bind' + options.format + 'Texture3D',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/load3DTexture.kl',
          entryFunctionName: 'bind' + options.format + 'Texture3D',
          parameterLayout: [
            'image.width',
            'image.height',
            'image.depth',
            'image.pixels',
            'self.oglTexture3D',
            'textureStub.textureUnit',
            'self.forceSingleRefresh'
          ]
        }));
      }
      else if(options.createResourceLoadNode){
        redrawEventHandler.setScope('resource', resourceloaddgnode);
        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'loadAndBindUShortTexture',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/load3DTexture.kl',
          entryFunctionName: 'loadAndBindUShortTexture',
          parameterLayout: [
            'resource.resource',
            'self.oglTexture3D',
            'textureStub.textureUnit'
          ]
        }));
      }
    }

    imageNode.pub.getUrl = function() {
      return resourceLoadNode ? resourceLoadNode.pub.getUrl() : '';
    };

    imageNode.pub.setUrl = function(url) {
      if(resourceLoadNode) {
        resourceLoadNode.pub.setUrl(url);
      }
    };
    
    var parentWriteData = imageNode.writeData;
    var parentReadData = imageNode.readData;
    imageNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      constructionOptions.createDgNode = options.createDgNode;
      constructionOptions.createResourceLoadNode = options.createResourceLoadNode;
      constructionOptions.createLoadTextureEventHandler = options.createLoadTextureEventHandler;
      if(resourceLoadNode){
        nodeData.url = resourceLoadNode.pub.getUrl();
      }
    };
    imageNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      if(nodeData.url){
        imageNode.pub.setUrl(nodeData.url);
      }
    };

    return imageNode;
  }});

FABRIC.SceneGraph.registerNodeType('CubeMap', {
  briefDesc: 'The CubeMap node contains 6 Image nodes which can be used to texture with cubic mapping. ',
  detailedDesc: 'The CubeMap node contains 6 Image nodes which can be used to texture with cubic mapping. ' +
                'This means it requires six images to be aligned in a cubic fashion.',
  parentNodeDesc: 'Texture',
  optionsDesc: {
    urls: 'An array of six URLs to the six images to load.',
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      urls: []
    });
    
    var cubMapNode = scene.constructNode('Texture', options);
    var redrawEventHandler = cubMapNode.constructEventHandlerNode('Redraw');
    redrawEventHandler.addMember('cubeMap', 'OGLTextureCube', FABRIC.RT.cubeMapLDR());
    
    var createTextureLoaderNode = function(url, faceId){
      var resourceLoadNode = scene.constructNode('ResourceLoad', { url: url });
      var resourceloaddgnode = resourceLoadNode.getDGLoadNode();
      redrawEventHandler.setScope('resource'+faceId, resourceloaddgnode);
    }
    createTextureLoaderNode(options.urls[0], 0);
    createTextureLoaderNode(options.urls[1], 1);
    createTextureLoaderNode(options.urls[2], 2);
    createTextureLoaderNode(options.urls[3], 3);
    createTextureLoaderNode(options.urls[4], 4);
    createTextureLoaderNode(options.urls[5], 5);

    redrawEventHandler.preDescendBindings.append(scene.constructOperator({
        operatorName: 'bindCubeMap',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadCubeMap.kl',
        entryFunctionName: 'bindCubeMap',
        parameterLayout: [
          'resource0.resource',
          'resource1.resource',
          'resource2.resource',
          'resource3.resource',
          'resource4.resource',
          'resource5.resource',
          'self.cubeMap',
          'textureStub.textureUnit'
        ]
      }));
    return cubMapNode;
  }});

FABRIC.SceneGraph.registerNodeType('Video', {
  briefDesc: 'The Video node is an Image node that changes over time.',
  detailedDesc: 'The Video node uses the ffmpeg extension to load animated video footage into an Image node. ' +
                'For that the image content is repeatedly copied onto the GPU.',
  parentNodeDesc: 'Image',
  optionsDesc: {
    localFileName: 'The Video node currently only supports a local filename, not an URL.',
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        url: '',
        loop: false,
        nbCachedFrames: 0,
        animationControllerNode: undefined
      });

    // ensure to use the right settings for video
    options.createResourceLoadNode = true;
    options.createLoadTextureEventHandler = false;
    options.createDgNode = false;
    options.initImage = false;
    options.wantHDR = false;
    options.wantRGBA = false;

    // check if we have an url
    if (!options.url) {
        throw ('You need to specify a valid url for a video node!');
    }

    var videoNode = scene.constructNode('Image', options);
    // add all members
    var dgnode = scene.getPrivateInterface(videoNode.pub.getResourceLoadNode()).getDGLoadNode();
    dgnode.addMember('handle', 'VideoHandle');
    dgnode.addMember('pixels', 'RGB[]');

    videoNode.addMemberInterface(dgnode, 'handle');
  
    // decode the resource
    dgnode.bindings.append(scene.constructOperator({
      operatorName: 'videoLoadResource',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadVideo.kl',
      entryFunctionName: 'videoLoadResource',
      parameterLayout: [
        'self.resource',
        'self.handle'
      ]
    }));

    // make it dependent on the scene time
    var timeBinding = 'globals.time';
    if(options.loop || options.animationControllerNode)
    {
      // use the options animation controller
      var animationController = undefined;
      if(options.animationControllerNode)
      {
        animationController = scene.getPrivateInterface(options.animationControllerNode);
        var animationControllerDGNode = animationController.getDGNode();
        dgnode.setDependency(animationControllerDGNode, 'controller');
      }
      else
      {
        animationController = scene.constructNode('AnimationController');
        var animationControllerDGNode = animationController.getDGNode();
        dgnode.setDependency(animationControllerDGNode, 'controller');
        dgnode.bindings.append(scene.constructOperator({
          operatorName: 'videoSetTimeRange',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadVideo.kl',
          entryFunctionName: 'videoSetTimeRange',
          parameterLayout: [
            'self.handle',
            'controller.timeRange',
          ]
        }));
      }
      videoNode.pub.getAnimationController = function(){
        return animationController.pub;
      };
      timeBinding = 'controller.localTime';
    }
    else
      dgnode.setDependency(scene.getGlobalsNode(), 'globals');

    // use a dict for storing the already parsed frames
    dgnode.addMember('pixelCache','RGB[][]')
    dgnode.addMember('pixelCacheIndex','Integer[]')
    dgnode.addMember('pixelCacheLimit','Integer',options.nbCachedFrames)
    
    dgnode.bindings.append(scene.constructOperator({
      operatorName: 'videoSeekTimeCached',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadVideo.kl',
      entryFunctionName: 'videoSeekTime',
      parameterLayout: [
        'self.handle',
        timeBinding,
        'self.pixels',
        'self.pixelCache',
        'self.pixelCacheIndex',
        'self.pixelCacheLimit'
      ]
    }));

    var redrawEventHandler = videoNode.constructEventHandlerNode('Redraw');
    redrawEventHandler.setScope('video', dgnode);
    redrawEventHandler.addMember('bufferID', 'Size', 0);
    redrawEventHandler.preDescendBindings.append(scene.constructOperator({
        operatorName: 'videoLoadToGPU',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadVideo.kl',
        entryFunctionName: 'videoLoadToGPU',
        parameterLayout: [
          'video.handle',
          'video.pixels',
          'self.bufferID',
          'textureStub.textureUnit'
        ]
    }));
    
    // extend public interface
    videoNode.pub.forceEvaluate = function(){
      dgnode.evaluate();
    };
    videoNode.pub.getDuration = function(){
      return dgnode.getData("duration");
    };
    videoNode.pub.getFps = function(){
      return dgnode.getData("fps");
    };

    return videoNode;
  }});

FABRIC.SceneGraph.registerNodeType('PointSpriteTexture', {
  briefDesc: 'The PointSpriteTexture is an Image node that can be used to texture a point using a sprite.',
  detailedDesc: 'The PointSpriteTexture is an Image node that can be used to texture a point using a sprite. ' +
                'You can specify the resolution of the sprite.',
  parentNodeDesc: 'Texture',
  optionsDesc: {
    spriteResolution: 'The resolution of the sprite texture.',
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        spriteResolution: 32
      });
    
    var pointSpriteTextureNode = scene.constructNode('Texture', options);
    
    var dgnode = pointSpriteTextureNode.constructDGNode('DGNode');
    dgnode.addMember('resolution', 'Integer', options.spriteResolution);

    var redrawEventHandler = pointSpriteTextureNode.constructEventHandlerNode('Redraw');
    redrawEventHandler.setScope('image', dgnode);
    redrawEventHandler.addMember('bufferID', 'Size', 0);
    redrawEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: 'createSpriteTexture',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadTexture.kl',
            entryFunctionName: 'createSpriteTexture',
            parameterLayout: [
              'image.resolution',
              'self.bufferID',
              'textureStub.textureUnit'
            ]
          }));

    return pointSpriteTextureNode;
  }});


FABRIC.SceneGraph.registerNodeType('ScreenGrab', {
  briefDesc: 'The ScreenGrab node stores the viewport to an Image node.',
  detailedDesc: 'The ScreenGrab node stores the viewport to an Image node by encoding PNG using a ResourceLoadNode. ',
  parentNodeDesc: 'SceneGraphNode',
  factoryFn: function(options, scene) {
    //TO refine: right now this node is grabbing a screenshot constantly!
    //We should include a way to 'mute' and 'unmute' it. The problem
    //is how to know when it is filled with content; an event should be sent. Can
    //we do this without modifying the core?
    var screenGrabNode = scene.constructNode('SceneGraphNode', options),
    screenGrabEventHandler;

    screenGrabEventHandler = screenGrabNode.constructEventHandlerNode('ScreenGrab');
    screenGrabEventHandler.addMember('width', 'Size');
    screenGrabEventHandler.addMember('height', 'Size');
    screenGrabEventHandler.addMember('pixels', 'RGBA[]');
    screenGrabEventHandler.addMember('resource', 'FabricResource');

    screenGrabEventHandler.postDescendBindings.append(scene.constructOperator({
      operatorName: 'grabViewport',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/grabViewport.kl',
      entryFunctionName: 'grabViewport',
      parameterLayout: [
            'self.width',
            'self.height',
            'self.pixels'
          ]
    }));

    screenGrabEventHandler.postDescendBindings.append(scene.constructOperator({
      operatorName: 'encodeImage',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/encodeImage.kl',
      entryFunctionName: 'encodeImageLDR',
      parameterLayout: [
            'self.width',
            'self.height',
            'self.pixels',
            'self.resource'
          ]
    }));

    scene.getScenePostRedrawEventHandler().appendChildEventHandler(screenGrabEventHandler);

    screenGrabNode.pub.saveAs = function() {
      try {
          screenGrabEventHandler.putResourceToUserFile('resource', 'Save Screen Grab Image As...', undefined, 'fabricScreenGrab');
      }
      catch (e) { }
    };

    return screenGrabNode;
  }
});
