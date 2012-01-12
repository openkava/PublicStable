
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.SceneGraph.registerNodeType('Geometry', {
  briefDesc: 'The Geometry node is a base abstract node for all geometry nodes.',
  detailedDesc: 'The Geometry node defines the basic structure of a geometry, such as the uniforms, attributes, '+
                'and bounding box dgnodes. It also configures services for raycasting, and uploading vertex ' +
                'attributes to the GPU for rendering.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    createBoundingBoxNode: 'Flag instructing whether to construct a bounding box node. Bounding boxes are used in raycasting, so not always necessary'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        createBoundingBoxNode: true,
        drawable: true,
        dynamicIndices: false
      });
  
    var geometryNode = geometryNode = scene.constructNode('SceneGraphNode', options),
      uniformsdgnode = geometryNode.constructDGNode('UniformsDGNode'),
      attributesdgnode = geometryNode.constructDGNode('AttributesDGNode'),
      bboxdgnode,
      redrawEventHandler,
      shaderUniforms = [],
      shaderAttributes = [];
      
    if(options.drawable){
      redrawEventHandler = geometryNode.constructEventHandlerNode('Redraw');
      redrawEventHandler.setScope('uniforms', uniformsdgnode);
      redrawEventHandler.setScope('attributes', attributesdgnode);
    }

    attributesdgnode.setDependency(uniformsdgnode, 'uniforms');
    
    if (options.createBoundingBoxNode) {
      bboxdgnode = geometryNode.constructDGNode('BoundingBoxDGNode');
      bboxdgnode.addMember('min', 'Vec3');
      bboxdgnode.addMember('max', 'Vec3');
      bboxdgnode.setDependency(attributesdgnode, 'attributes');
      bboxdgnode.bindings.append(scene.constructOperator({
        operatorName: 'calcBoundingBox',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/calcBoundingBox.kl',
        entryFunctionName: 'calcBoundingBox',
        parameterLayout: [
          'attributes.positions<>',
          'self.min',
          'self.max'
        ]
      }));
    }
    
    // Here we are ensuring that the generator ops are added to the
    // beginning of the operator list. if there are operators for
    // generating tangents, or any other data, they should always go after
    // the generator ops.
    // Currently the generator ops are bound to the attributesdgnode,
    // but with node nesting, we should put them on the outer node.
    // this would mean 2 things. We could generate geometry in a
    // single operator, and we wouldn't need this code here....
    geometryNode.setGeneratorOps = function(opBindings) {
      var i;
      if (attributesdgnode.bindings.empty()) {
        for (i = 0; i < opBindings.length; i++) {
          attributesdgnode.bindings.append(opBindings[i]);
        }
      }
      else {
        for (i = 0; i < opBindings.length; i++) {
          attributesdgnode.bindings.insert(opBindings[i], i);
        }
      }
    };
    geometryNode.getRayIntersectionOperator = function() {
      throw ('Derived Geometries must define a ray intersection operator.');
    };
    
    var capitalizeFirstLetter = function(str) {
      return str[0].toUpperCase() + str.substr(1);
    };
    
    // extend public interface
    geometryNode.pub.addUniformValue = function(name, type, value, addGetterSetterInterface) {
      uniformsdgnode.addMember(name, type, value);
      if (addGetterSetterInterface) {
        geometryNode.addMemberInterface(uniformsdgnode, name, true);
      }
      if(name == 'indices'){
        var registeredTypes = scene.getContext().RegisteredTypesManager.getRegisteredTypes();
        var attributeID = FABRIC.SceneGraph.getShaderParamID(name);
        var indicesBuffer = new FABRIC.RT.OGLBuffer(name, attributeID, registeredTypes.Integer);
        indicesBuffer.dynamic = options.dynamicIndices;
        redrawEventHandler.addMember('indicesBuffer', 'OGLBuffer', indicesBuffer);

        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'genVBO',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadVBO.kl',
          preProcessorDefinitions: {
            DATA_TYPE: 'Integer'
          },
          entryFunctionName: 'genVBO',
          parameterLayout: [
            'uniforms.indices',
            'self.indicesBuffer'
          ]
        }));
      }
    };
    var attributes = {};
    geometryNode.pub.addVertexAttributeValue = function(name, type, attributeoptions) {
      attributesdgnode.addMember(name, type, attributeoptions ? attributeoptions.defaultValue : undefined);
      if(attributeoptions){
        if(attributeoptions.genVBO && redrawEventHandler){
          var registeredTypes = scene.getContext().RegisteredTypesManager.getRegisteredTypes();
          var typeDesc = registeredTypes[type];
          var attributeID = FABRIC.SceneGraph.getShaderParamID(name);
          var bufferMemberName = name + 'Buffer';
          
          var buffer = new FABRIC.RT.OGLBuffer(name, attributeID, typeDesc);
          if(attributeoptions.dynamic){
            buffer.bufferUsage = FABRIC.SceneGraph.OpenGLConstants.GL_DYNAMIC_DRAW;
          }
          redrawEventHandler.addMember(bufferMemberName, 'OGLBuffer', buffer);
          redrawEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: 'load' + capitalizeFirstLetter(type) +'VBO',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadVBO.kl',
            preProcessorDefinitions: {
              DATA_TYPE: type
            },
            entryFunctionName: 'genAndBindVBO',
            parameterLayout: [
              'shader.shaderProgram',
              'attributes.' + name + '<>',
              'self.' + bufferMemberName
            ]
          }));
        }
      }
      attributes[name] = { type:type, attributeoptions:attributeoptions };
    };
    // Trigger a reloading of the vbo. This is useull when modifiying geometry
    // attributes that are not dynamic, but change from time to time.
    geometryNode.pub.reloadVBO = function(name) {
      var buffer = redrawEventHandler.getData(name + 'Buffer');
      buffer.reload = true;
      redrawEventHandler.setData(name + 'Buffer', 0, buffer);
    };
    geometryNode.pub.setAttributeDynamic = function(name) {
      var buffer = redrawEventHandler.getData(name + 'Buffer');
      buffer.bufferUsage = FABRIC.SceneGraph.OpenGLConstants.GL_DYNAMIC_DRAW;
      redrawEventHandler.setData(name + 'Buffer', 0, buffer);
    };
    geometryNode.pub.setAttributeStatic = function(name) {
      var buffer = redrawEventHandler.getData(name + 'Buffer');
      buffer.bufferUsage = FABRIC.SceneGraph.OpenGLConstants.GL_STATIC_DRAW;
      redrawEventHandler.setData(name + 'Buffer', 0, buffer);
    };
    geometryNode.pub.getUniformValue = function(name) {
      return uniformsdgnode.getData(name);
    };
    geometryNode.pub.setUniformValue = function(name, value) {
      return uniformsdgnode.setData(name, 0, value);
    };
    geometryNode.pub.getVertexCount = function(count) {
      return attributesdgnode.getCount();
    };
    geometryNode.pub.setVertexCount = function(count) {
      attributesdgnode.setCount(count);
    };
    geometryNode.pub.getBulkAttributeData = function( pointIds ) {
      return attributesdgnode.getSlicesBulkData( pointIds );
    };
    geometryNode.pub.setBulkAttributeData = function( attributeData ) {
      return attributesdgnode.setSlicesBulkData( attributeData );
    };
    // This method is used by the parsers such as the collada parser to load
    // parsed data into a constructed geometry.
    geometryNode.pub.loadGeometryData = function(data, datatype, sliceindex) {
      var i,
        uniformMembers = uniformsdgnode.getMembers(),
        attributeMembers = attributesdgnode.getMembers(),
        uniformData = {},
        attributeData = {};
      for (i in uniformMembers) {
        if (data[i]) {
          uniformsdgnode.setData(i, 0, data[i]);
        }
      }
      for (i in attributeMembers) {
        if (data[i]) {
          attributeData[i] = data[i];
        }
      }
      if (attributeData.positions) {
        attributesdgnode.setCount(attributeData.positions.length);
      }
      attributesdgnode.setBulkData(attributeData);
    };
    geometryNode.pub.getBoundingBox = function(){
      if(!bboxdgnode){
        throw("Goemetry does not support a Bounding Box");
      }
      bboxdgnode.evaluate();
      return {
        min: bboxdgnode.getData('min'),
        max: bboxdgnode.getData('max')
      }
    };
    
    var parentWriteData = geometryNode.writeData;
    var parentReadData = geometryNode.readData;
    /*
    geometryNode.writeGeometryData = function(sceneSerializer, constructionOptions, nodeData) {
      nodeData.attributes = attributes;
      nodeData.sliceCount = attributesdgnode.getCount();
      nodeData.attributeData = attributesdgnode.getBulkData();
      var uniformMembers = uniformsdgnode.getMembers();
      if(uniformMembers.indices){
        nodeData.indices = uniformsdgnode.getData('indices');
      }
    }
    geometryNode.readGeometryData = function(sceneDeserializer, nodeData) {
      var attributeMembers = attributesdgnode.getMembers();
      if(nodeData.attributes){
        for(var attributeName in nodeData.attributes){
          if(!attributeMembers[attributeName]){
            geometryNode.pub.addVertexAttributeValue(
              attributeName,
              nodeData.attributes[attributeName].type,
              nodeData.attributes[attributeName].attributeoptions
            );
          }
        }
      }
      if(nodeData.sliceCount){
        attributesdgnode.setCount(nodeData.sliceCount);
      }
      if(nodeData.attributeData){
        attributesdgnode.setBulkData(nodeData.attributeData);
      }
      var uniformMembers = uniformsdgnode.getMembers();
      if(nodeData.indices){
        uniformsdgnode.setData('indices', 0, nodeData.indices );
      }
    }
    */
    var writeGeometryAttributes = true;
    geometryNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      constructionOptions.drawable = options.drawable;
      constructionOptions.createBoundingBoxNode = options.createBoundingBoxNode;
      nodeData.attributes = attributes;
      if(writeGeometryAttributes){
        var attributeMembers = [];
        for(var i in attributes) attributeMembers.push(i);
        sceneSerializer.writeDGNodesData(geometryNode.pub.getName(), {
          uniforms:{
            dgnode: uniformsdgnode,
            members: ['indices']
          },
          attributes:{
            dgnode: attributesdgnode,
            members: attributeMembers
          }
        });
      }
    };
    geometryNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      
      var attributeMembers = attributesdgnode.getMembers();
      for(var attributeName in nodeData.attributes){
        if(!attributeMembers[attributeName]){
          geometryNode.pub.addVertexAttributeValue(
            attributeName,
            nodeData.attributes[attributeName].type,
            nodeData.attributes[attributeName].attributeoptions
          );
        }
      }
      sceneDeserializer.loadDGNodesData(geometryNode.pub.getName(), {
        uniforms:{
          dgnode: uniformsdgnode
        },
        attributes:{
          dgnode: attributesdgnode
        }
      });
    };
    
    return geometryNode;
  }});


FABRIC.SceneGraph.registerNodeType('GeometryDataCopy', {
  briefDesc: 'The GeometryDataCopy node is created using an existing Geometry node, and is used to build deformation stacks.',
  detailedDesc: 'When a node is evaluated, all the operators are evaluated. By splitting geometry into multiple data copies,' +
                ' only operators on nodes that are dirty will be evaluated. This makes it easy to build chains of nodes which,'+
                ' provide the same behavior as the operator stacks you would find in a traditional DCC application like 3dsmax/Maya/Softimage.',
  parentNodeDesc: 'Geometry',
  optionsDesc: {
    baseGeometryNode: 'The original data copy to use as the bases of this geometries data. '
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        baseGeometryNode: undefined,
        createBoundingBoxNode: false
      });
    
    options.createDrawOperator = false;
    var geometryDataCopyNode = scene.constructNode('Geometry', options);
    
    // The data copy must always have the same count on the attributes node,
    // as the original geometry node.
    geometryDataCopyNode.getAttributesDGNode().bindings.append(
      scene.constructOperator({
        operatorName: 'matchCount',
        srcCode: '\n'+
        'operator matchCount(Size parentCount, io Size selfCount) {\n' +
        '  selfCount = parentCount;\n' +
        '}',
        entryFunctionName: 'matchCount',
        parameterLayout: [
          'parentattributes.count',
          'self.newCount'
        ]
      }));
    var redrawEventHandler = geometryDataCopyNode.getRedrawEventHandler();
    var uniformsdgnode = geometryDataCopyNode.getUniformsDGNode();
    var attributesdgnode = geometryDataCopyNode.getAttributesDGNode();
    var baseGeometryNode;
    geometryDataCopyNode.addReferenceInterface('BaseGeometry', 'Geometry',
      function(nodePrivate){
        baseGeometryNode = nodePrivate;
        if(baseGeometryNode){
          uniformsdgnode.setDependency(baseGeometryNode.getUniformsDGNode(), 'parentuniforms');
          uniformsdgnode.setDependency(baseGeometryNode.getAttributesDGNode(), 'parentattributes');
          attributesdgnode.setDependency(baseGeometryNode.getUniformsDGNode(), 'parentuniforms');
          attributesdgnode.setDependency(baseGeometryNode.getAttributesDGNode(), 'parentattributes');
          if(baseGeometryNode.getBoundingBoxDGNode){
            uniformsdgnode.setDependency(baseGeometryNode.getBoundingBoxDGNode(), 'parentboundingbox');
            attributesdgnode.setDependency(baseGeometryNode.getBoundingBoxDGNode(), 'parentboundingbox');
          }
          redrawEventHandler.appendChildEventHandler(baseGeometryNode.getRedrawEventHandler());
        }
      });
    
    //////////////////////////////////////////
    // Persistence
    var parentAddDependencies = geometryDataCopyNode.addDependencies;
    geometryDataCopyNode.addDependencies = function(sceneSerializer) {
      parentAddDependencies(sceneSerializer);
      sceneSerializer.addNode(baseGeometryNode.pub);
    };
    
    var parentWriteData = geometryDataCopyNode.writeData;
    var parentReadData = geometryDataCopyNode.readData;
    geometryDataCopyNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      constructionOptions.createBoundingBoxNode = options.createBoundingBoxNode;
      nodeData.geometryDataCopyNode = geometryDataCopyNode.pub.getName();
    };
    geometryDataCopyNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      geometryDataCopyNode.pub.setBaseGeometryNode(sceneDeserializer.getNode(nodeData.geometryDataCopyNode));
    };
    
    
    if(options.baseGeometryNode){
      geometryDataCopyNode.pub.setBaseGeometryNode(options.baseGeometryNode);
    }
    return geometryDataCopyNode;
  }});


FABRIC.SceneGraph.registerNodeType('Points', {
  briefDesc: 'The Points node defines a renderable points geometry type.',
  detailedDesc: 'The Points node defines a renderable points geometry type. The Points node applies a custom draw operator and rayIntersection operator.',
  parentNodeDesc: 'Geometry',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    
    scene.assignDefaults(options, {
        assignDrawOperator: true
      });
    
    var pointsNode = scene.constructNode('Geometry', options);

    // implement the geometry relevant interfaces
    if(options.assignDrawOperator){
      pointsNode.getRedrawEventHandler().postDescendBindings.append( scene.constructOperator({
        operatorName: 'drawPoints',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawPoints.kl',
        entryFunctionName: 'drawPoints',
        parameterLayout: [
          'shader.shaderProgram',
          'instance.drawToggle'
        ]
      }));
    }

    pointsNode.getRayIntersectionOperator = function(transformNodeMember) {
      return scene.constructOperator({
          operatorName: 'rayIntersectPoints',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/rayIntersectPoints.kl',
          entryFunctionName: 'rayIntersectPoints',
          parameterLayout: [
            'raycastData.ray',
            'raycastData.threshold',
            'instance.drawToggle',
            'instance.raycastOverlaid',
            'transform.' + transformNodeMember,
            'geometry_attributes.positions<>',
            'boundingbox.min',
            'boundingbox.max'
          ]
        });
    };
    
    pointsNode.pub.addVertexAttributeValue('positions', 'Vec3', { genVBO:true } );
    return pointsNode;
  }});



FABRIC.SceneGraph.registerNodeType('Lines', {
  briefDesc: 'The Lines node defines a renderable lines geometry type.',
  detailedDesc: 'The Lines node defines a renderable lines geometry type. The Lines node applies a custom draw operator and rayIntersection operator.',
  parentNodeDesc: 'Geometry',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    
    scene.assignDefaults(options, {
        assignDrawOperator: true
      });
    
    var linesNode = scene.constructNode('Geometry', options);
    
    if(options.assignDrawOperator){
      linesNode.getRedrawEventHandler().postDescendBindings.append( scene.constructOperator({
        operatorName: 'drawLines',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawLines.kl',
        entryFunctionName: 'drawLines',
        parameterLayout: [
          'shader.shaderProgram',
          'self.indicesBuffer',
          'instance.drawToggle'
        ]
      }));
    }

    linesNode.getRayIntersectionOperator = function(transformNodeMember) {
      return scene.constructOperator({
          operatorName: 'rayIntersectLines',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/rayIntersectLines.kl',
          entryFunctionName: 'rayIntersectLines',
          parameterLayout: [
            'raycastData.ray',
            'raycastData.threshold',
            'instance.drawToggle',
            'instance.raycastOverlaid',
            'transform.' + transformNodeMember,
            'geometry_attributes.positions<>',
            'geometry_uniforms.indices',
            'boundingbox.min',
            'boundingbox.max'
          ]
        });
    };

    linesNode.pub.addVertexAttributeValue('positions', 'Vec3', { genVBO:true } );
    linesNode.pub.addUniformValue('indices', 'Integer[]');
    return linesNode;
  }});


FABRIC.SceneGraph.registerNodeType('LineStrip', {
  briefDesc: 'The Lines node defines a renderable lines geometry type.',
  detailedDesc: 'The Lines node defines a renderable lines geometry type. The Lines node applies a custom draw operator and rayIntersection operator.',
  parentNodeDesc: 'Geometry',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    
    scene.assignDefaults(options, {
        assignDrawOperator: true
      });
    
    var linesNode = scene.constructNode('Geometry', options);

    if(options.assignDrawOperator){
      linesNode.getRedrawEventHandler().postDescendBindings.append( scene.constructOperator({
        operatorName: 'drawLineStrip',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawLines.kl',
        entryFunctionName: 'drawLineStrip',
        parameterLayout: [
          'shader.shaderProgram',
          'self.indicesBuffer',
          'instance.drawToggle'
        ]
      }));
    }
    
    linesNode.getRayIntersectionOperator = function(transformNodeMember) {
      return scene.constructOperator({
          operatorName: 'rayIntersectLineStrip',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/rayIntersectLines.kl',
          entryFunctionName: 'rayIntersectLineStrip',
          parameterLayout: [
            'raycastData.ray',
            'raycastData.threshold',
            'instance.drawToggle',
            'instance.raycastOverlaid',
            'transform.' + transformNodeMember,
            'geometry_attributes.positions<>',
            'geometry_uniforms.indices',
            'boundingbox.min',
            'boundingbox.max'
          ]
        });
    };

    linesNode.pub.addVertexAttributeValue('positions', 'Vec3', { genVBO:true } );
    linesNode.pub.addUniformValue('indices', 'Integer[]');
    return linesNode;
  }});




FABRIC.SceneGraph.registerNodeType('Triangles', {
  briefDesc: 'The Triangles node defines a renderable triangles geometry type.',
  detailedDesc: 'The Triangles node defines a renderable triangles geometry type. The Lines node applies a custom draw operator and rayIntersection operator.',
  parentNodeDesc: 'Geometry',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        uvSets: undefined,
        tangentsFromUV: undefined,
        assignDrawOperator: true
      });

    var trianglesNode = scene.constructNode('Geometry', options);
    
    if(options.assignDrawOperator){
      trianglesNode.getRedrawEventHandler().postDescendBindings.append( scene.constructOperator({
        operatorName: 'drawTriangles',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawTriangles.kl',
        entryFunctionName: 'drawTriangles',
        parameterLayout: [
          'shader.shaderProgram',
          'self.indicesBuffer',
          'instance.drawToggle'
        ]
      }));
    }
    
    trianglesNode.getRayIntersectionOperator = function(transformNodeMember) {
      return scene.constructOperator({
          operatorName: 'rayIntersectTriangles',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/rayIntersectTriangles.kl',
          entryFunctionName: 'rayIntersectTriangles',
          parameterLayout: [
            'raycastData.ray',
            'instance.drawToggle',
            'instance.raycastOverlaid',
            'transform.' + transformNodeMember,
            'geometry_attributes.positions<>',
            'geometry_uniforms.indices',
            'boundingbox.min',
            'boundingbox.max'
          ]
        });
    };
    
    var parentWriteData = trianglesNode.writeData;
    var parentReadData = trianglesNode.readData;
    trianglesNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      if (options.uvSets) constructionOptions.uvSets = options.uvSets;
      if (options.tangentsFromUV) constructionOptions.tangentsFromUV = options.tangentsFromUV;
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
    };

    trianglesNode.pub.addUniformValue('indices', 'Integer[]');
    trianglesNode.pub.addVertexAttributeValue('positions', 'Vec3', { genVBO:true } );
    trianglesNode.pub.addVertexAttributeValue('normals', 'Vec3', { genVBO:true } );

    var nbUVs = 0;
    if (typeof options.uvSets === 'number') {
      var nbUVs = parseInt(options.uvSets);
      for (var i = 0; i < nbUVs; i++) {
        trianglesNode.pub.addVertexAttributeValue('uvs' + i, 'Vec2', { genVBO:true } );
      }
      if (typeof options.tangentsFromUV === 'number') {
        var tangentUVIndex = parseInt(options.tangentsFromUV);
        if (tangentUVIndex >= nbUVs)
          throw 'Invalid UV index for tangent space generation';

        trianglesNode.pub.addVertexAttributeValue('tangents', 'Vec4', { genVBO:true } );
        trianglesNode.getAttributesDGNode().bindings.append(scene.constructOperator({
          operatorName: 'computeTriangleTangents',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateTangents.kl',
          entryFunctionName: 'computeTriangleTangents',
          parameterLayout: [
            'uniforms.indices',
            'self.positions<>',
            'self.normals<>',
            'self.uvs' + tangentUVIndex + '<>',
            'self.tangents<>'
          ]
        }));
      }
    }
    return trianglesNode;
  }});


FABRIC.SceneGraph.registerNodeType('Instance', {
  briefDesc: 'The Instance node represents a rendered geometry on screen.',
  detailedDesc: 'The Instance node represents a rendered geometry on screen. The Instance node propagates render events from Materials to Geometries, and also provides facilities for raycasting.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    transformNode: 'Optional. Specify a Transform node to transform the geometry during rendering',
    transformNodeMember: 'Default Value:\'globalXfo\'. Specify the XFo member or member array on the transform node to use as the model matrix in rendering',
    transformNodeIndex: 'Default Value:undefined. Specify the index of the XFo in the member array on the transform node to use as the model matrix in rendering',
    constructDefaultTransformNode: 'Flag specify whether to construct a default transform node if none is provided by the \'transformNode\' option above.',
    geometryNode: 'Optional. Specify a Geometry node to draw during rendering',
    enableRaycasting: 'Flag specify whether this Instance should support raycasting.',
    enableDrawing: 'Flag specify whether this Instance should support drawing.',
    enableShadowCasting: 'Flag specify whether this Instance should support shadow casting.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        transformNode: undefined,
        transformNodeMember: 'globalXfo',
        transformNodeIndex: undefined,
        constructDefaultTransformNode: true,
        geometryNode: undefined,
        enableRaycasting: false,
        enableDrawing: true,
        raycastOverlaid: false
      });
    
    var instanceNode = scene.constructNode('SceneGraphNode', options),
      dgnode = instanceNode.constructDGNode('DGNode'),
      redrawEventHandler = instanceNode.constructEventHandlerNode('Redraw'),
      transformNode,
      transformNodeMember = options.transformNodeMember,
      geometryNode,
      materialNodes = [];

    dgnode.addMember('drawToggle', 'Boolean', options.enableDrawing);
    instanceNode.addMemberInterface(dgnode, 'drawToggle', true);
    
    
    redrawEventHandler.setScope('instance', dgnode);
    
    var preProcessorDefinitions = {
      MODELMATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('modelMatrix'),
      MODELMATRIXINVERSE_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('modelMatrixInverse'),
      VIEWMATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('viewMatrix'),
      CAMERAMATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('cameraMatrix'),
      CAMERAPOS_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('cameraPos'),
      PROJECTIONMATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('projectionMatrix'),
      PROJECTIONMATRIXINV_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('projectionMatrixInv'),
      NORMALMATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('normalMatrix'),
      MODELVIEW_MATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('modelViewMatrix'),
      MODELVIEWPROJECTION_MATRIX_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('modelViewProjectionMatrix')
    };
    redrawEventHandler.preDescendBindings.append(scene.constructOperator({
        operatorName: 'loadProjectionMatrices',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadModelProjectionMatrices.kl',
        entryFunctionName: 'loadProjectionMatrices',
        preProcessorDefinitions: preProcessorDefinitions,
        parameterLayout: [
          'shader.shaderProgram',
          'camera.cameraMat44',
          'camera.projectionMat44'
        ]
      }));

    var assignLoadTransformOperators = function() {
      // check if we have a sliced transform!
      if(options.transformNodeIndex == undefined && transformNode.getDGNode().getCount() > 1) {
        options.transformNodeIndex = 0
      }
      var transformdgnode = transformNode.getDGNode();
      redrawEventHandler.setScope('transform', transformdgnode);
      if(options.transformNodeIndex == undefined){
        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: 'loadModelProjectionMatrices',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadModelProjectionMatrices.kl',
            entryFunctionName: 'loadModelProjectionMatrices',
            preProcessorDefinitions: preProcessorDefinitions,
            parameterLayout: [
              'shader.shaderProgram',
              'transform.' + transformNodeMember,
              'camera.cameraMat44',
              'camera.projectionMat44'
            ]
          }));
      }else{
        redrawEventHandler.addMember('transformNodeIndex', 'Size', options.transformNodeIndex);
        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'loadIndexedModelProjectionMatrices',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadModelProjectionMatrices.kl',
          entryFunctionName: 'loadIndexedModelProjectionMatrices',
          preProcessorDefinitions: preProcessorDefinitions,
          parameterLayout: [
            'shader.shaderProgram',
            'transform.' + transformNodeMember + '<>',
            'self.transformNodeIndex',
            'camera.cameraMat44',
            'camera.projectionMat44'
          ]
        }));
      }
    }
    var rayIntersectionOperatorsAssigned = false;
    var assignRayIntersectionOperators = function() {
      ///////////////////////////////////////////////
      // Ray Cast Event Handling
      if (scene.getSceneRaycastEventHandler() &&
        options.enableRaycasting &&
        geometryNode.getRayIntersectionOperator
      ) {
        dgnode.addMember('raycastOverlaid', 'Boolean', options.raycastOverlaid);
        // check if this is a sliced transform node
        if(transformNode.getRaycastEventHandler) {
          // In some cases, the transform node can provide raycasting services.
          // For example, the bullet Transfrom node is associated with a geometry,
          // and can provide the raycast. 
          scene.getSceneRaycastEventHandler().appendChildEventHandler(transformNode.getRaycastEventHandler());
        } else {
          var raycastOperator = geometryNode.getRayIntersectionOperator(transformNodeMember);
          raycastEventHandler = instanceNode.constructEventHandlerNode('Raycast');
          raycastEventHandler.setScope('geometry_uniforms', geometryNode.getUniformsDGNode());
          raycastEventHandler.setScope('geometry_attributes', geometryNode.getAttributesDGNode());
          raycastEventHandler.setScope('boundingbox', geometryNode.getBoundingBoxDGNode());
          raycastEventHandler.setScope('transform', transformNode.getDGNode());
          raycastEventHandler.setScope('instance', dgnode);
          // The selector will return the node bound with the given binding name.
          raycastEventHandler.setSelector('instance', raycastOperator);
  
          // the sceneRaycastEventHandler propogates the event throughtout the scene.
          scene.getSceneRaycastEventHandler().appendChildEventHandler(raycastEventHandler);
        }
      }
      rayIntersectionOperatorsAssigned = true;
    }


    // extend public interface
    instanceNode.addReferenceInterface('Geometry', 'Geometry',
      function(nodePrivate){
        geometryNode = nodePrivate;
        if(transformNode && !rayIntersectionOperatorsAssigned){
          assignRayIntersectionOperators();
        }
        redrawEventHandler.appendChildEventHandler(geometryNode.getRedrawEventHandler());
      });
    
    instanceNode.addReferenceInterface('Transform', 'Transform',
      function(nodePrivate, member){
        if (member){
          if(!nodePrivate.getDGNode().getMembers()[member]) {
            var message = 'Error in Transform node assignement on :' + nodePrivate.name +
              ' \n member not found :' + member + '\n\n';
            message += ' Members:' + JSON.stringify(nodePrivate.getDGNode().getMembers());
            throw (message);
          }
          transformNodeMember = member;
        }
        var transformNodeAssigned = (transformNode != undefined);
        transformNode = nodePrivate;
        if(!transformNodeAssigned){
          assignLoadTransformOperators();
        }
        if(geometryNode && !rayIntersectionOperatorsAssigned){
          assignRayIntersectionOperators();
        }
      });
    
    instanceNode.pub.getTransformNodeMember = function() {
      return transformNodeMember;
    };
    instanceNode.addReferenceListInterface('Material', 'Material',
      function(nodePrivate, index){
        nodePrivate.getRedrawEventHandler().appendChildEventHandler(redrawEventHandler);
        materialNodes.push(nodePrivate);
      },
      function(nodePrivate, index){
        nodePrivate.getRedrawEventHandler().removeChildEventHandler(redrawEventHandler);
        materialNodes.splice(index, 1);
      });
    
    //////////////////////////////////////////
    // Persistence
    var parentAddDependencies = instanceNode.addDependencies;
    instanceNode.addDependencies = function(sceneSerializer) {
      parentAddDependencies(sceneSerializer);
      sceneSerializer.addNode(transformNode.pub);
      sceneSerializer.addNode(geometryNode.pub);
      for (var i = 0; i < materialNodes.length; i++) {
        sceneSerializer.addNode(materialNodes[i].pub);
      }
    };
    
    var parentWriteData = instanceNode.writeData;
    var parentReadData = instanceNode.readData;
    instanceNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      // the transform node will be loaded with the instance. 
      constructionOptions.constructDefaultTransformNode = false;
      constructionOptions.enableRaycasting = options.enableRaycasting;

      if(sceneSerializer.isNodeBeingSaved(transformNode.pub)){
        nodeData.transformNode = transformNode.pub.getName();
        nodeData.transformNodeMember = transformNodeMember;
      }
      if(sceneSerializer.isNodeBeingSaved(geometryNode.pub)){
        nodeData.geometryNode = geometryNode.pub.getName();
      }
      nodeData.materialNodes = [];
      for (var i = 0; i < materialNodes.length; i++) {
        if(sceneSerializer.isNodeBeingSaved(materialNodes[i].pub)){
          nodeData.materialNodes.push(materialNodes[i].pub.getName());
        }
      }
    };
    instanceNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      
      if (nodeData.transformNode) {
        var transformNode = sceneDeserializer.getNode(nodeData.transformNode);
        if (transformNode) {
          instanceNode.pub.setTransformNode(transformNode, nodeData.transformNodeMember);
        }
      }
      if (nodeData.geometryNode) {
        var geometryNode = sceneDeserializer.getNode(nodeData.geometryNode);
        if (geometryNode) {
          instanceNode.pub.setGeometryNode(geometryNode);
        }
      }
      for (var i in nodeData.materialNodes) {
        if (nodeData.materialNodes.hasOwnProperty(i)) {
          var materialNode = sceneDeserializer.getNode(nodeData.materialNodes[i]);
          if (materialNode) {
            instanceNode.pub.addMaterialNode(materialNode);
          }
        }
      }
    };
    // Mouse events are fired on Instance nodes.
    // These events are generated using raycasting.
    scene.addEventHandlingFunctions(instanceNode);

    if (options.transformNode) {
      instanceNode.pub.setTransformNode(options.transformNode, options.transformNodeMember);
    }
    else if (options.constructDefaultTransformNode) {
      instanceNode.pub.setTransformNode(scene.pub.constructNode('Transform', { hierarchical: false }));
    }
    if (options.geometryNode) {
      instanceNode.pub.setGeometryNode(options.geometryNode);
    }
    if (options.materialNode) {
      instanceNode.pub.addMaterialNode(options.materialNode);
    }
    return instanceNode;
  }
});
