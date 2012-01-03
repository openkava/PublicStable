
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
      name = options.name;

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
    
    return solver;
  }
});

/*
FABRIC.SceneGraph.CharacterSolvers.registerSolver('ReferencePoseSolver',{
  constructSolver: function(options, scene) {

    options.bones = [];
    options.identifiers = [];

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      constantsNode = scene.getPrivateInterface(rigNode.pub.getConstantsNode()),
      skeletonNode = rigNode.getSkeletonNode(),
      bones = skeletonNode.getBones(),
      name = options.name,
      boneIndices = options.boneIndices;

    // if no id list is specified,
    // create a complete one
    if (!boneIndices) {
      boneIndices = [];
      for (var i = 0; i < bones.length; i++) {
        boneIndices.push(i);
      }
    }

    constantsNode.addMember(name + 'boneIndices', 'Integer[]', boneIndices);

    rigNode.addSolverOperator({
        operatorName: 'solveReferencePose',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveReferencePose.kl',
        entryFunctionName: 'solveReferencePose',
        parameterLayout: [
          'self.pose',
          'skeleton.bones',
          'constants.' + name + 'boneIndices'
        ]
      });

    return solver;
  }
});


// An FK Chain assumes a linear list of bones that can be
// connected together with a set of chained manipulators.
FABRIC.SceneGraph.CharacterSolvers.registerSolver('FKChainSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
        color: FABRIC.RT.rgb(0, 0.6, 0),
        chainManipulators: true,
        twistManipulators: true,
        twistManipulatorRadius: undefined
      });

    options.identifiers = [['bones']];

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      constantsNode = scene.getPrivateInterface(rigNode.pub.getConstantsNode()),
      skeletonNode = rigNode.pub.getSkeletonNode(),
      bones = skeletonNode.getBones(),
      referenceLocalPose = skeletonNode.getReferenceLocalPose(),
      referencePose = skeletonNode.getReferencePose(),
      boneIDs = solver.getBoneIDs(),
      size,
      name = options.name,
      i,
      manipulators = [],
      chainLocalReferencePose = [],
      chainGlobalReferencePose = [];

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

    constantsNode.addMember(name + 'boneIndices', 'Integer[]', boneIDs.bones);
    rigNode.addVariable(name + 'localXfos', 'Xfo[]', chainLocalReferencePose);
    // This member is used only to bind the manipulators to.
    rigNode.addMember(name + 'globalXfos', 'Xfo[]', chainGlobalReferencePose);

    rigNode.addSolverOperator({
        operatorName: 'solveChain',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveFKHierarchy.kl',
        entryFunctionName: 'solveChain',
        parameterLayout: [
          'self.pose',
          'self.' + name + 'globalXfos',
          'skeleton.bones',
          'constants.' + name + 'boneIndices',
          'variables.' + name + 'localXfos'
        ]
      });

    if (options.createManipulators) {
      for (i = 0; i < boneIDs.bones.length; i++) {
        manipulators.push(solver.constructManipulator(name + 'FKChainManipulator' + i, 'BoneManipulator', {
          targetNode: variablesNode.pub,
          targetMember: name + 'localXfos',
          targetMemberIndex: i,
          parentNode: rigNode.pub,
          parentMember: (i > 0 ? name + 'globalXfos' : 'pose'),
          parentMemberIndex: (i > 0 ? i - 1 : bones[boneIDs.bones[i]].parent),
          length: bones[boneIDs.bones[i]].length,
          color: FABRIC.RT.rgb(0, 0, 1)
        }));

        if (options.twistManipulators == true || options.twistManipulators[i] == true) {
          solver.constructManipulator(name + 'FKChainTwistManipulator' + i, 'RotationManipulator', {
            targetNode: variablesNode.pub,
            targetMember: name + 'localXfos',
            targetMemberIndex: i,
            parentNode: rigNode.pub,
            parentMember: (i > 0 ? name + 'globalXfos' : 'pose'),
            parentMemberIndex: (i > 0 ? i - 1 : bones[boneIDs.bones[i]].parent),
            localXfo: new FABRIC.RT.Xfo({
              ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0, 0, 1), -Math.HALF_PI)
            }),
            color: FABRIC.RT.rgb(0, .5, 0),
            radius: (options.twistManipulatorRadius ?
              options.twistManipulatorRadius : bones[boneIDs.bones[i]].length * 0.25)
          });
        }
      }
      if(options.chainManipulators){
        for (i = 0; i < manipulators.length; i++) {
          if(i>0){
            manipulators[i].setParentManipulatorNode( manipulators[i-1].pub );
          }
          if(i<manipulators.length-1){
            manipulators[i].setChildManipulatorNode( manipulators[i+1].pub );
          }
        }
      }
    }

    return solver;
  }
});
 */

/*
FABRIC.SceneGraph.CharacterSolvers.registerSolver('RootBoneSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
      manipulatorSize: 3
      });

    options.identifiers = ['bone'];

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      constantsNode = scene.getPrivateInterface(rigNode.pub.getConstantsNode()),
      variablesNode = scene.getPrivateInterface(rigNode.pub.getVariablesNode()),
      skeletonNode = rigNode.pub.getSkeletonNode(),
      bones = skeletonNode.getBones(),
      referencePose = skeletonNode.getReferencePose(),
      boneIDs = solver.getBoneIDs(),
      size = options.manipulatorSize,
      name = options.name;

    constantsNode.addMember(name + 'boneIndex', 'Integer', boneIDs.bone);
    rigNode.addVariable(name + 'rootXfo', 'Xfo', referencePose[boneIDs.bone]);

    rigNode.addSolverOperator({
        operatorName: 'solveFKBone',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveFKHierarchy.kl',
        entryFunctionName: 'solveFKBone',
        parameterLayout: [
          'self.pose',
          'skeleton.bones',
          'constants.' + name + 'boneIndex',
          'variables.' + name + 'rootXfo'
        ]
      });

    if (options.createManipulators) {

      solver.constructManipulator(name + 'Translation', 'ScreenTranslationManipulator', {
          parentNode: variablesNode.pub,
          parentMember: name + 'rootXfo',
          geometryNode: scene.pub.constructNode('BoundingBox', {
            bboxmin: new FABRIC.RT.Vec3(size * -0.3, size * -0.5, size * -0.5),
            bboxmax: new FABRIC.RT.Vec3(size * 0.3, size * 0.5, size * 0.5)
          }),
          color: FABRIC.RT.rgb(1, 0, 0)
        });

      solver.constructManipulator(name + 'Twist', 'RotationManipulator', {
          parentNode: variablesNode.pub,
          parentMember: name + 'rootXfo',
          localXfo: new FABRIC.RT.Xfo({ ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0, 0, 1), Math.HALF_PI) }),
          color: FABRIC.RT.rgb(0, .5, 0),
          radius: size
        });

      solver.constructManipulator(name + 'Alignment', 'BoneManipulator', {
          parentNode: variablesNode.pub,
          parentMember: name + 'rootXfo',
          length: size * 1.5,
          boneVector: new FABRIC.RT.Vec3(1, 0, 0),
          color: FABRIC.RT.rgb(0, 0, 1)
        });
    }

    return solver;
  }
});
 */

/*
FABRIC.SceneGraph.CharacterSolvers.registerSolver('IK2BoneSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
      });

    options.identifiers = ['boneA', 'boneB', 'targetParent', 'upvectorParent'];

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      constantsNode = scene.getPrivateInterface(rigNode.pub.getConstantsNode()),
      skeletonNode = rigNode.pub.getSkeletonNode(),
      bones = skeletonNode.getBones(),
      referencePose = skeletonNode.getReferencePose(),
      boneIDs = solver.getBoneIDs(),
      name = options.name,
      targetPos,
      target,
      lengthCenter,
      center,
      height,
      upvectorPos,
      upvector;

    // check if the bones have a length
    if (!bones[boneIDs.boneA].length) {
      throw ('Cannot solve IK2Bone for bone ' + bones[boneIDs.boneA].name + ', because it has length == 0.');
    }
    if (!bones[boneIDs.boneB].length) {
      throw ('Cannot solve IK2Bone for bone ' + bones[boneIDs.boneB].name + ', because it has length == 0.');
    }

    // compute the target
    targetPos = referencePose[boneIDs.boneB].transformVector(new FABRIC.RT.Vec3(bones[boneIDs.boneB].length, 0, 0));
    targetXfo = referencePose[boneIDs.targetParent].inverse().multiply(new FABRIC.RT.Xfo({ tr: targetPos }));

    // compute the upvector
    targetPos = targetPos.subtract(referencePose[boneIDs.boneA].tr);
    lengthCenter = targetPos.unit().dot(referencePose[boneIDs.boneA].ori.getXaxis()) * bones[boneIDs.boneB].length;
    center = targetPos.unit().multiplyScalar(lengthCenter).add(referencePose[boneIDs.boneA].tr);
    center = center.add(targetPos).add(referencePose[boneIDs.boneA].tr);
    center.multiplyScalar(0.5);

    height = referencePose[boneIDs.boneB].tr.subtract(center);
    upvectorPos = referencePose[boneIDs.boneB].tr.add(height).add(height);
    upvector = referencePose[boneIDs.upvectorParent].inverse().multiply(new FABRIC.RT.Xfo({ tr: upvectorPos }));
    
    if(options.projectTargetToUpvectorFactor != undefined) {
      upvector.tr = targetXfo.tr.multiplyScalar(options.projectTargetToUpvectorFactor);
    }

    constantsNode.addMember(name + 'boneA', 'Integer', boneIDs.boneA);
    constantsNode.addMember(name + 'boneB', 'Integer', boneIDs.boneB);
    constantsNode.addMember(name + 'targetParent', 'Integer', boneIDs.targetParent);
    constantsNode.addMember(name + 'upvectorParent', 'Integer', boneIDs.upvectorParent);

    rigNode.addVariable(name + 'target', 'Xfo', targetXfo);
    rigNode.addVariable(name + 'upvector', 'Xfo', upvector);

    rigNode.addVariable(name + 'directionBlend', 'Scalar',
      options.directionBlend != undefined ? options.directionBlend : 1.0);
    rigNode.addVariable(name + 'upvectorBlend', 'Scalar',
      options.upvectorBlend != undefined ? options.upvectorBlend : 1.0);

    rigNode.addSolverOperator({
        operatorName: 'solveIK2Bone',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveIK2Bone.kl',
        entryFunctionName: 'solveIK2Bone',
        parameterLayout: [
          'self.pose',
          'skeleton.bones',
          'constants.' + name + 'boneA',
          'constants.' + name + 'boneB',
          'constants.' + name + 'targetParent',
          'constants.' + name + 'upvectorParent',

          'variables.' + name + 'target',
          'variables.' + name + 'upvector',
          'variables.' + name + 'directionBlend',
          'variables.' + name + 'upvectorBlend'
        ]
      });

    if (options.createManipulators) {
      // add a manipulation for target and upvector
      solver.constructManipulator(name + 'targetScreen', 'ScreenTranslationManipulator', {
        targetNode: variablesNode.pub,
        targetMember: name + 'target',
        parentNode: rigNode.pub,
        parentMember: 'pose',
        parentMemberIndex: boneIDs['targetParent'],
        color: options.color,
        radius: options.radius
      });
      solver.constructManipulator(name + 'upvectorScreen', 'ScreenTranslationManipulator', {
        targetNode: variablesNode.pub,
        targetMember: name + 'upvector',
        parentNode: rigNode.pub,
        parentMember: 'pose',
        parentMemberIndex: boneIDs['upvectorParent'],
        color: options.color,
        radius: options.radius
      });
    }

    return solver;
  }
});
 */
/*
FABRIC.SceneGraph.CharacterSolvers.registerSolver('SpineSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
        createBaseManipulators: true,
        manipulatorSize: undefined
      });
    
    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = rigNode.pub.getSkeletonNode(),
      skeletonNode = scene.getPrivateInterface(skeletonNode.pub.getVariablesNode()),
      bones = skeletonNode.getBones(),
      referencePose = skeletonNode.getReferencePose(),
      boneIDs = solver.generateBoneMapping(options.bones, [['vertebra'], 'hub']),
      baseVertebreIndex = boneIDs.vertebra[0],
      size,
      name = options.name;

    // first, we will compute the local transform of the end inside the start's space
    var rootXfo = referencePose[skeletonNode.getParentId(baseVertebreIndex)];
    var startXfo = referencePose[baseVertebreIndex];
    var startlocalXfo = rootXfo ? rootXfo.projectInv(startXfo) : startXfo;
    var endXfo = referencePose[boneIDs.end];
    var endlocalXfo = rootXfo ? rootXfo.projectInv(endXfo) : startXfo.projectInv(endXfo);

    // check if we know the U values
    var uValues = [0];
    var endLength = startXfo.projectInv(endXfo).tr.norm();
    for (var i = 1; i < boneIDs.vertebra.length; i++) {
      var local = startXfo.projectInv(referencePose[boneIDs.vertebra[i]]);
      var u = local.tr.norm() / endLength;
      if (u > 1.0) {
        throw ("Unexpected U value, vertebra '" + bones[boneIDs.vertebra[i]].name + "' outside of  spine.");
      }
      uValues.push(u);
    }
    uValues.push(1.0);

    skeletonNode.addMember(name + 'end', 'Integer', boneIDs.end);
    skeletonNode.addMember(name + 'vertebra', 'Integer[]', boneIDs.vertebra);

    skeletonNode.addMember(name + 'uvalues', 'Scalar[]', uValues);
    skeletonNode.addMember(name + 'startTangent', 'Scalar', 0.3);
    skeletonNode.addMember(name + 'endTangent', 'Scalar', 0.3);

    var startXfoID = rigNode.addVariable(name + 'startlocalXfo', 'Xfo', startlocalXfo);
    var endXfoID = rigNode.addVariable(name + 'endlocalXfo', 'Xfo', endlocalXfo);
    
    skeletonNode.addMember(name + 'startXfoID', 'Integer', startLocalID);
    skeletonNode.addMember(name + 'endXfoID', 'Integer', endXfoID);

    rigNode.addSolverOperator({
        operatorName: 'solveSpine',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveSpine.kl',
        entryFunctionName: 'solveSpine',
        parameterLayout: [
          'self.pose',
          
          'skeleton.bones',
          'skeleton.' + name + 'end',
          'skeleton.' + name + 'vertebra',
          'skeleton.' + name + 'uvalues',
          'skeleton.' + name + 'startTangent',
          'skeleton.' + name + 'endTangent',
          'skeleton.' + name + 'startXfoID',
          'skeleton.' + name + 'endXfoID',
          
          'variables.poseVariables<>'
        ]
      });

    if (options.createManipulators) {
      size = options.manipulatorSize ? options.manipulatorSize :
        referencePose[baseVertebreIndex].tr.dist(referencePose[boneIDs['end']].tr) * 0.3;

      if (options.createBaseManipulators) {
        solver.constructManipulator(name + 'startRotation', 'RotationManipulator', {
            targetNode: rigNode.addVariable.pub,
            targetMember: 'poseVariables',
            targetMemberIndex: startXfoID,
            parentNode: rigNode.pub,
            parentMember: 'pose',
            parentMemberIndex: skeletonNode.getParentId(baseVertebreIndex),
            localXfo: FABRIC.RT.xfo({ ori: FABRIC.RT.Quat.makeFromAxisAndAngle(FABRIC.RT.vec3(0, 0, 1), Math.HALF_PI) }),
            color: FABRIC.RT.rgb(0, .5, 0),
            radius: size
          });

        solver.constructManipulator(name + 'startBoneRotation', 'BoneManipulator', {
            targetNode: rigNode.addVariable.pub,
            targetMember: 'poseVariables',
            targetMemberIndex: startXfoID,
            parentNode: rigNode.pub,
            parentMember: 'pose',
            parentMemberIndex: skeletonNode.getParentId(baseVertebreIndex),
            length: size * 1.5,
            boneVector: FABRIC.RT.vec3(1, 0, 0),
            color: FABRIC.RT.rgb(0, 0, 1)
          });

        // add two translations for the start and end as well
        solver.constructManipulator(name + 'startTranslation', 'ScreenTranslationManipulator', {
            targetNode: rigNode.addVariable.pub,
            targetMember: 'poseVariables',
            targetMemberIndex: startXfoID,
            parentNode: rigNode.pub,
            parentMember: 'pose',
            parentMemberIndex: skeletonNode.getParentId(baseVertebreIndex),
            geometryNode: scene.pub.constructNode('BoundingBox', {
              bboxmin: FABRIC.RT.vec3(size * -0.3, size * -0.5, size * -0.5),
              bboxmax: FABRIC.RT.vec3(size * 0.3, size * 0.5, size * 0.5)
            }),
            color: FABRIC.RT.rgb(1, 0, 0)
          });
      }
      solver.constructManipulator(name + 'endRotation', 'RotationManipulator', {
          targetNode: rigNode.addVariable.pub,
          targetMember: 'poseVariables',
          targetMemberIndex: endXfoID,
          parentNode: rigNode.pub,
          parentMember: 'pose',
          parentMemberIndex: skeletonNode.getParentId(baseVertebreIndex),
          localXfo: FABRIC.RT.xfo({ ori: FABRIC.RT.Quat.makeFromAxisAndAngle(FABRIC.RT.vec3(0, 0, 1), Math.HALF_PI) }),
          color: FABRIC.RT.rgb(0, .5, 0),
          radius: size
        });


      solver.constructManipulator(name + 'endBoneRotation', 'BoneManipulator', {
          targetNode: rigNode.addVariable.pub,
          targetMember: 'poseVariables',
          targetMemberIndex: endXfoID,
          parentNode: rigNode.pub,
          parentMember: 'pose',
          parentMemberIndex: skeletonNode.getParentId(baseVertebreIndex),
          length: size * 1.5,
          boneVector: FABRIC.RT.vec3(-1, 0, 0),
          color: FABRIC.RT.rgb(0, 0, 1)
        });


      solver.constructManipulator(name + 'endTranslation', 'ScreenTranslationManipulator', {
          targetNode: rigNode.addVariable.pub,
          targetMember: 'poseVariables',
          targetMemberIndex: endXfoID,
          parentNode: rigNode.pub,
          parentMember: 'pose',
          parentMemberIndex: skeletonNode.getParentId(baseVertebreIndex),
          geometryNode: scene.pub.constructNode('BoundingBox', {
            bboxmin: FABRIC.RT.vec3(size * -0.3, size * -0.5, size * -0.5),
            bboxmax: FABRIC.RT.vec3(size * 0.3, size * 0.5, size * 0.5)
          }),
          color: FABRIC.RT.rgb(1, 0, 0)
        });
    }

    return solver;
  },
  constructInverseSolver: function(options, scene) {
    
    
  }
});
*/


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

    
/*
FABRIC.SceneGraph.CharacterSolvers.registerSolver('NCFIKSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
        rigNode: undefined
      });

    options.identifiers = [['bones'], 'targetParent'];

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      constantsNode = scene.getPrivateInterface(rigNode.pub.getConstantsNode()),
      skeletonNode = rigNode.pub.getSkeletonNode(),
      bones = skeletonNode.getBones(),
      referencePose = skeletonNode.getReferencePose(),
      boneIDs = solver.getBoneIDs(),
      name = options.name,
      targetPos,
      targetXfo,
      i,
      lastBoneIndex;

    // check if the bones have a length
    for (i = 0; i < boneIDs.bones.length; i++) {
      if (bones[boneIDs.bones[i]].length <= 0.0) {
        throw ('Cannot solve IK2Bone for bone ' + bones[boneIDs.bones[i]].name + ', because it has length == 0.');
      }
    }

    lastBoneIndex = boneIDs.bones[boneIDs.bones.length - 1];

    // compute the target
    targetPos = referencePose[lastBoneIndex].transformVector(new FABRIC.RT.Vec3(bones[lastBoneIndex].length, 0, 0));
    targetXfo = referencePose[boneIDs.targetParent].inverse().multiply(new FABRIC.RT.Xfo({ tr: targetPos }));

    var targetXfoId = rigNode.addVariable('Xfo', targetXfo);
    
    constantsNode.addMember(name + 'boneIndices', 'Integer[]', boneIDs.bones);
    constantsNode.addMember(name + 'targetXfoId', 'Integer', targetXfoId);

    rigNode.addSolverOperator({
        operatorName: 'solveNCFIK',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveNCFIK.kl',
        entryFunctionName: 'solveNCFIK',
        parameterLayout: [
          'self.pose',
          'skeleton.bones',
          'constants.' + name + 'boneIndices',
          'constants.' + name + 'targetXfoId',
          
          'variables.poseVariables<>'
        ]
      });

    if (options.createManipulators) {
      // add a manipulation for target and upvector
      solver.constructManipulator(name + 'targetScreen', 'ScreenTranslationManipulator', {
        targetNode: variablesNode.pub,
        targetMember: name + 'target',
        parentNode: rigNode.pub,
        parentMember: 'pose',
        parentMemberIndex: boneIDs.targetParent
      });
    }
    return solver;
  }
});
*/



FABRIC.SceneGraph.CharacterSolvers.registerSolver('ArmSolver', {
  constructSolver: function(options, scene) {
    scene.assignDefaults(options, {
      });

    var solver = FABRIC.SceneGraph.CharacterSolvers.constructSolver('CharacterSolver', options, scene);

    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = rigNode.pub.getSkeletonNode(),
      bones = skeletonNode.getBones(),
      referencePose = skeletonNode.getReferencePose(),
      referenceLocalPose = skeletonNode.getReferenceLocalPose(),
      boneIDs = solver.generateBoneMapping(options.bones, [['bones']]),
      name = options.name,
      footPlatformXfo,
      wristOffsetXfo,
      i,
      lastBoneIndex,
      twistManipulators = [];

    // check if the bones have a length
    for (i = 0; i < boneIDs.bones.length; i++) {
      if (bones[boneIDs.bones[i]].length <= 0.0) {
        throw ('Cannot solve IK2Bone for bone ' + bones[boneIDs.bones[i]].name + ', because it has length == 0.');
      }
      twistManipulators.push(i != 0 /*&&  i != 2 */);
    }

    /*
    rigNode.pub.addSolver(name + 'fk', 'FKChainSolver', {
      chainManipulators: false,
      bones: options.bones,
      color: options.color,
      twistManipulators: twistManipulators,
      twistManipulatorRadius: bones[boneIDs.bones[0]].length * 0.3
    });
    */
    
    // compute the target
    var wristBoneIndex = boneIDs.bones[boneIDs.bones.length - 1];
    handControlXfo = referencePose[wristBoneIndex].clone();
    handControlXfo.tr = referencePose[wristBoneIndex].transformVector(new FABRIC.RT.Vec3(bones[wristBoneIndex].length, 0, 0));
    wristOffsetXfo = handControlXfo.inverse().multiply(referencePose[wristBoneIndex]);
    
    var handControlXfoId = rigNode.addVariable('Xfo', handControlXfo);
    var ikblendId = rigNode.addVariable('Scalar', 1.0);
    
    skeletonNode.addMember(name + 'bones', 'Integer[]', boneIDs.bones);
    skeletonNode.addMember(name + 'wristOffsetXfo', 'Xfo', wristOffsetXfo);
    skeletonNode.addMember(name + 'handControlXfoId', 'Integer', handControlXfoId);
    skeletonNode.addMember(name + 'ikblendId', 'Integer', ikblendId);
    
    rigNode.addSolverOperator({
        operatorName: 'solveArmRig',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/solveLimbIK.kl',
        entryFunctionName: 'solveArmRig',
        parameterLayout: [
          'self.pose',
          'skeleton.bones',
          'skeleton.' + name + 'bones',
          'skeleton.' + name + 'wristOffsetXfo',
          'skeleton.' + name + 'handControlXfoId',
          'skeleton.' + name + 'ikblendId',

          'variables.poseVariables<>'
        ]
      });

/*
    if (options.createManipulators) {
      var size = bones[wristBoneIndex].length;

      solver.constructManipulator(name + 'WristTwistRotation', 'RotationManipulator', {
          parentNode: variablesNode.pub,
          parentMember: 'poseVariables',
          parentMemberIndex: handControlXfoId,
          localXfo: new FABRIC.RT.Xfo({ ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0, 0, 1), Math.HALF_PI) }),
          color: FABRIC.RT.rgb(0, .5, 0),
          radius: size
        });

      solver.constructManipulator(name + 'WristRollRotation', 'RotationManipulator', {
          parentNode: variablesNode.pub,
          parentMember: 'poseVariables',
          parentMemberIndex: handControlXfoId,
          color: FABRIC.RT.rgb(0, .5, 0),
          radius: size
        });

      solver.constructManipulator(name + 'WristRotation', 'BoneManipulator', {
          parentNode: variablesNode.pub,
          parentMember: 'poseVariables',
          parentMemberIndex: handControlXfoId,
          length: size,
          boneVector: new FABRIC.RT.Vec3(1, 0, 0),
          color: FABRIC.RT.rgb(0, 0, 1)
        });

      // add a manipulation for target and upvector
      solver.constructManipulator(name + 'targetScreen', 'ScreenTranslationManipulator', {
        parentNode: variablesNode.pub,
        parentMember: 'poseVariables',
        parentMemberIndex: handControlXfoId,
        geometryNode: scene.pub.constructNode('BoundingBox', {
          bboxmin: new FABRIC.RT.Vec3(size * -0.8, size * -0.7, size * -0.4),
          bboxmax: new FABRIC.RT.Vec3(size * 0.8, size * 0.7, size * 0.4)
        }),
        color: FABRIC.RT.rgb(1, 0, 0)
      });
    }
 */

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
      footPlatformXfo,
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
      footPlatformXfo = lastBoneTipXfo.clone();
      footPlatformXfo.tr.y = 0;
      var alignmentQuat = new FABRIC.RT.Quat();
      alignmentQuat.setFrom2Vectors(
        footPlatformXfo.ori.rotateVector(new FABRIC.RT.Vec3(1, 0, 0)),
        new FABRIC.RT.Vec3(0, -1, 0)
      );
      footPlatformXfo.ori = alignmentQuat.multiply(footPlatformXfo.ori);
      lastBoneOffsetXfo = footPlatformXfo.inverse().multiply(lastBoneTipXfo);
      
      var xfos = [];
      for(var i=0; i<boneIDs.bones.length; i++){
        xfos.push(referenceLocalPose[boneIDs.bones[i]]);
      }
      var xfoIds = rigNode.addVariable('Xfo[]', xfos);
      var footPlatformXfoId = rigNode.addVariable('Xfo', footPlatformXfo);
      var ikblendId = rigNode.addVariable('Scalar', 1.0);
      
      var leg = new FABRIC.RT.Limb(boneIDs.bones, -1, xfoIds, footPlatformXfoId, lastBoneOffsetXfo, footPlatformXfo, ikblendId);
      if(options.hubIds){
        leg.hubId = options.hubIds[j];
        var hubs = skeletonNode.getData('hubs');
        leg.ikGoalReferenceXfo = bones[hubs[leg.hubId].boneId].referencePose.inverse().multiply(footPlatformXfo);
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
      name = options.name,
      footPlatformXfo,
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
      footPlatformXfo = ankleTipXfo.clone();
      footPlatformXfo.tr.y = 0;
      var alignmentQuat = new FABRIC.RT.Quat();
      alignmentQuat.setFrom2Vectors(
        footPlatformXfo.ori.rotateVector(new FABRIC.RT.Vec3(1, 0, 0)),
        new FABRIC.RT.Vec3(0, -1, 0)
      );
      footPlatformXfo.ori = alignmentQuat.multiply(footPlatformXfo.ori);
      ankleOffsetXfo = footPlatformXfo.inverse().multiply(ankleTipXfo);
      
      var xfos = [];
      for(var i=0; i<boneIDs.bones.length; i++){
        xfos.push(referenceLocalPose[boneIDs.bones[i]]);
      }
      xfos.push(referenceLocalPose[boneIDs.ankle]);
      var xfoIds = rigNode.addVariable('Xfo[]', xfos);
      var footPlatformXfoId = rigNode.addVariable('Xfo', footPlatformXfo);
      var ikblendId = rigNode.addVariable('Scalar', 1.0);
      
      var leg = new FABRIC.RT.Limb(boneIDs.bones, boneIDs.ankle, xfoIds, footPlatformXfoId, ankleOffsetXfo, footPlatformXfo, ikblendId);
      legs.push(leg);
      
      if (options.createManipulators) {
        // add a manipulation for target and upvector
        solver.constructManipulator(name + 'FootTranslate'+j, 'ScreenTranslationManipulator', {
          rigNode: rigNode.pub,
          xfoIndex: footPlatformXfoId,
          geometryNode: scene.pub.constructNode('Cross', { size: bones[boneIDs.ankle].length * 0.5 }),
          color: FABRIC.RT.rgb(1, 0, 0),
          baseManipulatorType: 'CharacterManipulator'
        });
  
        /*
        solver.constructManipulator(name + 'FootRotate', 'PivotRotationManipulator', {
          parentNode: variablesNode.pub,
          parentMember: name + 'footPlatformXfo',
          radius: bones[boneIDs.ankle].length,
          geometryNode: scene.pub.constructNode('Rectangle', {
            length: bones[boneIDs.ankle].length * 2,
            width: bones[boneIDs.ankle].length
          }),
          color: FABRIC.RT.rgb(0, 0, 1)
        });
  
        solver.constructManipulator(name + 'AnkleRotate', 'BoneManipulator', {
          targetNode: variablesNode.pub,
          targetMember: name + 'ankleIKAnimationXfo',
          parentMember: name + 'footPlatformXfo',
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


FABRIC.RT.Hub = function(boneId, xfoId, hubParentSpaceId, inSpineBoneIds, inSpineBoneXfoId) {
  this.boneId = boneId != undefined ? boneId : -1;
  this.xfoId = xfoId != undefined ? xfoId : -1;
  this.hubParentSpaceId = hubParentSpaceId != undefined ? hubParentSpaceId : -1;
  this.inSpineBoneIds = inSpineBoneIds != undefined ? inSpineBoneIds : [];
  this.inSpineBoneXfoId = inSpineBoneXfoId != undefined ? inSpineBoneXfoId : [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('Hub', {
    members: {
      boneId: 'Integer',
      xfoId: 'Integer',
      hubParentSpaceId: 'Integer',
      inSpineBoneIds: 'Integer[]',
      inSpineBoneXfoId: 'Integer[]'
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
    for(j=0; j<options.hubs.length; j++){
      var boneIDs = solver.generateBoneMapping(options.hubs[j], ['bone', 'parentSpaceBone', ['inSpineBones']]);
      var hubXfo = referencePose[boneIDs.bone];
      var hubXfoId = rigNode.addVariable('Xfo', hubXfo);
      var inSpineBoneXfos = [];
      var inSpineBoneXfoIds;
      if(boneIDs.inSpineBones){
        for(var i=0; i<boneIDs.inSpineBones.length; i++){
          inSpineBoneXfos.push(bones[boneIDs.inSpineBones[i]].referenceLocalPose);
        }
        inSpineBoneXfoIds = rigNode.addVariable('Xfo[]', inSpineBoneXfos);
      }
      
      var hub = new FABRIC.RT.Hub(boneIDs.bone, hubXfoId, boneIDs.parentSpaceBone, boneIDs.inSpineBones, inSpineBoneXfoIds);
      hubs.push(hub);
      
      if (options.createManipulators) {
          // add a manipulation for target and upvector
          solver.constructManipulator(name + 'HubTranslate'+j, 'ScreenTranslationManipulator', {
            rigNode: rigNode.pub,
            xfoIndex: hubXfoId,
            geometryNode: scene.pub.constructNode('Cross', { size: hubXfo.tr.y * 0.15 }),
            color: FABRIC.RT.rgb(1, 0, 0),
            baseManipulatorType: 'CharacterManipulator'
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


