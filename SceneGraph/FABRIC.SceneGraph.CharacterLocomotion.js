





FABRIC.Characters.COM = function(xfoId, parameterVarIds) {
  this.xfoId = xfoId != undefined ? xfoId : -1;
  this.parameterVarIds = parameterVarIds != undefined ? parameterVarIds : [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('COM', {
    members: {
      xfoId: 'Integer',
      parameterVarIds: 'Integer[]'
    },
    constructor: FABRIC.Characters.COM
  });
});


FABRIC.SceneGraph.CharacterSolvers.registerSolver('COMSolver', {
  constructSolver: function(options, scene) {
    
    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode()),
      solver = {};

    var comXfoId = rigNode.addVariable('Xfo');
    // stepFrequency
    var comParam0VarId = rigNode.addVariable('Scalar');
    // speed
    var comParam1VarId = rigNode.addVariable('Scalar');
    // gradient
    var comParam2VarId = rigNode.addVariable('Scalar');
    // direction
    var comParam3VarId = rigNode.addVariable('Scalar');
    
    var com = new FABRIC.Characters.COM(comXfoId, [comParam0VarId, comParam1VarId, comParam2VarId, comParam3VarId] );
    skeletonNode.addMember('com', 'COM', com);
    /*
    solver.addTracks = function(trackSet, keyframeTrackBindings){
      var comXfoTrackBindings = trackSet.addXfoTrack('com');
      keyframeTrackBindings.addXfoBinding(comXfoId, comXfoTrackBindings);
      
      var comParam0TrackId = trackSet.addScalarTrack('stepFrequency', FABRIC.RT.Color.yellow);
      keyframeTrackBindings.addScalarBinding(comParam0VarId, comParam0TrackId);
      
      var comParam1TrackId = trackSet.addScalarTrack('speed', FABRIC.RT.Color.yellow);
      keyframeTrackBindings.addScalarBinding(comParam1VarId, comParam1TrackId);
      
      var comParam2TrackId = trackSet.addScalarTrack('gradient', FABRIC.RT.Color.yellow);
      keyframeTrackBindings.addScalarBinding(comParam2VarId, comParam2TrackId);
      
      var comParam3TrackId = trackSet.addScalarTrack('direction', FABRIC.RT.Color.yellow);
      keyframeTrackBindings.addScalarBinding(comParam3VarId, comParam3TrackId);
    }
    */
    return solver; 
  }
});


FABRIC.Characters.LocomotionFoot = function(limbId, stepTimeVarId) {
  this.limbId = limbId != undefined ? limbId : -1;
  this.stepTimeVarId = stepTimeVarId != undefined ? stepTimeVarId : -1;
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('LocomotionFoot', {
    members: {
      limbId: 'Integer',
      stepTimeVarId: 'Integer'
    },
    constructor: FABRIC.Characters.LocomotionFoot
  });
});



FABRIC.SceneGraph.CharacterSolvers.registerSolver('LocomotionFeetSolver', {
  constructSolver: function(options, scene) {
    
    var rigNode = scene.getPrivateInterface(options.rigNode),
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode()),
      solver = {};

    var limbs = skeletonNode.getData('legs');
    var locomotionFeet = [];
    var stepTimeVarIds = [];
    for(var i=0; i<limbs.length; i++){
      var stepTimeVarId = rigNode.addVariable('Scalar');
      locomotionFeet.push(new FABRIC.Characters.LocomotionFoot(i, stepTimeVarId));
      stepTimeVarIds.push(stepTimeVarId);
    }
    skeletonNode.addMember('locomotionFeet', 'LocomotionFoot[]', locomotionFeet);
    /*
    solver.addTracks = function(trackSet, keyframeTrackBindings){
      for(var i=0; i<limbs.length; i++){
        var stepTimeTrackId = trackSet.addScalarTrack('foot'+i+'StepTime', FABRIC.RT.Color.yellow);
        keyframeTrackBindings.addScalarBinding(stepTimeVarIds[i], stepTimeTrackId);
      }
    }*/
    return solver; 
  }
});



FABRIC.SceneGraph.registerNodeType('LocomotionCharacterRig', {
  parentNodeDesc: 'CharacterRig',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      baseCharacterRig: undefined
    });
  
    if (!options.baseCharacterRigNode) {
      throw ('Must Provide a base rig.');
    }
    if (!options.baseCharacterRigNode.isTypeOf('CharacterRig')) {
      throw ('Incorrect type. Must be a CharacterRig');
    }
    var locomotionRigNode = scene.getPrivateInterface(options.baseCharacterRigNode);
  //  var locomotionRigNode = scene.constructNode('CharacterRig', options);
    
    locomotionRigNode.pub.addSolver('COM', 'COMSolver');
    locomotionRigNode.pub.addSolver('Feet', 'LocomotionFeetSolver');
    
    return locomotionRigNode;
  }});

FABRIC.RT.LocomotionMarker = function() {
  this.localtime = 0.0;
  this.params = [0,0,0];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('LocomotionMarker', {
    members: {
      localtime: 'Scalar',
      params: 'Scalar[3]'
    },
    constructor: FABRIC.RT.LocomotionMarker
  });
});

FABRIC.RT.FootStep = function() {
  this.liftTime = 0.0;
  this.plantTime = 0.0;
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('FootStep', {
    members: {
      liftTime: 'Scalar',
      plantTime: 'Scalar'
    },
    constructor: FABRIC.RT.FootStep
  });
});

FABRIC.RT.FootStepTrack = function() {
  this.footSteps = [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('FootStepTrack', {
    members: {
      footSteps: 'FootStep[]'
    },
    constructor: FABRIC.RT.FootStepTrack
  });
});


FABRIC.SceneGraph.registerNodeType('LocomotionAnimationLibrary', {
  parentNodeDesc: 'AnimationLibrary',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    
    var animationLibraryNode = scene.constructNode('LinearKeyAnimationLibrary', options);
    var dgnode = animationLibraryNode.getDGNode();
    dgnode.addMember('markers', 'LocomotionMarker[]');
    dgnode.addMember('footStepTracks', 'FootStepTrack[]');
    
    var paramsdgnode, debugGeometryDraw;
    animationLibraryNode.pub.preProcessTracks = function(
      sourceAnimationLibrary,
      keyframeTrackBindings,
      rigNode,
      sampleFrequency,
      footMovementThreshold,
      callback
    ){
      if (!sourceAnimationLibrary.isTypeOf('AnimationLibrary')) {
        throw ('Incorrect type. Must be a AnimationLibrary');
      }
      if (!rigNode.isTypeOf('CharacterRig')) {
        throw ('Incorrect type. Must be a CharacterRig');
      }
      rigNode = scene.getPrivateInterface(rigNode);
      sourceAnimationLibrary = scene.getPrivateInterface(sourceAnimationLibrary);
    //  variablesNode = scene.getPrivateInterface(rigNode.getVariablesNode());
      skeletonNode = scene.getPrivateInterface(rigNode.pub.getSkeletonNode());
      dgnode.setDependency(sourceAnimationLibrary.getDGNode(), 'sourceAnimationLibrary');
      dgnode.setDependency(skeletonNode.getDGNode(), 'skeleton');
    //  dgnode.setDependency(variablesNode.getDGNode(), 'variables');
      
      if(!paramsdgnode){
        paramsdgnode = animationLibraryNode.constructDGNode('ParamsDGNode');
        dgnode.setDependency(paramsdgnode, 'params');
        paramsdgnode.addMember('sampleFrequency', 'Scalar', sampleFrequency);
        paramsdgnode.addMember('footMovementThreshold', 'Scalar', footMovementThreshold);
        
        
        paramsdgnode.addMember('bindings', 'KeyframeTrackBindings', keyframeTrackBindings);
        paramsdgnode.addMember('poseVariables', 'PoseVariables', rigNode.getVariables());
        dgnode.addMember('bindings', 'KeyframeTrackBindings');
      }
      
    //  var trackSet = sourceAnimationLibrary.pub.getTrackSet(0);
      
    //  var comsolver = rigNode.addSolver('COM', 'COMSolver');
    //  comsolver.addTracks(trackSet, keyframeTrackBindings);
      
    //  var locomotionFeedSolver = rigNode.addSolver('Feet', 'LocomotionFeetSolver');
    //  locomotionFeedSolver.addTracks(trackSet, keyframeTrackBindings);
      
    //  variablesNode.setBindings(keyframeTrackBindings);
      
      if(!debugGeometryDraw){
        dgnode.addMember('debugGeometry', 'DebugGeometry' );
        debugGeometryDraw = scene.constructNode('DebugGeometryDraw', {
            dgnode: dgnode,
            debugGemetryMemberName: 'debugGeometry'
        });
      }
      
      // Clone the data from the sourceAnimationLibrary
    //  dgnode.setData('trackSet', 0, trackSet);
    //  dgnode.setCount(sourceAnimationLibrary.pub.getTrackSetCount());
    //  for(var i=0; i<trackSets.length; i++){
    //    dgnode.setData('trackSet', i, sourceAnimationLibrary.pub.getTrackSet());
    //  }
      
      dgnode.bindings.append(scene.constructOperator({
        operatorName: 'locomotionPreProcessing',
        srcFile: 'FABRIC_ROOT/SceneGraph/KL/locomotionPreProcessing.kl',
        entryFunctionName: 'locomotionPreProcessing',
        parameterLayout: [
          'sourceAnimationLibrary.trackSet<>',
          'params.poseVariables',
          'params.bindings',
                          
          'skeleton.bones',
          'skeleton.hubs',
          'skeleton.legs',
          'skeleton.com',
          'skeleton.locomotionFeet',
          
          'params.sampleFrequency',
          'params.footMovementThreshold',
          
          'self.trackSet',
          'self.bindings',
          'self.markers',
          'self.footStepTracks',
          'self.index',
          
          'self.debugGeometry'
        ],
        async: false
      }));
      
    //  dgnode.evaluate();
    //  dgnode.bindings.remove(0);
    //  dgnode.removeDependency('skeleton');
    }
    return animationLibraryNode;
  }});


FABRIC.Characters.CharacterControllerParams = function() {
  this.displacement = new FABRIC.RT.Xfo();
  this.displacementDir = new FABRIC.RT.Vec3();
  this.trail = [];
  this.trailLength = 0;
  this.trailCircularArrayIndex = 0;
  this.balanceXfo = new FABRIC.RT.Xfo();
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('CharacterControllerParams', {
    members: {
      displacement: 'Xfo',
      displacementDir: 'Vec3',
      trail: 'Xfo[]',
      trailLength: 'Scalar',
      trailCircularArrayIndex: 'Integer',
      balanceXfo: 'Xfo'
    },
    constructor: FABRIC.Characters.CharacterControllerParams
  });
});



FABRIC.SceneGraph.registerNodeType('LocomotionCharacterController', {
  parentNodeDesc: 'CharacterController',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      controllerXfo: new FABRIC.RT.Xfo(),
      controllerLinearVelocity: new FABRIC.RT.Vec3(),
      controllerAngularVelocity: new FABRIC.RT.Euler(),
      translationControls: new FABRIC.RT.Vec2(),
      orientationControls: new FABRIC.RT.Vec2(),
    
      maxLinearVelocity: new FABRIC.RT.Vec2(10.0, 10.0),
      maxAngularVelocity: new FABRIC.RT.Euler(0.0,0.1,0.0),
      maxLinearAcceleration: 1.0,
      maxAngularAcceleration: 1.0,
      
      cameraNode: undefined,
      
      trailLength: 120.0,
      numTrailSegments: 0,
      
      gravity: -25.0,
      comHeight: 15,
      circleSize: 20
    });
    var cameraNode = scene.getPrivateInterface(options.cameraNode);
    var characterControllerNode = scene.constructNode('CharacterController', options);
    var dgnode = characterControllerNode.getDGNode();
    
    var controllerparams = new FABRIC.Characters.CharacterControllerParams();
    if(options.numTrailSegments > 0){
      var trailDir = options.controllerXfo.ori.rotateVector(new FABRIC.RT.Vec3(0,0,-1));
      var trail = [];
      for(var i=options.numTrailSegments; i>0; i--){
        var trailxfo = options.controllerXfo.clone();
        trailxfo.tr = trailxfo.tr.add(trailDir.multiplyScalar((i/options.numTrailSegments) * options.trailLength));
        trail.push(trailxfo);
      }
      controllerparams.trail = trail;
      controllerparams.trailLength = options.trailLength;
      controllerparams.trailCircularArrayIndex = options.numTrailSegments-1;
    }
    dgnode.addMember('controllerparams', 'CharacterControllerParams', controllerparams);
    dgnode.addMember('liftVec', 'Vec3');
    
    dgnode.setDependency(scene.getGlobalsNode(), 'globals');
    dgnode.setDependency(cameraNode.getDGNode(), 'camera');
    
    dgnode.addMember('gravity', 'Scalar', options.gravity);
    dgnode.addMember('comHeight', 'Scalar', options.comHeight);
    dgnode.addMember('circleSize', 'Scalar', options.circleSize);
    
    dgnode.addMember('translationControls', 'Vec2', options.translationControls);
    dgnode.addMember('orientationControls', 'Vec2', options.orientationControls);
    
    characterControllerNode.addMemberInterface(dgnode, 'translationControls', true);
    characterControllerNode.addMemberInterface(dgnode, 'orientationControls', true);
    
    dgnode.addMember('maxLinearVelocity', 'Vec2', options.maxLinearVelocity);
    dgnode.addMember('maxAngularVelocity', 'Euler', options.maxAngularVelocity);
    dgnode.addMember('maxLinearAcceleration', 'Scalar', options.maxLinearAcceleration);
    dgnode.addMember('maxAngularAcceleration', 'Scalar', options.maxAngularAcceleration);
    
    dgnode.addMember('debugGeometry1', 'DebugGeometry' );
    var debugGeometryDraw = scene.constructNode('DebugGeometryDraw', {
        dgnode: dgnode,
        debugGemetryMemberName: 'debugGeometry1'
    });
    dgnode.addMember('debugGeometry2', 'DebugGeometry' );
    var debugGeometryDraw = scene.constructNode('DebugGeometryDraw', {
        dgnode: dgnode,
        debugGemetryMemberName: 'debugGeometry2'
    });
  
    dgnode.bindings.append(scene.constructOperator({
      operatorName: 'evaluateCharacterController',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/characterController.kl',
      entryFunctionName: 'evaluateCharacterController',
      parameterLayout: [
        'self.xfo',
        'self.controllerparams',
        'self.translationControls',
        'self.orientationControls',
    
        'self.maxLinearVelocity',
        'self.maxAngularVelocity',
        'self.maxLinearAcceleration',
        'self.maxAngularAcceleration',
        
        'self.gravity',
        'self.comHeight',
        'self.circleSize',
        
        'self.debugGeometry1',
        'self.debugGeometry2',
        
        'camera.cameraMat44',
        'camera.projectionMat44',
        
        'globals.timestep'
      ]
    }));
    
    return characterControllerNode;
  }});


FABRIC.RT.TrackSetController = function( activeTrackSet ) {
  this.activeTrackSet = activeTrackSet != undefined ? activeTrackSet : -1;
  this.time = 0.0;
  this.currKeys = [];
  this.trackWeight = 0.0;
  this.tick = 0;
  this.comParams = [];
  this.stepIds = [];
  this.deactivate = false;;
  this.pivotFoot = 0;
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('TrackSetController', {
    members: {
      activeTrackSet: 'Integer',
      time: 'Scalar',
      currKeys: 'Integer[]',
      trackWeight: 'Scalar',
      tick: 'Size',
      comParams: 'Scalar[]',
      stepIds: 'Integer[]',
      deactivate: 'Boolean',
      pivotFoot: 'Integer'
    },
    constructor: FABRIC.RT.TrackSetController
  });
});

FABRIC.SceneGraph.registerNodeType('LocomotionPoseVariables', {
  parentNodeDesc: 'CharacterVariables',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      bulletWorldNode: undefined
    });
    
    var locomotionVariables = scene.constructNode('CharacterVariables', options);
    var dgnode = locomotionVariables.getDGNode();
    
    if(options.bulletWorldNode){
      var bulletWorldNode = scene.getPrivateInterface(options.bulletWorldNode);
      dgnode.setDependency(bulletWorldNode.getDGNode(), 'bulletworld');
    }
    dgnode.addMember('trackcontroller', 'TrackSetController');
    dgnode.addMember('plantedFeet', 'Boolean[]');
    dgnode.addMember('plantLocations', 'Vec3[]');
    dgnode.addMember('rayDeltas', 'Vec3[]');
    
    var poseError = locomotionVariables.getVariables();
    poseError.setIdentity();
    dgnode.addMember('poseError', 'PoseVariables', poseError);
    dgnode.addMember('prevUpdatePose', 'PoseVariables', poseError);
    dgnode.addMember('clipActivationTime', 'Scalar', -1.0);
    
    dgnode.addMember('debugGeometry', 'DebugGeometry' );
    var debugGeometryDraw = scene.constructNode('DebugGeometryDraw', {
        dgnode: dgnode,
        debugGemetryMemberName: 'debugGeometry'
    });
    
    dgnode.setDependency(scene.getGlobalsNode(), 'globals');
  
    dgnode.bindings.append(scene.constructOperator({
      operatorName: 'evaluateLocomotionPoseVariables',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/locomotion.kl',
      entryFunctionName: 'evaluateLocomotionPoseVariables',
      parameterLayout: [
        'globals.timestep',
        'charactercontroller.xfo',
        'charactercontroller.controllerparams',
        
        'bulletworld.world',
        
        'animationlibrary.trackSet<>',
        'animationlibrary.markers<>',
        'animationlibrary.footStepTracks<>',
        
        'animationlibrary.bindings',
        'self.trackcontroller',
        
        'skeleton.hubs',
        'skeleton.legs',
        'skeleton.com',
        'skeleton.locomotionFeet',
        
        'self.plantedFeet',
        'self.plantLocations',
        'self.rayDeltas',
        
        'self.poseVariables',
        'self.prevUpdatePose',
        'self.poseError',
        'self.clipActivationTime',
        
        'self.debugGeometry'
      ]
    }));
    
    locomotionVariables.pub.setCharacterController = function(characterController){
      if (!characterController.isTypeOf('LocomotionCharacterController')) {
        throw ('Incorrect type assignment. Must assign a LocomotionCharacterController');
      }
      characterController = scene.getPrivateInterface(characterController);
      dgnode.setDependency(characterController.getDGNode(), 'charactercontroller');
    };
    
    locomotionVariables.pub.setAnimationLibrary = function(animationLibraryNode, keyframeTrackBindings){
      if (!animationLibraryNode.isTypeOf('AnimationLibrary')) {
        throw ('Incorrect type assignment. Must assign a AnimationLibrary');
      }
      animationLibraryNode = scene.getPrivateInterface(animationLibraryNode);
      dgnode.setDependency(animationLibraryNode.getDGNode(), 'animationlibrary');
      if(keyframeTrackBindings){
        dgnode.addMember('bindings', 'KeyframeTrackBindings', keyframeTrackBindings);
      }
    };
    
    var skeletonNode;
    locomotionVariables.pub.setSkeletonNode = function(node) {
      if (!node.isTypeOf('CharacterSkeleton')) {
        throw ('Incorrect type assignment. Must assign a CharacterSkeleton');
      }
      skeletonNode = scene.getPrivateInterface(node);
      dgnode.setDependency(skeletonNode.getDGNode(), 'skeleton');
    }
    locomotionVariables.pub.getSkeletonNode = function() {
      return scene.getPublicInterface(skeletonNode);
    };
    
    return locomotionVariables;
  }});

