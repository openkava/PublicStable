
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.SceneGraph.registerNodeType('VolumeSlices', {
  briefDesc: '',
  detailedDesc: '',
  parentNodeDesc: 'Triangles',
  optionsDesc: {
    cropMin: 'Crop min param (0 to 1 for XYZ)',
    cropMax: 'Crop max param (0 to 1 for XYZ)',
    nbSlices: 'The number of slices to be generated.',
    cameraNode: 'The cameraNode being used',
    transformNode: 'The transformNode for the volume instance'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        cropMin: new FABRIC.RT.Vec3(0.0, 0.0, 0.0),
        cropMax: new FABRIC.RT.Vec3(1.0, 1.0, 1.0),
        nbSlices: 64,
        cropInTransformedSpace: true
      });

    var volumeSlicesNode = scene.constructNode('Triangles', options);
    volumeSlicesNode.pub.addUniformValue('cropMin', 'Vec3', options.cropMin);
    volumeSlicesNode.pub.addUniformValue('cropMax', 'Vec3', options.cropMax);
    volumeSlicesNode.pub.addUniformValue('cropInTransformedSpace', 'Boolean', options.cropInTransformedSpace);
    volumeSlicesNode.pub.addUniformValue('nbSlices', 'Size', options.nbSlices);
    volumeSlicesNode.pub.addUniformValue('halfPixelCrop', 'Vec3', new FABRIC.RT.Vec3(0, 0, 0));

    volumeSlicesNode.pub.setAttributeDynamic('positions');
    volumeSlicesNode.pub.setAttributeDynamic('normals');
    volumeSlicesNode.pub.setAttributeDynamic('indices');

    // getters and setters
    var uniforms = volumeSlicesNode.getUniformsDGNode();
    volumeSlicesNode.addMemberInterface(uniforms, 'cropMin', true);
    volumeSlicesNode.addMemberInterface(uniforms, 'cropMax', true);
    volumeSlicesNode.addMemberInterface(uniforms, 'nbSlices', true);

    var attributes = volumeSlicesNode.getAttributesDGNode();
    var cameraNode = scene.getPrivateInterface(options.cameraNode);
    var transformNode = scene.getPrivateInterface(options.transformNode);
    attributes.setDependency(cameraNode.getDGNode(), 'camera');
    attributes.setDependency(transformNode.getDGNode(), 'transform');

    volumeSlicesNode.setGeneratorOps([
      //Upper bound for count: 10 per slices (6 with volume bbox cropping, plus 4 for view volume cropping)
      scene.constructOperator({
        operatorName: 'setVolumeSlicesVertexCount',
        srcCode: 'operator setVolumeSlicesVertexCount(io Size nbSlices, io Size count){count = nbSlices*6;}',
        entryFunctionName: 'setVolumeSlicesVertexCount',
        parameterLayout: [
          'uniforms.nbSlices',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generateVolumeSlices',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateVolumeSlices.kl',
        entryFunctionName: 'generateVolumeSlices',
        parameterLayout: [
          'uniforms.cropMin',
          'uniforms.cropMax',
          'uniforms.halfPixelCrop',
          'uniforms.cropInTransformedSpace',
          'uniforms.nbSlices',
          'transform.globalXfo',
          'camera.cameraMat44',
          'camera.projectionMat44',
          'uniforms.indices',
          'self.positions<>',
          'self.normals<>' //We encode the 3D texture in the normals
        ]
      })
    ]);
    return volumeSlicesNode;
  }});

FABRIC.SceneGraph.registerNodeType('VolumeOpacityInstance', {
  briefDesc: '',
  detailedDesc: '',
  parentNodeDesc: 'VolumeMaterial',
  optionsDesc: {
    opacityTextureNode: 'Image3D of UShorts containing a 3D opacity texture',
    gradientTextureNode: 'Optional (else, generated): Image3D of RGBA containing a gradient vector in RGB, plus a gradient weight in A' + 
                         'Gradient is basically a surface normal to be used for shading areas where the opacity changes, ' +
                         'and the gradient weight is the gradient amplitude (length); areas of constant opacity should have ' +
                         'a gradient weight of 0.',
    reducedGradientTexture: 'If true, the gradient texture will be 2X smaller than the opacity one, which is usually ok since it reduces ' +
                          'noise and makes the computations much faster.',
    smoothGradient: 'If true, a smoothing pass will be applied to the computed gradient (only if gradientTextureNode is undefined)',
    cropMin: '3D vector specifying the min cropping bbox coordinate (in the range 0..1 for X, Y, Z)',
    cropMax: '3D vector specifying the max cropping bbox coordinate (in the range 0..1 for X, Y, Z)',
    transformNode: 'Transform applied to the volume geometry.',
    viewportNode: 'Active viewport',
    lightNode: 'Active light',
    resolutionFactor: 'Resolution of the volume render, in the range 0.0 to 1.0, 1.0 being 2 slices per voxel slice',
    brightnessFactor: 'Brightness from 0 to 1',
    transparency: 'Transparency from 0 to 1',
    minOpacity: 'MinOpacity from 0 to 1',
    maxOpacity: 'MaxOpacity from 0 to 1',
    specularFactor: 'Specular amount from 0 to 1',
    backgroundColor: 'Background color',
    invertColor: 'Invert color: inverted if == 1'
  },
  factoryFn: function(options, scene) {

    scene.assignDefaults(options, {
      reducedGradientTexture: true,
      smoothGradient: true,
      resolutionFactor: 1.0,
      brightnessFactor: 0.5,
      transparency: 1.0,
      minOpacity: 0.0,
      maxOpacity: 1.0,
      specularFactor: 0.7,
      backgroundColor: new FABRIC.RT.Color(0,0,0,1),
      invertColor: 0
    });

    options.cameraNode = options.viewportNode.getCameraNode();

    var volumeNodePub = scene.pub.constructNode('VolumeSlices', options);
    var volumeNode = scene.getPrivateInterface(volumeNodePub);
    var opacityTextureDGNode = scene.getPrivateInterface(options.opacityTextureNode).getDGNode();

    volumeNode.getOpacityDGNode = function(){return opacityTextureDGNode;};
    volumeNode.pub.getOpacityNode = function(){return options.opacityTextureNode;};
    volumeNode.pub.getVolumeSlicesNode = function(){return volumeNodePub;};

    //Generate gradient if not available
    if(options.gradientTextureNode === undefined) {

      var sourceOpacityDGNodeForGradient = opacityTextureDGNode;

      if(options.reducedGradientTexture) {
        var generatorNodeOpacity = scene.constructNode('Image3D', {
          name: 'ReducedOpacity',
          format: 'UShort',
          createResourceLoadNode: false,
          createDgNode: true,
          glRepeat: false
        });
        var reducedOpacityDGNode = generatorNodeOpacity.getDGNode();
        reducedOpacityDGNode.setDependency(opacityTextureDGNode, 'opacity');
        reducedOpacityDGNode.bindings.append(scene.constructOperator({
          operatorName: 'reduceOpacityTexture',
          parameterLayout: [
            'opacity.width',
            'opacity.height',
            'opacity.depth',
            'opacity.pixels',
            'self.width',
            'self.height',
            'self.depth',
            'self.pixels'
          ],
          entryFunctionName: 'reduceOpacityTexture',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateVolumeSlices.kl'
        }));
        var sourceOpacityDGNodeForGradient = reducedOpacityDGNode;
      }

      //Here, to optimize, we will slice only in the image depth component, so we can still save a lot of common precomputation overhead
      var dgnodeSlicedGradient = volumeNode.constructDGNode('SlicedGradientDGNode');
      dgnodeSlicedGradient.addMember('sliceGradients', 'RGBA[]');
      dgnodeSlicedGradient.setDependency(sourceOpacityDGNodeForGradient, 'opacity');

      dgnodeSlicedGradient.bindings.append(scene.constructOperator({
        operatorName: 'initCount',
        parameterLayout: [
          'opacity.depth',
          'self.newCount'
        ],
        entryFunctionName: 'initCount',
        srcCode: 'operator initCount(io Size depth, io Size newCount){newCount = depth;}'
      }));

      dgnodeSlicedGradient.bindings.append(scene.constructOperator({
        operatorName: 'computeGradients_depthSliced',
        parameterLayout: [
          'opacity.width',
          'opacity.height',
          'opacity.depth',
          'opacity.pixels',
          'self.sliceGradients',
          'self.index'
        ],
        entryFunctionName: 'computeGradients_depthSliced',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateVolumeSlices.kl'
      }));

      if(options.smoothGradient) {
        dgnodeSlicedGradient.addMember('smoothedSliceGradients', 'RGBA[]');
        dgnodeSlicedGradient.bindings.append(scene.constructOperator({
          operatorName: 'smoothGradients_depthSliced',
          parameterLayout: [
            'opacity.width',
            'opacity.height',
            'opacity.depth',
            'self.sliceGradients<>',
            'self.smoothedSliceGradients',
            'self.index'
          ],
          entryFunctionName: 'smoothGradients_depthSliced',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateVolumeSlices.kl'
        }));
      }

      var generatorNodeGradientPub = scene.pub.constructNode('Image3D', {
        name: '3DTextureGenerator_Gradient',
        format: 'RGBA',
        createResourceLoadNode: false,
        createDgNode: true,
        glRepeat: false
      });
      var generatorNodeGradient = scene.getPrivateInterface(generatorNodeGradientPub);
      var dgnodeGradient = generatorNodeGradient.getDGNode();
      dgnodeGradient.setDependency(sourceOpacityDGNodeForGradient, 'opacity');
      dgnodeGradient.setDependency(dgnodeSlicedGradient, 'slicedGradient');

      dgnodeGradient.bindings.append(scene.constructOperator({
        operatorName: 'setGradients',
        parameterLayout: [
          'opacity.width',
          'opacity.height',
          'opacity.depth',
          options.smoothGradient ? 'slicedGradient.smoothedSliceGradients<>' : 'slicedGradient.sliceGradients<>',
          'self.width',
          'self.height',
          'self.depth',
          'self.pixels'
        ],
        entryFunctionName: 'setGradients',
        srcCode: 'operator setGradients(io Size inW, io Size inH, io Size inD, io RGBA sliceGradients<>[], io Size W, io Size H, io Size D, io RGBA pixels[]) {\n' +
                                'W = inW; H = inH; D = inD;\n' +
                                'pixels.resize(W*H*D);\n' +
                                'Size dst = 0, i, j;\n' +
                                'for( i = 0; i < sliceGradients.size(); ++i ) {\n' +
                                '  Size nb = sliceGradients[i].size();\n' +
                                '  for( j = 0; j < nb; ++j ) {\n' +
                                '    pixels[dst++] = sliceGradients[i][j];\n' +
                                '  }\n' +
                                '}\n' +
                              '}\n'
      }));

      options.gradientTextureNode = generatorNodeGradientPub;

      volumeNode.getGradientDGNode = function(){return dgnodeGradient;};
      volumeNode.pub.getGradientTextureNode = function(){return generatorNodeGradientPub;};
    }

    var volumeUniformsDGNode = volumeNode.getUniformsDGNode();

    volumeUniformsDGNode.addMember('resolutionFactor', 'Scalar', options.resolutionFactor );
    volumeNode.addMemberInterface(volumeUniformsDGNode, 'resolutionFactor', true);

    volumeUniformsDGNode.setDependency(opacityTextureDGNode, 'opacityImage3D');
    volumeUniformsDGNode.bindings.append(scene.constructOperator({
      operatorName: 'setNbSlicesFrom3DImage',
      parameterLayout: [
        'self.resolutionFactor',
        'opacityImage3D.width',
        'opacityImage3D.height',
        'opacityImage3D.depth',
        'self.nbSlices',
        'self.halfPixelCrop'
      ],
      entryFunctionName: 'setNbSlicesFrom3DImage',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateVolumeSlices.kl'
    }));
    
    var offscreenNode = scene.constructNode('SceneGraphNode', { name: (options.name + "RT") } );
    var offscreenNodeRedrawEventHandler = offscreenNode.constructEventHandlerNode('Redraw');
    scene.getSceneRedrawTransparentObjectsEventHandler().appendChildEventHandler(offscreenNodeRedrawEventHandler);
    offscreenNodeRedrawEventHandler.addMember('renderTargetToViewShaderProgram', 'Integer');
    offscreenNodeRedrawEventHandler.addMember('renderTarget', 'OGLRenderTarget', FABRIC.RT.oglRenderTarget(0,0,[
      new FABRIC.RT.OGLRenderTargetTextureDesc (
          2,
          new FABRIC.RT.OGLTexture2D (
            FABRIC.SceneGraph.OpenGLConstants.GL_RGBA16,
            FABRIC.SceneGraph.OpenGLConstants.GL_RGBA,
            FABRIC.SceneGraph.OpenGLConstants.GL_FLOAT)
        )
    ]));
    
    offscreenNodeRedrawEventHandler.addMember('backgroundColor', 'Color', options.backgroundColor );
    offscreenNodeRedrawEventHandler.setScope('volumeUniforms', volumeUniformsDGNode);

    offscreenNodeRedrawEventHandler.preDescendBindings.append(
      scene.constructOperator({
          operatorName: 'setBackgroundColor',
          srcCode: 'use OGLRenderTarget; operator setBackgroundColor(io Color color, io OGLRenderTarget renderTarget){ renderTarget.clearColor = color; }',
          entryFunctionName: 'setBackgroundColor',
          parameterLayout: [
            'self.backgroundColor',
            'self.renderTarget'
          ]
        }));

    offscreenNodeRedrawEventHandler.preDescendBindings.append(
      scene.constructOperator({
          operatorName: 'setResolutionFactor',
          srcCode: 'use OGLRenderTarget; operator setResolutionFactor(io Scalar factor, io OGLRenderTarget renderTarget){ renderTarget.resolution = (factor < 0.95 ? factor : 1.0); }',
          entryFunctionName: 'setResolutionFactor',
          parameterLayout: [
            'volumeUniforms.resolutionFactor',
            'self.renderTarget'
          ]
        }));

    offscreenNodeRedrawEventHandler.preDescendBindings.append(
      scene.constructOperator({
          operatorName: 'bindScreenRenderTarget',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/renderTarget.kl',
          entryFunctionName: 'bindScreenRenderTarget',
          parameterLayout: [
            'window.width',
            'window.height',
            'self.renderTarget'
          ]
        }));

    offscreenNodeRedrawEventHandler.postDescendBindings.insert(
      scene.constructOperator({
          operatorName: 'unbindRenderTarget',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/renderTarget.kl',
          entryFunctionName: 'unbindRenderTarget',
          parameterLayout: [
            'self.renderTarget'
          ]
        }), 0);

    offscreenNodeRedrawEventHandler.postDescendBindings.append(
      scene.constructOperator({
          operatorName: 'drawVolumeRenderTargetToView',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/renderTarget.kl',
          entryFunctionName: 'drawRenderTargetToView_progID',
          parameterLayout: [
            'self.renderTarget',
            'self.renderTargetToViewShaderProgram'
          ]
        }));

    //Create transfer function images
    var transferFunctionImageNode = scene.constructNode('Image', {
      wantHDR: true,
      createDgNode: true,
      createResourceLoadNode: false,
      createLoadTextureEventHandler: true,
      width: 1024,
      height: 1,
      forceRefresh: true,
      initImage: false,
      glRepeat: false
    });
    var transferFunctionImageDGNode = transferFunctionImageNode.getDGNode();
    
    transferFunctionImageDGNode.addMember('minOpacity', 'Scalar', options.minOpacity);
    transferFunctionImageDGNode.addMember('maxOpacity', 'Scalar', options.maxOpacity);
    volumeNode.addMemberInterface(transferFunctionImageDGNode, 'minOpacity', true);
    volumeNode.addMemberInterface(transferFunctionImageDGNode, 'maxOpacity', true);
    
    if(options.opacityFactorsNode && options.opacityColorsNode){
      var opacityFactorsNode = scene.getPrivateInterface(options.opacityFactorsNode);
      transferFunctionImageDGNode.setDependency(opacityFactorsNode.getDGNode(), 'opacityFactors');
      var opacityColorsNode = scene.getPrivateInterface(options.opacityColorsNode);
      transferFunctionImageDGNode.setDependency(opacityColorsNode.getDGNode(), 'opacityColors');
      
      transferFunctionImageDGNode.bindings.append(scene.constructOperator({
        operatorName: 'updateTransferFunctionImage',
        parameterLayout: [
          'self.minOpacity',
          'self.maxOpacity',
          'opacityFactors.values',
          'opacityColors.values',
          'self.pixels'
        ],
        entryFunctionName: 'updateTransferFunctionImage',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateVolumeSlices.kl'
      }));
    }

    var volumeMaterialNodePub = scene.pub.constructNode('VolumeMaterial', {
        parentEventHandler: offscreenNodeRedrawEventHandler,
        opacityTextureNode: options.opacityTextureNode,
        gradientTextureNode: options.gradientTextureNode,
        transferFunctionTextureNode: transferFunctionImageNode.pub,
        lightNode: options.lightNode,
        storeUniformsInDGNode: true,
        separateShaderNode: false //We don't want it to be shared with VolumeSliceRender nodes
    });

    var volumeMaterialNode = scene.getPrivateInterface(volumeMaterialNodePub);
    var volumeMaterialDGNode = volumeMaterialNode.getDGNode();
    volumeMaterialDGNode.setDependency(volumeUniformsDGNode, 'volumeUniforms');

    volumeMaterialDGNode.addMember('specularFactor', 'Scalar', options.specularFactor );
    volumeMaterialDGNode.addMember('brightnessFactor', 'Scalar', options.brightnessFactor );

    volumeNode.addMemberInterface(volumeMaterialDGNode, 'transparency', true);
    volumeNode.addMemberInterface(volumeMaterialDGNode, 'specularFactor', true);
    volumeNode.addMemberInterface(volumeMaterialDGNode, 'brightnessFactor', true);
    volumeNode.addMemberInterface(volumeMaterialDGNode, 'invertColor', true);
    volumeNode.addMemberInterface(offscreenNodeRedrawEventHandler, 'backgroundColor', true);

    volumeNode.pub.setTransparency(options.transparency);
    volumeNode.pub.setInvertColor(options.invertColor);

    volumeMaterialDGNode.bindings.append(scene.constructOperator({
      operatorName: 'setFactors',
      srcCode: 'operator setFactors(io Integer invertColor, io Scalar specular, io Scalar brightnessFactor, io Scalar transparency, io Size nbSlices, io Scalar alphaFactor, io Scalar scaledSpecular, io Scalar brightness){ \n' +
                    'scaledSpecular = 1.00001 / (1.00001 - specular) - 1.0;\n' +
                    'scaledSpecular *= scaledSpecular;\n' +
                    'brightness = 1.00001 / (1.00001 - (invertColor == 0 ? brightnessFactor : 1.0 - brightnessFactor)) - 1.0;\n' +
                    'Scalar ajustedBrightness = 0.5;//transparency*sqrt(brightness) + (1.0-transparency)*brightness;\n' +
                    'Scalar opacityPerSlice = 1.0 / Scalar(nbSlices);\n' +
                    'Scalar brightnessExp = log( opacityPerSlice ) / log( 0.5 );' + //We modulate by an exponential function else all the interesting values are close to brightness 0.5
                    'Scalar ajustedAlphaPerSlice = pow( ajustedBrightness, brightnessExp);' +
                    'alphaFactor = ajustedAlphaPerSlice / (1.0 - pow(ajustedAlphaPerSlice, Scalar(nbSlices))*0.99999);}\n',
      entryFunctionName: 'setFactors',
      parameterLayout: [
        'self.invertColor',
        'self.specularFactor',
        'self.brightnessFactor',
        'self.transparency',
        'volumeUniforms.nbSlices',
        'self.alphaFactor',
        'self.scaledSpecularFactor',
        'self.brightness'
      ]
    }));

    var instanceNode = scene.constructNode('Instance', {
        transformNode: options.transformNode,
        geometryNode: volumeNodePub,
        materialNode: volumeMaterialNodePub
    });

    volumeNode.getMaterialNode = function(){return volumeMaterialNodePub;}
    volumeNode.getOpacityTextureNode = function(){return options.opacityTextureNode;}
    volumeNode.getGradientTextureNode = function(){return options.gradientTextureNode;}
    volumeNode.getTransferFunctionTextureNode = function(){return transferFunctionImageNode.pub;}
    volumeNode.getLightNode = function(){return options.lightNode;}
    volumeNode.getTransformNode = function(){return options.transformNode;}

    return volumeNode;
  }
});

FABRIC.SceneGraph.registerNodeType('DrawRectangle', {
  briefDesc: '',
  detailedDesc: '',
  parentNodeDesc: 'Image',
  optionsDesc: {
    tl: 'Top left screenspace coord',
    br: 'Bottom right screenspace coord',
    color: 'Rectangle color'
  },

  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        tl: new FABRIC.RT.Vec2(-1.0, 1.0),
        br: new FABRIC.RT.Vec2(1.0, -1.0),
        color: new FABRIC.RT.RGBA(255, 255, 255, 255),
        forceRefresh: false,
        width: 1,
        height: 1,
        createDgNode: false,
        parentEventHandler: scene.getSceneRedrawOverlayObjectsEventHandler()
      });

    options.wantHDR = false;
    options.createResourceLoadNode = false;
    options.createLoadTextureEventHandler = true;

    var rectangleImageNode = scene.constructNode('Image', options);

    var preRedrawEventHandler = rectangleImageNode.constructEventHandlerNode('PreRedraw');
    preRedrawEventHandler.appendChildEventHandler(rectangleImageNode.getRedrawEventHandler());

    preRedrawEventHandler.addMember('tl', 'Vec2', options.tl );
    preRedrawEventHandler.addMember('br', 'Vec2', options.br );

    rectangleImageNode.addMemberInterface(preRedrawEventHandler, 'tl', true);
    rectangleImageNode.addMemberInterface(preRedrawEventHandler, 'br', true);

    preRedrawEventHandler.setScopeName('textureStub');
    preRedrawEventHandler.addMember('textureUnit', 'Integer', 0);
    preRedrawEventHandler.addMember('program', 'Integer', 0);
    preRedrawEventHandler.postDescendBindings.append(
      scene.constructOperator({
          operatorName: 'drawTexture',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawTexture.kl',
          entryFunctionName: 'drawTextureAt',
          parameterLayout: [
            'self.tl',
            'self.br',
            'self.textureUnit',
            'self.program'
          ]
        }
    ));

    var enabled = false;
    rectangleImageNode.pub.enable = function() {
      if(!enabled) {
        options.parentEventHandler.appendChildEventHandler(preRedrawEventHandler);
        enabled = true;
      }
    };

    rectangleImageNode.pub.disable = function() {
      if(enabled) {
        options.parentEventHandler.removeChildEventHandler(preRedrawEventHandler);
        enabled = false;
      }
    };
    rectangleImageNode.pub.enable();

    return rectangleImageNode;
  }
});

FABRIC.SceneGraph.registerNodeType('VolumeSliceRender', {
  briefDesc: '',
  detailedDesc: '',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    volumeOpacityInstanceNode: 'VolumeOpacityInstanceNode to be sliced',
    axis: 'Axis that should be sliced (0=X, 1=Y, 2=Z)',
    ratio: 'Slice ratio from 0 to 1',
    tl: 'Top left screenspace coord',
    br: 'Bottom right screenspace coord',
    backgroundColor: 'Background color',
    sharedVolumeSliceRender: 'In case more than one VolumeSliceRender is created for the same 3D texture, various nodes can be shared.'
  },
  factoryFn: function(options, scene) {

    scene.assignDefaults(options, {
      axis: 0,
      ratio: 0.5,
      tl: new FABRIC.RT.Vec2(-1.0, 1.0),
      br: new FABRIC.RT.Vec2(-0.5, 0.5),
      backgroundColor: new FABRIC.RT.Color(0,0,0,1),
      name: 'VolumeSliceRender'
    });

    var sliceNode = scene.constructNode('SceneGraphNode', options );

    //We create a root event handler which will hold background and material render passes.
    //This enbles to share with other compatible VolumeSliceRender.
    var backgroundRootEventHandler, volumeMaterialNodePub, sliceRedrawEventHandler;

    if( options.sharedVolumeSliceRender !== undefined ) {
      var sharedVolumeSliceRenderPriv = scene.getPrivateInterface(options.sharedVolumeSliceRender);
      backgroundRootEventHandler = sharedVolumeSliceRenderPriv.getBackgroundRedrawEventHandler();
      volumeMaterialNodePub = sharedVolumeSliceRenderPriv.getMaterial();
      sliceRedrawEventHandler = sharedVolumeSliceRenderPriv.getSliceRedrawEventHandler();
    } else {
      backgroundRootEventHandler = sliceNode.constructEventHandlerNode('BackgroundRedraw');
      scene.getSceneRedrawOverlayObjectsEventHandler().appendChildEventHandler(backgroundRootEventHandler);
    }

    options.createDgNode = true;
    options.forceRefresh = true;
    options.parentEventHandler = backgroundRootEventHandler;

    var backgroundDrawNode = scene.constructNode('DrawRectangle', options );
    var backgroundImageDGNode = backgroundDrawNode.getDGNode();

    sliceNode.pub.setBackgroundColor = function(color) {
      backgroundDrawNode.pub.setColor( color );
    };
    sliceNode.pub.setBackgroundColor(options.backgroundColor);

    var volumeOpacityInstanceNode = scene.getPrivateInterface(options.volumeOpacityInstanceNode);

    if(volumeMaterialNodePub == undefined) {

      volumeMaterialNodePub = scene.pub.constructNode('VolumeMaterial', {
          parentEventHandler: scene.getSceneRedrawOverlayObjectsEventHandler(),
          opacityTextureNode: volumeOpacityInstanceNode.getOpacityTextureNode(),
          gradientTextureNode: volumeOpacityInstanceNode.getGradientTextureNode(),
          transferFunctionTextureNode: volumeOpacityInstanceNode.getTransferFunctionTextureNode(),
          lightNode: volumeOpacityInstanceNode.getLightNode(),
          storeUniformsInDGNode: true,
          separateShaderNode: false
      });

      var volumeMaterialNode = scene.getPrivateInterface(volumeMaterialNodePub);
      var volumeMaterialDGNode = volumeMaterialNode.getDGNode();

      var volumeOpacityInstanceMaterialDGNode = scene.getPrivateInterface( volumeOpacityInstanceNode.getMaterialNode() ).getDGNode();
      volumeMaterialDGNode.setDependency(volumeOpacityInstanceMaterialDGNode, 'source');

      volumeMaterialDGNode.bindings.append(scene.constructOperator({
        operatorName: 'copyParams',
        srcCode: 'operator copyParams(io Integer srcInvertColor, io Integer dstInvertColor, io Scalar srcBrightness, io Scalar dstBrightness){ \n' +
                      'dstInvertColor = srcInvertColor;\n' +
                      'dstBrightness = srcBrightness;}\n',
        entryFunctionName: 'copyParams',
        parameterLayout: [
          'source.invertColor',
          'self.invertColor',
          'source.brightness',
          'self.brightness'
        ]
      }));

      volumeMaterialNodePub.setAlphaFactor(1.0);
      volumeMaterialNodePub.setTransparency(1.0);
      volumeMaterialNodePub.setScaledSpecularFactor(0.0);

      sliceRedrawEventHandler = sliceNode.constructEventHandlerNode('SliceRedraw');
      scene.getPrivateInterface(volumeMaterialNodePub).getRedrawEventHandler().appendChildEventHandler(sliceRedrawEventHandler);;


      var preProcessorDefinitions = {
        POSITIONS_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('positions'),
        NORMALS_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('normals'),
        NORMALMATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('normalMatrix'),
        MODELVIEW_MATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('modelViewMatrix'),
        MODELVIEWPROJECTION_MATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('modelViewProjectionMatrix')
      };

      sliceRedrawEventHandler.preDescendBindings.append(
        scene.constructOperator({
            operatorName: 'preSliceRender',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawVolume2DSlice.kl',
            entryFunctionName: 'preSliceRender',
            preProcessorDefinitions: preProcessorDefinitions,
            parameterLayout: [
              'shader.shaderProgram'
            ]
          }));

      sliceRedrawEventHandler.postDescendBindings.append(
        scene.constructOperator({
            operatorName: 'postSliceRender',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawVolume2DSlice.kl',
            entryFunctionName: 'postSliceRender',
            preProcessorDefinitions: preProcessorDefinitions
          }));
    }

    var sliceGeomRedrawEventHandler = sliceNode.constructEventHandlerNode(options.name+'SliceGeomRedraw');
    sliceRedrawEventHandler.appendChildEventHandler(sliceGeomRedrawEventHandler);

    sliceGeomRedrawEventHandler.addMember('ratio', 'Scalar', options.ratio );
    sliceGeomRedrawEventHandler.addMember('axis', 'Size', options.axis );
    sliceGeomRedrawEventHandler.addMember('tl', 'Vec2', options.tl );
    sliceGeomRedrawEventHandler.addMember('br', 'Vec2', options.br );

    sliceNode.addMemberInterface(sliceGeomRedrawEventHandler, 'ratio', true);
    sliceNode.addMemberInterface(sliceGeomRedrawEventHandler, 'axis', true);

    sliceNode.pub.getTl = function(val) {
      return backgroundDrawNode.pub.getTl();
    };
    sliceNode.pub.getBr = function(val) {
      return backgroundDrawNode.pub.getBr();
    };
    sliceNode.pub.setTl = function(val) {
      sliceGeomRedrawEventHandler.setData('tl', 0, val);
      backgroundDrawNode.pub.setTl(val);
    };
    sliceNode.pub.setBr = function(val) {
      sliceGeomRedrawEventHandler.setData('br', 0, val);
      backgroundDrawNode.pub.setBr(val);
    };

    var transformNode = scene.getPrivateInterface(volumeOpacityInstanceNode.getTransformNode());
    sliceGeomRedrawEventHandler.setScope('transform', transformNode.getDGNode());

    sliceGeomRedrawEventHandler.preDescendBindings.append(scene.constructOperator({
      operatorName: 'drawVolumeSlice',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawVolume2DSlice.kl',
      entryFunctionName: 'draw2DVolumeSlice',
      preProcessorDefinitions: preProcessorDefinitions,
      parameterLayout: [
        'transform.globalXfo',
        'self.axis',
        'self.ratio',
        'self.tl',
        'self.br',
        'shader.shaderProgram'
      ]
    }));

    var enabled = true;
    sliceNode.pub.enable = function() {
      if(options.sharedVolumeSliceRender === undefined && !enabled) {
        scene.getSceneRedrawOverlayObjectsEventHandler().appendChildEventHandler(backgroundRootEventHandler);
        scene.getSceneRedrawOverlayObjectsEventHandler().appendChildEventHandler(scene.getPrivateInterface(volumeMaterialNodePub).getRedrawEventHandler());
        enabled = true;
      }
    };

    sliceNode.pub.disable = function() {
      if(options.sharedVolumeSliceRender === undefined && enabled) {
        scene.getSceneRedrawOverlayObjectsEventHandler().removeChildEventHandler(backgroundRootEventHandler);
        scene.getSceneRedrawOverlayObjectsEventHandler().removeChildEventHandler(scene.getPrivateInterface(volumeMaterialNodePub).getRedrawEventHandler());
        enabled = false;
      }
    };
    sliceNode.pub.enable();

    sliceNode.getMaterial = function(){
      return volumeMaterialNodePub;
    };

    return sliceNode;
  }
});

