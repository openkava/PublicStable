
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

//////////////////////////////////////////////////////////////////////////////
// The SceneSerializer is a tool for writing out all the data for a collection
// of nodes that the developer wishes to save.
// Note: it is the responsibility of the developer to determine which nodes
// need to be persisted and which ones done't.
// For example, a scene graph might always construct a Viewport or Grid object
// which shouldn't be persisted.
//
// Example usage
//   var sceneSerializer = FABRIC.SceneGraph.constructManager('SceneSerializer');
//  for(i in savableNodes){
//    sceneSerializer.addNode(savableNodes[i]);
//  }
//  // Now put the data somewhere. Maybe in a database or file.
//  FABRIC.FS.write(sceneSerializer.save());

/**
 * Constructor for a SceneSerializer object.
 * @param {object} scene The scene to save.
 * @return {object} The created SceneSerializer object.
 */
FABRIC.SceneGraph.registerManagerType('SceneSerializer', {
  briefDesc: 'The SceneSerializer manages the persistence of objects.',
  detailedDesc: '.',
  parentManagerDesc: '',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      filteredNodeTypes: [],
      typeRemappings: {}
    });
  
    var filteredNodes = [];
    var savedNodes = [];
    var savedData = [];
    var currentIndex = 0;
    var storedDGNodes = {};
    var isNodeBeingSaved = function(node) {
      return (savedNodes.indexOf(node) !== -1);
    };
    var isNodeExcluded = function(node) {
      if(filteredNodes.indexOf(node) === -1){
        for(var i=0; i<options.filteredNodeTypes.length; i++){
          if (node.isTypeOf(options.filteredNodeTypes[i])) {
            return true;
          }
        }
        return false;
      }
      return true;
    };
    var sceneSerializer = {
      addNode: function(node) {
        if (!node || !node.isTypeOf || !node.isTypeOf('SceneGraphNode')) {
          throw 'SceneSaver can only save SceneGraphNodes';
        }
        if(!isNodeBeingSaved(node) && !isNodeExcluded(node)){
          var constructionOptions = {};
          var nodeData = {};
          var nodePrivate = scene.getPrivateInterface(node);
          nodePrivate.addDependencies(sceneSerializer);
          savedNodes.push(node);
        };
      },
      isNodeBeingSaved: function(node) {
        return isNodeBeingSaved(node);
      },
      isNodeExcluded: function(node) {
        return isNodeExcluded(node);
      },
      writeDGNodesData: function(sgnodename, desc) {
        if(!storedDGNodes[sgnodename]){
          storedDGNodes[sgnodename] = {};
        }
        for(var dgnodeName in desc){
          storedDGNodes[sgnodename][dgnodeName] = desc[dgnodeName];
        }
      },
      getTypeRemapping: function(type){
        if(options.typeRemappings[type]){
          return options.typeRemappings[type];
        }
        return type;
      },
      pub:{
        // Add the filter nodes first, and then add the nodes you wish to save.
        filterNode: function(node) {
          return filteredNodes.push(node);
        },
        addNode: function(node) {
          return sceneSerializer.addNode(node);
        },
        wrapQuotes: function(val) {
          return '\"' + ((typeof val === 'string') ? val : val.toString()) + '\"';
        },
        serialize: function() {
          for(var i=0;i<savedNodes.length;i++) {
            var nodePrivate = scene.getPrivateInterface(savedNodes[i]);
            var constructionOptions = { };
            var nodeData = {};
            nodePrivate.writeData(sceneSerializer, constructionOptions, nodeData);
            savedData[i] = {
              options: constructionOptions,
              data: nodeData
            };
          }
        },
        save: function(writer) {
          this.serialize();
          var binaryStorageNode;
          if(writer.getBinaryStorageNode){
            binaryStorageNode = scene.getPrivateInterface(writer.getBinaryStorageNode());
          }
          var str = '{';
          str +=  '\n  \"metadata\":{';
          if(binaryStorageNode){
            str += '\n    \"binaryStorage\":' + true;
            str += ',\n    \"binaryFilePath\":' + this.wrapQuotes(writer.getBinaryFileName());
          }
          str +=  '\n  },';
          str +=  '\n  \"sceneGraphNodes\":[';
          for (var i = 0; i < savedNodes.length; i++) {
            if (i > 0) {
              str += ',';
            }
            var name = savedNodes[i].getName();
            var type = sceneSerializer.getTypeRemapping(savedNodes[i].getType());
            str +=  '\n    {';
            str +=  '\n      \"name\":' + this.wrapQuotes(name);
            str += ',\n      \"type\":' + this.wrapQuotes(type);
            str += ',\n      \"options\":' + JSON.stringify(savedData[i].options);
            str += ',\n      \"data\":' + JSON.stringify(savedData[i].data);
            
            if(storedDGNodes[name]){
              
              if(binaryStorageNode){
                binaryStorageNode.storeDGNodes( name, storedDGNodes[name]);
              }else{
                str += ',\n      \"dgnodedata\":{';
                var nodecnt = 0;
                for(var dgnodename in storedDGNodes[name]){
                  var dgnodeDataDesc = storedDGNodes[name][dgnodename];
                  var dgnode = dgnodeDataDesc.dgnode;
                  if (nodecnt > 0) {
                    str += ',';
                  }
                  str += '\n        \"'+dgnodename+'\":{';
                  if(dgnodeDataDesc.members){
                    str += '\n          \"members\":' + JSON.stringify(dgnodeDataDesc.members);
                  }
                  else{
                    str += ',\n          \"sliceCount\":' + dgnode.getCount();
                    str += ',\n          \"memberData\":' + JSON.stringify(dgnode.getBulkData());
                  }
                  str +=  '\n        }';
                  nodecnt++;
                }
                
                str +=  '\n      }';
            
              }
            }
            str +=  '\n    }';
          }
          str += '\n  ]';
          str += '\n}';
          writer.write(str);
          return true;
        }
      }
    };
    return sceneSerializer;
  }});


//////////////////////////////////////////////////////////////////////////////
// The SceneDeserializer is a tool for loading a collection of nodes and
// re-binding dependencies.
//
// Example usage
//   var sceneDeserializer = FABRIC.SceneGraph.constructManager('SceneDeserializer');
//  var loadedNodes = sceneDeserializer.load();

/**
 * Constructor for a SceneDeserializer object.
 * @param {object} scene The scene to load into.
 * @param {object} reader The reader object to read from.
 * @return {object} The created SceneDeserializer object.
 */
FABRIC.SceneGraph.registerManagerType('SceneDeserializer', {
  briefDesc: 'The SceneDeserializer manages the loading of persisted objects.',
  detailedDesc: '.',
  parentManagerDesc: '',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      preLoadScene: true
    });
    
    var dataObj;
    var preLoadedNodes = {};
  
    var constructedNodeMap = {};
    var nodeNameRemapping = {};
    
    if(options.preLoadScene){
      var sceneNodes = scene.getSceneGraphNodes();
      for(var name in sceneNodes){
        preLoadedNodes[name] = sceneNodes[name].pub;
      }
    };
    var nodeData;
    var loadNodeBinaryFileNode;
    var sceneDeserializer = {
      getNode: function(nodeName) {
        nodeName = nodeNameRemapping[ nodeName ]
        if (constructedNodeMap[nodeName]) {
          return constructedNodeMap[nodeName];
        }else {
          return scene.pub.getSceneGraphNode(nodeName);
        }
      },
      loadDGNodeData: function(dgnode, dgnodename) {
        var data = nodeData.dgnodedata[dgnodename];
        if(dataObj.metadata.binaryStorage){
          var sgnodename = nodeData.name;
          loadNodeBinaryFileNode.pub.addOnLoadSuccessCallback(function(){
            // once the archive is loaded, construct a Triangles node from it
            var nodemap = {};
            nodemap[sgnodename + '_' + dgnodename] = dgnode;
            loadNodeBinaryFileNode.pub.loadDGNodes(nodemap);
          });
        }
        else{
          dgnode.setCount(data.sliceCount);
          var members = dgnode.getMembers();
          var memberData = {};
          for(var memberName in members){
            if(data.memberData[memberName]){
              memberData[memberName] = data.memberData[memberName];
            }
          }
          dgnode.setBulkData(memberData);
        }
      },
      pub: {
        setPreLoadedNode: function(node, nodeName) {
          if(!nodeName) nodeName = node.getName();
          preLoadedNodes[nodeName] = node;
        },
        load: function(storage) {
          dataObj = storage.read();
          if(typeof dataObj == 'string'){
            dataObj = JSON.parse(dataObj);
          }
          if(dataObj.metadata.binaryStorage){
            
            var binaryFilePath = dataObj.metadata.binaryFilePath;
            if(storage.getUrl){
              var pathArray = storage.getUrl().split('/');
              pathArray.pop();
              binaryFilePath = pathArray.join('/') + '/' + binaryFilePath;
            }
            loadNodeBinaryFileNode = scene.constructNode('LoadBinaryDataNode', {
              url: binaryFilePath,
              secureKey: 'secureKey'
            });
          }
          
          for (var i = 0; i < dataObj.sceneGraphNodes.length; i++) {
            nodeData = dataObj.sceneGraphNodes[i];
            var node = preLoadedNodes[nodeData.name];
            if (!node) {
              node = scene.pub.constructNode(nodeData.type, nodeData.options);
            }
            // in case a name collision occured, store a name remapping table.
            nodeNameRemapping[ nodeData.name ] = node.getName();
            var nodePrivate = scene.getPrivateInterface(node);
            nodePrivate.readData(sceneDeserializer, nodeData.data);
            constructedNodeMap[node.getName()] = node;
          }
          return constructedNodeMap;
        }
      }
    };
    return sceneDeserializer;
  }});

/**
 * Constructor to create a logWriter object.
 * @constructor
 */
FABRIC.SceneGraph.LogWriter = function() {
  var str = "";
  this.write = function(instr) {
    str = instr;
    console.log(str);
  }
  this.log = function(instr) {
    console.log(str);
  }
};

FABRIC.SceneGraph.LocalStorage = function(name) {
  this.write = function(instr) {
    localStorage.setItem(name, instr);
  }
  this.read = function(){
    return localStorage.getItem(name);
  }
  this.log = function() {
    console.log(localStorage.getItem(name));
  }
};

/**
 * Constructor to create a FileWriter object.
 * @constructor
 * @param {string} filepath The path to the file to write to.
 */
FABRIC.SceneGraph.FileWriter = function(scene, title, suggestedFileName) {
  
  var path = scene.IO.queryUserFileAndFolderHandle(scene.IO.forOpenWithWriteAccess, title, "json", suggestedFileName);
  var str = "";
  this.write = function(instr) {
    str = instr;
    scene.IO.putTextFile(str, path);
  }
  this.log = function(instr) {
    console.log(str);
  }
};

FABRIC.SceneGraph.FileWriterWithBinary = function(scene, title, suggestedFileName) {
  
  var path = scene.IO.queryUserFileAndFolderHandle(scene.IO.forOpenWithWriteAccess, title, "json", suggestedFileName);
  var jsonFilename = path.fileName.split('.')[0];
  var binarydatapath = scene.IO.queryUserFileAndFolderHandle(scene.IO.forOpenWithWriteAccess, "Secure ", "fez", jsonFilename);
  
  var writeBinaryDataNode = scene.constructNode('WriteBinaryDataNode', {
    secureKey: 'secureKey'
  });
      
  var str = "";
  this.getBinaryStorageNode = function(){
    return writeBinaryDataNode;
  }
  
  this.getBinaryFileName = function(){
    return binarydatapath.fileName;
  }
  
  this.write = function(instr) {
    str = instr;
  //  console.log(str);
    scene.IO.putTextFile(str, path);
  //  writeBinaryDataNode.write('resource',binarydatapath);
  }
  this.log = function(instr) {
    console.log(str);
  }
};

/**
 * Constructor to create a FileReader object.
 * @constructor
 * @param {string} filepath The path to the file to read from.
 */
FABRIC.SceneGraph.FileReader = function(scene, title, suggestedFileName) {
  var path = scene.IO.queryUserFileAndFolderHandle(scene.IO.forOpen, title, "json", suggestedFileName);

  this.read = function() {
    return JSON.parse(scene.IO.getTextFile(path));
  }
};

/**
 * Constructor to create a FileReader object.
 * @constructor
 * @param {string} filepath The path to the file to read from.
 */
FABRIC.SceneGraph.XHRReader = function(url) {
  var file = FABRIC.loadResourceURL(url);
  this.getUrl = function() {
    return url;
  }
  this.read = function() {
    return JSON.parse(file);
  }
};

/**
 * Constructor to create a FileReader object.
 * @constructor
 * @param {string} filepath The path to the file to read from.
 */
FABRIC.SceneGraph.URLReader = function(url) {
  this.read = function() {
    return FABRIC.loadResourceURL(url);
  }
};


/*
FABRIC.SceneGraph.registerParser('fez', function(scene, assetUrl, options) {
  options.url = assetUrl;
  return scene.constructNode('LoadBinaryDataNode', options);
});
*/


FABRIC.SceneGraph.registerNodeType('LoadBinaryDataNode', {
  briefDesc: 'The SecureStorageNode node is a ResourceLoad node able to load or save Fabric Engine Secure files.',
  detailedDesc: 'The SecureStorageNode node is a ResourceLoad node able to load or save Fabric Engine Secure. ' +
                ' It utilizes a C++ based extension as well as zlib.',
  parentNodeDesc: 'ResourceLoad',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      secureKey: undefined,
      version: 1
    });

    var loadBinaryDataNode = scene.constructNode('ResourceLoad', options),
      resourceloaddgnode = loadBinaryDataNode.getDGLoadNode();
    
    loadBinaryDataNode.pub.getResourceFromFile = resourceloaddgnode.getResourceFromFile;

    resourceloaddgnode.addMember('container', 'SecureContainer');
    resourceloaddgnode.addMember('elements', 'SecureElement[]');
    resourceloaddgnode.addMember('secureKey', 'String', options.secureKey);
    
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
    var bounds = {};
    loadBinaryDataNode.pub.addOnLoadSuccessCallback(function(pub) {
      // define the new nodes based on the identifiers in the file
      resourceloaddgnode.evaluate();
      elements = resourceloaddgnode.getData('elements',0);
      
      // first, parse for slicecounts
      for(var i=0;i<elements.length;i++) {
        var tokens = elements[i].name.split('.');
        var dgnodeName = tokens[0];
      //  if(!dgnodes[dgnodeName]){
      //    console.log('Warning: SecureStorage: Unknown DGNode: '+dgnodeName);
      //    continue;
      //  }
      
        if(!bounds[dgnodeName]){
          bounds[dgnodeName] = {};
          bounds[dgnodeName].slices = elements[i].slicecount
        }
        if(!bounds[dgnodeName][tokens[1]]){
          bounds[dgnodeName][tokens[1]] = {};
        }
        if(bounds[dgnodeName][tokens[1]].first == undefined){
          bounds[dgnodeName][tokens[1]].first = i;
        }
        bounds[dgnodeName][tokens[1]].last = i;
      }
    });
    
    var opSrc = {};
    var metaDataDGNodes = {};
    loadBinaryDataNode.pub.loadDGNodes = function(dgnodes) {
      
      var resizeOps = {};
      
      for(var i=0;i<elements.length;i++) {
        var tokens = elements[i].name.split('.');
        var dgnodeName = tokens[0];
        if(!dgnodes[dgnodeName]){
          continue;
        }
        
        // check if we already did this
        if(bounds[dgnodeName][tokens[1]].done){
          continue;
        }
        
        var dgnode = dgnodes[dgnodeName];
        
        // check if the member exists
        var dgnodeMembers = dgnode.getMembers();
        if(!dgnodeMembers[tokens[1]]){
        //  console.log("SecureStorage: Warning: Adding missing member '"+tokens[1]+"'.")
        //  dgnode.addMember(tokens[1],elements[i].type);
        //  dgnodeMembers[tokens[1]] = {};
        //  dgnodeMembers[tokens[1]].type = elements[i].type;
          console.warn("SecureStorage: missing member '"+tokens[1]+"'.")
          continue;
        }
        
        // check if the member has the right type
        var originalType = dgnodeMembers[tokens[1]].type;
        var type = elements[i].type;
        var isArray = type.indexOf('[]') > -1;
        var isString = type.indexOf('String') > -1;
        var storedType = originalType.replace('Size','Integer');
        if(storedType != type) {
          console.warn('SecureStorage: Member '+tokens[1]+' has wrong type: '+originalType);
          continue;
        }

        var binaryloadmetadatadgnode = metaDataDGNodes[dgnodeName];
        if(!binaryloadmetadatadgnode){
          binaryloadmetadatadgnode = loadBinaryDataNode.constructDGNode(dgnodeName+'MetaDataDGNode');
          metaDataDGNodes[dgnodeName] = binaryloadmetadatadgnode;
          
          dgnode.setDependency(binaryloadmetadatadgnode,'metaData');
          dgnode.setDependency(resourceloaddgnode,'secureStorage');
        }
        
        binaryloadmetadatadgnode.addMember(tokens[1]+'_first','Integer',bounds[dgnodeName][tokens[1]].first);
        binaryloadmetadatadgnode.addMember(tokens[1]+'_last','Integer',bounds[dgnodeName][tokens[1]].last);
        
        
        // check if we need to resize this node
        if(!resizeOps[dgnodeName] && bounds[dgnodeName].slices > 1) {
          dgnode.bindings.append(scene.constructOperator({
            operatorName: 'secureContainerResize',
            parameterLayout: [
              'self.newCount',
              'metaData.'+tokens[1]+'_first',
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
        if(isArray){
          operatorName += 'Array';
        }
        if(!opSrc[operatorName]){
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
          }
          else if(isString) {
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
          }
          else {
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
          opSrc[operatorName] = srcCode;
        }
        dgnode.bindings.append(scene.constructOperator({
            operatorName: operatorName,
            srcCode: opSrc[operatorName],
            entryFunctionName: operatorName,
            parameterLayout: [
              'secureStorage.container',
              'secureStorage.elements',
              'metaData.'+tokens[1]+'_first',
              'metaData.'+tokens[1]+'_last',
              'self.'+tokens[1]+'<>'
            ],
            mainThreadOnly: true
          }));
        
        // mark this element as done
        bounds[dgnodeName][tokens[1]].done = true;
        
        //console.log(srcCode);
      }
    };
    
    /*
    loadBinaryDataNode.pub.loadNode = function(node) {
      node = scene.getPrivateInterface(node);
      loadBinaryDataNode.pub.loadDGNodes(node.getDGNodes());
    }
    */
    
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
    
    var writeBinaryDataNodeEvent = writeBinaryDataNode.constructEventNode('WriteBinaryEvent');
    var binarydatadgnode = writeBinaryDataNode.constructResourceLoadNode('DGLoadNode');
    
    writeBinaryDataNode.pub.write = function(resource, path){
      var dgErrors = writeBinaryDataNodeEvent.getErrors();
      if(dgErrors.length > 0){
        throw dgErrors;
      }
      writeBinaryDataNodeEvent.fire();
      binarydatadgnode.putResourceToFile(resource, path);
    }
    
    binarydatadgnode.addMember('container', 'SecureContainer');
    binarydatadgnode.addMember('elements', 'SecureElement[]');
    binarydatadgnode.addMember('secureKey', 'String', options.secureKey);
    binarydatadgnode.addMember('compressionLevel', 'Integer', options.compressionLevel);

    var writeEventHandler = writeBinaryDataNode.constructEventHandlerNode('Propagation');
    writeEventHandler.setScope('container', binarydatadgnode);
    // attach an operator to clear the container!
    var nbOps = 0;
    var opSrc = {};
    writeEventHandler.preDescendBindings.append(scene.constructOperator({
      operatorName: 'secureContainerClear',
      parameterLayout: [
        'container.container',
        'container.elements'
      ],
      entryFunctionName: 'secureContainerClear',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadSecure.kl',
      async: false,
      mainThreadOnly: true
    }));
    nbOps++;
    
    writeEventHandler.postDescendBindings.append(scene.constructOperator({
      operatorName: 'secureContainerSave',
      parameterLayout: [
        'container.container',
        'container.resource',
        'container.compressionLevel',
        'container.secureKey'
      ],
      entryFunctionName: 'secureContainerSave',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadSecure.kl',
      async: false,
      mainThreadOnly: true
    }));
    nbOps++;
    
    writeBinaryDataNodeEvent.appendEventHandler(writeEventHandler);
    
    // methods to store nodes
    writeBinaryDataNode.storeDGNodes = function(sgnodename, dgnodeDescs) {
      var writeDGNodesEventHandler = writeBinaryDataNode.constructEventHandlerNode('Write'+sgnodename);
      writeEventHandler.appendChildEventHandler(writeDGNodesEventHandler);
      
      for(dgnodeName in dgnodeDescs){
        var dgnode = dgnodeDescs[dgnodeName].dgnode;
        var memberNames = dgnodeDescs[dgnodeName].members;
        var members = dgnode.getMembers();
        if(!memberNames) {
          memberNames = [];
          for(var name in members){
            memberNames.push(name);
          }
        }
        
        writeDGNodesEventHandler.setScope(dgnodeName, dgnode);
        
        // now setup the operators to store the data
        for(var i=0;i<memberNames.length;i++) {
          var memberName = memberNames[i];
          var memberType = members[memberName].type;
          
          var isArray = memberType.indexOf('[]') > -1;
          var isString = memberType.indexOf('String') > -1;
          if(isArray && isString){
            console.log("SecureStorage: Warning: Skipping member '"+memberName+"'. String Array members are not supported at this stage.");
            continue;
          }
          
          var operatorName = 'storeSecureElement'+memberType.replace('[]','');
          if(isArray)
            operatorName += 'Array';
          
          // We currenlty store 'Size' values as 'Integer' values to be platform neutral.
          var storedType = memberType.replace('Size','Integer');
  
          writeDGNodesEventHandler.addMember(memberName+'_name','String',dgnodeName+'.'+memberName);
          writeDGNodesEventHandler.addMember(memberName+'_type','String',storedType);
          writeDGNodesEventHandler.addMember(memberName+'_version','Integer',options.version);
          
          if(!opSrc[operatorName]){
            var srcCode = 'use FabricSECURE;\noperator '+operatorName+'(\n';
            srcCode += '  io SecureContainer container,\n';
            srcCode += '  io SecureElement elements[],\n';
            srcCode += '  io String memberName,\n';
            srcCode += '  io String memberType,\n';
            srcCode += '  io Integer memberVersion,\n';
            srcCode += '  io '+memberType.replace('[]','')+' member<>'+(isArray ? '[]' : '')+'\n';
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
              if(storedType != memberType) {
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
              if(storedType != memberType) {
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
            opSrc[operatorName] = srcCode;
          }
          writeDGNodesEventHandler.preDescendBindings.append(scene.constructOperator({
            operatorName: operatorName,
            srcCode: opSrc[operatorName],
            entryFunctionName: operatorName,
            parameterLayout: [
              'container.container',
              'container.elements',
              'self.'+name+'_name',
              'self.'+name+'_type',
              'self.'+name+'_version',
              dgnodeName+'.'+name+'<>'
            ],
            async: false
          }));
        }
      }
    };
    
    return writeBinaryDataNode;
  }
});

