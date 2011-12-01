
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * The rendering scene graph is the core implementation of Fabric's
 * additional rendering pipeline and capabilities.
 */
FABRIC.SceneGraph.registerNodeType('OffscreenViewport', {
  briefDesc: 'The OffscreenViewport node offers framebuffer based drawing.',
  detailedDesc: 'Utilizing a custom event and eventhandler, the OffscreenViewport node offers a powerful OpenGL framebuffer, ' +
                'which custom shaders can draw to. You can use this for computing depth as well as UV channels, for example. '+
                'The OffscreenViewport node uses the same camera and window as the standard viewport.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    mainViewportNode: 'The viewport this OffscreenViewport will use the window and and viewport dgnode from.',
    cameraNode: 'An optional camera to be used for this OffscreenViewport. If not specified, the camera of the mainViewportNode is used.',
    eventName: 'The name of the event to be created. The OffscreenViewport will listen to this event to be fired for a redraw.',
    textureDescription: 'An optional parameter to define the texture definition which is used to store the OffscreenViewport\'s image.',
    clearColor: 'clear (background) color' //TODO: make this work properly by initializing colors from current ones (background texture)
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        mainViewportNode: undefined,
        cameraNode: undefined,
        eventName: undefined,
        textureDescription: undefined,
        clearColor: FABRIC.RT.rgba(0.0,0.0,0.0,0.0)
      });

    // query the main viewport
    if (!options.mainViewportNode) {
      throw ('Must provide a mainViewportNode to this constructor');
    }
    var mainViewportNode = options.mainViewportNode;
    if(!mainViewportNode.isTypeOf('Viewport')) {
      throw ('mainViewport is not a Viewport!');
    }
    
    // query the camera
    var cameraNode;
    if(options.cameraNode)
      cameraNode = options.cameraNode;
    else
      cameraNode = mainViewportNode.getCameraNode();
    if(!cameraNode.isTypeOf('Camera')) {
      throw ('cameraNode is not a Camera!');
    }
    
    // query the eventName
    if(!options.eventName) {
      throw(' Must provide an eventName to this constructor!');
    }
    var eventName = options.eventName+'';
    
    // create the offscreennode
    var offscreenNode = scene.constructNode('SceneGraphNode', {name: eventName});
    var dgnode = offscreenNode.constructDGNode('DGNode');
    
    // create the event and handler
    var eventNode = offscreenNode.constructEventNode(eventName);
    var eventHandlerNode = offscreenNode.constructEventHandlerNode('Redraw');
    eventNode.appendEventHandler(eventHandlerNode);
    
    // create the render target
    var textureDescription = options.textureDescription;
    if(!textureDescription) {
      textureDescription = new FABRIC.RT.OGLRenderTargetTextureDesc (
        2,
        new FABRIC.RT.OGLTexture2D (
          FABRIC.SceneGraph.OpenGLConstants.GL_RGBA16,
          FABRIC.SceneGraph.OpenGLConstants.GL_RGBA,
          FABRIC.SceneGraph.OpenGLConstants.GL_FLOAT)
      );
    }
    dgnode.addMember('renderTarget', 'OGLRenderTarget', FABRIC.RT.oglRenderTarget(0,0,[textureDescription]));
    
    // extend the public interface
    offscreenNode.pub.getEventName = function() {
      return eventName;
    };
    offscreenNode.pub.getEventHandler = function() {
      return eventHandlerNode;
    };
    offscreenNode.pub.redraw = function() {
      return eventNode.fire();
    };
    offscreenNode.pub.setViewportNode = function(viewport) {
      var viewport = scene.getPrivateInterface(viewport);
      if(!viewport.pub.isTypeOf('Viewport')) {
        throw ('Provided viewport is not of type Viewport!');
      }
      eventHandlerNode.setScope('viewPort',viewport.getDGNode());
      eventHandlerNode.setScope('window',viewport.getFabricWindowObject().windowNode);
    }
    offscreenNode.pub.setCameraNode = function(camera) {
      var camera = scene.getPrivateInterface(camera);
      if(!camera.pub.isTypeOf('Camera')) {
        throw ('Provided camera is not of type Camera!');
      }
      eventHandlerNode.setScope('camera',camera.getDGNode());
    }
    offscreenNode.pub.constructMaterial = function(name) {
      var materialNode = scene.constructNode(name, {
        parentEventHandler: eventHandlerNode
      });
      offscreenNode.pub['get'+name] = function() {
        return materialNode.pub;
      }
      return materialNode.pub;
    }

    // setup the known scopes
    offscreenNode.pub.setViewportNode(mainViewportNode);
    offscreenNode.pub.setCameraNode(cameraNode);
    eventHandlerNode.setScope('data',dgnode);

    // setup the prebindings in order
    var preBindings = eventHandlerNode.preDescendBindings;
    preBindings.append(
      scene.constructOperator({
          operatorName: 'bindScreenRenderTarget',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/renderTarget.kl',
          entryFunctionName: 'bindScreenRenderTarget',
          parameterLayout: [
            'window.width',
            'window.height',
            'data.renderTarget'
          ]
        }));
    
    preBindings.append(scene.constructOperator({
      operatorName: 'UpdateCameraProjection',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/updateCameraProjection.kl',
      entryFunctionName: 'updateCameraProjection',
      parameterLayout: [
        'camera.projectionMat44',
        'window.width',
        'window.height',
        'camera.nearDistance',
        'camera.farDistance',
        'camera.fovY',
        'camera.orthographic',
        'camera.screenOffset'
      ]
    }));

    eventHandlerNode.postDescendBindings.insert(
      scene.constructOperator({
          operatorName: 'unbindRenderTarget',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/renderTarget.kl',
          entryFunctionName: 'unbindRenderTarget',
          parameterLayout: [
            'data.renderTarget'
          ]
        }), 0);
    

    return offscreenNode;
  }});

FABRIC.SceneGraph.registerNodeType('BaseDeferredRenderer', {
  briefDesc: '',
  detailedDesc: '',
  parentNodeDesc: '',
  optionsDesc: {
    colorBuffers: "array of render targets specifying 'nbChannels' (1 to 4) and 'name' for each target. Note that for a target name N, it is expected that the deferred shader will have a " + 
                  "texture parameter name NTexture. Ex: [{nbChannels: 4, name: 'diffuse'}, {nbChannels: 3, name: 'normal'}], matching params diffuseTexture and normalTexture in deferred shader material." +
                  "Note that your prePass shaders should then write to targets of the corresponding indices: glFragData[0]=diffuseColor; glFragData[1]=normal;",
    addDepth: "if depth should be attached",
    showDebug: "Display view of the render targets",
    lightNode: "to be used by the Phong shading pass"
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      showDebug: false,
      addDepth: true,
      colorBuffers: []
      });

    if(options.colorBuffers.length == 0)
      throw ('Error: no render target provided');

    var deferredRenderNode = scene.constructNode('SceneGraphNode');
    var redrawEventHandler = deferredRenderNode.constructEventHandlerNode('DeferredDraw');
    redrawEventHandler.setScopeName('deferredDraw');

    scene.getSceneRedrawOpaqueObjectsEventHandler().appendChildEventHandler(redrawEventHandler);

    var renderTargetRedrawEventHandler = deferredRenderNode.constructEventHandlerNode('RenderTargetRedraw');
    redrawEventHandler.appendChildEventHandler(renderTargetRedrawEventHandler);

    var channelToInternalFormat = [0, FABRIC.SceneGraph.OpenGLConstants.GL_R16F, FABRIC.SceneGraph.OpenGLConstants.GL_RG16F, FABRIC.SceneGraph.OpenGLConstants.GL_RGB16F, FABRIC.SceneGraph.OpenGLConstants.GL_RGBA16F];
    var channelToFormat = [0, FABRIC.SceneGraph.OpenGLConstants.GL_RED, FABRIC.SceneGraph.OpenGLConstants.GL_RG, FABRIC.SceneGraph.OpenGLConstants.GL_RGB, FABRIC.SceneGraph.OpenGLConstants.GL_RGBA];

    var oglRenderTargetTextureDescs = [];
    var oglRenderTargetTextureNames = [];

    var i;
    for( i = 0; i < options.colorBuffers.length; ++i ) {
      var nbChannels = options.colorBuffers[i].nbChannels;
      if(nbChannels === undefined || nbChannels < 1 || nbChannels > 4)
        throw ('Error: unsupported number of channels for render target ' + i);
      if(options.colorBuffers[i].name === undefined)
        throw ('Error: name not specified for render target ' + i);

      oglRenderTargetTextureDescs.push(
          new FABRIC.RT.OGLRenderTargetTextureDesc (
              2, // COLOR_BUFFER
              new FABRIC.RT.OGLTexture2D (
                channelToInternalFormat[nbChannels],
                channelToFormat[nbChannels],
                FABRIC.SceneGraph.OpenGLConstants.GL_FLOAT)
            ));
      oglRenderTargetTextureNames.push(options.colorBuffers[i].name);
    }

    if(options.addDepth) {
      oglRenderTargetTextureDescs.push(
          new FABRIC.RT.OGLRenderTargetTextureDesc (
              1, // DEPTH_BUFFER
              new FABRIC.RT.OGLTexture2D (
                FABRIC.SceneGraph.OpenGLConstants.GL_DEPTH_COMPONENT,
                FABRIC.SceneGraph.OpenGLConstants.GL_DEPTH_COMPONENT,
                FABRIC.SceneGraph.OpenGLConstants.GL_FLOAT)
            ));
      oglRenderTargetTextureNames.push('depth');
    }

    var nbRenderTargets = oglRenderTargetTextureNames.length;
    var renderTarget = FABRIC.RT.oglRenderTarget(0,0,
      oglRenderTargetTextureDescs,
      {
        clearColor: options.clearColor
      }
    );
    redrawEventHandler.addMember('renderTarget', 'OGLRenderTarget', renderTarget);

    renderTargetRedrawEventHandler.preDescendBindings.append(
      scene.constructOperator({
          operatorName: 'bindScreenRenderTarget',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/renderTarget.kl',
          entryFunctionName: 'bindScreenRenderTarget',
          parameterLayout: [
            'window.width',
            'window.height',
            'deferredDraw.renderTarget'
          ]
        }));

    renderTargetRedrawEventHandler.postDescendBindings.append(
      scene.constructOperator({
          operatorName: 'unbindRenderTarget',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/renderTarget.kl',
          entryFunctionName: 'unbindRenderTarget',
          parameterLayout: [
            'deferredDraw.renderTarget'
          ]
        }));

    if(options.addDepth) {
      /*
      renderTargetRedrawEventHandler.postDescendBindings.append(
        scene.constructOperator({
            operatorName: 'copyDepth',
            srcCode: 'use OGLRenderTarget; operator copyDepth(io Integer width, io Integer height, io OGLRenderTarget renderTarget){\n'+
                        'glBindFramebuffer(GL_READ_FRAMEBUFFER, renderTarget.fbo);\n' +
                        'glBlitFramebuffer(0,0,width-1,height-1,0,0,width-1,height-1,GL_DEPTH_BUFFER_BIT, GL_NEAREST);\n'+
                        'glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);}',
            entryFunctionName: 'copyDepth',
            parameterLayout: [
              'window.width',
              'window.height',
              'deferredDraw.renderTarget'
            ]
          }));
      */
    }

    redrawEventHandler.addMember('debugDrawToggle', 'Boolean', options.showDebug);
    redrawEventHandler.addMember('debugShaderProgID', 'Integer', 0);
    deferredRenderNode.addMemberInterface(redrawEventHandler, 'debugDrawToggle', true);

    redrawEventHandler.postDescendBindings.append(
      scene.constructOperator({
          operatorName: 'debugDrawRenderTargets',
          srcCode:'use OGLTexture2D;\n' +
                  'operator debugDrawRenderTargets(io OGLRenderTarget renderTarget, io Integer progId, io Boolean draw){ \n' +
                  'if(!draw)return;\n' +
                  'Size i, j, n = renderTarget.textures.size, curr = 0;\n' +
                  'for(i = 0; i < 3; ++i){\n' +
                  '  for(j = 0; j < 3; ++j){\n' +
                  '    while(true) {\n' +
                  '      if(curr == n)\n' +
                  '        return;\n' +
                  '      if(renderTarget.textures[curr].type == 2)\n' +
                  '        break;\n' +
                  '      curr++;\n' +
                  '    }\n' +
                  '    Scalar x = -1.0 + Scalar(i)/1.5;\n' +
                  '    Scalar y = 1.0 - Scalar(j)/1.5;\n' +
                  '    renderTarget.textures[curr++].texture.bind(0);\n' +
                  '    drawTexture(0, progId, Vec2(x,y), Vec2(x+0.66666,y-0.66666), false);\n' +
                  '    if(curr == n)\n' +
                  '      return;}}}\n',
          entryFunctionName: 'debugDrawRenderTargets',
          parameterLayout: [
            'self.renderTarget',
            'self.debugShaderProgID',
            'self.debugDrawToggle'
          ]
    }));
    
    //Shading pass
    var shaderPassRedrawEventHandler = deferredRenderNode.constructEventHandlerNode('ShaderPass');
    redrawEventHandler.appendChildEventHandler(shaderPassRedrawEventHandler);

    shaderPassRedrawEventHandler.preDescendBindings.append(
      scene.constructOperator({
          operatorName: 'preDeferredRenderShading',
          srcCode: 'use FabricOGL; operator preDeferredRenderShading(){\n'+
                      'glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);\n' +
                      'glDepthMask(GL_FALSE);\n' +
                      'glEnable(GL_BLEND);\n' +
                      'glBlendFunc(GL_SRC_ALPHA, GL_ONE);\n' +
                      '}',
          entryFunctionName: 'preDeferredRenderShading',
          parameterLayout: []
        }));

    shaderPassRedrawEventHandler.postDescendBindings.append(
      scene.constructOperator({
          operatorName: 'postDeferredRenderShading',
          srcCode: 'use FabricOGL; operator postDeferredRenderShading(){\n'+
                      'glPopAttrib();}',
          entryFunctionName: 'postDeferredRenderShading',
          parameterLayout: []
        }));

    //Build event handlers for loading each render target
    var renderTargetTextureRedrawEventHandler = [];
    for(i = 0; i < nbRenderTargets; ++i) {
      var handler = deferredRenderNode.constructEventHandlerNode(oglRenderTargetTextureNames[i]+'TextureRedraw');
      renderTargetTextureRedrawEventHandler.push(handler);
      handler.preDescendBindings.append(scene.constructOperator({
        operatorName: 'bind' + oglRenderTargetTextureNames[i] + 'Texture',
        preProcessorDefinitions: {
          TEXTURE_INDEX: i
        },
        srcCode: 'use OGLRenderTarget; operator bindTexture( io OGLRenderTarget renderTarget, io Integer textureUnit ) {\n' +
                     ' renderTarget.textures[TEXTURE_INDEX].texture.bind(textureUnit);}',
        entryFunctionName: 'bindTexture',
        parameterLayout: [
          'deferredDraw.renderTarget',
          'textureStub.textureUnit'
        ]
      }));
    }

    var capitalizeFirstLetter = function(str) {
      return str[0].toUpperCase() + str.substr(1);
    };

    deferredRenderNode.pub.addPostPassMaterial = function(materialName, options) {

      options.parentEventHandler = shaderPassRedrawEventHandler;
      var materialNodePub = scene.pub.constructNode(materialName, options);
      var material = scene.getPrivateInterface(materialNodePub);

      for(i = 0; i < nbRenderTargets; ++i) {
        var setTextureFuncName = 'set' + capitalizeFirstLetter(oglRenderTargetTextureNames[i]) + 'Texture' + 'RedrawEventHandler';
        if(material[setTextureFuncName] !== undefined) {
          material[setTextureFuncName]( renderTargetTextureRedrawEventHandler[i] );
        }
      }

      var materialRedrawHandler = material.getRedrawEventHandler();

      if(options.shadeFullScreen !== undefined && options.shadeFullScreen) {
        materialRedrawHandler.postDescendBindings.insert(
          scene.constructOperator({
              operatorName: 'drawShaderQuad',
              srcCode:'use OGLTexture2D, OGLShaderProgram;\n' +
                      'operator drawShaderQuad(io OGLShaderProgram program){ \n' +
                      '  drawScreenQuad(program.programId, Vec2(-1.0,1.0), Vec2(1.0,-1.0), false);}',
              entryFunctionName: 'drawShaderQuad',
              parameterLayout: [
                'shader.shaderProgram'
              ]
        }),0);
      }
      return materialNodePub;
    };
   
    deferredRenderNode.pub.addPrePassMaterial = function(materialName, options) {
      options.parentEventHandler = renderTargetRedrawEventHandler;
      return scene.pub.constructNode(materialName, options);
    };

    return deferredRenderNode;
}});

var definedDeferredPhongMaterials = false;

FABRIC.SceneGraph.registerNodeType('PhongDeferredRenderer', {
  briefDesc: '',
  detailedDesc: '',
  parentNodeDesc: 'BaseDeferredRenderer',
  optionsDesc: {
    addPhongShadingLayer: "Adds a global Phong shading by default"
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      addPhongShadingLayer: true
      });

    if( definedDeferredPhongMaterials == false ) {
      FABRIC.SceneGraph.defineEffectFromFile('DeferredPrePhongMaterial', 'FABRIC_ROOT/SceneGraph/Shaders/DeferredPrePhongShader.xml');
      FABRIC.SceneGraph.defineEffectFromFile('DeferredPrePhongInstancingExtMaterial', 'FABRIC_ROOT/SceneGraph/Shaders/DeferredPrePhongInstancingExtShader.xml');
      FABRIC.SceneGraph.defineEffectFromFile('DeferredPostPhongMaterial', 'FABRIC_ROOT/SceneGraph/Shaders/DeferredPostPhongShader.xml');
      definedDeferredPhongMaterials = true;
    }

    options.colorBuffers = [{name: 'positions', nbChannels: 3}, {name: 'normals', nbChannels: 3}, {name: 'diffuseAndSpecularFactor', nbChannels: 4}];
    options.addDepth = true;

    var deferredRenderNode = scene.constructNode('BaseDeferredRenderer', options);
   
    deferredRenderNode.pub.addPhongShadingLayer = function(options) {
      options.shadeFullScreen = true;
      deferredRenderNode.pub.addPostPassMaterial('DeferredPostPhongMaterial', options);
      options.shadeFullScreen = undefined;
    };

    deferredRenderNode.pub.addPrePassPhongMaterial = function(options) {
      return deferredRenderNode.pub.addPrePassMaterial('DeferredPrePhongMaterial', options);
    };

    deferredRenderNode.pub.addPrePassPhongInstancingMaterial = function(options) {
      return deferredRenderNode.pub.addPrePassMaterial('DeferredPrePhongInstancingExtMaterial', options);
    };

    if(options.addPhongShadingLayer) {
      deferredRenderNode.pub.addPhongShadingLayer(options);
    }

    return deferredRenderNode;
}});
