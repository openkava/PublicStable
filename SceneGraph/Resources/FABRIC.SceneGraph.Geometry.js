
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//


FABRIC.SceneGraph.registerNodeType('Geometry',
  function(options, scene) {
    scene.assignDefaults(options, {
        dynamicMembers: [],
        createBoundingBoxNode: true,
        tesselationSupported: false,
        tesselationVertices: 3
      });
    options.dgnodenames.push('UniformsDGNode');
    options.dgnodenames.push('AttributesDGNode');
    if (options.createBoundingBoxNode) {
      options.dgnodenames.push('BoundingBoxDGNode');
    }

    var geometryNode,
      redrawEventHandler,
      uniformsdgnode,
      attributesdgnode,
      bboxdgnode,
      deformationbufferinterfaces = [],
      tesselationSupported = options.tesselationSupported,
      tesselationVertices = options.tesselationVertices;

    var geometryNode = scene.constructNode('SceneGraphNode', options);
    uniformsdgnode = geometryNode.getUniformsDGNode();
    attributesdgnode = geometryNode.getAttributesDGNode();
    if(options.positionsVec4 == true ){
      attributesdgnode.addMember('positions', 'Vec4');
    }else{
      attributesdgnode.addMember('positions', 'Vec3');
    }
    attributesdgnode.addDependency(uniformsdgnode, 'uniforms');

    var uniformsname = uniformsdgnode.getName();
    var attributesname = attributesdgnode.getName();

    if (options.createBoundingBoxNode) {
      bboxdgnode = geometryNode.getBoundingBoxDGNode();
      bboxdgnode.addMember('min', 'Vec3');
      bboxdgnode.addMember('max', 'Vec3');
      bboxdgnode.addDependency(attributesdgnode, 'attributes');
      bboxdgnode.bindings.append(scene.constructOperator({
        operatorName: 'calcBoundingBox',
        srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/calcBoundingBox.kl',
        entryFunctionName: 'calcBoundingBox',
        parameterBinding: [
          'attributes.positions[]',
          'self.min',
          'self.max'
        ]
      }));
    }

    // extend public interface
    geometryNode.pub.addUniformValue = function(name, type, value, addGetterSetterInterface) {
      uniformsdgnode.addMember(name, type, value);

      // Note: I have added this here simply to get the perpoint deformation demo going.
      // We could generate methods like this on the 'SceneGraphNode'. This would simplify
      // loads of code as we could add getters and setters as we add members.
      if (addGetterSetterInterface) {
        geometryNode.addMemberInterface(uniformsdgnode, name, true);
      }
    };
    geometryNode.pub.addVertexAttributeValue = function(name, type, defaultValue, dynamic) {
      attributesdgnode.addMember(name, type, defaultValue);
      if (dynamic === true) {
        options.dynamicMembers.push(name);
      }
    };
    geometryNode.pub.setAttributeDynamic = function(name) {
      dynamicMember = options.dynamicMembers.indexOf(name) != -1;
      if (!dynamicMember)
        options.dynamicMembers.push(name);
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
    geometryNode.pub.getTesselationSupported = function() {
      return tesselationSupported;
    };
    geometryNode.pub.getTesselationVertices= function() {
      return tesselationVertices;
    };
    geometryNode.pub.setTesselationSupported = function(enabled) {
      tesselationSupported = enabled;
    };
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
    geometryNode.pub.loadResourceFiles = function(filepath) {

      var fileArray = filepath.split('.');
      fileArray.pop();
      filepath = fileArray.join();

      var uniformsDatablob = scene.loadResourceURL(filepath + '_uniforms' + '.base64');
      var attributesDatablob = scene.loadResourceURL(filepath + '_attributes' + '.base64');
      if (uniformsDatablob) {
        uniformsdgnode.setBinary(uniformsDatablob);
      }else {
        throw ('File not found:' + filepath);
      }
      if (attributesDatablob) {
        attributesdgnode.setBinary(attributesDatablob);
      }else {
        throw ('File not found:' + filepath);
      }
    };
    geometryNode.pub.writeResourceFiles = function(filepath) {

      var fileArray = filepath.split('.');
      fileArray.pop();
      filepath = fileArray.join();

      scene.writeResourceFile(filepath + '_uniforms' + '.base64', uniformsdgnode.getBinary());
      scene.writeResourceFile(filepath + '_attributes' + '.base64', attributesdgnode.getBinary());
    };

    // extend private interface
    // ---------
    // Here we are ensuring that the generator ops are added to the
    // beginning of the operator list. if there are operators for
    // generating tangents, or any other data, they should always go after
    // the generator ops.
    // Currently the generator ops are bound to the attributesdgnode,
    // but with node nesting, we should put them on the outer node.
    // this would mean 2 trhings. We could generate geometry in a
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
    geometryNode.checkVBORequirements = function(vboRequirements) {
      var vertexAttributes = attributesdgnode.getMembers(),
        attributeName,
        message;
      for (attributeName in vboRequirements) {
        if (!vertexAttributes[attributeName]) {
          message = 'Geometry: ' + this.name + ' does not meet shader requirements.\n';
          message += 'Shader requires :' + JSON.stringify(vboRequirements) + '\n';
          message += 'But geometry does not support attribute:' + JSON.stringify(attributeName) + '\n';

          message += 'Geometry supports :\n';
          for (attributeName in vertexAttributes) {
            message += '\t\t' + attributeName + ' : ' + vertexAttributes[attributeName].type + '\n';
          }
          throw (message);
        }
      }
      return true;
    };
    geometryNode.getRedrawEventHandler = function() {
      if (redrawEventHandler) {
        return redrawEventHandler;
      }
      var vertexAttributes,
        uniformValues,
        memberName,
        memberType,
        bufferIDMemberName,
        countMemberName,
        reloadMemberName,
        dynamicMember = false,
        attributeNodeBinding,
        i;

      redrawEventHandler = scene.constructEventHandlerNode(options.name + '_GeometryDraw');
      redrawEventHandler.addScope('uniforms', uniformsdgnode);
      redrawEventHandler.addScope('attributes', attributesdgnode);
      for (i = 0; i < deformationbufferinterfaces.length; i++) {
        redrawEventHandler.addScope('attributes' + (i + 1), deformationbufferinterfaces[i].getDGNode());
      }

      if (uniformsdgnode.getMembers().indices) {
        redrawEventHandler.addMember('indicesBufferID', 'Integer', 0);
        redrawEventHandler.addMember('indicesCount', 'Size', 0);

        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'genAndLoadIndicesVBOs',
          srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/loadVBOs.kl',
          entryFunctionName: 'genAndLoadIndicesVBOs',
          parameterBinding: [
            'uniforms.indices',
            'self.indicesCount',
            'self.indicesBufferID'
          ]
        }));
      }

      vertexAttributes = attributesdgnode.getMembers();
      uniformValues = uniformsdgnode.getMembers();
      for (memberName in vertexAttributes) {
        if (!FABRIC.shaderAttributeTable[memberName]) {
          continue;
        }
        memberType = vertexAttributes[memberName].type;
        bufferIDMemberName = memberName + 'BufferID';
        countMemberName = memberName + 'Count';
        
        redrawEventHandler.addMember(countMemberName, 'Size', 0);
        
        if(uniformValues[bufferIDMemberName] && uniformValues[countMemberName]){
          redrawEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: 'bindVBO',
            srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/genAndLoadVBO.kl',
            preProcessorDefinitions: {
              DATA_TYPE: memberType,
              ATTRIBUTE_NAME: memberName,
              ATTRIBUTE_ID: FABRIC.shaderAttributeTable[memberName].id
            },
            entryFunctionName: 'bindVBO',
            parameterBinding: [
              'shader.attributeValues',
              'uniforms.' + bufferIDMemberName,
              'uniforms.' + countMemberName,
              'self.' + countMemberName
            ]
          }));
          continue;
        }
        
        redrawEventHandler.addMember(bufferIDMemberName, 'Integer', 0);
        
        dynamicMember = options.dynamicMembers.indexOf(memberName) != -1;
        attributeNodeBinding = 'attributes';
        for (i = 0; i < deformationbufferinterfaces.length; i++) {
          if (deformationbufferinterfaces[i].getDGNode().getMembers()[memberName]) {
            attributeNodeBinding = 'attributes' + (i + 1);
            dynamicMember = true;
          }
        }
        
        /*
        if(dynamicMember){
          redrawEventHandler.preDescendBindings.append(scene.constructOperator({
              operatorName:"genAndLoadDynamicVBO"+memberType+FABRIC.shaderAttributeTable[memberName].id,
              srcFile:"../../../SceneGraph/Resources/KL/genAndLoadVBO.kl",
              preProcessorDefinitions:{
                DATA_TYPE:memberType,
                ATTRIBUTE_NAME:memberName,
                ATTRIBUTE_ID:FABRIC.shaderAttributeTable[memberName].id
              },
              entryFunctionName:"genAndLoadDynamicVBO",
              parameterBinding:[
                "shader.attributeValues",
                attributeNodeBinding+"."+ memberName+"[]",
                "self."+countMemberName,
                "self."+bufferIDMemberName
              ]
            }));
        }
        else{ */
          reloadMemberName = memberName + 'Reload';
          dynamicMemberName = memberName + 'Dynamic';
          redrawEventHandler.addMember(reloadMemberName, 'Boolean', false);
          redrawEventHandler.addMember(dynamicMemberName, 'Boolean', dynamicMember);

          redrawEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: 'genAndLoad' + memberName + 'VBO',
            srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/genAndLoadVBO.kl',
            preProcessorDefinitions: {
              DATA_TYPE: memberType,
              ATTRIBUTE_NAME: memberName,
              ATTRIBUTE_ID: FABRIC.shaderAttributeTable[memberName].id
            },
            entryFunctionName: 'genAndLoadVBO',
            parameterBinding: [
              'shader.attributeValues',
              attributeNodeBinding + '.' + memberName + '[]',
              'self.' + countMemberName,
              'self.' + dynamicMemberName,
              'self.' + reloadMemberName,
              'self.' + bufferIDMemberName
            ]
          }));
        //}
      }
      
      
      redrawEventHandler.postDescendBindings.append(this.getDrawOperator());

      return redrawEventHandler;
    };
    geometryNode.pub.reloadVBO = function(memberName) {
      redrawEventHandler.setData(memberName + 'Reload', true);
    };
    geometryNode.getDrawOperator = function() {
      throw ('Geometry must define this');
    };
    geometryNode.getRayintersectionOperator = function() {
      throw ('Geometry must define this');
    };
    // This method creates a new dgnode that enables multi-threaded
    // execution on a copy of the data from the parentBuffer.
    // This is a very simple example on how to create an operator stack
    // similar to those found in traditional DCC applications. Each
    // buffer creates a cache of results, meaning that it is possible
    // to separate out operators for geometry generation and animation.
    geometryNode.pub.addDeformationBuffer = function(deformableAttributes) {
      var parentbufferdgnode,
        bufferdgnode = scene.constructDependencyGraphNode(
          options.name + '_attributes' + (deformationbufferinterfaces.length + 1)),
        bufferInterface;

      if (deformationbufferinterfaces.length == 0) {
        parentbufferdgnode = attributesdgnode;
      }
      else {
        parentbufferdgnode = deformationbufferinterfaces[deformationbufferinterfaces.length - 1].getDGNode();
      }
      bufferdgnode.addDependency(parentbufferdgnode, 'parentattributes');
      bufferdgnode.addDependency(uniformsdgnode, 'uniforms');

      bufferdgnode.bindings.append(scene.constructOperator({
        operatorName: 'matchCount',
        srcCode: 'operator matchCount(Size parentCount, io Size selfCount) {\n' +
            '  selfCount = parentCount;\n' +
            '}',
        entryFunctionName: 'matchCount',
        parameterBinding: [
          'parentattributes.count',
          'self.newCount'
        ]
      }));

      bufferInterface = {
        getDGNode: function() {
          return bufferdgnode;
        },
        propagateMember: function(attributeName) {
          var attributeType = parentbufferdgnode.getMembers()[attributeName].type;
          bufferdgnode.addMember(attributeName, attributeType);
          bufferdgnode.bindings.append(scene.constructOperator({
            operatorName: 'copyAttribute',
            srcCode: 'operator copyAttribute(io ' + attributeType + ' elements[], ' +
                'io ' + attributeType + ' value, in Size index) {\n' +
                '  value = elements[ index ];\n' +
                '}',
            entryFunctionName: 'copyAttribute',
            parameterBinding: [
              'parentattributes.' + attributeName + '[]',
              'self.' + attributeName,
              'self.index'
            ]
          }));
          if (bboxdgnode && attributeName == 'positions') {
            bboxdgnode.addDependency(bufferdgnode, 'attributes');
          }
        },
        pub: {
          addDependency: function( dgnode, dependencyName){
            bufferdgnode.addDependency(dgnode, dependencyName);
          },
          assignOperator: function(operatorDef) {
            bufferdgnode.bindings.append(scene.constructOperator(operatorDef));
          }
        }
      };
      if(deformableAttributes){
        for (i = 0; i < deformableAttributes.length; i++) {
          bufferInterface.propagateMember(deformableAttributes[i]);
        }
      }
      deformationbufferinterfaces.push(bufferInterface);
      return bufferInterface.pub;
    };

    return geometryNode;
  });

FABRIC.SceneGraph.registerNodeType('Points',
  function(options, scene) {

    var pointsNode = scene.constructNode('Geometry', options);

    // implement the geometry relevant interfaces
    pointsNode.getDrawOperator = function() {
      return scene.constructOperator({
          operatorName: 'drawPoints',
          srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/drawPoints.kl',
          entryFunctionName: 'drawPoints',
          parameterBinding: [
            'self.positionsCount',
            'instance.drawToggle'
          ]
        });
    };
    pointsNode.getRayintersectionOperator = function(transformNodeMember) {
      return scene.constructOperator({
          operatorName: 'rayIntersectPoints',
          srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/rayIntersectPoints.kl',
          entryFunctionName: 'rayIntersectPoints',
          parameterBinding: [
            'raycastData.ray',
            'raycastData.threshold',
            'transform.' + transformNodeMember,
            'geometry_attributes.positions[]',
            'boundingbox.min',
            'boundingbox.max'
          ]
        });
    };

    return pointsNode;
  });



FABRIC.SceneGraph.registerNodeType('Lines',
  function(options, scene) {

    var linesNode = scene.constructNode('Geometry', options);

    // implement the geometry relevant interfaces
    linesNode.getDrawOperator = function() {
      return scene.constructOperator({
          operatorName: 'drawLines',
          srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/drawLines.kl',
          parameterBinding: [
            'self.indicesCount',
            'self.indicesBufferID',
            'instance.drawToggle'
          ],
          entryFunctionName: 'drawLines'
        });
    };
    linesNode.getRayintersectionOperator = function(transformNodeMember) {
      return scene.constructOperator({
          operatorName: 'rayIntersectLines',
          srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/rayIntersectLines.kl',
          entryFunctionName: 'rayIntersectLines',
          parameterBinding: [
            'raycastData.ray',
            'raycastData.threshold',
            'transform.' + transformNodeMember,
            'geometry_attributes.positions[]',
            'geometry_uniforms.indices',
            'boundingbox.min',
            'boundingbox.max'
          ]
        });
    };

    linesNode.pub.addUniformValue('indices', 'Integer[]');
    linesNode.pub.addUniformValue('thickness', 'Scalar', 3.0);
    return linesNode;
  });




FABRIC.SceneGraph.registerNodeType('Triangles',
  function(options, scene) {
    scene.assignDefaults(options, {
        uvSets: undefined,
        tangentsFromUV: undefined
      });

    var trianglesNode = scene.constructNode('Geometry', options);

    // implement the geometry relevant interfaces
    trianglesNode.getDrawOperator = function() {
      if(trianglesNode.pub.getTesselationSupported()) {
        return scene.constructOperator({
            operatorName: 'drawPatches',
            srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/drawPatches.kl',
            parameterBinding: [
              'instance.elementCount',
              'instance.indicesBufferID',
              'instance.drawToggle'
            ],
            entryFunctionName: 'drawPatches'
          });
      } else {
        return scene.constructOperator({
            operatorName: 'drawTriangles',
            srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/drawTriangles.kl',
            parameterBinding: [
              'instance.elementCount',
              'instance.indicesBufferID',
              'instance.drawToggle'
            ],
            entryFunctionName: 'drawTriangles'
          });
      }
    };
    trianglesNode.getRayintersectionOperator = function(transformNodeMember) {
      return scene.constructOperator({
          operatorName: 'rayIntersectTriangles',
          srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/rayIntersectTriangles.kl',
          entryFunctionName: 'rayIntersectTriangles',
          parameterBinding: [
            'raycastData.ray',
            'transform.' + transformNodeMember,
            'geometry_attributes.positions[]',
            'geometry_uniforms.indices',
            'boundingbox.min',
            'boundingbox.max'
          ]
        });
    };
    trianglesNode.writeData = function(sceneSaver, constructionOptions, nodeData) {
      if (options.uvSets) constructionOptions.uvSets = options.uvSets;
      if (options.tangentsFromUV) constructionOptions.tangentsFromUV = options.tangentsFromUV;
      nodeData.data = this.getAttributesDGNode().getBinary();
    };
    trianglesNode.readData = function(sceneLoader, nodeData) {
      if (nodeData.data) {
        this.getAttributesDGNode().setBinary(nodeData.data);
      }
    };

    trianglesNode.pub.addUniformValue('indices', 'Integer[]');
    trianglesNode.pub.addVertexAttributeValue('normals', 'Vec3');

    var nbUVs = 0;

    if (typeof options.uvSets === 'number') {
      var nbUVs = parseInt(options.uvSets);
      for (var i = 0; i < nbUVs; i++) {
        trianglesNode.pub.addVertexAttributeValue('uvs' + i, 'Vec2');
      }
      if (typeof options.tangentsFromUV === 'number') {
        var tangentUVIndex = parseInt(options.tangentsFromUV);
        if (tangentUVIndex >= nbUVs)
          throw 'Invalid UV index for tangent space generation';

        trianglesNode.pub.addVertexAttributeValue('tangents', 'Vec4');
        trianglesNode.getAttributesDGNode().bindings.append(scene.constructOperator({
          operatorName: 'computeTriangleTangents',
          srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/generateTangents.kl',
          entryFunctionName: 'computeTriangleTangents',
          parameterBinding: [
            'uniforms.indices',
            'self.positions[]',
            'self.normals[]',
            'self.uvs' + tangentUVIndex + '[]',
            'self.tangents[]'
          ]
        }));
      }
    }
    return trianglesNode;
  });


FABRIC.SceneGraph.registerNodeType('Instance',
  function(options, scene) {
    scene.assignDefaults(options, {
        transformNode: undefined,
        transformNodeMember: 'globalXfo',
        transformNodeIndex: undefined,
        geometryNode: undefined,
        enableRaycasting: false,
        enableDrawing: true,
        enableShadowCasting: false,
        constructDefaultTransformNode: true
      });
    // TODO: once the 'selector' system can be replaced with JavaScript event
    // generation from KL, then we can eliminate this dgnode. It currently serves
    // no other purpose. 
    options.dgnodenames.push('DGNode');
    options.ehnodenames.push('RedrawEventHandler');
    var instanceNode = scene.constructNode('SceneGraphNode', options),
      dgnode = instanceNode.getDGNode(),
      redrawEventHandler = instanceNode.getRedrawEventHandler(),
      transformNode,
      transformNodeMember = options.transformNodeMember,
      geometryNode,
      materialNodes = [];

    dgnode.addMember('drawToggle', 'Boolean', options.enableDrawing);
    // TODO: once event handler data can be bound for child events, then
    // we should move these members onto the event handler.
    dgnode.addMember('elementCount', 'Size', 0);
    dgnode.addMember('indicesBufferID', 'Integer', 0);
    
    redrawEventHandler.addScope('instance', dgnode);

    var bindToSceneGraph = function() {
      redrawEventHandler.addScope('transform', transformNode.getDGNode());
      var preProcessorDefinitions = {
              MODELMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable.modelMatrix.id,
              VIEWMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable.viewMatrix.id,
              PROJECTIONMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable.projectionMatrix.id,
              PROJECTIONMATRIXINV_ATTRIBUTE_ID: FABRIC.shaderAttributeTable.projectionMatrixInv.id,
              NORMALMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable.normalMatrix.id,
              MODELVIEW_MATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable.modelViewMatrix.id,
              MODELVIEWPROJECTION_MATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable.modelViewProjectionMatrix.id
            };
      if(!options.transformNodeIndex){
        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: 'loadModelProjectionMatrices',
            srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/loadModelProjectionMatrices.kl',
            entryFunctionName: 'loadModelProjectionMatrices',
            preProcessorDefinitions: preProcessorDefinitions,
            parameterBinding: [
              'shader.uniformValues',
              'transform.' + transformNodeMember,
              'camera.cameraMat44',
              'camera.projectionMat44'
            ]
          }));
      }else{
        redrawEventHandler.addMember('transformNodeIndex', 'Size', options.transformNodeIndex);
        redrawEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: 'loadIndexedModelProjectionMatrices',
            srcFile: 'FABRIC_ROOT/SceneGraph/Resources/KL/loadModelProjectionMatrices.kl',
            entryFunctionName: 'loadIndexedModelProjectionMatrices',
            preProcessorDefinitions: preProcessorDefinitions,
            parameterBinding: [
              'shader.uniformValues',
              'transform.' + transformNodeMember + '[]',
              'self.transformNodeIndex',
              'camera.cameraMat44',
              'camera.projectionMat44'
            ]
          }));
      }
      ///////////////////////////////////////////////
      // Ray Cast Event Handling
      if (scene.getSceneRaycastEventHandler() &&
        options.enableRaycasting &&
        geometryNode.getRayintersectionOperator
      ) {
        var raycastOperator = geometryNode.getRayintersectionOperator(transformNodeMember);
        raycastEventHandler = scene.constructEventHandlerNode(options.name + '_raycast');
        raycastEventHandler.addScope('geometry_uniforms', geometryNode.getUniformsDGNode());
        raycastEventHandler.addScope('geometry_attributes', geometryNode.getAttributesDGNode());
        raycastEventHandler.addScope('boundingbox', geometryNode.getBoundingBoxDGNode());
        raycastEventHandler.addScope('transform', transformNode.getDGNode());
        raycastEventHandler.addScope('instance', dgnode);
        // The selector will return the node bound with the given binding name.
        raycastEventHandler.setSelector('instance', raycastOperator);

        // the sceneRaycastEventHandler propogates the event throughtout the scene.
        scene.getSceneRaycastEventHandler().appendChildEventHandler(raycastEventHandler);
      }
    }

    // extend private interface
    instanceNode.getRedrawEventHandler = function() {
      return redrawEventHandler;
    };
    instanceNode.writeData = function(sceneSaver, constructionOptions, nodeData) {
      constructionOptions.enableRaycasting = options.enableRaycasting;

      nodeData.transformNode = transformNode.name;
      nodeData.transformNodeMember = transformNodeMember;

      nodeData.geometryNode = geometryNode.name;
      nodeData.materialNodes = [];
      for (var i = 0; i < materialNodes.length; i++) {
        nodeData.materialNodes.push(materialNodes[i].name);
      }
    };
    instanceNode.readData = function(sceneLoader, nodeData) {
      if (nodeData.transformNode) {
        var transformNode = sceneLoader.getNode(nodeData.transformNode);
        if (transformNode) {
          this.setTransformNode(transformNode, nodeData.transformNodeMember);
        }
      }
      if (nodeData.geometryNode) {
        var geometryNode = sceneLoader.getNode(nodeData.geometryNode);
        this.setGeometryNode(geometryNode);
      }
      for (i in nodeData.materialNodes) {
        if (nodeData.materialNodes.hasOwnProperty(i)) {
          this.setMaterialNode(sceneLoader.getNode(nodeData.materialNodes[i]));
        }
      }
    };

    // extend public interface
    instanceNode.pub.getGeometryNode = function() {
      return geometryNode;
    };
    instanceNode.pub.setGeometryNode = function(node) {
      if (!node.isTypeOf('Geometry')) {
        throw ('Incorrect type assignment. Must assign a Geometry');
      }
      node = scene.getPrivateInterface(node);
      for (var i = 0; i < materialNodes.length; i++) {
        node.checkVBORequirements(materialNodes[i].getVBORequirements());
      }
      geometryNode = node;
      
      redrawEventHandler.appendChildEventHandler(geometryNode.getRedrawEventHandler());
      if (transformNode) {
        bindToSceneGraph();
      }
    };
    instanceNode.pub.getTransformNode = function() {
      return transformNode.pub;
    };
    instanceNode.pub.setTransformNode = function(node, member) {
      if (member) {
        transformNodeMember = member;
      }
      node = scene.getPrivateInterface(node);
      if (!(node.getDGNode() && node.getDGNode().getMembers()[transformNodeMember])) {
        var message = 'Error in Transform node assignement on :' + node.name +
          ' \n member not found :' + transformNodeMember + '\n\n';
        message += 'Members:' + JSON.stringify(node.getDGNode().getMembers());
        throw (message);
      }
      transformNode = node;
      if (geometryNode) {
        bindToSceneGraph();
      }
    };
    instanceNode.pub.getTransformNodeMember = function() {
      return transformNodeMember;
    };
    instanceNode.pub.getMaterialNode = function(index) {
      return scene.getPublicInterface(materialNodes[index ? index : 0]);
    };
    instanceNode.pub.setMaterialNode = function(node) {
      if (!node.isTypeOf('Material')) {
        throw (':Incorrect type assignment. Must assign a Material');
      }
      if (!geometryNode) {
        throw (':Geometry node not assigned. Geometry must be assigned before setting materials.');
      }
      node = scene.getPrivateInterface(node);
      geometryNode.checkVBORequirements(node.getVBORequirements());
      node.getRedrawEventHandler().appendChildEventHandler(redrawEventHandler);
      materialNodes.push(node);
    };
    instanceNode.pub.removeMaterialNode = function(node) {
      node = scene.getPrivateInterface(node);
      var index = materialNodes.indexOf(node);
      if (index === -1) {
        throw (':Material not assigned');
      }
      materialNodes[index].getRedrawEventHandler().removeChildEventHandler(redrawEventHandler);
      materialNodes.splice(index, 1);
    };

    // custom getter and setter for castShadows
    instanceNode.pub.getIsShadowCasting = function() {
      return materialNodes.indexOf(scene.getShadowMapMaterial()) != -1;
    };
    instanceNode.pub.getIsShadowCasting = function(val) {
      if (val) {
        if (options.shadowMappingMaterial) {
          instanceNode.pub.setMaterialNode(scene.pub.constructNode(options.shadowMappingMaterial));
        }
        else {
          instanceNode.pub.setMaterialNode(scene.getShadowMapMaterial());
        }
      }
      else {
        instanceNode.pub.removeMaterialNode(scene.getShadowMapMaterial());
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
      instanceNode.pub.setMaterialNode(options.materialNode);
    }
    if (options.enableShadowCasting) {
      instanceNode.pub.castShadows = true;
    }
    return instanceNode;
  });

