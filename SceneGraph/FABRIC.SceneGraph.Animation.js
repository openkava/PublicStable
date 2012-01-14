
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//


FABRIC.SceneGraph.registerNodeType('Animation', {
  briefDesc: 'The AnimationTrack node implements an array of animation tracks.',
  detailedDesc: 'The AnimationTrack node is an abstract node type that stores an animation track per-slice. Drived nodes must specify the KeyframeType that is being interpollated',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    keyframetype: 'Specified by the derrived node, this option specifies the type of the keyframe tract that will be interpollated.',
    name:'The name of the track. This value is displayed in the curve Editor'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        keyframetype: undefined
      });
    
    if (options.keyframetype == undefined) {
      throw ('Please specify a type of data to interpollate');
    }
    var keyframeType = options.keyframetype;
    var keyframeTrackType = options.keyframetype+'Track';
    var rTypes = scene.getRegisteredTypesManager().getRegisteredTypes();
    if (!rTypes[options.keyframetype]) {
      throw ('Type "' + options.keyframetype + '" is not registered. Load the RT file.');
    }
    if (!rTypes[keyframeTrackType]) {
      throw ('Type "' + keyframeTrackType + '" is not registered. Load the RT file.');
    }
    var defaultKeyframeValue = rTypes[options.keyframetype].defaultValue;
    if (!defaultKeyframeValue.valueType) {
      throw ('The keyframe registered type must provide a ' +
          'valueType that indicates what the keyframes will evaluate to. ' +
          'Normally this is a Scalar, but could be a Vec2, Color or any other value');
    }
    
    var animationNode = scene.constructNode('SceneGraphNode', options);
    
    animationNode.pub.getKeyframeType = function() {
      return keyframeType;
    };
    animationNode.getTrackType = function() {
      return keyframeTrackType;
    };
    
    animationNode.pub.getValueType = function(trackid) {
      // To determing what kind of data this evaluator should evaluate to,
      // we look up the keyframe type in the type manager, and request the
      // default value, which when is created on demand, enables us to call the valueType
      return defaultKeyframeValue.valueType;
    };
     
    animationNode.getEvaluateCurveOperator = function() {
      throw "Derrived nodes must impliment this function.";
    };
    return animationNode;
  }
});



FABRIC.SceneGraph.registerNodeType('AnimationTrack', {
  briefDesc: 'The AnimationTrack node implements an array of animation tracks.',
  detailedDesc: 'The AnimationTrack node is an abstract node type that stores an animation track per-slice. Drived nodes must specify the KeyframeType that is being interpollated',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    keyframetype: 'Specified by the derrived node, this option specifies the type of the keyframe tract that will be interpollated.',
    name:'The name of the track. This value is displayed in the curve Editor'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        keyframetype: undefined
      });
    
    var animationTrackNode = scene.constructNode('Animation', options);
    var dgnode = animationTrackNode.constructDGNode('DGNode');
    dgnode.addMember('track', animationTrackNode.getTrackType());
    
    var firstTrackAdded = false;
    animationTrackNode.pub.addTrack = function(track) {
      var trackId;
      if(!firstTrackAdded){
        // Nodes default to having 1 slice, so we use up the first slot here.
        trackId = 0;
        firstTrackAdded = true;
      }
      else{
        trackId = dgnode.getCount();
        dgnode.setCount(trackId+1);
      }
      dgnode.setData('track', trackId, track);
      return trackId;
    };
    animationTrackNode.pub.getTimeRange = function() {
      var range = new FABRIC.RT.Vec2();
      var numTracks = dgnode.getCount();
      for(var i=0; i<numTracks; i++){
        var track = dgnode.getData('track', i);
        if(track.keys.length<=1){
          continue;
        }
        if(i==0){
          range.set(track.keys[0].time, track.keys[track.keys.length-1].time);
        }else{
          if(track.keys[0].time < range.x) range.x = track.keys[0].time;
          if(track.keys[track.keys.length-1].time > range.y) range.y = track.keys[track.keys.length-1].time;
        }
      }
      return range;
    };
    
    animationTrackNode.pub.getTrack = function(trackId) {
      return dgnode.getData('track', trackId);
    };
    animationTrackNode.pub.setTrack = function(track, trackId) {
      dgnode.setData('track', trackId ? trackId : 0, track);
    };
    animationTrackNode.pub.getTracks = function() {
      return dgnode.getMemberBulkData('track');
    };
    animationTrackNode.pub.setTracks = function(tracks) {
      return dgnode.setMemberBulkData(trackSet, trackSetId);
    };
    
    animationTrackNode.pub.getTrackCount = function() {
      return dgnode.getCount();
    };
    
    // Because we store all tracks in a track set, getting and setting the
    // tracks causes significant performance issues. So the solution for
    // now is to disable track evaluation while manipulating, and only store
    // the tracks once manipulaiton is complete. 
    var m_track, m_trackId;
    animationTrackNode.pub.setValues = function(time, trackIds, values) {
    //  var trackSet = this.getTrackSet(trackSetId);
    //  trackSet.setValues(time, trackIds, values);
    //  this.setTrackSet(trackSet, trackSetId);
    //  animationTrackNode.pub.fireEvent('valuechanged', {});
    
      m_track.setValues(time, trackIds, values);
    };
    animationTrackNode.beginManipulation = function(trackId) {
      m_trackId = trackId;
      m_track = animationTrackNode.pub.getTrack(trackId);
    }
    animationTrackNode.endManipulation = function() {
      animationTrackNode.pub.setTrack(m_track, m_trackId);
      animationTrackNode.pub.fireEvent('valuechanged', {});
    }
        
    animationTrackNode.getEvaluateCurveOperator = function() {
      return scene.constructOperator({
          operatorName: 'evaluate'+animationTrackNode.pub.getKeyframeType()+'Curve',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/evaluateKeyframeAnimationTrack.kl',
          preProcessorDefinitions: {
            KEYFRAMETYPE: animationTrackNode.pub.getKeyframeType(),
            KEYFRAME_EVALUATEDTYPE: animationTrackNode.pub.getValueType()
          },
          entryFunctionName: 'evaluateTrackCurve',
          parameterLayout: [
            'animationlibrary.track<>',
            'parameters.timeRange',
            'parameters.segmentCount',
            'self.values',
            'self.index',
          ],
          async: false
        });
    };
    
    animationTrackNode.pub.bindNodeMembersToTracks = function(targetnode, memberBindings, trackSetId, targetName) {
      // Generate a unique operator that binds this 
      // animation curves to the target node's members.
      trackSetId = trackSetId ? trackSetId : 0;

      targetnode = scene.getPrivateInterface(targetnode);
      targetnode.getDGNode().setDependency(dgnode, 'animationlibrary');
      targetnode.getDGNode().setDependency(scene.getGlobalsNode(), 'globals');

      var targetNodeMembers = targetnode.getDGNode().getMembers();
      var trackDataType = defaultKeyframeValue.valueType;
      var operatorName;
      if(targetName){
        operatorName = 'bindKeyframeTracksTo' + targetName;
      }
      else{
        operatorName = 'bindKeyframeTracksTo' + JSON.stringify(memberBindings).replace(/[^a-zA-Z 0-9]+/g, '');
      }
      
      var operatorHeaderSrc = '\nuse Vec3, Euler, Quat, RotationOrder;\n';
      operatorHeaderSrc += 'use '+keyframeType+';\n';
      operatorHeaderSrc += 'use '+keyframeTrackType+';\n';
      operatorHeaderSrc += 'use '+keyframeTrackSetType+';\n';
      operatorHeaderSrc += '\n\noperator ' + operatorName + '(\n';
      operatorHeaderSrc += '  io Scalar time,\n';
      operatorHeaderSrc += '  io ' + keyframeTrackSetType + ' trackSets<>';
      var operatorArraySrc = {};
      var operatorBodySrc = '  '+keyframeTrackSetType + ' trackSet = trackSets['+trackSetId+'];\n';
      var parameterLayout = ['globals.time', 'animationlibrary.trackSet<>'];
      var numEvaluatedTracks = 0;
      var addTrackEvaluation = function(trackid){
        numEvaluatedTracks++;
        return 'trackSet.tracks['+trackid+'].evaluate(time, currKeys['+ (numEvaluatedTracks-1) +'])';
      }
      var tempVariables = {};
      for (var memberAccessor in memberBindings) {
        var memberBinding = memberBindings[memberAccessor];
        var splitMemberAccessor = memberAccessor.split('.');
        var memberName = splitMemberAccessor[0];
        var memberIndex = undefined;
        if (memberName.indexOf('[') > 0) {
          memberIndex = memberName.substr(memberName.indexOf('[') + 1);
          memberIndex = parseInt(memberIndex.substr(0, memberIndex.indexOf(']')));
          memberName = memberName.substr(0, memberName.indexOf('['));
          if (operatorArraySrc[memberName]) {
            if (operatorArraySrc[memberName] < memberIndex) {
              operatorArraySrc[memberName] = memberIndex;
            }
          }
          else {
            operatorArraySrc[memberName] = memberIndex;
          }
        }
        var boundMemberType = targetNodeMembers[memberName].type;
        var isArray = false;
        if (boundMemberType.indexOf('[') > 0) {
          boundMemberType = boundMemberType.substr(0, boundMemberType.indexOf('['));
          isArray = true;
        }
        var memberType = boundMemberType;
        var typeDesc = rTypes[memberType];
        for (i = 1; i < splitMemberAccessor.length; i++) {
          var newMemberType = null;
          for (var j = 0; j < typeDesc.members.length; ++j) {
            if (typeDesc.members[j].name == splitMemberAccessor[i]) {
              newMemberType = typeDesc.members[j].type;
              break;
            }
          }
          if (!newMemberType)
            throw 'unable to find member "' + splitMemberAccessor[i] + '" of type "' + memberType + '"';
          memberType = newMemberType;
          typeDesc = rTypes[memberType];
        }
        var memberBindingCode = '';
        switch (memberType) {
          case 'Integer':

            break;
          case 'Scalar':
            memberBindingCode = memberAccessor + ' = ' + addTrackEvaluation(memberBinding);
            break;
          case 'Vec3':
            if (memberBinding[0] >= 0)
              memberBindingCode += '  ' + memberAccessor + '.x = ' + addTrackEvaluation(memberBinding[0]) + ';\n';
            if (memberBinding[1] >= 0)
              memberBindingCode += '  ' + memberAccessor + '.y = ' + addTrackEvaluation(memberBinding[1]) + ';\n';
            if (memberBinding[2] >= 0)
              memberBindingCode += '  ' + memberAccessor + '.z = ' + addTrackEvaluation(memberBinding[2]) + ';\n';
            break;
          case 'Quat':
            if (memberBinding.length === 3) {
              if (!tempVariables['tempVec3']) {
                tempVariables['tempVec3'] = '  var Vec3 tempVec3;\n';
              }
              if (!tempVariables['tempEuler']) {
                tempVariables['tempEuler'] = '  var Euler tempEuler;\n';
              }
              if (memberBinding[0] >= 0)
                memberBindingCode += '  tempVec3.x = ' + addTrackEvaluation(memberBinding[0]) + ';\n';
              else
                memberBindingCode += '  tempVec3.x = 0.0;\n';
              if (memberBinding[1] >= 0)
                memberBindingCode += '  tempVec3.y = ' + addTrackEvaluation(memberBinding[1]) + ';\n';
              else
                memberBindingCode += '  tempVec3.y = 0.0;\n';
              if (memberBinding[2] >= 0)
                memberBindingCode += '  tempVec3.z = ' + addTrackEvaluation(memberBinding[2]) + ';\n';
              else
                memberBindingCode += '  tempVec3.z = 0.0;\n';
              memberBindingCode += '  tempEuler = Euler(tempVec3);\n';
              memberBindingCode += '  ' + memberAccessor + '.setFromEuler(tempEuler);\n';
            }else if (memberBinding.length === 4) {
              if (memberBinding[0] < 0 || memberBinding[1] < 0 || memberBinding[2] < 0  || memberBinding[3] < 0 ){
                throw 'incorrect binding for Quat:' + JSON.stringify(memberBinding);
              }
              memberBindingCode += '  ' + memberAccessor + '.set( '+ addTrackEvaluation(memberBinding[0]) + ',\n';
              memberBindingCode += '    ' + addTrackEvaluation(memberBinding[1]) + ',\n';
              memberBindingCode += '    ' + addTrackEvaluation(memberBinding[2]) + ',\n';
              memberBindingCode += '    ' + addTrackEvaluation(memberBinding[3]) + ');\n';
              memberBindingCode += '  ' + memberAccessor + ' = ' + memberAccessor + '.unit();\n';
            }
            break;

        }
        
        operatorBodySrc += '\n' + memberBindingCode;
        
        if (parameterLayout.indexOf('self.' + memberName) == -1) {
          operatorHeaderSrc += ',\n  io ' + boundMemberType + ' ' + memberName + (isArray ? '[]' : '');
          parameterLayout.push('self.' + memberName);
        }
      }

      // add the array resize code to ensure that they are big enough
      operatorHeaderSrc += '\n){';
      for (var arrayName in operatorArraySrc) {
        operatorHeaderSrc += '\n  if(' + arrayName + '.size() < ' + (operatorArraySrc[arrayName] + 1) + '){';
        operatorHeaderSrc += '\n    ' + arrayName + '.resize(' + (operatorArraySrc[arrayName] + 1) + ');';
        operatorHeaderSrc += '\n  }';
      }

      // add the temporary varibale declaration at the top of the function body.
      for (i in tempVariables) {
        operatorBodySrc = tempVariables[i] + operatorBodySrc;
      }
      operatorBodySrc = '  Integer currKeys['+numEvaluatedTracks+'];\n' + operatorBodySrc;
      operatorBodySrc = '\n' + operatorBodySrc + '}';

      if (targetnode.getDGNode().bindings.length > 0) {
        targetnode.getDGNode().bindings.insert(scene.constructOperator({
          operatorName: operatorName,
          srcCode: operatorHeaderSrc + operatorBodySrc,
          entryFunctionName: operatorName,
          parameterLayout: parameterLayout
        }), 0);
      }
      else {
        targetnode.getDGNode().bindings.append(scene.constructOperator({
          operatorName: operatorName,
          srcCode: operatorHeaderSrc + operatorBodySrc,
          entryFunctionName: operatorName,
          parameterLayout: parameterLayout
        }));
      }
    };
    
    animationTrackNode.pub.openCurveEditor = function(options) {
      options = options ? options : undefined;
      var curveEditorWindow = window.open(
        FABRIC.processURL('FABRIC_ROOT/SceneGraph/CurveEditor/CurveEditor.html') + '?id=' + scene.pub.getContextId(),
        'Fabric Curve Editor:' + options.name,
        'status=1,resizable=1,width='+window.innerWidth+',height='+(window.innerHeight * 0.6)
      );
      curveEditorWindow.animationTrackNode = animationTrackNode.pub;
      curveEditorWindow.trackSetId = options.trackSetId ? options.trackSetId : 0;
      curveEditorWindow.drawKeys = options.drawKeys ? options.drawKeys : true;
      curveEditorWindow.trackFilters = options.trackFilters ? options.trackFilters : [];
      curveEditorWindow.scene = scene.pub;
      return curveEditorWindow;
    }
    
    scene.addEventHandlingFunctions(animationTrackNode);
    
    var parentWriteData = animationTrackNode.writeData;
    var parentReadData = animationTrackNode.readData;
    animationTrackNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {

    };
    animationTrackNode.readData = function(sceneDeserializer, nodeData) {

    };
    
    return animationTrackNode;
  }});



FABRIC.SceneGraph.registerNodeType('AnimationLibrary', {
  briefDesc: 'The AnimationTrack node implements an array of animation tracks.',
  detailedDesc: 'The AnimationTrack node is an abstract node type that stores an animation track per-slice. Drived nodes must specify the KeyframeType that is being interpollated',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    keyframetype: 'Specified by the derrived node, this option specifies the type of the keyframe tract that will be interpollated.',
    name:'The name of the track. This value is displayed in the curve Editor'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        keyframetype: undefined
      });

    if (options.keyframetype == undefined) {
      throw ('Please specify a type of data to interpollate');
    }
    var keyframeType = options.keyframetype;
    var keyframeTrackType = options.keyframetype+'Track';
    var keyframeTrackSetType = options.keyframetype+'TrackSet';
    var keyframeTrackSetBindingsType = options.keyframetype+'TrackSetBindings';
    var rTypes = scene.getRegisteredTypesManager().getRegisteredTypes();
    if (!rTypes[options.keyframetype]) {
      throw ('Type "' + options.keyframetype + '" is not registered. Load the RT file.');
    }
    if (!rTypes[keyframeTrackType]) {
      throw ('Type "' + keyframeTrackType + '" is not registered. Load the RT file.');
    }
    if (!rTypes[keyframeTrackSetType]) {
      throw ('Type "' + keyframeTrackSetType + '" is not registered. Load the RT file.');
    }
    if (!rTypes[keyframeTrackSetBindingsType]) {
      throw ('Type "' + keyframeTrackSetBindingsType + '" is not registered. Load the RT file.');
    }
    
    var defaultKeyframeValue = rTypes[options.keyframetype].defaultValue;
    if (!defaultKeyframeValue.valueType) {
      throw ('The keyframe registered type must provide a ' +
          'valueType that indicates what the keyframes will evaluate to. ' +
          'Normally this is a Scalar, but could be a Vec2, Color or any other value');
    }
    
    
    var animationLibraryNode = scene.constructNode('SceneGraphNode', options);
    var dgnode = animationLibraryNode.constructDGNode('DGNode');
    dgnode.addMember('trackSet', keyframeTrackSetType);
    dgnode.addMember('bindings', keyframeTrackSetBindingsType);
    
    var firstTrackAdded = false;
    animationLibraryNode.pub.addTrackSet = function(trackset, bindings) {
      var trackSetId;
      if(!firstTrackAdded){
        // Nodes default to having 1 slice, so we use up the first slot here.
        trackSetId = 0;
        firstTrackAdded = true;
      }
      else{
        trackSetId = dgnode.getCount();
        dgnode.setCount(trackSetId+1);
      }
      dgnode.setData('trackSet', trackSetId, trackset);
      if(bindings){
        dgnode.setData('bindings', trackSetId, bindings);
      }
      return trackSetId;
    };
    animationLibraryNode.pub.getTimeRange = function(trackSetId) {
      var calcTimeRange = function(trackSet){
        var range = new FABRIC.RT.Vec2();
        for(var i=0; i<trackSet.tracks.length; i++){
          var track = trackSet.tracks[i];
          if(track.keys.length<=1){
            continue;
          }
          if(i==0){
            range.set(track.keys[0].time, track.keys[track.keys.length-1].time);
          }else{
            if(track.keys[0].time < range.x) range.x = track.keys[0].time;
            if(track.keys[track.keys.length-1].time > range.y) range.y = track.keys[track.keys.length-1].time;
          }
        }
        return range;
      }
      return calcTimeRange(dgnode.getData('trackSet', trackSetId ? trackSetId : 0));
    };

    animationLibraryNode.pub.getKeyframeType = function() {
      return keyframeType;
    };
    animationLibraryNode.getTrackType = function() {
      return keyframeTrackType;
    };
    animationLibraryNode.getTrackSetType = function() {
      return keyframeTrackSetType;
    };
    
    animationLibraryNode.pub.getValueType = function(trackid) {
      // To determing what kind of data this evaluator should evaluate to,
      // we look up the keyframe type in the type manager, and request the
      // default value, which when is created on demand, enables us to call the valueType
      return defaultKeyframeValue.valueType;
    };
    animationLibraryNode.newTrackSet = function(trackSetName) {
      return new FABRIC.RT[keyframeTrackSetType](trackSetName);
    };
    
    animationLibraryNode.pub.getTrackSetCount = function() {
      return dgnode.getCount();
    };
    animationLibraryNode.pub.getTrackSet = function(trackSetId) {
      return dgnode.getData('trackSet', trackSetId);
    };
    animationLibraryNode.pub.setTrackSet = function(trackSet, trackSetId) {
      return dgnode.setData('trackSet', trackSetId ? trackSetId : 0, trackSet);
    };
    animationLibraryNode.pub.getTrackSetName = function(trackSetId) {
      return dgnode.getData('trackSet', trackSetId).name;
    };
    
    animationLibraryNode.pub.getTracks = function(trackSetId) {
      return dgnode.getData('trackSet', trackSetId).tracks;
    };
    animationLibraryNode.pub.setTracks = function(tracks, trackSetId) {
      var trackSet = animationLibraryNode.pub.getTrackSet(trackSetId);
      trackSet.tracks = tracks;
      return animationLibraryNode.pub.setTrackSet(trackSet, trackSetId);
    };
    animationLibraryNode.pub.setTrack = function(track, trackId, trackSetId) {
      var trackSet = animationLibraryNode.pub.getTrackSet(trackSetId);
      trackSet.tracks[trackId] = track;
      return animationLibraryNode.pub.setTrackSet(trackSet, trackSetId);
    };
    
    // Because we store all tracks in a track set, getting and setting the
    // tracks causes significant performance issues. So the solution for
    // now is to disable track evaluation while manipulating, and only store
    // the tracks once manipulaiton is complete. 
    var m_trackSet, m_trackSetId;
    animationLibraryNode.pub.setValues = function(trackSetId, time, trackIds, values) {
    //  var trackSet = this.getTrackSet(trackSetId);
    //  trackSet.setValues(time, trackIds, values);
    //  this.setTrackSet(trackSet, trackSetId);
    //  animationLibraryNode.pub.fireEvent('valuechanged', {});
    
      m_trackSet.setValues(time, trackIds, values);
    };
    animationLibraryNode.beginManipulation = function(trackSetId) {
      m_trackSetId = trackSetId;
      m_trackSet = animationLibraryNode.pub.getTrackSet(trackSetId);
    }
    animationLibraryNode.endManipulation = function() {
      animationLibraryNode.pub.setTrackSet(m_trackSet, m_trackSetId);
      animationLibraryNode.pub.fireEvent('valuechanged', {});
    }
    /*
    var paramsdgnode;
    animationLibraryNode.pub.bindToRig = function(rigNode, trackSetName){
      if (!rigNode.isTypeOf('CharacterRig')) {
        throw ('Incorrect type. Must be a CharacterRig');
      }
      
      rigNode = scene.getPrivateInterface(rigNode);
      
      var trackBindings = new FABRIC.RT.KeyframeTrackBindings();
      var trackSet = new FABRIC.RT.KeyframeTrackSet(trackSetName);
      var variables = rigNode.getVariables();
      
      // generate the bindings.
      // TODO: look up the solver that uses a particular binding,
      // and generate a trackName for the track.
      var i, binding;
      for (i = 0; i < variables.scalarValues.length; i++) {
        /*
        binding = [];
        trackSet.addTrack("ScalarTrack"+i, FABRIC.RT.rgb(1,1,0), binding);
        trackBindings.addScalarBinding(i, binding[0]);
        * /
        trackSet.addScalarTrack("ScalarTrack"+i, undefined, trackBindings, i);
      }
      for (i = 0; i < variables.vec3Values.length; i++) {
        /*
        binding = [];
        trackSet.addTrack("Vec3Track"+i, FABRIC.RT.rgb(1,0,0), binding);
        trackSet.addTrack("Vec3Track"+i+".x", FABRIC.RT.rgb(1, 0, 0), binding);
        trackSet.addTrack("Vec3Track"+i+".y", FABRIC.RT.rgb(0, 1, 0), binding);
        trackSet.addTrack("Vec3Track"+i+".z", FABRIC.RT.rgb(0, 0, 1), binding);
        trackBindings.addVec3Binding(i, binding);
        * /
        trackSet.addVec3Track("Vec3Track"+i, undefined, trackBindings, i);
      }
      for (i = 0; i < variables.quatValues.length; i++) {
        /*
        binding = [];
        trackSet.addTrack("QuatTrack"+i+".v.x", FABRIC.RT.rgb(1, 0, 0), binding);
        trackSet.addTrack("QuatTrack"+i+".v.y", FABRIC.RT.rgb(0, 1, 0), binding);
        trackSet.addTrack("QuatTrack"+i+".v.z", FABRIC.RT.rgb(0, 0, 1), binding);
        trackSet.addTrack("QuatTrack"+i+".w", FABRIC.RT.rgb(1, 1, 0), binding);
        trackBindings.addQuatBinding(i, binding);
        * /
        trackSet.addQuatTrack("QuatTrack"+i, undefined, trackBindings, i);
      }
      
      for (i = 0; i < variables.xfoValues.length; i++) {
        /*
        binding = [];
        trackSet.addTrack("XfoTrack"+i+".tr.x", FABRIC.RT.rgb(1, 0, 0), binding);
        trackSet.addTrack("XfoTrack"+i+".tr.y", FABRIC.RT.rgb(0, 1, 0), binding);
        trackSet.addTrack("XfoTrack"+i+".tr.z", FABRIC.RT.rgb(0, 0, 1), binding);
        
        trackSet.addTrack("XfoTrack"+i+".ori.v.x", FABRIC.RT.rgb(1, 0, 0), binding);
        trackSet.addTrack("XfoTrack"+i+".ori.v.y", FABRIC.RT.rgb(0, 1, 0), binding);
        trackSet.addTrack("XfoTrack"+i+".ori.v.z", FABRIC.RT.rgb(0, 0, 1), binding); 
        trackSet.addTrack("XfoTrack"+i+".ori.w", FABRIC.RT.rgb(1, 1, 0), binding);
        trackBindings.addXfoBinding(i, binding);
        * /
      //  trackBindings.addXfoBinding(i, trackSet.addXfoTrack("XfoTrack"+i));
        trackSet.addQuatTrack("XfoTrack"+i, undefined, trackBindings, i);
      }
      var trackSetId = animationLibraryNode.pub.addTrackSet(trackSet);
      
      if(!paramsdgnode){
        paramsdgnode = animationLibraryNode.constructDGNode('ParamsDGNode');
        
        dgnode.setDependency(paramsdgnode, 'params');
        paramsdgnode.addMember('boundTrack', 'Integer', trackSetId);
      }else{
        paramsdgnode.setData('boundTrack', 0, trackSetId);
      }
      dgnode.setData('bindings', trackSetId, trackBindings);
      
      animationLibraryNode.pub.plotKeyframes = function(trackSetId, timeRange, sampleFrequency){
        var variablesNode = scene.getPrivateInterface(rigNode.pub.getVariablesNode());
        dgnode.setDependency(variablesNode.getDGNode(), 'variables');
        dgnode.setDependency(scene.getGlobalsNode(), 'globals');
        
        paramsdgnode.setData('boundTrack', 0, trackSetId);
        dgnode.bindings.append(scene.constructOperator({
          operatorName: 'keyCurvesFromVariables',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/keyCurvesFromVariables.kl',
          preProcessorDefinitions: {
            KEYFRAMETRACKSETTYPE: keyframeTrackSetType
          },
          entryFunctionName: 'keyCurvesFromVariables',
          parameterLayout: [
            'globals.time',
            'variables.poseVariables',
            'params.boundTrack',
            'self.trackSet<>',
            'self.bindings<>'
          ],
          async: false
        }));
        var t = 0;
        for(var i=0; i <= Math.round((timeRange.y-timeRange.x) / sampleFrequency); i++){
          scene.pub.animation.setTime(t, false);
          dgnode.evaluate();
          t += sampleFrequency;
        }
        dgnode.bindings.remove(0);
        dgnode.removeDependency('variables');
        dgnode.removeDependency('globals');
        scene.pub.animation.setTime(0, false);
      }
      return trackBindings;
    }
        */
        
    animationLibraryNode.getEvaluateCurveOperator = function() {
      return scene.constructOperator({
          operatorName: 'evaluate'+options.keyframetype+'Curve',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/evaluateKeyframeAnimationTrackSet.kl',
          preProcessorDefinitions: {
            KEYFRAMETYPE: options.keyframetype,
            TRACKTYPE: keyframeTrackType,
            TRACKSETTYPE: keyframeTrackSetType,
            KEYFRAME_EVALUATEDTYPE: defaultKeyframeValue.valueType
          },
          entryFunctionName: 'evaluateTrackSetCurve',
          parameterLayout: [
            'animationlibrary.trackSet<>',
            'parameters.trackSetId',
            'parameters.timeRange',
            'parameters.segmentCount',
            'self.values',
            'self.index',
          ],
          async: false
        });
    };
    
    animationLibraryNode.pub.openCurveEditor = function(options) {
      options = options ? options : undefined;
      var curveEditorWindow = window.open(
        FABRIC.processURL('FABRIC_ROOT/SceneGraph/CurveEditor/CurveEditor.html') + '?id=' + scene.pub.getContextId(),
        'Fabric Curve Editor:' + options.name,
        'status=1,resizable=1,width='+window.innerWidth+',height='+(window.innerHeight * 0.6)
      );
      curveEditorWindow.animationLibraryNode = animationLibraryNode.pub;
      curveEditorWindow.trackSetId = options.trackSetId ? options.trackSetId : 0;
      curveEditorWindow.drawKeys = options.drawKeys ? options.drawKeys : true;
      curveEditorWindow.trackFilters = options.trackFilters ? options.trackFilters : [];
      curveEditorWindow.scene = scene.pub;
      return curveEditorWindow;
    }
    
    scene.addEventHandlingFunctions(animationLibraryNode);
    
    
    var parentWriteData = animationLibraryNode.writeData;
    var parentReadData = animationLibraryNode.readData;
    animationLibraryNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      constructionOptions.keyframetype = options.keyframetype;
      nodeData.numTracks = animationLibraryNode.pub.getTrackSetCount();
      nodeData.trackSets = [];
      nodeData.bindings = [];
      for(var i=0; i<nodeData.numTracks; i++){
        nodeData.trackSets.push(animationLibraryNode.pub.getTrackSet(i));
        nodeData.bindings.push(dgnode.getData('bindings', i));
      }
    };
    animationLibraryNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      for(var i=0; i<nodeData.numTracks; i++){
        animationLibraryNode.pub.addTrackSet(nodeData.trackSets[i]);
        dgnode.setData('bindings', i, nodeData.bindings[i]);
      }
    };
    
    return animationLibraryNode;
  }});


FABRIC.SceneGraph.registerNodeType('LinearKeyAnimationTrack', {
  briefDesc: 'The LinearKeyAnimationTrack node implements an array of linear keyframe animation tracks.',
  detailedDesc: 'The LinearKeyAnimationTrack node derrives from AnimationTrack and specifies that the tracks should contains \'LinearKeyframes\'',
  parentNodeDesc: 'AnimationTrack',
  factoryFn: function(options, scene) {
    options.keyframetype = 'LinearKeyframe';
    return scene.constructNode('AnimationTrack', options);
  }});

FABRIC.SceneGraph.registerNodeType('LinearKeyAnimationLibrary', {
  briefDesc: 'The LinearKeyAnimationTrack node implements an array of linear keyframe animation tracks.',
  detailedDesc: 'The LinearKeyAnimationTrack node derrives from AnimationTrack and specifies that the tracks should contains \'LinearKeyframes\'',
  parentNodeDesc: 'AnimationTrack',
  factoryFn: function(options, scene) {
    options.keyframetype = 'LinearKeyframe';
    return scene.constructNode('AnimationLibrary', options);
  }});


FABRIC.SceneGraph.registerNodeType('BezierKeyAnimationTrack', {
  briefDesc: 'The BezierKeyAnimationTrack node implements an array of bezier keyframe animation tracks.',
  detailedDesc: 'The BezierKeyAnimationTrack node derrives from AnimationTrack and specifies that the tracks should contains \'BezierKeyframe\'',
  parentNodeDesc: 'AnimationTrack',
  factoryFn: function(options, scene) {
    options.keyframetype = 'BezierKeyframe';
    return scene.constructNode('AnimationTrack', options);
  }});

FABRIC.SceneGraph.registerNodeType('BezierKeyAnimationLibrary', {
  briefDesc: 'The BezierKeyAnimationTrack node implements an array of bezier keyframe animation tracks.',
  detailedDesc: 'The BezierKeyAnimationTrack node derrives from AnimationTrack and specifies that the tracks should contains \'BezierKeyframe\'',
  parentNodeDesc: 'AnimationTrack',
  factoryFn: function(options, scene) {
    options.keyframetype = 'BezierKeyframe';
    return scene.constructNode('AnimationLibrary', options);
  }});


FABRIC.SceneGraph.registerNodeType('ColorKeyAnimationTrack', {
  briefDesc: 'The ColorKeyAnimationLibrary node implements an array of color keyframe animation tracks.',
  detailedDesc: 'The ColorKeyAnimationLibrary node derrives from AnimationTrack and specifies that the tracks should contains \'ColorKeyframe\'',
  parentNodeDesc: 'AnimationTrack',
  factoryFn: function(options, scene) {
    options.keyframetype = 'ColorKeyframe';
    return scene.constructNode('AnimationTrack', options);
  }});


FABRIC.SceneGraph.registerNodeType('AnimationController', {
  briefDesc: 'The AnimationController node controls the time at which an AnimationTrack is evaluated.',
  detailedDesc: 'The Animation Controller only maintains time values which can be used by'+
                'animation evaluators. Many evaluators can share an Animation Controller',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    playbackRate: 'The rate to increment the time values, relative to the global scene time increments.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        playbackRate: 1.0,
        bindToGlobalTime: true,
        timeRange: new FABRIC.RT.Vec2(0, 10),
        timeControl: 0, /* 0: absolute, 1:increment */
        outOfRange: 1 /* 0: linear, 1:loop, 3:clamp */
      });

    var animationControllerNode = scene.constructNode('SceneGraphNode', options);
    var dgnode = animationControllerNode.constructDGNode('DGNode');
    dgnode.addMember('timeControl', 'Integer', options.timeControl);
    dgnode.addMember('playbackRate', 'Scalar', options.playbackRate);
    dgnode.addMember('localTime', 'Scalar', options.timeRange.x);
    dgnode.addMember('timeRange', 'Vec2', options.timeRange);
    dgnode.addMember('outOfRange', 'Integer', options.outOfRange);
    
    // create a getter and setter for the local time
    animationControllerNode.addMemberInterface(dgnode, 'playbackRate', true);
    animationControllerNode.addMemberInterface(dgnode, 'localTime', true);
    animationControllerNode.addMemberInterface(dgnode, 'timeRange', true);

    // extend public interface
    animationControllerNode.pub.getTime = function(time) {
      return dgnode.getData('localTime');
    };
    animationControllerNode.pub.setTime = function(time) {
      dgnode.setData('localTime', 0, time);
    };

    // Here, the animation controllers time is locked to
    // global time. Often the time context for an animation
    // is computed, or simulated based on scene events.
    if(options.bindToGlobalTime) {
      dgnode.setDependency(scene.getGlobalsNode(), 'globals');
      
      // This operator might simulate the platback local time by incrementing
      // it each frame by a rate specified by the playbackRate parameter.
      dgnode.bindings.append(scene.constructOperator( {
          operatorName:'incrementControllerLocalTime',
          srcFile:'FABRIC_ROOT/SceneGraph/KL/incrementControllerLocalTime.kl',
          entryFunctionName:'incrementControllerLocalTime',
          parameterLayout: [
            'globals.time',
            'globals.timestep',
            'self.timeControl',
            'self.playbackRate',
            'self.timeRange',
            'self.outOfRange',
            'self.localTime'
          ]
        }));
    }
    return animationControllerNode;
  }});



FABRIC.SceneGraph.registerNodeType('TrackDisplay', {
  briefDesc: 'The TrackDisplay evaluates AnimationTracks across time ranges ready for display in the CurveEditor.',
  detailedDesc: 'The TrackDisplay evaluates AnimationTracks across time ranges ready for display in the CurveEditor.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
      animationLibraryNode: 'The AnimationLibrary node to evaluate.',
      trackIndex: 'The index of the track in the AnimationTrack node to evaluate.',
      timeRange: 'The time rage to evaluate.',
      segmentCount: 'The number of steps to divide the timeRange by.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        animationTrackNode: undefined,
        animationLibraryNode: undefined,
        segmentCount: 100,
        timeRange: new FABRIC.RT.Vec2(0, 1),
        trackSetId: 0
      });

    var trackDisplayNode = scene.constructNode('SceneGraphNode', options);
    var dgnode = trackDisplayNode.constructDGNode('DGNode');
    var parametersdgnode = trackDisplayNode.constructDGNode('ParametersDGNode');
    var animationLibraryNode;

    parametersdgnode.addMember('trackSetId', 'Integer');
    parametersdgnode.addMember('timeRange', 'Vec2', options.timeRange);
    parametersdgnode.addMember('segmentCount', 'Size', options.segmentCount);

    dgnode.setDependency(parametersdgnode, 'parameters');


    // extend the public interface
    
    trackDisplayNode.addReferenceInterface('AnimationLibrary', 'Animation',
      function(nodePrivate, trackSetId){
        animationLibraryNode = nodePrivate;
        var trackDataType = animationLibraryNode.pub.getValueType();
        var tracks = animationLibraryNode.pub.getTracks(trackSetId);
        dgnode.setDependency(animationLibraryNode.getDGNode(), 'animationlibrary');
  
        dgnode.addMember('values', trackDataType+'[]');
        parametersdgnode.setData('trackSetId', trackSetId ? trackSetId : 0);
        dgnode.setCount(tracks.length);
  
        dgnode.bindings.append(animationLibraryNode.getEvaluateCurveOperator());
      });
    trackDisplayNode.pub.setTimeRange = function(timeRange) {
      parametersdgnode.setData('timeRange', timeRange);
    };
    trackDisplayNode.pub.getTimeRange = function(timeRange) {
      return parametersdgnode.getData('timeRange');
    };
    trackDisplayNode.pub.getCurveData = function() {
      dgnode.evaluate();
      return dgnode.getBulkData();
    }

    if (options.animationTrackNode) {
      trackDisplayNode.pub.setAnimationLibraryNode(options.animationTrackNode);
    }
    if (options.animationLibraryNode) {
      trackDisplayNode.pub.setAnimationLibraryNode(options.animationLibraryNode, options.trackSetId);
    }
    return trackDisplayNode;
  }});


