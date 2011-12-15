
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
    resourceloaddgnode.addMember('handle', 'Data');
  
    resourceloaddgnode.addMember('splitObjects', 'Boolean', options.splitObjects);
    resourceloaddgnode.addMember('splitGroups', 'Boolean', options.splitMaterials);
    resourceloaddgnode.addMember('splitMaterials', 'Boolean', options.splitGroups);
    resourceloaddgnode.addMember('printDetailedInfo', 'Boolean', options.printDetailedInfo);
    
    resourceloaddgnode.addMember('objectNames', 'String[]');
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
        materialNames: resourceloaddgnode.getData('materialNames')
      });
    });

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
      removeParsersOnLoad: false,
      entityIndex: -1
    });

    options.uvSets = 1; //To refine... what if there is no UV set??
    var trianglesNode = scene.constructNode('Triangles', options);
    var resourceLoadNode = scene.getPrivateInterface(options.resourceLoadNode);

    trianglesNode.getAttributesDGNode().setDependency( resourceLoadNode.getDGLoadNode(), 'resource');
    trianglesNode.getUniformsDGNode().setDependency( resourceLoadNode.getDGLoadNode(), 'resource');

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
    /////////////////////////////////////////////////////////////////////

    trianglesNode.pub.getResourceLoadNode = function() {
      return resourceLoadNode;
    };
    
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
        loadedGeometries[evt.objectNames[i]] = scene.constructNode('ObjTriangles', {
          resourceLoadNode: resourceLoadNode,
          entityIndex: i,
          name: evt.objectNames[i]
        });
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

