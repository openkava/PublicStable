
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.RT.SolverHelperFunctions = function() {
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('SolverHelperFunctions', {
    members: {
    },
    constructor: FABRIC.RT.SolverHelperFunctions,
    klBindings: {
      filename: 'solverHelperFunctions.kl',
      sourceCode: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/KL/solverHelperFunctions.kl')
    }
  });
});


/**
 * A character solver represents one computation in
 * the evaluation of a character's pose.
 */
FABRIC.SceneGraph.CharacterSolvers = {
  solvers: {},
  registerSolver: function(type, factoryFn) {
    if (this.solvers[type]) {
      throw ("CharacterSolver '" + type + "' already Registered");
    }else {
      this.solvers[type] = factoryFn;
    }
  },
  constructSolver: function(type, options, scene) {
    if (!this.solvers[type]) {
      throw ("CharacterSolver '" + type + "' is not registered.");
    }
    var solver = this.solvers[type].constructSolver(options, scene);
    solver.name = options.name;
    solver.type = type;
    return solver;
  }
};

FABRIC.SceneGraph.CharacterSolvers.registerSolver('CharacterSolver', {
  constructSolver: function(options, scene) {

    if (!options.rigNode) {
      throw ('You must specify the rigNode.');
    }

    var name = options.name,
      parameterLayout,
      skeletonNode = options.rigNode.getSkeletonNode(),
      manipulators = {},
      name2id;
    
    var solver = {
      getName: function(){
        return name;
      },
      constructManipulator: function(name, manipulatorType, options) {
        if (manipulators[name]) {
          throw (" Manipulator names must be unique. Solver '" + name + "' already contains :'" + name + "'");
        }
        manipulators[name] = scene.constructNode(manipulatorType, options);
        return manipulators[name];
      },
      getManipulators: function() {
        return manipulators;
      },
      generateBoneMapping: function(bones, identifiers){
        // Define the mapping between the named bones and the bone ids.
        if (!bones) {
          throw ('We need to specify the bones dictionary for any CharacterSolver.');
        }
        if (!identifiers) {
          throw ('We need to specify the identifiers for any CharacterSolver.');
        }
        if(!name2id){
          name2id = {};
          var boneNames = skeletonNode.getBoneNames();
          for (var i = 0; i < boneNames.length; i++) {
            name2id[boneNames[i]] = i;
          }
        }
    
        // create a temp map for boneName 2 id
        var boneIDs = {};
        // let's check the identifiers
        for (var i = 0; i < identifiers.length; i++){
          if (identifiers[i].constructor.name === 'Array') {
            var name = identifiers[i][0];
            if (!bones[name]) {
              console.warn ("'" + name + "' were not specified.");
              continue;
            }
            if (typeof(bones[name]) !== typeof(identifiers[i])) {
              throw ('Bone ' + name + ' was uncorrectly specified. Should be name array.');
            }
            var chainBones = [];
            var dynNames = bones[name];
            for (var j = 0; j < dynNames.length; j++) {
              if (name2id[dynNames[j]] == undefined) {
                throw ('Bone ' + dynNames[j] + ' is not part of the skeleton.');
              }
              chainBones.push(name2id[dynNames[j]]);
            }
            boneIDs[name] = chainBones;
          }
          else {
            var name = identifiers[i];
            if (!bones[name]) {
              console.warn ("'" + name + "' were not specified.");
              continue;
            }
            if (name2id[bones[name]] == undefined) {
              throw ('Bone ' + bones[name] + ' is not part of the skeleton.');
            }
            boneIDs[name] = name2id[bones[name]];
          }
        }
        return boneIDs;
      }
    };

    return solver;
  }
});


FABRIC.RT.FKHierarchy = function(boneIds, xfoIds) {
  this.boneIds = boneIds ? boneIds : [];
  this.xfoIds = xfoIds ? xfoIds : [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('FKHierarchy', {
    members: {
      boneIds: 'Integer[]',
      xfoIds: 'Integer[]'
    },
    constructor: FABRIC.RT.FKHierarchy
  });
});


FABRIC.SceneGraph.CharacterSolvers.registerSolver('FKHierarchySolver',{
  constructSolver: function(options, scene) {
    
    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode()),
      bones = skeletonNode.pub.getBones(),
      referenceLocalPose = skeletonNode.pub.getReferenceLocalPose();

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);
    
    if(!options.bones){
      // Collect all the bones in the skeleton.
      options.bones = { bones:[] };
      for(var i=0; i<bones.length; i++){
        options.bones.bones.push(bones[i].name);
      }
    }
    
    var boneIDs = solver.generateBoneMapping(options.bones, [['bones']]),
      name = solver.getName();

    var defaultValues = [];
    for(var i=0; i<boneIDs.bones.length; i++){
      defaultValues.push(referenceLocalPose[boneIDs.bones[i]]);
    }
    var xfoIds = rigNode.addVariable('Xfo[]', defaultValues);
    
    var binding = new FABRIC.RT.FKHierarchy(boneIDs.bones, xfoIds);
    skeletonNode.addMember(name + 'bindings', 'FKHierarchy', binding);
    
    rigNode.addSolverOperator({
      operatorName: 'solveFKHierarchy',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveFKHierarchy.kl',
      entryFunctionName: 'solveFKHierarchy',
      parameterLayout: [
        'self.pose',
        'skeleton.bones',
        'skeleton.' + name + 'bindings',
        'self.index',
        'variables.poseVariables<>'
      ]
    });
    
    solver.getXfoVarBindings = function(){
      var boneXfoVarBindings = {};
      for(var i=0; i<options.bones.bones.length; i++){
        boneXfoVarBindings[options.bones.bones[i]] = xfoIds[i];
      }
      return boneXfoVarBindings;
    }
    
    solver.invert = function(variablesNode){
      variablesNode.getDGNode().bindings.append(scene.constructOperator({
          operatorName: 'invertFKHierarchy',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveFKHierarchy.kl',
          entryFunctionName: 'invertFKHierarchy',
          parameterLayout: [
            'sourcerig.pose',
            'skeleton.bones',
            'skeleton.' + name + 'bindings',
            
            'self.poseVariables'
          ]
        }));
    }
    
    solver.generateTracks = function(trackSet, trackBindings){
      var color = FABRIC.RT.rgb(1, 0, 0);
      var storeEulerAngles = false;
      for (var i = 0; i < binding.boneIds.length; i++) {
        trackSet.addXfoTrack(solver.getName()+i, color, storeEulerAngles, trackBindings, xfoIds[i]);
      }
    }
 
    solver.setPose = function(pose, variablesNode) {
      var fkpose = [];
      for (var i = 0; i < binding.boneIds.length; i++) {
        if (bones[binding.boneIds[i]].parent == -1)
          fkpose[i] = pose[binding.boneIds[i]];
        else
          fkpose[i] = pose[bones[binding.boneIds[i]].parent].inverse().multiply(pose[binding.boneIds[i]]);
      }
      variablesNode.setValues(fkpose, xfoIds);
    };
    
    return solver;
  }
});




FABRIC.SceneGraph.CharacterSolvers.registerSolver('FKChainSolver',{
  constructSolver: function(options, scene) {
    
    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode()),
      bones = skeletonNode.pub.getBones(),
      referenceLocalPose = skeletonNode.pub.getReferenceLocalPose();

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);
    
    var boneIDs = solver.generateBoneMapping(options.bones, [['bones']]),
      name = solver.getName();

    // check if the bones have a length
    for (i = 0; i < boneIDs.bones.length; i++) {
      if (bones[boneIDs.bones[i]].length <= 0.0) {
        throw ('Cannot apply FKChainSolver to bone ' +
          bones[boneIDs.bones[i]].name + ', because it has length == 0.');
      }
      if (i > 0 && bones[boneIDs.bones[i]].parent != boneIDs.bones[i - 1]) {
        throw ('Cannot apply FKChainSolver to bone ' +
          bones[boneIDs.bones[i]].name + ', because is not part of a single chain.');
      }
      chainLocalReferencePose.push(referenceLocalPose[boneIDs.bones[i]]);
      chainGlobalReferencePose.push(referencePose[boneIDs.bones[i]]);
    }

    var defaultValues = [];
    for(var i=0; i<boneIDs.bones.length; i++){
      defaultValues.push(referenceLocalPose[boneIDs.bones[i]]);
    }
    var xfoIds = rigNode.addVariable('Xfo[]', defaultValues);
    
    var binding = new FABRIC.RT.FKHierarchy(boneIDs.bones, xfoIds);
    skeletonNode.addMember(name + 'bindings', 'FKHierarchy', binding);
    
    rigNode.addSolverOperator({
      operatorName: 'solveFKHierarchy',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveFKHierarchy.kl',
      entryFunctionName: 'solveFKHierarchy',
      parameterLayout: [
        'self.pose',
        'skeleton.bones',
        'skeleton.' + name + 'bindings',
        'self.index',
        'variables.poseVariables<>'
      ]
    });
    
    solver.getXfoVarBindings = function(){
      var boneXfoVarBindings = {};
      for(var i=0; i<options.bones.bones.length; i++){
        boneXfoVarBindings[options.bones.bones[i]] = xfoIds[i];
      }
      return boneXfoVarBindings;
    }
    
    solver.invert = function(variablesNode){
      variablesNode.getDGNode().bindings.append(scene.constructOperator({
          operatorName: 'invertFKHierarchy',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveFKHierarchy.kl',
          entryFunctionName: 'invertFKHierarchy',
          parameterLayout: [
            'sourcerig.pose',
            'skeleton.bones',
            'skeleton.' + name + 'bindings',
            
            'self.poseVariables'
          ]
        }));
    }
    
    solver.generateTracks = function(trackSet, trackBindings){
      var color = FABRIC.RT.rgb(1, 0, 0);
      var storeEulerAngles = false;
      for (var i = 0; i < binding.boneIds.length; i++) {
        trackSet.addXfoTrack(solver.getName()+i, color, storeEulerAngles, trackBindings, xfoIds[i]);
      }
    }
 
    solver.setPose = function(pose, variablesNode) {
      var fkpose = [];
      for (var i = 0; i < binding.boneIds.length; i++) {
        if (bones[binding.boneIds[i]].parent == -1)
          fkpose[i] = pose[binding.boneIds[i]];
        else
          fkpose[i] = pose[bones[binding.boneIds[i]].parent].inverse().multiply(pose[binding.boneIds[i]]);
      }
      variablesNode.setValues(fkpose, xfoIds);
    };
    
    return solver;
  }
});



FABRIC.SceneGraph.CharacterSolvers.registerSolver('TwistBoneSolver', {
  constructSolver: function(options, scene) {

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene),
      parameterLayout,
      bindToRig;

    bindToRig = function() {

      var rigNode = scene.getPrivateInterface(options.rigNode),
        skeletonNode = rigNode.pub.getSkeletonNode(),
        bones = skeletonNode.getBones(),
        referencePose = skeletonNode.getReferencePose(),
        boneIDs = solver.generateBoneMapping(options.bones, ['start', 'end', ['twistBones']]),
        size,
        name = options.name;

      // first, we will compute the local transform of the end inside the start's space
      var startXfo = referencePose[boneIDs.start];
      var endXfo = referencePose[boneIDs.end];

      // check if we know the U values
      var uValues = options.uValues;
      if (!uValues) {
        uValues = [];
        var boneLength = startXfo.inverse().multiply(endXfo).tr.length();
        for (var i = 0; i < boneIDs.twistBones.length; i++) {
          var local = startXfo.inverse().multiply(referencePose[boneIDs.twistBones[i]]);
          var u = local.tr.length() / boneLength;
          if (u > 1.0) {
            throw ("Unexpected U value, twistBone '" + bones[boneIDs.twistBones[i]].name + "' outside of bone.");
          }
          uValues.push(u);
        }
      }
      skeletonNode.addMember(name + 'start', 'Integer', boneIDs.start);
      skeletonNode.addMember(name + 'end', 'Integer', boneIDs.end);
      skeletonNode.addMember(name + 'twistBones', 'Integer[]', boneIDs.twistBones);
      skeletonNode.addMember(name + 'uvalues', 'Scalar[]', uValues);

      rigNode.addSolverOperator({
          operatorName: 'solveTwistBones',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveTwistBones.kl',
          entryFunctionName: 'solveTwistBones',
          parameterLayout: [
            'self.pose',
            'skeleton.bones',
            'skeleton.' + name + 'start',
            'skeleton.' + name + 'end',
            'skeleton.' + name + 'twistBones',
            'skeleton.' + name + 'uvalues'
          ]
        });
    };

    bindToRig();

    return solver;
  }
});


/*
  The Blend Bone solver is usefull for procedural bones such as knee caps.
  It interpolates 2 source bones onto a set of blend bones
*/
FABRIC.SceneGraph.CharacterSolvers.registerSolver('BlendBoneSolver', {
  constructSolver: function(options, scene) {

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = rigNode.pub.getSkeletonNode(),
      bones = skeletonNode.getBones(),
      referencePose = skeletonNode.getReferencePose(),
      boneIDs = solver.generateBoneMapping(options.bones, ['start', 'end', ['blendBones']]),
      size,
      name = options.name;

    if (!options.blendWeights) {
      throw ('Error in BlendBoneSolver: blendWeights not specified ');
    }
    if (options.blendWeights.length != boneIDs.blendBones.length) {
      throw ('Error in BlendBoneSolver: blendWeights.length != blendBones.length ');
    }

    // first, we will compute the local transform of the end inside the start's space
    var startXfo = referencePose[boneIDs.start];
    var endXfo = referencePose[boneIDs.end];

    var blendBoneOffsets = [];
    for (var i = 0; i < boneIDs.blendBones.length; i++) {
      var blendedXfo = startXfo.clone();
      blendedXfo.ori.sphericalLinearInterpolate(startXfo.ori, options.blendWeights[i]);
      blendBoneOffsets.push(startXfo.inverse().multiply(blendedXfo));
    }
    skeletonNode.addMember(name + 'start', 'Integer', boneIDs.start);
    skeletonNode.addMember(name + 'end', 'Integer', boneIDs.end);
    skeletonNode.addMember(name + 'blendBones', 'Integer[]', boneIDs.blendBones);
    skeletonNode.addMember(name + 'blendBoneOffsets', 'Xfo[]', blendBoneOffsets);
    skeletonNode.addMember(name + 'blendWeights', 'Scalar[]', options.blendWeights);

    rigNode.addSolverOperator({
        operatorName: 'solveBlendBones',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveTwistBones.kl',
        entryFunctionName: 'solveBlendBones',
        parameterLayout: [
          'self.pose',
          'skeleton.bones',
          'skeleton.' + name + 'start',
          'skeleton.' + name + 'end',
          'skeleton.' + name + 'blendBones',
          'skeleton.' + name + 'blendBoneOffsets',
          'skeleton.' + name + 'blendWeights'
        ]
      });

    return solver;
  }
});




FABRIC.RT.Limb = function(boneIds, ankleId, xfoIds, ikGoalXfoId, ikGoalOffsetXfo, ikGoalReferenceXfo, ikblendId) {
  this.boneIds = boneIds != undefined ? boneIds : [];
  this.ankleId = ankleId != undefined ? ankleId : -1;
  this.xfoIds = xfoIds != undefined ? xfoIds : [];
  this.ikGoalXfoId = ikGoalXfoId != undefined ? ikGoalXfoId : -1;
  this.ikGoalOffsetXfo = ikGoalOffsetXfo != undefined ? ikGoalOffsetXfo : new FABRIC.RT.Xfo();
  this.ikGoalReferenceXfo = ikGoalReferenceXfo != undefined ? ikGoalReferenceXfo : new FABRIC.RT.Xfo();
  this.ikblendId = ikblendId != undefined ? ikblendId : -1;
  this.hubId = -1; // This is used by the locomotion system.
};


FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('Limb', {
    members: {
      boneIds: 'Integer[]',
      ankleId: 'Integer',
      xfoIds: 'Integer[]',
      ikGoalXfoId: 'Integer',
      ikGoalOffsetXfo: 'Xfo',
      ikGoalReferenceXfo: 'Xfo',
      ikblendId: 'Integer',
      hubId: 'Integer'
    },
    constructor: FABRIC.RT.Limb
  });
});



FABRIC.SceneGraph.CharacterSolvers.registerSolver('ArmSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
        rigNode: undefined
      });
    
    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode()),
      bones = skeletonNode.pub.getBones(),
      referencePose = skeletonNode.pub.getReferencePose(),
      referenceLocalPose = skeletonNode.pub.getReferenceLocalPose(),
      name = options.name,
      handControlXfo,
      ankleOffsetXfo,
      ankleTipXfo,
      i,
      twistManipulators = [];

  var arms = [];
  for(j=0; j<options.limbs.length; j++){
      var boneIDs = solver.generateBoneMapping(options.limbs[j], [['bones'], 'clavicle', 'wrist']);
      
    /*
      // check if the bones have a length
      for (i = 0; i < boneIDs.bones.length; i++) {
        if (bones[boneIDs.bones[i]].length <= 0.0) {
          throw ('Cannot solve IK2Bone for bone ' + bones[boneIDs.bones[i]].name + ', because it has length == 0.');
        }
        twistManipulators.push(true); //i != 1);
      }
      rigNode.pub.addSolver(name + 'fk', 'FKChainSolver', {
        chainManipulators: false,
        bones: options.bones,
        color: options.color,
        twistManipulators: twistManipulators,
        twistManipulatorRadius: bones[boneIDs.bones[0]].length * 0.3
      });
   */
    
      // compute the target
      handControlXfo = referencePose[boneIDs.wrist].clone();
      handControlXfo.tr = referencePose[boneIDs.wrist].transformVector(new FABRIC.RT.Vec3(bones[boneIDs.wrist].length, 0, 0));

      wristOffsetXfo = handControlXfo.inverse().multiply(referencePose[boneIDs.wrist]);
      
      var xfos = [];
      for(var i=0; i<boneIDs.bones.length; i++){
        xfos.push(referenceLocalPose[boneIDs.bones[i]]);
      }
      xfos.push(referenceLocalPose[boneIDs.wrist]);
      var xfoIds = rigNode.addVariable('Xfo[]', xfos);
      var handControlXfoId = rigNode.addVariable('Xfo', handControlXfo);
      var ikblendId = rigNode.addVariable('Scalar', 1.0);
      
      var arm = new FABRIC.RT.Limb(boneIDs.bones, boneIDs.wrist, xfoIds, handControlXfoId, wristOffsetXfo, handControlXfo, ikblendId);
      arms.push(arm);
      if (options.createManipulators) {
        // add a manipulation for target and upvector
        solver.constructManipulator(name+j+'Foot', 'XfoManipulator', {
          rigNode: rigNode.pub,
          xfoIndex: handControlXfoId,
          geometryNode: scene.pub.constructNode('Cross', { size: bones[boneIDs.wrist].length * 0.5 }),
          color: FABRIC.RT.rgb(1, 0, 0),
          baseManipulatorType: 'CharacterManipulator',
          targetName: solver.getName()+j+'IKGoal',
          size: 1.0,
          radius: 1.0
        });
        /*
        solver.constructManipulator(name + 'FootRotate', 'PivotRotationManipulator', {
          parentNode: variablesNode.pub,
          parentMember: name + 'handControlXfo',
          radius: bones[boneIDs.wrist].length,
          geometryNode: scene.pub.constructNode('Rectangle', {
            length: bones[boneIDs.wrist].length * 2,
            width: bones[boneIDs.wrist].length
          }),
          color: FABRIC.RT.rgb(0, 0, 1)
        });
  
        solver.constructManipulator(name + 'AnkleRotate', 'BoneManipulator', {
          targetNode: variablesNode.pub,
          targetMember: name + 'wristIKAnimationXfo',
          parentMember: name + 'handControlXfo',
          length: bones[boneIDs.wrist].length * 2.0,
          boneVector: new FABRIC.RT.Vec3(-1, 0, 0),
          color: FABRIC.RT.rgb(0, 0, 1)
        });
        */
        
        for (i = 0; i < boneIDs.bones.length; i++) {
          
          
          
        }
      }
    }
    
    skeletonNode.addMember('arms', 'Limb[]', arms);
    rigNode.addSolverOperator({
      operatorName: 'solveArmRig',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveArmRig.kl',
      entryFunctionName: 'solveArmRig',
      parameterLayout: [
        'charactercontroller.xfo<>',
        'self.pose',
        'skeleton.bones',
        'skeleton.arms',
        'variables.poseVariables<>',
        'self.index',
        'self.debugGeometry'
      ]
    });
    
    solver.generateTracks = function(trackSet, trackBindings){
      for(j=0; j<arms.length; j++){
        var color = FABRIC.RT.rgb(1, 0, 0);
        var storeEulerAngles = false;
        
        
        trackSet.addXfoTrack(solver.getName()+j+'IKGoal', color, storeEulerAngles, trackBindings, arms[j].ikGoalXfoId);
        /*
        for(i=0; i<arms[j].xfoIds.length; i++){
          if(arms[j].wristId > 0 && i==arms[j].xfoIds.length-1){
            trackSet.addXfoTrack(solver.getName()+j+'Ankle', color, storeEulerAngles, trackBindings, arms[j].xfoIds[i]);
          }else{
            trackSet.addXfoTrack(solver.getName()+j+'Bone'+i, color, storeEulerAngles, trackBindings, arms[j].xfoIds[i]);
          }
        }
        */
        trackSet.addScalarTrack(solver.getName()+j+'IKBlend', color, trackBindings, arms[j].ikblendId);
      }
    }
 
 
    solver.setPose = function(pose, variablesNode) {
      for( var i=0; i<arms.length; i++){
        var arm = arms[i];
      //  var xfoValues = [];
      //  for (var j = 0; j < arm.boneIds.length; j++) {
      //    if (bones[arm.boneIds[j]].parent == - 1)
      //      xfoValues[j] = pose[arm.boneIds[j]];
      //    else
      //      xfoValues[j] = pose[bones[arm.boneIds[j]].parent].inverse().multiply(pose[arm.boneIds[j]]);
      //  }
      //  xfoValues[arm.boneIds.length] = pose[bones[arm.wristId].parent].inverse().multiply(pose[arm.wristId]);
      //  
      //  variablesNode.setValues(xfoValues, arm.xfoIds);
        
        var wristTipXfo = pose[arm.wristId];
        wristTipXfo.tr = wristTipXfo.transformVector(new FABRIC.RT.Vec3(bones[arm.wristId].length, 0.0, 0.0));
        var ikGoalXfo = wristTipXfo.multiply(arm.ikGoalOffsetXfo.inverse());
        variablesNode.setValue(ikGoalXfo, arm.ikGoalXfoId);
      }
    };
 
    
    solver.invert = function(variablesNode){
      variablesNode.getDGNode().bindings.append(scene.constructOperator({
          operatorName: 'invertHumanoidLegRig',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveLegRig.kl',
          entryFunctionName: 'invertHumanoidLegRig',
          parameterLayout: [
            'sourcerig.pose',
            'skeleton.bones',
            'skeleton.arms',
            'self.poseVariables',
            'sourcerig.debugGeometry'
          ]
        }));
    }
    
    
    return solver; 
  }
});




FABRIC.SceneGraph.CharacterSolvers.registerSolver('InsectLegSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
        rigNode: undefined
      });
    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode()),
      bones = skeletonNode.pub.getBones(),
      referencePose = skeletonNode.pub.getReferencePose(),
      referenceLocalPose = skeletonNode.pub.getReferenceLocalPose(),
      name = options.name,
      footControlXfo,
      lastBoneOffsetXfo,
      lastBoneTipXfo,
      i, j;
      
    var legs = [];
    for(j=0; j<options.limbs.length; j++){
      var boneIDs = solver.generateBoneMapping(options.limbs[j], [['bones']]);
      var lastBoneId = boneIDs.bones[boneIDs.bones.length-1];
      
      // compute the target
      lastBoneTipXfo = referencePose[lastBoneId].clone();
      lastBoneTipXfo.tr = referencePose[lastBoneId].transformVector(new FABRIC.RT.Vec3(bones[lastBoneId].length, 0, 0));
      footControlXfo = lastBoneTipXfo.clone();
      footControlXfo.tr.y = 0;
      var alignmentQuat = new FABRIC.RT.Quat();
      alignmentQuat.setFrom2Vectors(
        footControlXfo.ori.rotateVector(new FABRIC.RT.Vec3(1, 0, 0)),
        new FABRIC.RT.Vec3(0, -1, 0)
      );
      footControlXfo.ori = alignmentQuat.multiply(footControlXfo.ori);
      lastBoneOffsetXfo = footControlXfo.inverse().multiply(lastBoneTipXfo);
      
      var xfos = [];
      for(var i=0; i<boneIDs.bones.length; i++){
        xfos.push(referenceLocalPose[boneIDs.bones[i]]);
      }
      var xfoIds = rigNode.addVariable('Xfo[]', xfos);
      var footControlXfoId = rigNode.addVariable('Xfo', footControlXfo);
      var ikblendId = rigNode.addVariable('Scalar', 1.0);
      
      var leg = new FABRIC.RT.Limb(boneIDs.bones, -1, xfoIds, footControlXfoId, lastBoneOffsetXfo, footControlXfo, ikblendId);
      if(options.hubIds){
        leg.hubId = options.hubIds[j];
        var hubs = skeletonNode.getData('hubs');
        leg.ikGoalReferenceXfo = bones[hubs[leg.hubId].boneId].referencePose.inverse().multiply(footControlXfo);
      }
      legs.push(leg);
    }
    
    skeletonNode.addMember('legs', 'Limb[]', legs);
    rigNode.addSolverOperator({
      operatorName: 'solveInsectLegRig',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveLegRig.kl',
      entryFunctionName: 'solveInsectLegRig',
      parameterLayout: [
        'charactercontroller.xfo<>',
        'self.pose',
        'skeleton.bones',
        'skeleton.legs',
        'variables.poseVariables<>',
        'self.index',
        'self.debugGeometry'
      ]
    });
 
    solver.invert = function(variablesNode){
      variablesNode.getDGNode().bindings.append(scene.constructOperator({
        operatorName: 'invertInsectLegRig',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveLegRig.kl',
        entryFunctionName: 'invertInsectLegRig',
        parameterLayout: [
          'sourcerig.pose',
          'skeleton.bones',
          'skeleton.legs',
          'self.poseVariables',
          'sourcerig.debugGeometry'
        ]
      }));
    }
    return solver; 
  }
});


FABRIC.SceneGraph.CharacterSolvers.registerSolver('HumanoidLegSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
        rigNode: undefined
      });
    
    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode()),
      bones = skeletonNode.pub.getBones(),
      referencePose = skeletonNode.pub.getReferencePose(),
      referenceLocalPose = skeletonNode.pub.getReferenceLocalPose(),
      name = solver.getName(),
      footControlXfo,
      ankleOffsetXfo,
      ankleTipXfo,
      i,
      twistManipulators = [];

  var legs = [];
  for(j=0; j<options.limbs.length; j++){
      var boneIDs = solver.generateBoneMapping(options.limbs[j], [['bones'], 'ankle']);
      
  /*
      // check if the bones have a length
      for (i = 0; i < boneIDs.bones.length; i++) {
        if (bones[boneIDs.bones[i]].length <= 0.0) {
          throw ('Cannot solve IK2Bone for bone ' + bones[boneIDs.bones[i]].name + ', because it has length == 0.');
        }
        twistManipulators.push(true); //i != 1);
      }
      rigNode.pub.addSolver(name + 'fk', 'FKChainSolver', {
        chainManipulators: false,
        bones: options.bones,
        color: options.color,
        twistManipulators: twistManipulators,
        twistManipulatorRadius: bones[boneIDs.bones[0]].length * 0.3
      });
   */
  
      // compute the target
      ankleTipXfo = referencePose[boneIDs.ankle].clone();
      ankleTipXfo.tr = referencePose[boneIDs.ankle].transformVector(new FABRIC.RT.Vec3(bones[boneIDs.ankle].length, 0, 0));
      footControlXfo = ankleTipXfo.clone();
      footControlXfo.tr.y = 0;
      var alignmentQuat = new FABRIC.RT.Quat();
      alignmentQuat.setFrom2Vectors(
        footControlXfo.ori.rotateVector(new FABRIC.RT.Vec3(1, 0, 0)),
        new FABRIC.RT.Vec3(0, -1, 0)
      );
      footControlXfo.ori = alignmentQuat.multiply(footControlXfo.ori);
      ankleOffsetXfo = footControlXfo.inverse().multiply(ankleTipXfo);
      
      var xfos = [];
      for(var i=0; i<boneIDs.bones.length; i++){
        xfos.push(referenceLocalPose[boneIDs.bones[i]]);
      }
      xfos.push(referenceLocalPose[boneIDs.ankle]);
      var xfoIds = rigNode.addVariable('Xfo[]', xfos);
      var footControlXfoId = rigNode.addVariable('Xfo', footControlXfo);
      var ikblendId = rigNode.addVariable('Scalar', 1.0);
      
      var leg = new FABRIC.RT.Limb(boneIDs.bones, boneIDs.ankle, xfoIds, footControlXfoId, ankleOffsetXfo, footControlXfo, ikblendId);
      legs.push(leg);
      if (options.createManipulators) {
        // add a manipulation for target and upvector
        solver.constructManipulator(name+j+'Foot', 'XfoManipulator', {
          rigNode: rigNode.pub,
          xfoIndex: footControlXfoId,
          geometryNode: scene.pub.constructNode('Cross', { size: bones[boneIDs.ankle].length * 0.5 }),
          color: FABRIC.RT.rgb(1, 0, 0),
          baseManipulatorType: 'CharacterManipulator',
          targetName: solver.getName()+j+'IKGoal',
          size: 1.0,
          radius: 1.0
        });
        
        solver.constructManipulator(name+j+'FootRotate', 'PivotRotationManipulator', {
          rigNode: rigNode.pub,
          xfoIndex: footControlXfoId,
          baseManipulatorType: 'CharacterManipulator',
          targetName: name+j+'IKGoal',
          radius: bones[boneIDs.ankle].length,
          geometryNode: scene.pub.constructNode('Rectangle', {
            length: bones[boneIDs.ankle].length * 2,
            width: bones[boneIDs.ankle].length
          }),
          color: FABRIC.RT.rgb(0, 0, 1),
          localXfo: new FABRIC.RT.Xfo({
            ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0, 0, 1), Math.HALF_PI)
          })
        });
  
        /*
        solver.constructManipulator(name + 'AnkleRotate', 'BoneManipulator', {
          rigNode: rigNode.pub,
          targetMember: name + 'ankleIKAnimationXfo',
          parentMember: name + 'footControlXfo',
          length: bones[boneIDs.ankle].length * 2.0,
          boneVector: new FABRIC.RT.Vec3(-1, 0, 0),
          color: FABRIC.RT.rgb(0, 0, 1)
        });
        */
      }
    }
    
    skeletonNode.addMember('legs', 'Limb[]', legs);
    rigNode.addSolverOperator({
      operatorName: 'solveHumanoidLegRig',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveLegRig.kl',
      entryFunctionName: 'solveHumanoidLegRig',
      parameterLayout: [
        'charactercontroller.xfo<>',
        'self.pose',
        'skeleton.bones',
        'skeleton.legs',
        'variables.poseVariables<>',
        'self.index',
        'self.debugGeometry'
      ]
    });
    
    solver.generateTracks = function(trackSet, trackBindings){
      for(j=0; j<legs.length; j++){
        var color = FABRIC.RT.rgb(1, 0, 0);
        var storeEulerAngles = false;
        
        
        trackSet.addXfoTrack(solver.getName()+j+'IKGoal', color, storeEulerAngles, trackBindings, legs[j].ikGoalXfoId);
        /*
        for(i=0; i<legs[j].xfoIds.length; i++){
          if(legs[j].ankleId > 0 && i==legs[j].xfoIds.length-1){
            trackSet.addXfoTrack(solver.getName()+j+'Ankle', color, storeEulerAngles, trackBindings, legs[j].xfoIds[i]);
          }else{
            trackSet.addXfoTrack(solver.getName()+j+'Bone'+i, color, storeEulerAngles, trackBindings, legs[j].xfoIds[i]);
          }
        }
        */
        trackSet.addScalarTrack(solver.getName()+j+'IKBlend', color, trackBindings, legs[j].ikblendId);
      }
    }
 
 
    solver.setPose = function(pose, variablesNode) {
      for( var i=0; i<legs.length; i++){
        var leg = legs[i];
      //  var xfoValues = [];
      //  for (var j = 0; j < leg.boneIds.length; j++) {
      //    if (bones[leg.boneIds[j]].parent == - 1)
      //      xfoValues[j] = pose[leg.boneIds[j]];
      //    else
      //      xfoValues[j] = pose[bones[leg.boneIds[j]].parent].inverse().multiply(pose[leg.boneIds[j]]);
      //  }
      //  xfoValues[leg.boneIds.length] = pose[bones[leg.ankleId].parent].inverse().multiply(pose[leg.ankleId]);
      //  
      //  variablesNode.setValues(xfoValues, leg.xfoIds);
        
        var ankleTipXfo = pose[leg.ankleId];
        ankleTipXfo.tr = ankleTipXfo.transformVector(new FABRIC.RT.Vec3(bones[leg.ankleId].length, 0.0, 0.0));
        var ikGoalXfo = ankleTipXfo.multiply(leg.ikGoalOffsetXfo.inverse());
        variablesNode.setValue(ikGoalXfo, leg.ikGoalXfoId);
      }
    };
 
    
    solver.invert = function(variablesNode){
      variablesNode.getDGNode().bindings.append(scene.constructOperator({
          operatorName: 'invertHumanoidLegRig',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveLegRig.kl',
          entryFunctionName: 'invertHumanoidLegRig',
          parameterLayout: [
            'sourcerig.pose',
            'skeleton.bones',
            'skeleton.legs',
            'self.poseVariables',
            'sourcerig.debugGeometry'
          ]
        }));
    }
    
    
    return solver; 
  }
});


FABRIC.RT.Hub = function(boneId, xfoId, hubParentSpaceId, spineBoneIds, spineBoneXfoId) {
  this.boneId = boneId != undefined ? boneId : -1;
  this.xfoId = xfoId != undefined ? xfoId : -1;
  this.hubParentSpaceId = hubParentSpaceId != undefined ? hubParentSpaceId : -1;
  this.spineBoneIds = spineBoneIds != undefined ? spineBoneIds : [];
  this.spineBoneXfoId = spineBoneXfoId != undefined ? spineBoneXfoId : [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('Hub', {
    members: {
      boneId: 'Integer',
      xfoId: 'Integer',
      hubParentSpaceId: 'Integer',
      spineBoneIds: 'Integer[]',
      spineBoneXfoId: 'Integer[]'
    },
    constructor: FABRIC.RT.Hub
  });
});

FABRIC.SceneGraph.CharacterSolvers.registerSolver('HubSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
        rigNode: undefined
      });
    
    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode()),
      bones = skeletonNode.pub.getBones(),
      referencePose = skeletonNode.pub.getReferencePose(),
      referenceLocalPose = skeletonNode.pub.getReferenceLocalPose(),
      name = options.name;
    
    var hubs = [];
    for(i=0; i<options.hubs.length; i++){
      var boneIDs = solver.generateBoneMapping(options.hubs[i], ['bone', 'parentSpaceBone', ['spineBones']]);
      var hubXfo = referencePose[boneIDs.bone];
      var spineBoneXfos = [];
      var spineBoneXfoIds;
      if(boneIDs.spineBones){
        for(var j=0; j<boneIDs.spineBones.length; j++){
          spineBoneXfos.push(bones[boneIDs.spineBones[j]].referenceLocalPose);
        }
        spineBoneXfoIds = rigNode.addVariable('Xfo[]', spineBoneXfos);
        hubXfo = referencePose[bones[boneIDs.spineBones[0]].parent].inverse().multiply(hubXfo);
      }
      
      var hubXfoId = rigNode.addVariable('Xfo', hubXfo);
      var hub = new FABRIC.RT.Hub(boneIDs.bone, hubXfoId, boneIDs.parentSpaceBone, boneIDs.spineBones, spineBoneXfoIds);
      hubs.push(hub);
      
      if (options.createManipulators) {
        solver.constructManipulator(name + 'Hub'+i, 'XfoManipulator', {
          baseManipulatorType: 'CharacterManipulator',
          rigNode: rigNode.pub,
          xfoIndex: hubXfoId,
          targetName: solver.getName()+i+'Xfo',
          color: FABRIC.RT.rgb(1, 0, 0),
          size: 1,
          radius: 1,
          structIndex: i,
          attachmentOperator:{
            operatorName: 'calcHubManipulatorAttachmentXfo',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveHubRig.kl',
            entryFunctionName: 'calcHubManipulatorAttachmentXfo',
            parameterLayout: [
              'skeleton.bones',
              'skeleton.hubs',
              'rig.pose',
              'charactercontroller.xfo',
              'variables.poseVariables',
              'self.structIndex',
              'self.localXfo',
              'self.parentXfo',
              'self.targetXfo',
              'self.globalXfo'
            ]
          }
        });
      }
    }
    skeletonNode.addMember('hubs', 'Hub[]', hubs);
    rigNode.addSolverOperator({
      operatorName: 'solveHubRigs',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveHubRig.kl',
      entryFunctionName: 'solveHubRigs',
      parameterLayout: [
        'charactercontroller.xfo<>',
        'self.pose',
        'skeleton.bones',
        'skeleton.hubs',
        'variables.poseVariables<>',
        'self.index',
        'self.debugGeometry'
      ]
    });
    
    solver.generateTracks = function(trackSet, trackBindings){
      for(i=0; i<hubs.length; i++){
        var color = FABRIC.RT.rgb(1, 0, 0);
        var storeEulerAngles = false;
        trackSet.addXfoTrack(solver.getName()+i+'Xfo', color, storeEulerAngles, trackBindings, hubs[i].xfoId);
      }
    }
    
    solver.setPose = function(pose, variablesNode) {
      for(var i=0; i < hubs.length; i++){
        var hub = hubs[i];
        var xfo;
        if(hub.spineBoneIds.length > 0){
          if(hub.hubParentSpaceId == -1){
          //  xfo = pose[hub.boneId];
            xfo = pose[bones[hub.spineBoneIds[0]].parent].inverse().multiply(pose[hub.boneId]);
          }
          else
            xfo = pose[hub.hubParentSpaceId].inverse().multiply(pose[hub.boneId]);
        }
        else{
          // TODO: Re-impliment hubs with parents. This made animation pre processing more cmoplex once I added the RigRoot node.
       //   if (bones[hub.boneId].parent == -1)
            xfo = pose[hub.boneId];
       //   else
       //     poseVariables.xfoValues[hub.xfoId] = pose[bones[hub.boneId].parent].inverse() * pose[hub.boneId];
        }
        variablesNode.setValue(xfo, hub.xfoId);
        if(hub.spineBoneIds.length > 0){
          var spineXfos = [];
          for (var j = 0; j < hub.spineBoneIds.size; j++) {
            if (bones[hub.spineBoneIds[j]].parent == - 1)
              spineXfos[j] = pose[hub.spineBoneIds[j]];
            else
              spineXfos[j] = pose[bones[hub.spineBoneIds[j]].parent].inverse().multiply(pose[hub.spineBoneIds[j]]);
          }
          variablesNode.setValues(spineXfos, hub.spineBoneIds);
        }
      }
    }
    
    solver.invert = function(variablesNode){
      variablesNode.getDGNode().bindings.append(scene.constructOperator({
        operatorName: 'invertHubRigs',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveHubRig.kl',
        entryFunctionName: 'invertHubRigs',
        parameterLayout: [
          'sourcerig.pose',
          'skeleton.bones',
          'skeleton.hubs',
          'self.poseVariables',
          'sourcerig.debugGeometry'
        ]
      }));
    }
    
    
    return solver; 
  }
});


