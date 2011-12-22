
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.SceneGraph.registerNodeType('LoadObj', {
  briefDesc: 'The ObjLoadTriangles node is a resource load node able to load OBJ files.',
  detailedDesc: 'The ObjLoadTriangles node is a resource load node able to load OBJ files. It utilizes a C++ based extension to load OBJ files in a very fast fashion.',
  parentNodeDesc: 'ResourceLoad',
  optionsDesc: {
    removeParsersOnLoad: 'If set to true, the parser operator will be removed after parsing.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      splitObjects: false,
      splitMaterials: false,
      splitGroups: false,
      printDetailedInfo: false
    });
    var resourceLoadNode = scene.constructNode('ResourceLoad', options ),
    resourceloaddgnode = resourceLoadNode.getDGLoadNode();
    
    resourceloaddgnode.addMember('reload', 'Boolean', true);
    resourceloaddgnode.addMember('handle', 'OBJDataHandle');
  
    resourceloaddgnode.addMember('splitObjects', 'Boolean', options.splitObjects);
    resourceloaddgnode.addMember('splitGroups', 'Boolean', options.splitMaterials);
    resourceloaddgnode.addMember('splitMaterials', 'Boolean', options.splitGroups);
    resourceloaddgnode.addMember('printDetailedInfo', 'Boolean', options.printDetailedInfo);
    
    resourceloaddgnode.addMember('objectNames', 'String[]');
    resourceloaddgnode.addMember('groupNames', 'String[]');
    resourceloaddgnode.addMember('materialNames', 'String[]');
    
    resourceloaddgnode.bindings.append(scene.constructOperator({
        operatorName: 'parseObj',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadObj.kl',
        entryFunctionName: 'parseObj',
        parameterLayout: [
          'self.resource',
          'self.handle',
          'self.splitObjects',
          'self.splitGroups',
          'self.splitMaterials',
          'self.reload',
          'self.printDetailedInfo',
          
          'self.objectNames',
          'self.groupNames',
          'self.materialNames'
        ]
      }));
    
    resourceLoadNode.pub.addOnLoadSuccessCallback(function(){
      try{
        resourceloaddgnode.evaluate();
      }catch(e){
        console.warn(e);
      }
      resourceLoadNode.pub.fireEvent('objloadsuccess', {
        objectNames: resourceloaddgnode.getData('objectNames'),
        groupNames: resourceloaddgnode.getData('groupNames'),
        materialNames: resourceloaddgnode.getData('materialNames')
      });
    });
    
    
    var parentWriteData = resourceLoadNode.writeData;
    resourceLoadNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      constructionOptions.splitObjects = options.splitObjects;
      constructionOptions.splitMaterials = options.splitMaterials;
      constructionOptions.splitGroups = options.splitGroups;
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
    };
    
    return resourceLoadNode;
  }
});

FABRIC.SceneGraph.registerNodeType('ObjTriangles', {
  briefDesc: 'The ObjLoadTriangles node is a resource load node able to load OBJ files.',
  detailedDesc: 'The ObjLoadTriangles node is a resource load node able to load OBJ files. It utilizes a C++ based extension to load OBJ files in a very fast fashion.',
  parentNodeDesc: 'ResourceLoad',
  optionsDesc: {
    removeParsersOnLoad: 'If set to true, the parser operator will be removed after parsing.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      resourceLoadNode: undefined,
      removeParsersOnLoad: false,
      entityIndex: -1
    });
    

    options.uvSets = 1; //To refine... what if there is no UV set??
    var trianglesNode = scene.constructNode('Triangles', options);

    trianglesNode.pub.addUniformValue('entityIndex', 'Integer', options.entityIndex);

    trianglesNode.setGeneratorOps([
                                   
      scene.constructOperator({
        operatorName: 'setVertexCount',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadObj.kl',
        entryFunctionName: 'setVertexCount',
        parameterLayout: [
          'resource.handle',
          'uniforms.entityIndex',
          'resource.reload',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'setObjGeom',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadObj.kl',
        entryFunctionName: 'setObjGeom',
        parameterLayout: [
          'resource.handle',
          'uniforms.entityIndex',
          'uniforms.indices',
          'self.positions<>',
          'self.normals<>',
          'self.uvs0<>',
          'resource.reload'
        ]
      })
    ]);
    
    var resourceLoadNode;
    
    trianglesNode.pub.getResourceLoadNode = function() {
      return resourceLoadNode;
    };
    trianglesNode.pub.setResourceLoadNode = function(node) {
      if (!node || !node.isTypeOf('LoadObj')) {
        throw ('Must pass in a LoadObj resource loader');
      }

      resourceLoadNode = scene.getPrivateInterface(node);
      trianglesNode.getAttributesDGNode().setDependency( resourceLoadNode.getDGLoadNode(), 'resource');
    //  trianglesNode.getUniformsDGNode().setDependency( resourceLoadNode.getDGLoadNode(), 'resource');
    }
    
    
    //////////////////////////////////////////
    // Persistence
    var parentAddDependencies = trianglesNode.addDependencies;
    trianglesNode.addDependencies = function(sceneSerializer) {
      parentAddDependencies(sceneSerializer);
      if(sceneSerializer.getTypeRemapping(trianglesNode.pub.getType()) == trianglesNode.pub.getType()){
        sceneSerializer.addNode(resourceLoadNode.pub);
      }
    };
    
    var parentWriteData = trianglesNode.writeData;
    var parentReadData = trianglesNode.readData;
    trianglesNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      if(sceneSerializer.getTypeRemapping(trianglesNode.pub.getType()) == trianglesNode.pub.getType()){
        constructionOptions.entityIndex = options.entityIndex;
        nodeData.resourceLoadNode = resourceLoadNode.pub.getName();
      }
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
    };
    trianglesNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      trianglesNode.pub.setResourceLoadNode(sceneDeserializer.getNode(nodeData.resourceLoadNode));
    };
    
    /////////////////////////////////////////////////////////////////////

    if(options.resourceLoadNode){
      trianglesNode.pub.setResourceLoadNode(options.resourceLoadNode);
    }
    
    return trianglesNode;
  }
});


FABRIC.SceneGraph.registerParser('obj', function(scene, assetUrl, options) {
  options.url = assetUrl;
  var resourceLoadNode = scene.constructNode('LoadObj', options );
  
  resourceLoadNode.addEventListener('objloadsuccess', function(evt){
    var loadedGeometries = {};
    if(evt.objectNames.length > 0){
      for(var i=0; i<evt.objectNames.length; i++){
        var objectName = evt.objectNames[i].length > 0 ? evt.objectNames[i] : (evt.groupNames[i].length > 0 ? evt.groupNames[i] : options.baseName);
        loadedGeometries[objectName] = scene.constructNode('ObjTriangles', {
          resourceLoadNode: resourceLoadNode,
          entityIndex: i,
          name: objectName
        });
        evt.objectNames[i] = objectName;
      }
    }else{
      evt.objectNames = [options.baseName];
      loadedGeometries[options.baseName] = scene.constructNode('ObjTriangles', {
        resourceLoadNode: resourceLoadNode,
        entityIndex: -1,
        name: options.baseName
      })
    }
    resourceLoadNode.fireEvent('objparsesuccess', {
      loadedGeometries: loadedGeometries,
      objectNames: evt.objectNames,
      materialNames: evt.materialNames
    })
  });
  return resourceLoadNode;
});

