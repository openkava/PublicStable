
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//
FABRIC.SceneGraph.registerParser('fez', function(scene, assetUrl, options) {
  options.url = assetUrl;
  return scene.constructNode('LoadBinaryDataNode', options);
});



FABRIC.SceneGraph.registerNodeType('LoadBinaryDataNode', {
  briefDesc: 'The SecureStorageNode node is a ResourceLoad node able to load or save Fabric Engine Secure files.',
  detailedDesc: 'The SecureStorageNode node is a ResourceLoad node able to load or save Fabric Engine Secure. It utilizes a C++ based extension as well as zlib.',
  parentNodeDesc: 'ResourceLoad',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      secureKey: undefined
    });

    var loadBinaryDataNode = scene.constructNode('ResourceLoad', options),
      resourceloaddgnode = loadBinaryDataNode.getDGLoadNode();
    
    loadBinaryDataNode.pub.getResourceFromFile = resourceloaddgnode.getResourceFromFile;

    resourceloaddgnode.addMember('container', 'SecureContainer');
    resourceloaddgnode.addMember('elements', 'SecureElement[]');
    resourceloaddgnode.addMember('secureKey', 'String', options.secureKey);
    resourceloaddgnode.addMember('compressionLevel', 'Integer', options.compressionLevel);
    
    resourceloaddgnode.bindings.append(scene.constructOperator({
      operatorName: 'secureContainerLoad',
      parameterLayout: [
        'self.url',
        'self.resource',
        'self.secureKey',
        'self.container',
        'self.elements'
      ],
      entryFunctionName: 'secureContainerLoad',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadSecure.kl',
      async: false,
      mainThreadOnly: true
    }));

    var elements;
    loadBinaryDataNode.pub.addOnLoadSuccessCallback(function(pub) {
      // define the new nodes based on the identifiers in the file
      resourceloaddgnode.evaluate();
      elements = resourceloaddgnode.getData('elements',0);
    });


    loadBinaryDataNode.pub.loadDGNodes = function(dgnodes) {
      
      var bounds = {};
      var resizeOps = {};
      
      // first, parse for slicecounts
      for(var i=0;i<elements.length;i++) {
        var tokens = elements[i].name.split('.');
        var dgnodeName = tokens[0];
        if(!dgnodes[dgnodeName])
        {
          console.log('Warning: SecureStorage: Unknown DGNode: '+dgnodeName);
          continue;
        }
        if(!bounds[dgnodeName])
        {
          bounds[dgnodeName] = {};
          bounds[dgnodeName].slices = elements[i].slicecount
        }
        if(!bounds[dgnodeName][tokens[1]])
          bounds[dgnodeName][tokens[1]] = {};
        if(bounds[dgnodeName][tokens[1]].first == undefined)
          bounds[dgnodeName][tokens[1]].first = i;
        bounds[dgnodeName][tokens[1]].last = i;
      }
      
      for(var i=0;i<elements.length;i++) {
        var tokens = elements[i].name.split('.');
        var dgnodeName = tokens[0];
        if(!dgnodes[dgnodeName])
          continue;
        
        // check if we already did this
        if(bounds[dgnodeName][tokens[1]].done)
          continue;
        
        var dgnode = dgnodes[dgnodeName];
        
        // check if the member exists
        var dgnodeMembers = dgnode.getMembers();
        if(!dgnodeMembers[tokens[1]])
        {
          console.log("SecureStorage: Warning: Adding missing member '"+tokens[1]+"'.")
          dgnode.addMember(tokens[1],elements[i].type);
          dgnodeMembers[tokens[1]] = {};
          dgnodeMembers[tokens[1]].type = elements[i].type;
        }
        
        // check if the member has the right type
        var originalType = dgnodeMembers[tokens[1]].type;
        var type = elements[i].type;
        var isArray = type.indexOf('[]') > -1;
        var isString = type.indexOf('String') > -1;
        var storedType = originalType.replace('Size','Integer');
        if(storedType != type) {
          console.log('SecureStorage: Warning: Member '+tokens[1]+' has wrong type: '+originalType);
          continue;
        }

        dgnode.setDependency(resourceloaddgnode,'secureStorage');
        resourceloaddgnode.addMember(dgnodeName+'_'+tokens[1]+'_first','Integer',bounds[dgnodeName][tokens[1]].first);
        resourceloaddgnode.addMember(dgnodeName+'_'+tokens[1]+'_last','Integer',bounds[dgnodeName][tokens[1]].last);
        
        // check if we need to resize this node
        if(!resizeOps[dgnodeName] && bounds[dgnodeName].slices > 1) {
          dgnode.bindings.append(scene.constructOperator({
            operatorName: 'secureContainerResize',
            parameterLayout: [
              'self.newCount',
              'secureStorage.'+dgnodeName+'_'+tokens[1]+'_first',
              'secureStorage.elements'
            ],
            entryFunctionName: 'secureContainerResize',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadSecure.kl',
            async: false,
            mainThreadOnly: true
          }));
          resizeOps[dgnodeName] = true;
        }
        
        // create the operator to load it
        var operatorName = 'restoreSecureElement'+originalType.replace('[]','');
        if(isArray)
          operatorName += 'Array';

        var srcCode = 'use FabricSECURE;\noperator '+operatorName+'(';
        srcCode += '  io SecureContainer container,\n';
        srcCode += '  io SecureElement elements[],\n';
        srcCode += '  io Integer first,\n';
        srcCode += '  io Integer last,\n';
        srcCode += '  io '+originalType.replace('[]','')+' member<>'+(isArray ? '[]' : '')+'\n';
        srcCode += ') {\n';
        if(isArray || bounds[dgnodeName][tokens[1]].first != bounds[dgnodeName][tokens[1]].last) {
          srcCode += '  Integer outIndex = 0;\n';
          srcCode += '  for(Integer i=first;i<=last;i++) {\n';
          if(storedType != originalType) {
            srcCode += '    '+storedType.replace('[]','')+' memberArray[];\n';
            srcCode += '    memberArray.resize(elements[i].datacount);\n';
            srcCode += '    member[outIndex].resize(elements[i].datacount);\n';
            srcCode += '    container.getElementData(i,memberArray.data(),memberArray.dataSize());\n'
            srcCode += '    for(Size j=0;j<elements[i].datacount;j++) {\n';
            srcCode += '      member[i][j] = '+originalType.replace('[]','')+'(memberArray[j]);\n';
            srcCode += '    }\n';
          } else {
            srcCode += '    member[outIndex].resize(elements[i].datacount);\n';
            srcCode += '    container.getElementData(i,member[outIndex].data(),member[outIndex].dataSize());\n'
          }
          srcCode += '    outIndex++;\n'
          srcCode += '  }\n'
        } else if(isString) {
          srcCode += '  Integer outIndex = 0;\n';
          srcCode += '  for(Integer i=first;i<=last;i++) {\n';
          srcCode += '    Size length = Size(elements[i].datacount);\n';
          srcCode += '    member[outIndex] = "";\n';
          srcCode += '    while(length > 0) {\n';
          srcCode += '      if(length > 9) {\n';
          srcCode += '        member[outIndex] += "0000000000";\n';
          srcCode += '        length -= 10;\n';
          srcCode += '      } else {\n';
          srcCode += '        member[outIndex] += "0";\n';
          srcCode += '        length -= 1;\n';
          srcCode += '      }\n';
          srcCode += '    }\n';
          srcCode += '    container.getElementData(i,member[outIndex].data(),member[outIndex].length());\n'
          srcCode += '    outIndex++;\n'
          srcCode += '  }\n'
        } else {
          if(storedType != originalType) {
            srcCode += '  '+storedType.replace('[]','')+' memberArray[];\n';
            srcCode += '  memberArray.resize(member.size());\n';
            srcCode += '  container.getElementData(first,memberArray.data(),memberArray.dataSize());\n'
            srcCode += '  for(Size i=0;i<member.size();i++) {\n';
            srcCode += '    member[i] = '+originalType.replace('[]','')+'(memberArray[i]);\n';
            srcCode += '  }\n';
          } else {
            srcCode += '  container.getElementData(first,member.data(),member.dataSize());\n'
          }
        }
        srcCode += '}\n';

        dgnode.bindings.append(scene.constructOperator({
          operatorName: operatorName,
          srcCode: srcCode,
          entryFunctionName: operatorName,
          parameterLayout: [
            'secureStorage.container',
            'secureStorage.elements',
            'secureStorage.'+dgnodeName+'_'+tokens[1]+'_first',
            'secureStorage.'+dgnodeName+'_'+tokens[1]+'_last',
            'self.'+tokens[1]+'<>'
          ],
          mainThreadOnly: true
        }));
        
        // mark this element as done
        bounds[dgnodeName][tokens[1]].done = true;
        
        //console.log(srcCode);
      }
    };
    
    loadBinaryDataNode.pub.loadNode = function(node) {
      node = scene.getPrivateInterface(node);
      loadBinaryDataNode.pub.loadDGNodes(node.getDGNodes());
    }
    
    return loadBinaryDataNode;
  }
});


FABRIC.SceneGraph.registerNodeType('WriteBinaryDataNode', {
  briefDesc: 'The SecureStorageNode node is a ResourceLoad node able to load or save Fabric Engine Secure files.',
  detailedDesc: 'The SecureStorageNode node is a ResourceLoad node able to load or save Fabric Engine Secure. It utilizes a C++ based extension as well as zlib.',
  parentNodeDesc: 'ResourceLoad',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      compressionLevel: 9, // 0 - 9
      secureKey: undefined
    });
      
    var writeBinaryDataNode = scene.constructNode('SceneGraphNode', options);
    var binarydatadgnode = writeBinaryDataNode.constructResourceLoadNode('DGLoadNode');
    var dependencyCount = 0;
    
    writeBinaryDataNode.pub.putResourceToFile = function(resource, path){
      
      var dgErrors = binarydatadgnode.getErrors();
      if(dgErrors.length > 0){
        throw dgErrors;
      }
      binarydatadgnode.evaluate();
      
      binarydatadgnode.putResourceToFile(resource, path);
    }
    binarydatadgnode.addMember('container', 'SecureContainer');
    binarydatadgnode.addMember('elements', 'SecureElement[]');
    binarydatadgnode.addMember('secureKey', 'String', options.secureKey);
    binarydatadgnode.addMember('compressionLevel', 'Integer', options.compressionLevel);

    
    // attach an operator to clear the container!
    var nbOps = 0;
    binarydatadgnode.bindings.append(scene.constructOperator({
      operatorName: 'secureContainerClear',
      parameterLayout: [
        'self.container',
        'self.elements'
      ],
      entryFunctionName: 'secureContainerClear',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadSecure.kl',
      async: false,
      mainThreadOnly: true
    }));
    nbOps++;
    
    binarydatadgnode.bindings.append(scene.constructOperator({
      operatorName: 'secureContainerSave',
      parameterLayout: [
        'self.container',
        'self.resource',
        'self.compressionLevel',
        'self.secureKey'
      ],
      entryFunctionName: 'secureContainerSave',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadSecure.kl',
      async: false,
      mainThreadOnly: true
    }));
    nbOps++;
    
    // methods to store nodes
    writeBinaryDataNode.storeDGNode = function(dgnode, storeOptions) {
      scene.assignDefaults(storeOptions, {
        members: undefined,
        dgnodeName: undefined,
        version: 1
      });
      dgnode.evaluate();
      var memberNames = storeOptions.members;
      var members = dgnode.getMembers();
      if(!memberNames) {
        memberNames = [];
        for(var name in members)
          memberNames.push(name);
      }
      var dgnodeName = storeOptions.dgnodeName;
      if(!dgnodeName) dgnodeName = dgnode.getName();
      var count = dgnode.getCount();
      
      // now setup the operators to store the data
      for(var i=0;i<memberNames.length;i++) {
        var name = memberNames[i];
        var type = members[name].type;
        
        var isArray = type.indexOf('[]') > -1;
        var isString = type.indexOf('String') > -1;
        if(isArray && isString)
        {
          console.log("SecureStorage: Warning: Skipping member '"+name+"'. String Array members are not supported at this stage.");
          continue;
        }
        
        var operatorName = 'storeSecureElement'+type.replace('[]','');
        if(isArray)
          operatorName += 'Array';
        
        var storedType = type.replace('Size','Integer');

        binarydatadgnode.setDependency(dgnode,dgnodeName+'_'+dependencyCount);
        binarydatadgnode.addMember(dgnodeName+'_'+name+'_name','String',dgnodeName+'.'+name);
        binarydatadgnode.addMember(dgnodeName+'_'+name+'_type','String',storedType);
        binarydatadgnode.addMember(dgnodeName+'_'+name+'_version','Integer',storeOptions.version);
        
        var srcCode = 'use FabricSECURE;\noperator '+operatorName+'(\n';
        srcCode += '  io SecureContainer container,\n';
        srcCode += '  io SecureElement elements[],\n';
        srcCode += '  io String memberName,\n';
        srcCode += '  io String memberType,\n';
        srcCode += '  io Integer memberVersion,\n';
        srcCode += '  io '+type.replace('[]','')+' member<>'+(isArray ? '[]' : '')+'\n';
        srcCode += ') {\n';
        srcCode += '  SecureElement element;\n';
        srcCode += '  element.name = memberName;\n';
        srcCode += '  element.type = memberType;\n';
        srcCode += '  element.version = memberVersion;\n';
        srcCode += '  element.slicecount = member.size();\n';
        if(isArray) {
          srcCode += '  for(Size i=0;i<member.size();i++){\n';
          srcCode += '    element.sliceindex = Integer(i);\n';
          srcCode += '    element.datacount = member[i].size();\n';
          if(storedType != type) {
            srcCode += '    '+storedType.replace('[]','')+' memberArray[];\n';
            srcCode += '    memberArray.resize(member[i].size());\n';
            srcCode += '    for(Size j=0;j<member[i].size();j++) {\n';
            srcCode += '      memberArray[j] = '+storedType.replace('[]','')+'(member[i][j]);\n';
            srcCode += '    }\n';
            srcCode += '    container.addElement(element,memberArray.data(),memberArray.dataSize());\n'
          } else {
            srcCode += '    container.addElement(element,member[i].data(),member[i].dataSize());\n'
          }
          srcCode += '  }\n'
        } else if(isString) {
          srcCode += '  for(Size i=0;i<member.size();i++){\n';
          srcCode += '    element.sliceindex = Integer(i);\n';
          srcCode += '    element.datacount = member[i].length();\n';
          srcCode += '    container.addElement(element,member[i].data(),member[i].length());\n'
          srcCode += '  }\n'
        } else {
          srcCode += '  element.sliceindex = -1;\n';
          srcCode += '  element.datacount = member.size();\n';
          if(storedType != type) {
            srcCode += '  '+storedType.replace('[]','')+' memberArray[];\n';
            srcCode += '  memberArray.resize(member.size());\n';
            srcCode += '  for(Size i=0;i<member.size();i++) {\n';
            srcCode += '    memberArray[i] = '+storedType.replace('[]','')+'(member[i]);\n';
            srcCode += '  }\n';
            srcCode += '  container.addElement(element,memberArray.data(),memberArray.dataSize());\n'
          } else {
            srcCode += '  container.addElement(element,member.data(),member.dataSize());\n'
          }
        }
        srcCode += '  elements.push(element);\n';
        srcCode += '}\n';
        
        binarydatadgnode.bindings.insert(scene.constructOperator({
          operatorName: operatorName,
          srcCode: srcCode,
          entryFunctionName: operatorName,
          parameterLayout: [
            'self.container',
            'self.elements',
            'self.'+dgnodeName+'_'+name+'_name',
            'self.'+dgnodeName+'_'+name+'_type',
            'self.'+dgnodeName+'_'+name+'_version',
            dgnodeName+'_'+dependencyCount+'.'+name+'<>'
          ],
          mainThreadOnly: true,
          async: false
        }),nbOps-1);
        nbOps++;
      }
      dependencyCount++;
    };

    writeBinaryDataNode.pub.storeNode = function(node, storeOptions) {
      storeOptions = scene.assignDefaults(storeOptions, {
        dgnodeNames: undefined
      });
      node = scene.getPrivateInterface(node);
      var dgnodes = node.getDGNodes();
      var dgnodeNames = storeOptions.dgnodeNames;
      if(!dgnodeNames) {
        dgnodeNames = [];
        for(var name in dgnodes)
          dgnodeNames.push(name);
      }
      
      for(var i=0;i<dgnodeNames.length;i++) {
        writeBinaryDataNode.storeDGNode( dgnodes[dgnodeNames[i]], {
          dgnodeName: dgnodeNames[i]
        });
      }
    };
    
    return writeBinaryDataNode;
  }
});
