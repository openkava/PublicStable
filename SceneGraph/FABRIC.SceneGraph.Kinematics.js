
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.SceneGraph.registerNodeType('Transform', {
  briefDesc: 'The Transform node implements a global or hierarchical transform.',
  detailedDesc: 'The Transform node uses a redraw eventhandler to draw an OpenGL projection. The node can be used in '+
                'a hierarchical mode, in which is projects its own projection into another Transform node\'s projection. '+
                'This allows to compute hierarchies in 3D.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    hierarchical: 'Flag determining wether this Transform node should calculate a globalXfo based on a parents globalXfo',
    parentTransformNode: 'If the TransformNode is hierarchical, this option can optionally provide a parent transform node to immediately assign',
    globalXfo: 'Optional: Specify the intial globalXfo for this transform.',
    localXfo: 'Optional: Specify the intial localXfo for this transform.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        hierarchical: false,
        parentTransformNode: undefined,
        globalXfo: undefined,
        localXfo: undefined
      });

    var transformNode = scene.constructNode('SceneGraphNode', options),
      dgnode = transformNode.constructDGNode('DGNode'),
      textureNode = undefined,
      parentTransformNode,
      children = [];

    dgnode.addMember('globalXfo', 'Xfo', options.globalXfo);
    transformNode.addMemberInterface(dgnode, 'globalXfo');

    if (options.hierarchical) {
      dgnode.addMember('localXfo', 'Xfo', options.localXfo);
      dgnode.bindings.append(scene.constructOperator( {
          operatorName: 'calcGlobalXfo',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/calcGlobalXfo.kl',
          parameterLayout: [
            'self.localXfo',
            'parent.globalXfo',
            'self.globalXfo'
          ],
          entryFunctionName: 'calcGlobalXfo'
        }));

      transformNode.addMemberInterface(dgnode, 'localXfo', true, true);

      // use a custom getter
      transformNode.pub.setGlobalXfo = function(val) {
        if (parentTransformNode) {
          var parentXfo = parentTransformNode.getGlobalXfo();
          val = val.multiply(parentXfo.inverse());
          dgnode.setData('localXfo', val);
        }
        else {
          dgnode.setData('globalXfo', val);
        }
      };

      transformNode.pub.getParentNode = function() {
        return scene.getPublicInterface(parentTransformNode);
      };
      transformNode.pub.setParentNode = function(node, reciprocate) {
        if (!node.isTypeOf('Transform')) {
          throw ('Incorrect type assignment. Must assign a Transform');
        }
        parentTransformNode = scene.getPrivateInterface(node);
        dgnode.setDependency(parentTransformNode.getDGNode(), 'parent');
        if (reciprocate !== false && node.addChild) {
          node.addChild(this, false);
        }
      };
      transformNode.pub.addChild = function(node, reciprocate) {
        children.push(node);
        if (reciprocate !== false) {
          node.setParentNode(this, false);
        }
      };
      transformNode.pub.getChildren = function() {
        return children;
      };

      if (options.parentTransformNode) {
        transformNode.pub.setParentNode(options.parentTransformNode);
      }
    }else {
      transformNode.pub.setGlobalXfo = function(val) {
        if(val.constructor.name == "Array") {
          dgnode.setCount(val.length);
          dgnode.setBulkData({ globalXfo: val});
        }
        else {
          dgnode.setData('globalXfo', 0, val);
        }
      };
    }
    
    
    transformNode.pub.getTransformTexture = function(dynamic) {
      if(!textureNode){
        if(dgnode.getCount() <= 1){
          throw "Transform node has only 1 slices";
        }
        // create the operator to convert the matrices into a texture
        dgnode.addMember('textureMatrix', 'Mat44');
        dgnode.bindings.append(scene.constructOperator( {
            operatorName: 'calcGlobalTransposedMatrix',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/calcGlobalXfo.kl',
            parameterLayout: [
              'self.globalXfo',
              'self.textureMatrix'
            ],
            entryFunctionName: 'calcGlobalTransposedMatrix'
          }));
        textureNode = scene.constructNode('TransformTexture', {transformNode: transformNode.pub, dynamic: dynamic});
      }
      return textureNode.pub;
    }
    
    
    //////////////////////////////////////////
    // Persistence
    var parentAddDependencies = transformNode.addDependencies;
    transformNode.addDependencies = function(sceneSerializer) {
      parentAddDependencies(sceneSerializer);
      if(parentTransformNode){
        sceneSerializer.addNode(parentTransformNode.pub);
      }
    };
    
    var parentWriteData = transformNode.writeData;
    var parentReadData = transformNode.readData;
    transformNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      constructionOptions.hierarchical = options.hierarchical;
      if(parentTransformNode){
        nodeData.parentTransformNode = parentTransformNode.pub.getName();
      }
      nodeData.globalXfo = transformNode.pub.getGlobalXfo();
      
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
    };
    transformNode.readData = function(sceneDeserializer, nodeData) {
      if(nodeData.parentTransformNode){
        transformNode.pub.setParentNode(sceneDeserializer.getNode(nodeData.parentTransformNode));
      }
      transformNode.pub.setGlobalXfo(nodeData.globalXfo);
      
      parentReadData(sceneDeserializer, nodeData);
    };

    return transformNode;
  }});

FABRIC.SceneGraph.registerNodeType('TransformTexture', {
  briefDesc: 'The TransformTexture node is an Image node which can be used for storing matrices into a texture buffer.',
  detailedDesc: 'The TransformTexture node is an Image node which can be used for storing matrices into a texture buffer. This is used for efficient instance rendering.',
  parentNodeDesc: 'Texture',
  optionsDesc: {
    transformNode: 'A sliced transform node storing all of the transform to store.',
    dynamic: 'If set to true, the texture will be reloaded every frame.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      transformNode: undefined,
      dynamic: undefined
    });
    
    if(!options.transformNode) {
      throw('You need to specify a transformNode for this constructor!');
    }
    if(!options.transformNode.isTypeOf('Transform')) {
      throw('The specified transformNode is not of type \'Transform\'.');
    }
    var transformdgnode = scene.getPrivateInterface(options.transformNode).getDGNode();
    var textureNode = scene.constructNode('Texture', options);
    
    var redrawEventHandler = textureNode.constructEventHandlerNode('Redraw');
    textureNode.getRedrawEventHandler = function() { return redrawEventHandler; }
    
    var tex = FABRIC.RT.oglMatrixBuffer2D();
    if(!options.dynamic)
      tex.forceRefresh = false;
    redrawEventHandler.addMember('oglTexture2D', 'OGLTexture2D', tex);
    redrawEventHandler.addMember('matricesTempBuffer', 'Mat44[]');
    redrawEventHandler.addMember('textureHeight', 'Size');

    redrawEventHandler.setScope('transform', transformdgnode);
    redrawEventHandler.preDescendBindings.append(scene.constructOperator({
      operatorName: 'prepareTextureMatrix',
      preProcessorDefinitions: {
        TRANSFORM_TEXTURE_HEIGHT_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('transformTextureHeight')
      },
      srcCode: 'use OGLShaderProgram, OGLTexture2D;\n'+
               'operator prepareTextureMatrix(io Mat44 matrices<>, io Mat44 matricesTempBuffer[], io Size textureHeight, io OGLTexture2D oglTexture2D, io OGLShaderProgram shaderProgram, io Integer textureUnit){\n' +
               '  if(textureUnit > -1) {\n' +
               '    Size height = 0;\n' +
               '    oglTexture2D.bindImageMatrix(matrices, matricesTempBuffer, textureUnit, height);\n' +
               '    if(height != 0)//height == 0 if texture is already bound\n' +
               '      textureHeight = height;\n' +
               '  }else{\n' +
               '    report("debugging instance matrices: ");\n' +
               '    for(Size i=0;i<10;i++)\n' +
               '      report("matrix "+i+": "+matrices[i]);\n' +
               '    report("matrix "+(matrices.size()-1)+": "+matrices[matrices.size()-1]);\n' +
               '   }\n'+
               '  shaderProgram.numInstances = Integer(matrices.size());\n' +
               '  Integer location = shaderProgram.getUniformLocation( TRANSFORM_TEXTURE_HEIGHT_ATTRIBUTE_ID );\n' +
               '  if(location != -1)\n' +
               '    shaderProgram.loadScalarUniform(location, Scalar(textureHeight));\n' +
               '}',
      entryFunctionName: 'prepareTextureMatrix',
      parameterLayout: [
        'transform.textureMatrix<>',
        'self.matricesTempBuffer',
        'self.textureHeight',
        'self.oglTexture2D',
        'shader.shaderProgram',
        'textureStub.textureUnit'
      ]
    }));

    return textureNode;
  }});

FABRIC.SceneGraph.registerNodeType('AimTransform', {
  briefDesc: 'The AimTransform node implements a global lookat transform.',
  detailedDesc: 'The AimTransform uses the Transform node to implement a lookat transform, using a '+
                'root position as well as a target position. This node is used for the TargetCamera node.',
  parentNodeDesc: 'Transform',
  optionsDesc: {
    target: 'The target position for the initial direction of the Transform'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        target: new FABRIC.RT.Vec3(0, 0, 0)
      });
    
    if(options.position && options.target){
      options.globalXfo = new FABRIC.RT.Xfo({ tr: options.position });
      options.globalXfo.ori.setFromDirectionAndUpvector(
        options.position.subtract(options.target),
        new FABRIC.RT.Vec3(0,1,0));
    }

    var aimTransformNode = scene.constructNode('Transform', options);
    var dgnode = aimTransformNode.getDGNode();
    dgnode.addMember('target', 'Vec3', options.target);

    dgnode.bindings.append(scene.constructOperator({
      operatorName: 'aimTransform',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/aimTransform.kl',
      entryFunctionName: 'aimTransform',
      parameterLayout: [
        'self.globalXfo',
        'self.target'
      ]
    }));

    aimTransformNode.addMemberInterface(dgnode, 'target', true);
    return aimTransformNode;
  }});



