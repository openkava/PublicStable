
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//
FABRIC.SceneGraph.registerNodeType('LocalStorageNode', {
  briefDesc: 'The LocalStorageNode node is a ResourceLoad node able to load or save local files.',
  detailedDesc: 'The LocalStorageNode node is a ResourceLoad node able to load or save local files. It utilizes the optional FabricSTORAGE extension, '+
                'which is only available on local installations of Fabric Engine, not on www based ones.',
  parentNodeDesc: 'ResourceLoad',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      dependentNode: undefined,
      path: undefined
    });
    // force the url off...
    options.localPath = true;

    var resourceLoadNode = scene.constructNode('ResourceLoad', options),
      resourceloaddgnode = resourceLoadNode.getDGLoadNode();
      
    // create a global utility storage node
    // this should be done with extension function inside javascript
    // once fabric supports that.
    if(!scene.localStorage) {
      scene.localStorage = {};
      scene.localStorage.utilitydgnode = resourceLoadNode.constructDGNode('DGUtilityNode');
        
      // create the utility operators
      scene.localStorage.utilitydgnode.addMember('parentpath','String');
      scene.localStorage.utilitydgnode.addMember('childrenpaths','String[]');
      scene.localStorage.utilitydgnode.addMember('asciipath','String');
      scene.localStorage.utilitydgnode.addMember('asciidata','String');
      scene.localStorage.utilitydgnode.bindings.append(scene.constructOperator({
        operatorName: 'getStorageSubHandles',
        parameterLayout: [
          'self.parentpath',
          'self.childrenpaths'
        ],
        entryFunctionName: 'getStorageSubHandles',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/localStorage.kl',
        async: false,
        mainThreadOnly: true
      }));
      scene.localStorage.utilitydgnode.bindings.append(scene.constructOperator({
        operatorName: 'getStorageAsciiContent',
        parameterLayout: [
          'self.asciipath',
          'self.asciidata'
        ],
        entryFunctionName: 'getStorageAsciiContent',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/localStorage.kl',
        async: false,
        mainThreadOnly: true
      }));
    }
    
    // public interface
    resourceLoadNode.pub.getFilesInFolder = function(folder) {
      scene.localStorage.utilitydgnode.setData('parentpath',0,folder);
      scene.localStorage.utilitydgnode.evaluate();
      return scene.localStorage.utilitydgnode.getData('childrenpaths',0);
    };
    resourceLoadNode.pub.getFileTextContent = function(file) {
      scene.localStorage.utilitydgnode.setData('asciipath',0,file);
      scene.localStorage.utilitydgnode.evaluate();
      return scene.localStorage.utilitydgnode.getData('asciidata',0);
    };

    return resourceLoadNode;
  }
});

FABRIC.SceneGraph.registerParser('fez', function(scene, assetUrl, options) {
  options.url = assetUrl;
  return scene.constructNode('SecureStorageNode', options);
});

FABRIC.SceneGraph.registerNodeType('SecureStorageNode', {
  briefDesc: 'The SecureStorageNode node is a ResourceLoad node able to load or save Fabric Engine Secure files.',
  detailedDesc: 'The SecureStorageNode node is a ResourceLoad node able to load or save Fabric Engine Secure. It utilizes a C++ based extension as well as zlib.',
  parentNodeDesc: 'ResourceLoad',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      removeParsersOnLoad: false,
      dependentNode: undefined,
      compressionLevel: 9, // 0 - 9
      secureKey: undefined
    });

    var resourceLoadNode = scene.constructNode('ResourceLoad', options),
      resourceloaddgnode = resourceLoadNode.getDGLoadNode();
    var dependencyCount = 0;
      
    // make the dependent node, well, dependent
    if(options.dependentNode != undefined) {
      var priv = scene.getPrivateInterface(options.dependentNode);
      for(var dgnodeName in priv.getDGNodes()) {
        priv.getDGNodes()[dgnodeName].setDependency(resourceloaddgnode,resourceloaddgnode.getName());
      }
    }
    
    resourceLoadNode.pub.putResourceToFile = resourceloaddgnode.putResourceToFile;
    resourceLoadNode.pub.getResourceFromFile = resourceloaddgnode.getResourceFromFile;

    resourceloaddgnode.addMember('container', 'SecureContainer');
    resourceloaddgnode.addMember('elements', 'SecureElement[]');
    resourceloaddgnode.addMember('secureKey', 'String', options.secureKey);
    resourceloaddgnode.addMember('compressionLevel', 'Integer', options.compressionLevel);
    
    // interface for loading data
    if(options.url) {

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
  
      resourceLoadNode.pub.addOnLoadSuccessCallback(function(pub) {
        // define the new nodes based on the identifiers in the file
        resourceloaddgnode.evaluate();
        var elements = resourceloaddgnode.getData('elements',0);
        resourceLoadNode.pub.getSecureElements = function() {
          return elements;
        }
      });
      
    } else {
      
      // attach an operator to clear the container!
      var nbOps = 0;
      resourceloaddgnode.bindings.append(scene.constructOperator({
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
      
      resourceloaddgnode.bindings.append(scene.constructOperator({
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
      resourceLoadNode.storeDGNode = function(storeOptions) {
        scene.assignDefaults(storeOptions, {
          dgnode: undefined,
          members: undefined,
          dgnodeName: undefined,
          version: 1
        });
        var dgnode = storeOptions.dgnode;
        if(!dgnode)
          throw("storeoptions.dgnode is not defined!");
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

          resourceloaddgnode.setDependency(dgnode,dgnodeName+'_'+dependencyCount);
          resourceloaddgnode.addMember(dgnodeName+'_'+name+'_name','String',dgnodeName+'.'+name);
          resourceloaddgnode.addMember(dgnodeName+'_'+name+'_type','String',storedType);
          resourceloaddgnode.addMember(dgnodeName+'_'+name+'_version','Integer',storeOptions.version);
          
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
          
          resourceloaddgnode.bindings.insert(scene.constructOperator({
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
            mainThreadOnly: true
          }),nbOps-1);
          nbOps++;
        }
        dependencyCount++;
      };

      resourceLoadNode.pub.storeNode = function(storeOptions) {
        scene.assignDefaults(storeOptions, {
          node: undefined,
          dgnodeNames: undefined,
        });
        var node = storeOptions.node;
        if(!node)
          throw("storeoptions.node is not defined!");
        node = scene.getPrivateInterface(node);
        var dgnodes = node.getDGNodes();
        var dgnodeNames = storeOptions.dgnodeNames;
        if(!dgnodeNames) {
          dgnodeNames = [];
          for(var name in dgnodes)
            dgnodeNames.push(name);
        }
        
        for(var i=0;i<dgnodeNames.length;i++) {
          resourceLoadNode.storeDGNode({
            dgnode: dgnodes[dgnodeNames[i]],
            dgnodeName: dgnodeNames[i]
          });
        }
      };
    }
    
    resourceLoadNode.pub.constructNode = function(type,nodeOptions) {
      var node = scene.constructNode(type,nodeOptions);
      
      var elements = resourceLoadNode.pub.getSecureElements();
      var dgnodes = node.getDGNodes();
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
      
      return node.pub;
    };
    
    return resourceLoadNode;
  }
});
