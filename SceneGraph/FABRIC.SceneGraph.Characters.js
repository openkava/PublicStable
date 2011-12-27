
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.SceneGraph.registerNodeType('CharacterMesh', {
  briefDesc: 'The CharacterMesh node represents a deformable Triangles node.',
  detailedDesc: 'The CharacterMesh node represents a deformable Triangles node. It is typically deformed through a GLSL skinning operator, driven by a CharacterRig node.',
  parentNodeDesc: 'Triangles',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      skinningMatriciesTextureUnit: 7
    });

    var characterMeshNode = scene.constructNode('Triangles', options);
    
    characterMeshNode.pub.addVertexAttributeValue('boneIds', 'Vec4', { genVBO:true } );
    characterMeshNode.pub.addVertexAttributeValue('boneWeights', 'Vec4', { genVBO: true });
    
    characterMeshNode.pub.addUniformValue('invmatrices', 'Mat44[]');
    characterMeshNode.pub.addUniformValue('boneMapping', 'Integer[]');
    
    var redrawEventHandler = characterMeshNode.getRedrawEventHandler();
    var tex = FABRIC.RT.oglMatrixBuffer2D();
    tex.forceRefresh = true;
    redrawEventHandler.addMember('oglSkinningMatriciesTexture2D', 'OGLTexture2D', tex);
    redrawEventHandler.addMember('skinningMatriciesTextureUnit', 'Integer', options.skinningMatriciesTextureUnit);
    redrawEventHandler.addMember('textureHeight', 'Size');
    
    redrawEventHandler.preDescendBindings.append( scene.constructOperator({
      operatorName: 'loadSkinningMatricesTexture',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/loadSkinningMatrices.kl',
      preProcessorDefinitions: {
        SKINNING_MATRICES_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('skinningMatrices'),
        SKINNIMATRICIES_TEXTUREUNIT_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('skinningMatricesTextureUnit'),
        TRANSFORM_TEXTURE_HEIGHT_ATTRIBUTE_ID: FABRIC.SceneGraph.getShaderParamID('transformTextureHeight')
      },
      entryFunctionName: 'loadSkinningMatricesTexture',
      parameterLayout: [
        'shader.shaderProgram',
        'rig.pose<>',
        'uniforms.invmatrices',
        'uniforms.boneMapping',
        'self.oglSkinningMatriciesTexture2D',
        'self.skinningMatriciesTextureUnit',
        'self.textureHeight'
      ]
    }));
    
    characterMeshNode.pub.setInvMatrices = function(invmatrices, boneMapping) {
      characterMeshNode.getUniformsDGNode().setData('invmatrices', 0, invmatrices);
      characterMeshNode.getUniformsDGNode().setData('boneMapping', 0, boneMapping);
    };
    
    var parentWriteData = characterMeshNode.writeData;
    var parentReadData = characterMeshNode.readData;
    characterMeshNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      characterMeshNode.writeGeometryData(sceneSerializer, constructionOptions, nodeData);
      nodeData.invmatrices = characterMeshNode.getUniformsDGNode().getData('invmatrices');
      nodeData.boneMapping = characterMeshNode.getUniformsDGNode().getData('boneMapping');
    };
    characterMeshNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      characterMeshNode.readGeometryData(sceneDeserializer, nodeData);
      characterMeshNode.getUniformsDGNode().setData('invmatrices', 0, nodeData.invmatrices);
      characterMeshNode.getUniformsDGNode().setData('boneMapping', 0, nodeData.boneMapping);
    };
    
    return characterMeshNode;
  }});

FABRIC.SceneGraph.registerNodeType('CharacterSkeleton', {
  briefDesc: 'The CharacterSkeleton node represents a collection of bones forming a skeleton.',
  detailedDesc: 'The CharacterSkeleton node represents a collection of bones forming a skeleton. It defines the rest pose of a character, as well as all related settings.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    calcReferenceLocalPose: 'Set to true, this will enable to computation of the local reference pose based on the global one.',
    calcReferenceGlobalPose: 'Set to true, this will enable to computation of the global reference pose based on the local one.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      calcReferenceLocalPose: false,
      calcReferenceGlobalPose: false
    });

    var characterSkeletonNode = scene.constructNode('SceneGraphNode', options);
    var dgnode = characterSkeletonNode.constructDGNode('DGNode');
    dgnode.addMember('bones', 'Bone[]');

    // extend public interface
    characterSkeletonNode.pub.getBones = function(skeletonId) {
      return dgnode.getData('bones', skeletonId ? skeletonId : 0);
    };
    characterSkeletonNode.pub.getBone = function(boneId, skeletonId) {
      return dgnode.getData('bones', skeletonId ? skeletonId : 0)[boneId];
    };
    characterSkeletonNode.pub.getBoneIndex = function(name, skeletonId) {
      var bones = characterSkeletonNode.pub.getBones(skeletonId);
      for (var i = 0; i < bones.length; i++) {
        if (bones[i].name == name)
          return i;
      }
      return -1;
    };
    characterSkeletonNode.pub.getNumBones = function(skeletonId) {
      return characterSkeletonNode.pub.getBones().length;
    };
    characterSkeletonNode.pub.getHierarchy = function(skeletonId) {
      var result = [];
      var bones = characterSkeletonNode.pub.getBones();
      for (var i = 0; i < bones.length; i++) {
        result.push(bones[i].parent);
      }
      return result;
    };
    characterSkeletonNode.pub.getParentId = function(boneid, skeletonId) {
      return characterSkeletonNode.pub.getHierarchy(skeletonId)[boneid];
    };
    characterSkeletonNode.pub.getBoneNames = function(skeletonId) {
      var result = [];
      var bones = characterSkeletonNode.pub.getBones();
      for (var i = 0; i < bones.length; i++) {
        result.push(bones[i].name);
      }
      return result;
    };
    characterSkeletonNode.pub.getReferencePose = function(skeletonId) {
      var result = [];
      var bones = characterSkeletonNode.pub.getBones();
      for (var i = 0; i < bones.length; i++) {
        result.push(bones[i].referencePose);
      }
      return result;
    };
    characterSkeletonNode.pub.getReferenceLocalPose = function(skeletonId) {
      var result = [];
      var bones = characterSkeletonNode.pub.getBones();
      for (var i = 0; i < bones.length; i++) {
        result.push(bones[i].referenceLocalPose);
      }
      return result;
    };
    characterSkeletonNode.pub.getCount = function() {
      return dgnode.getCount();
    };
    characterSkeletonNode.pub.setCount = function(count) {
      dgnode.setCount(count);
    };
    characterSkeletonNode.pub.addBone = function(boneOptions, skeletonId) {
      var bones = characterSkeletonNode.pub.getBones(),
        boneMap = {};
      for (var i = 0; i < bones.length; i++) {
        boneMap[bones[i].name] = i;
      }
      
      if (boneOptions.name === undefined || boneMap[boneOptions.name]) {
        throw ('Invalid bone name specified: ' + boneOptions.name);
      }

      if (boneOptions.parent && typeof boneOptions.parent === 'string') {
        var parentIndex = boneMap[boneOptions.parent];
        if (parentIndex == -1) {
          throw ('Bone parent ' + boneOptions.parent + ' not found in skeleton.');
        }
        boneOptions.parent = parentIndex;
      }

      // compute global from local or vice versa
      if (boneOptions.referencePose && !boneOptions.referenceLocalPose) {
        if (!boneOptions.parent || boneOptions.parent == -1) {
          boneOptions.referenceLocalPose = boneOptions.referencePose;
        }
        else {
          var refPose = bones[boneOptions.parent].referencePose;
          boneOptions.referenceLocalPose = refPose.inverse().multiply(boneOptions.referencePose);
        }
      }
      else if (!boneOptions.referencePose && boneOptions.referenceLocalPose) {
        if (boneOptions.parent == -1) {
          boneOptions.referencePose = boneOptions.referenceLocalPose;
        }
        else {
          boneOptions.referencePose = boneOptions.referenceLocalPose.multiply(bones[boneOptions.parent].referencePose);
        }
      }

      var bone = new FABRIC.Characters.Bone(boneOptions);
      bones.push(bone);
      dgnode.setData('bones', skeletonId ? skeletonId : 0, bones );
      return bone;
    };
    
    characterSkeletonNode.pub.setBones = function(boneOptions, skeletonId) {
      var bones = [];
      for (var i = 0; i < boneOptions.length; i++) {
        bones.push(new FABRIC.Characters.Bone(boneOptions[i]));
      }
      dgnode.setData('bones', skeletonId ? skeletonId : 0, bones );
    };
    
    
    characterSkeletonNode.addMember = function(name, type, value) {
      dgnode.addMember(name, type, value);
    };
    characterSkeletonNode.getData = function(name, skeletonId) {
      return dgnode.getData(name, skeletonId ? skeletonId : 0);
    };
    characterSkeletonNode.setData = function(name, data, skeletonId) {
      dgnode.setData(name, skeletonId ? skeletonId : 0, data );
    };
    
    ////////////////////////////////////
    // Peristence
    var parentWriteData = characterSkeletonNode.writeData;
    var parentReadData = characterSkeletonNode.readData;
    characterSkeletonNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      nodeData.bones = dgnode.getData('bones');
    };
    characterSkeletonNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      dgnode.setData('bones', 0, nodeData.bones);
    };

    if (options.calcReferenceLocalPose) {
      // For skeletons that are built procedurally, or using
      // our rigging tools, this operator will run every time a
      // reference pose is modified.
      dgnode.bindings.append(scene.constructOperator({
        operatorName: 'calcReferenceLocalPose',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/characterSkeleton.kl',
        entryFunctionName: 'calcReferenceLocalPose',
        parameterLayout: [
          'self.bones'
        ]
      }));
    }
    if (options.calcReferenceGlobalPose) {
      // For skeletons that are built procedurally, or using
      // our rigging tools, this operator will run every time a
      // reference pose is modified.
      dgnode.bindings.append(scene.constructOperator({
        operatorName: 'calcReferenceGlobalPose',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/characterSkeleton.kl',
        entryFunctionName: 'calcReferenceGlobalPose',
        parameterLayout: [
          'self.bones'
        ]
      }));
    }

    return characterSkeletonNode;
  }});

FABRIC.SceneGraph.registerNodeType('CharacterSkeletonDebug', {
  briefDesc: 'The CharacterSkeletonDebug node is a debug node used to draw a CharacterSkeletonNode on screen.',
  detailedDesc: 'The CharacterSkeletonDebug node is a debug node used to draw a CharacterSkeletonNode on screen. It generates a collection of line segments for each bone in the skeleton.',
  parentNodeDesc: 'Lines',
  optionsDesc: {
    boneradius: 'The radius of the bones to use for drawing.',
    color: 'The color of the lines to use for drawing.',
    drawOverlaid: 'Set to true, this will enable overlaid drawing for the CharacterSkeletonDebug node.'
  },
  factoryFn: function(options, scene) {

    scene.assignDefaults(options, {
      boneradius: 1.0,
      color: FABRIC.RT.rgba(0.2, 0.2, 0.73, 1.0),
      drawOverlaid: true
    });

    var rigNode,
      instanceNode;

    var characterSkeletonDebug = scene.constructNode('Lines', options);

    // extend public interface
    characterSkeletonDebug.pub.setRigNode = function(node) {

      rigNode = scene.getPrivateInterface(node);
      var skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode());

      characterSkeletonDebug.getAttributesDGNode().setDependency(rigNode.getDGNode(), 'rig');
      characterSkeletonDebug.getAttributesDGNode().setDependency(skeletonNode.getDGNode(), 'skeleton');
      characterSkeletonDebug.pub.addUniformValue('boneradius', 'Scalar', options.boneradius);
      characterSkeletonDebug.pub.addUniformValue('offsetpose', 'Xfo', options.offsetpose);

      // now append the operator to create the lines
      
      // now append the operator to create the lines
      characterSkeletonDebug.getAttributesDGNode().bindings.append(scene.constructOperator({
          operatorName: 'setSkeletonVertexCount',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateSkeleton.kl',
          entryFunctionName: 'setSkeletonVertexCount',
          parameterLayout: [
            'skeleton.bones',
            'self.newCount'
          ]
        }));
      
      characterSkeletonDebug.getAttributesDGNode().bindings.append(scene.constructOperator({
          operatorName: 'generateSkeletonOp',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/generateSkeleton.kl',
          entryFunctionName: 'generateSkeleton',
          parameterLayout: [
            'skeleton.bones',
            'rig.pose',
            'self.positions<>',
            'uniforms.indices',
            'uniforms.boneradius',
            'uniforms.offsetpose'
          ]
        }));
    };
    characterSkeletonDebug.pub.getRigNode = function() {
      return scene.getPublicInterface(rigNode);
    };

    // make sure it deforms
    characterSkeletonDebug.pub.setAttributeDynamic('positions');

    if (options.characterRigNode) {
      characterSkeletonDebug.pub.setRigNode(options.characterRigNode);
    }

    instanceNode = scene.constructNode('Instance', {
        geometryNode: characterSkeletonDebug.pub,
        materialNode: scene.constructNode('FlatMaterial', {
          color: options.color,
          drawOverlaid: options.drawOverlaid
        }).pub
      });
    characterSkeletonDebug.pub.getInstanceNode = function() {
      return instanceNode.pub;
    }

    return characterSkeletonDebug;
  }});


FABRIC.SceneGraph.registerNodeType('CharacterVariables', {
  briefDesc: 'The CharacterVariables node is a collection of all parameters used for a pose computation.',
  detailedDesc: 'The CharacterVariables node is a collection of all parameters used for a pose computation. The members in this node are typically changing over time, for example they are based on FCurve animation.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      baseVariables: undefined,
      poseVariables: new FABRIC.RT.PoseVariables()
    });

    var characterVariablesNode = scene.constructNode('SceneGraphNode', options);
    var dgnode = characterVariablesNode.constructDGNode('DGNode');
    dgnode.addMember('poseVariables', 'PoseVariables', options.poseVariables);
    // extend the private interface
    characterVariablesNode.setVariables = function(poseVariables) {
      dgnode.setData('poseVariables', 0, poseVariables);
    };
    
    characterVariablesNode.getVariables = function(){
      return dgnode.getData('poseVariables');
    };
    
    var animationLibrary;
    
    characterVariablesNode.setBindings = function(bindings){
      if(dgnode.getMembers().bindings){
        dgnode.setData('bindings', 0, bindings);
      }
      else{
        dgnode.addMember('bindings', 'KeyframeTrackBindings', bindings);
      }
    }
    var boundToAnimationTracks = false;
    characterVariablesNode.pub.bindToAnimationTracks = function(animationLibraryNode, animationControllerNode, trackSetId, keyframeTrackBindings){
      if (!animationLibraryNode.isTypeOf('AnimationLibrary')) {
        throw ('Incorrect type assignment. Must assign a AnimationLibrary');
      }
      if (!animationControllerNode.isTypeOf('AnimationController')) {
        throw ('Incorrect type assignment. Must assign a AnimationController');
      }
      animationLibraryNode = scene.getPrivateInterface(animationLibraryNode);
      animationControllerNode = scene.getPrivateInterface(animationControllerNode);
      dgnode.setDependency(animationLibraryNode.getDGNode(), 'animationlibrary');
      dgnode.setDependency(animationControllerNode.getDGNode(), 'controller');
      if(boundToAnimationTracks){
        dgnode.setData('trackSetId', 0, trackSetId);
        return;
      }
      
      dgnode.addMember('keyIndices', 'Integer[]');
      dgnode.addMember('trackSetId', 'Integer', trackSetId);
      characterVariablesNode.setBindings(keyframeTrackBindings);
      
      dgnode.bindings.append(scene.constructOperator({
        operatorName: 'evaluatePoseTracks',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/evaluatePoseTracks.kl',
        preProcessorDefinitions: {
          KEYFRAMETRACKSETTYPE: 'LinearKeyframeTrackSet'
        },
        entryFunctionName: 'evaluatePoseTracks',
        parameterLayout: [
          'animationlibrary.trackSet<>',
          'controller.localTime',
          'self.trackSetId',
          'self.bindings',
          'self.keyIndices',
          'self.poseVariables'
        ]
      }));
      characterVariablesNode.pub.setBoundTrack = function(trackSetId){
        dgnode.setData('trackSetId', 0, trackSetId);
      }
      boundToAnimationTracks = true;
    }
    
    
    //////////////////////////////////////////
    // Persistence
    var parentWriteData = characterVariablesNode.writeData;
    var parentReadData = characterVariablesNode.readData;
    characterVariablesNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      if(dgnode.getMembers().bindings){
        nodeData.bindings = dgnode.getData('bindings');
      }
    };
    characterVariablesNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      if(nodeData.bindings){
        characterVariablesNode.setBindings(nodeData.bindings);
      }
    };
    // If a base Vaiables node is provided, we clone the values. 
    if(options.baseVariables){
      if (!options.baseVariables.isTypeOf('CharacterVariables')) {
        throw ('Incorrect type assignment. Must assign a CharacterVariables');
      }
      var baseVariables = scene.getPrivateInterface(options.baseVariables);
      poseVariables = baseVariables.getVariables();
      dgnode.setData('poseVariables', 0, poseVariables);
      
      if(baseVariables.getDGNode().getMembers().bindings){
        characterVariablesNode.setBindings(baseVariables.getDGNode().getData('bindings'));
      }
    }

    return characterVariablesNode;
  }});



FABRIC.SceneGraph.registerNodeType('CharacterController', {
  briefDesc: 'The CharacterController places a character in a scene.',
  detailedDesc: 'The CharacterController node is used to place a character in a scene. The animation of the character is relative to the controller. ' +
                'The Character controlle rprovides a base transfrom which the pose of the character built on.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    controllerXfo: 'The initial transfrom of the character.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      controllerXfo: new FABRIC.RT.Xfo()
    });
    var characterControllerNode = scene.constructNode('SceneGraphNode', options);
    var dgnode = characterControllerNode.constructDGNode('DGNode');
    
    dgnode.addMember('xfo', 'Xfo', options.controllerXfo);
    
    return characterControllerNode;
  }});


// The character rig computes the pose of a character
FABRIC.SceneGraph.registerNodeType('CharacterRig', {
  briefDesc: 'The CharacterRig node is used to compute the pose of a character.',
  detailedDesc: 'The CharacterRig node is used to compute the pose of a character. It utilizes the CharacterSkeleton as well as the CharacterVariables node, ' +
                'together with a series of operators, called CharacterSolvers, to compute the global pose of the character.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    skeletonNode: 'The skeletonNode to use for this CharacterRig',
    variablesNode: 'The variablesNode to use for this CharacterRig',
    baseCharacterRigNode: 'Copy all solvers from this base rig to male a clone'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      skeletonNode: undefined,
      variablesNode: undefined,
      controllerNode: undefined,
      baseCharacterRigNode: undefined,
      debug: true
    });
    
    var characterRigNode = scene.constructNode('SceneGraphNode', options);
    var dgnode = characterRigNode.constructDGNode('DGNode');
    var skeletonNode,
      variablesNode,
      controllerNode,
      poseVariables = new FABRIC.RT.PoseVariables(),
      solvers = [];
      
    dgnode.addMember('debug', 'Boolean', options.debug );
    dgnode.addMember('debugGeometry', 'DebugGeometry' );
    
    
    dgnode.bindings.append(scene.constructOperator({
      operatorName: 'matchCount',
      srcCode: 'operator matchCount(Size parentCount, io Size selfCount) { selfCount = parentCount; }',
      entryFunctionName: 'matchCount',
      parameterLayout: [
        'charactercontroller.count',
        'self.newCount'
      ],
      async: false
    }));
    // extend the public interface
    characterRigNode.addMemberInterface(dgnode, 'pose', true);
    
    characterRigNode.addReferenceInterface('Skeleton', 'CharacterSkeleton',
      function(nodePrivate){
        skeletonNode = nodePrivate;
        dgnode.setDependency(skeletonNode.getDGNode(), 'skeleton');
  
        // This member will store the computed pose.
        var referencePose = skeletonNode.pub.getReferencePose();
        dgnode.addMember('pose', 'Xfo[]', referencePose);
      });

    characterRigNode.addReferenceInterface('Controller', 'CharacterController',
      function(nodePrivate){
        controllerNode = nodePrivate;
        dgnode.setDependency(controllerNode.getDGNode(), 'charactercontroller');
      });
    
    //////////////////////////////////////////
    // Variables
    characterRigNode.addReferenceInterface('Variables', 'CharacterVariables',
      function(nodePrivate){
        variablesNode = nodePrivate;
        dgnode.setDependency(variablesNode.getDGNode(), 'variables');
      });
    characterRigNode.addVariable = function(type, value) {
      var id = poseVariables.addVariable(type, value);
      if(variablesNode){
        variablesNode.setVariables(poseVariables);
      }
      return id;
    };
    characterRigNode.getVariables = function(){
      return poseVariables;
    };
    characterRigNode.pub.constructVariablesNode = function(name, bindToRig) {
      var node = scene.constructNode('CharacterVariables', {
        name: name,
        poseVariables: poseVariables
      }).pub;
      if(bindToRig){
        characterRigNode.pub.setVariablesNode(node);
      }
      return node;
    };
    //////////////////////////////////////////
    // Solver Interfaces
    var solverParams = [];
    characterRigNode.pub.addSolver = function(name, type, solverOptions) {
      for (var i = 0; i < solvers.length; i++) {
        if (solvers[i].name == name) {
          throw (" Solver names must be unique. Solver '" + name +
                 "' already applied to :" + characterRigNode.pub.getName());
        }
      }
      solverOptions = solverOptions ? solverOptions : {};
      solverOptions.rigNode = characterRigNode.pub;
      solverOptions.name = name;
      var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver(type, solverOptions, scene);
      solvers.push(solver);
      delete solverOptions.rigNode;
      solverParams.push({
        name: name,
        type: type,
        options: solverOptions
      });
      return solver;
    };
    characterRigNode.pub.getSolver = function(name) {
      for (var i = 0; i < solvers.length; i++) {
        if(solvers[i].getName() == name)
          return solvers[i];
      }
      return undefined;
    };
    
    characterRigNode.getSolverParams = function() {
      return solverParams;
    };
    characterRigNode.pub.getSolvers = function() {
      return solvers;
    };
    characterRigNode.addSolverOperator = function(operatorDef) {
      operatorBinding = scene.constructOperator(operatorDef);
      dgnode.bindings.append(operatorBinding);
    }
    characterRigNode.pub.invertSolvers = function(node, bindToRig) {
      if (!node.isTypeOf('CharacterRig')) {
        throw ('Incorrect type assignment. Must assign a CharacterRig');
      }
      sourceRigNode = scene.getPrivateInterface(node);
      var invertedVariablesNode = characterRigNode.pub.constructVariablesNode('invertedVariables', bindToRig);
      invertedVariablesNode = scene.getPrivateInterface(invertedVariablesNode);
      invertedVariablesNode.getDGNode().setDependency(sourceRigNode.getDGNode(), 'sourcerig');
      invertedVariablesNode.getDGNode().setDependency(skeletonNode.getDGNode(), 'skeleton');
      for (var i = 0; i < solvers.length; i++) {
        if(!solvers[i].invert)
          continue;
        solvers[i].invert(invertedVariablesNode);
      }
      return invertedVariablesNode.pub;
    };
    //////////////////////////////////////////
    characterRigNode.pub.addMember = function(name, type, value) {
      dgnode.addMember(name, type, value);
    };
    characterRigNode.pub.getData = function(name, index) {
      var xfo = dgnode.getData(name, 0);
      if (index != undefined) {
        xfo = xfo[index];
      }
      return xfo;
    };
    characterRigNode.pub.setData = function(name, value, index) {
      var xfo;
      if (index != undefined) {
        xfo = dgnode.getData(name, 0);
        xfo[index] = value;
      }
      else {
        xfo = value;
      }
      dgnode.setData(name, 0, xfo);
    }
    
    //////////////////////////////////////////
    // Persistence
    var parentAddDependencies = characterRigNode.addDependencies;
    characterRigNode.addDependencies = function(sceneSerializer) {
      parentAddDependencies(sceneSerializer);
      sceneSerializer.addNode(skeletonNode.pub);
      sceneSerializer.addNode(variablesNode.pub);
    };
    
    var parentWriteData = characterRigNode.writeData;
    var parentReadData = characterRigNode.readData;
    characterRigNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      constructionOptions.debug = options.debug;
      nodeData.skeletonNode = skeletonNode.pub.getName();
      nodeData.variablesNode = variablesNode.pub.getName();
      nodeData.solvers = [];
      for (var i = 0; i < solverParams.length; i++) {
        nodeData.solvers[i] = solverParams[i];
      }
    };
    characterRigNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      characterRigNode.pub.setSkeletonNode(sceneDeserializer.getNode(nodeData.skeletonNode));
      characterRigNode.pub.setVariablesNode(sceneDeserializer.getNode(nodeData.variablesNode));
      for (var i = 0; i < nodeData.solvers.length; i++) {
        characterRigNode.pub.addSolver(nodeData.solvers[i].name, nodeData.solvers[i].type, nodeData.solvers[i].options);
      }
    };
  
    var debugGeometryDraw = scene.constructNode('DebugGeometryDraw', {
      dgnode: dgnode,
      debugGemetryMemberName: 'debugGeometry'
    });
    
    if (options.skeletonNode) {
      characterRigNode.pub.setSkeletonNode(options.skeletonNode);
    }
    
    if (options.variablesNode) {
      characterRigNode.pub.setVariablesNode(options.variablesNode);
    }
    
    if (options.controllerNode) {
      characterRigNode.pub.setControllerNode(options.controllerNode);
    }
    else {
      characterRigNode.pub.setControllerNode(scene.constructNode('CharacterController').pub);
    }
    
    if (options.baseCharacterRigNode) {
      // Apply the baseCharacterRigNode solvers.
      var baseCharacterRigNode = scene.getPrivateInterface(options.baseCharacterRigNode);
      characterRigNode.pub.setSkeletonNode(baseCharacterRigNode.pub.getSkeletonNode());
      var baseRigSolvers = baseCharacterRigNode.getSolverParams();
      for (var i = 0; i < baseRigSolvers.length; i++) {
        characterRigNode.pub.addSolver(baseRigSolvers[i].name, baseRigSolvers[i].type, baseRigSolvers[i].options);
      }
    }
    
    return characterRigNode;
  }});



FABRIC.SceneGraph.registerNodeType('CharacterRigDebug', {
  briefDesc: 'The CharacterRigDebug node is used to draw debug information of a CharacterRig on screen.',
  detailedDesc: 'The CharacterRigDebug node is used to draw debug information of a CharacterRig on screen.',
  parentNodeDesc: 'Points',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {

    scene.assignDefaults(options, {
      dynamic: true,
      color: FABRIC.RT.rgba(0.0, 1.0, 1.0, 1.0),
      size: 5.0,
      offsetpose: new FABRIC.RT.Xfo(),
      constructInstanceNode: true
      });

    var characterRigDebugNode = scene.constructNode('Points', options);
    var rigNode,
      instanceNode;

    // extend public interface
    characterRigDebugNode.pub.setRigNode = function(node) {

      rigNode = scene.getPrivateInterface(node);
      characterRigDebugNode.pub.addVertexAttributeValue('vertexColors', 'Color', { genVBO:true } );
      characterRigDebugNode.getUniformsDGNode().setDependency(rigNode.getDGNode(), 'rig');
      characterRigDebugNode.getUniformsDGNode().setDependency(rigNode.getVariablesNode().getDGNode(), 'variables');
      characterRigDebugNode.pub.addUniformValue('debugpose', 'Xfo[]');
      characterRigDebugNode.pub.addUniformValue('singlecolor', 'Color', options.color);
      characterRigDebugNode.pub.addUniformValue('offsetpose', 'Xfo', options.offsetpose);

      // now append the operator to create the lines
      var operators = characterRigDebugNode.getUniformsDGNode().bindings;
      operators.append(scene.constructOperator({
          operatorName: 'clearDebugXfos',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/characterDebug.kl',
          entryFunctionName: 'clearDebugXfos',
          parameterLayout: [
            'self.debugpose'
          ]
        }));

      characterRigDebugNode.getAttributesDGNode().bindings.append(scene.constructOperator({
          operatorName: 'generateDebugPoints',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/characterDebug.kl',
          entryFunctionName: 'generateDebugPoints',
          parameterLayout: [
            'uniforms.debugpose',
            'uniforms.offsetpose',
            'self.positions<>',
            'self.vertexColors[]',
            'uniforms.singlecolor'
          ]
        }));
    };
    characterRigDebugNode.pub.getRigNode = function() {
      return rigNode;
    };
    characterRigDebugNode.pub.constructInstanceNode = function() {
      if (instanceNode)
        return instanceNode;
      if (!rigNode)
        return undefined;

      var material = scene.constructNode('VertexColorMaterial', {
        prototypeMaterialType: 'PointMaterial',
        pointSize: 10.0,
        color: FABRIC.RT.rgb(0.8, 0, 0, 1)
      });

      instanceNode = scene.constructNode('Instance', {
          geometryNode: characterRigDebugNode.pub,
          materialNode: material
        });
      return instanceNode;
    };
    characterRigDebugNode.pub.getInstanceNode = function() {
      return scene.getPublicInterface(instanceNode);
    };

    characterRigDebugNode.pub.characterRigDebugNode.setAttributeDynamic('positions');

    if (options.characterRigNode) {
      characterRigDebugNode.pub.setRigNode(options.characterRigNode);
    }
    if (options.constructInstanceNode) {
      characterRigDebugNode.pub.constructInstanceNode();
    }

    return characterRigDebugNode;
  }});

// The character instance draws a deformed mesh on screen.
FABRIC.SceneGraph.registerNodeType('CharacterInstance', {
  briefDesc: 'The CharacterInstance node is used to draw a deformed CharacterMesh on screen.',
  detailedDesc: 'The CharacterInstance node is used to draw a deformed CharacterMesh on screen. It utilizes the CharacterRig as well as the CharacterMesh, and applies skinning / deformation through the use of GLSL.',
  parentNodeDesc: 'Instance',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      });

    var characterInstanceNode = scene.constructNode('Instance', options);
    var rigNode;

    // extend public interface
    characterInstanceNode.addReferenceInterface('Rig', 'CharacterRig',
      function(nodePrivate){
        rigNode = nodePrivate;
        characterInstanceNode.getRedrawEventHandler().setScope('rig', rigNode.getDGNode());
      });
    var parentSetGeometryNode = characterInstanceNode.pub.setGeometryNode;
    characterInstanceNode.addReferenceInterface('Geometry', 'CharacterMesh',
      function(nodePrivate){
        parentSetGeometryNode(nodePrivate.pub);
      });
    
    //////////////////////////////////////////
    // Persistence
    var parentWriteData = characterInstanceNode.writeData;
    var parentReadData = characterInstanceNode.readData;
    characterInstanceNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      sceneSerializer.addNode(rigNode.pub);
      nodeData.rigNode = rigNode.pub.getName();
    };
    characterInstanceNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      characterInstanceNode.pub.setRigNode(sceneDeserializer.getNode(nodeData.rigNode));
    };

    if (options.rigNode) {
      characterInstanceNode.pub.setRigNode(options.rigNode);
    }
    return characterInstanceNode;
  }});

