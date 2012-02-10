
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.define(["SG/SceneGraph", "RT/KeyframeTrack"], function() {
  

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
    var m_track, m_trackId, m_newUndoTransaction;
    animationTrackNode.pub.setValues = function(time, trackIds, values) {
    //  var trackSet = this.getTrackSet(trackSetId);
    //  trackSet.setValues(time, trackIds, values);
    //  this.setTrackSet(trackSet, trackSetId);
    //  animationTrackNode.pub.fireEvent('keyframetrackchanged', {});
    
      m_track.setValues(time, trackIds, values);
    };
    animationTrackNode.beginManipulation = function(trackId) {
      m_trackId = trackId;
      m_track = animationTrackNode.pub.getTrack(trackId);
      
      var undoManager = scene.getManager('UndoManager');
      if(undoManager){
        m_newUndoTransaction = false;
        if(!undoManager.undoTransactionOpen()){
          undoManager.openUndoTransaction();
          m_newUndoTransaction = true;
        }
        var newTrackSet, prevTrackSet = animationTrackNode.pub.getTrack(m_trackId);
        undoManager.addAction({
          onClose: function() {
            newTrackSet = m_track;//animationTrackNode.pub.getTrack(m_trackId);
          },
          onUndo: function() {
            animationTrackNode.pub.setTrack(prevTrackSet, m_trackId);
          },
          onRedo: function() {
            animationTrackNode.pub.setTrack(newTrackSet, m_trackId);
          }
        });
      }
    }
    animationTrackNode.endManipulation = function() {
      animationTrackNode.pub.setTrack(m_track, m_trackId);
      
      var undoManager = scene.getManager('UndoManager');
      if(undoManager && m_newUndoTransaction){
        undoManager.closeUndoTransaction();
      }
      animationTrackNode.pub.fireEvent('keyframetrackchanged', {});
    }
        
    animationTrackNode.getEvaluateCurveOperator = function() {
      return scene.constructOperator({
          operatorName: 'evaluate'+animationTrackNode.pub.getKeyframeType()+'Curve',
          srcFile: 'FABRIC_ROOT/SG/KL/evaluateKeyframeAnimationTrack.kl',
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
      targetnode.getDGNode().setDependency(dgnode, 'animationTracks');
      targetnode.getDGNode().setDependency(scene.getGlobalsNode(), 'globals');
      var rTypes = scene.getRegisteredTypesManager().getRegisteredTypes();

      var targetNodeMembers = targetnode.getDGNode().getMembers();
      var trackDataType = animationTrackNode.pub.getValueType();
      var operatorName;
      if(targetName){
        operatorName = 'bindKeyframeTracksTo' + targetName;
      }
      else{
        operatorName = 'bindKeyframeTracksTo' + JSON.stringify(memberBindings).replace(/[^a-zA-Z 0-9]+/g, '');
      }
      var operatorBodySrc = '';
      var operatorHeaderSrc = '\nuse Vec3, Euler, Quat, RotationOrder;\n';
      operatorHeaderSrc += 'use '+animationTrackNode.pub.getKeyframeType() +';\n';
      operatorHeaderSrc += 'use '+animationTrackNode.getTrackType()+';\n';
      operatorHeaderSrc += '\n\noperator ' + operatorName + '(\n';
      operatorHeaderSrc += '  io Scalar time,\n';
      operatorHeaderSrc += '  io ' + animationTrackNode.getTrackType() + ' tracks<>';
      var operatorArraySrc = {};
      var parameterLayout = ['globals.time', 'animationTracks.track<>'];
      var numEvaluatedTracks = 0;
      var addTrackEvaluation = function(trackid){
        numEvaluatedTracks++;
        return 'tracks['+trackid+'].evaluate(time, currKeys['+ (numEvaluatedTracks-1) +'])';
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
      options = options ? options : {};
      var curveEditorWindow = window.open(
        FABRIC.processURL('FABRIC_ROOT/SG/CurveEditor/CurveEditor.html') + '?id=' + scene.pub.getContextId(),
        'Fabric Curve Editor:' + (options.name ? options.name : animationTrackNode.pub.getName()),
        'status=1,resizable=1,width='+window.innerWidth+',height='+(window.innerHeight * 0.6)
      );
      curveEditorWindow.animationLibraryNode = animationTrackNode.pub;
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
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
      constructionOptions.keyframetype = options.keyframetype;
      nodeData.numTracks = dgnode.getCount();
      nodeData.tracks = dgnode.getBulkData('track');
    };
    animationTrackNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      dgnode.setCount(nodeData.numTracks);
      for(var i=0; i<nodeData.numTracks; i++){
        dgnode.setData('track', i, nodeData.tracks[i]);
      }
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

    
    var animationLibraryNode = scene.constructNode('Animation', options);
    var dgnode = animationLibraryNode.constructDGNode('DGNode');
    
    var keyframeTrackSetType = animationLibraryNode.pub.getKeyframeType()+'TrackSet';
    var keyframeTrackSetBindingsType = animationLibraryNode.pub.getKeyframeType()+'TrackSetBindings';
    
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

    animationLibraryNode.getTrackSetType = function() {
      return keyframeTrackSetType;
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
      return dgnode.getData('trackSet', trackSetId, trackSet);
    };
    
    animationLibraryNode.getBindings = function(trackSetId) {
      return dgnode.getData('bindings', trackSetId);
    };
    
    animationLibraryNode.pub.getTrackSetName = function(trackSetId) {
      return dgnode.getData('trackSet', trackSetId).name;
    };
    
    animationLibraryNode.pub.getTracks = function(trackSetId) {
      return dgnode.getData('trackSet', trackSetId).tracks;
    };
    animationLibraryNode.pub.setTracks = function(tracks, trackSetId) {
      var trackSet = dgnode.getData('trackSet', trackSetId);
      trackSet.tracks = tracks;
      dgnode.setData('trackSet', trackSetId, trackSet);
    };
    animationLibraryNode.pub.setTrack = function(track, trackId, trackSetId) {
      var trackSet = dgnode.getData('trackSet', trackSetId);
      trackSet.tracks[trackId] = track;
      dgnode.setData('trackSet', trackSetId, trackSet);
    };
    
    // Because we store all tracks in a track set, getting and setting the
    // tracks causes significant performance issues. So the solution for
    // now is to disable track evaluation while manipulating, and only store
    // the tracks once manipulaiton is complete. 
    var m_trackSet, m_trackSetId, m_newUndoTransaction;
    animationLibraryNode.pub.setValues = function(trackSetId, time, value, index, xfoTrackFilter) {
      
      var type = (typeof value == 'number') ? 'Number' : value.getType();
      var bindings = animationLibraryNode.getBindings(trackSetId);
      var findBinding = function(bindingsList){
        for(var i=0; i<bindingsList.length; i++){
          if(bindingsList[i].varId == index){
            return bindingsList[i];
          }
        }
        throw "Binding not found";
      }
      var binding, values;
      var trackIds;
      switch(type){
      case 'Number':
        binding = findBinding(bindings.scalarBindings);
        values = [value];
        trackIds = binding.trackIds;
        break;
      case 'FABRIC.RT.Vec3':
        binding = findBinding(bindings.vec3Bindings);
        values = [value.x, value.y, value.z];
        trackIds = binding.trackIds;
        break;
      case 'FABRIC.RT.Quat':
        binding = findBinding(bindings.quatBindings);
        if(binding.trackIds.length == 3){
          var euler = new FABRIC.RT.Euler();
          euler.setFromQuat(value);
          values = [euler.x, euler.y, euler.z];
        }else if(binding.trackIds.length == 4){
          values = [value.w, value.x, value.y, value.z];
        }
        trackIds = binding.trackIds;
        break;
      case 'FABRIC.RT.Xfo':
        binding = findBinding(bindings.xfoBindings);
        trackIds = binding.trackIds;
        if(binding.trackIds.length == 6){
          var euler = new FABRIC.RT.Euler();
          euler.setFromQuat(value.ori);
          values = [value.tr.x, value.tr.y, value.tr.z, euler.x, euler.y, euler.z];
        }else if(binding.trackIds.length == 7){
          
          if(xfoTrackFilter){
            var filteredValues = [];
            var filteredTrackIds = [];
            if(xfoTrackFilter.tr){
              filteredValues = [value.tr.x, value.tr.y, value.tr.z];
              filteredTrackIds = [trackIds[0], trackIds[1], trackIds[2]]
            }
            if(xfoTrackFilter.ori){
              filteredValues = filteredValues.concat([value.ori.v.x, value.ori.v.y, value.ori.v.z, value.ori.w]);
              filteredTrackIds = filteredTrackIds.concat([trackIds[3], trackIds[4], trackIds[5], trackIds[6]]);
            }
            if(xfoTrackFilter.sc){
              filteredValues = [value.sc.x, value.sc.y, value.sc.z];
              filteredTrackIds = [trackIds[6], trackIds[7], trackIds[8]]
            }
            values = filteredValues;
            trackIds = filteredTrackIds;
          }
          else{
            values = [value.tr.x, value.tr.y, value.tr.z, value.ori.v.x, value.ori.v.y, value.ori.v.z, value.ori.w];
            trackIds = binding.trackIds;
          }
        }
        break;
      default:
        throw 'Unhandled type:' + val;
      }
      
      // This is commented out, because we fetch the
      // track set at the beginning of the manipulation
      // and push it only at the end.
    //  var trackSet = this.getTrackSet(trackSetId);
    //  trackSet.setValues(time, trackIds, values);
    //  this.setTrackSet(trackSet, trackSetId);
    //  animationLibraryNode.pub.fireEvent('keyframetrackchanged', {});
    
      m_trackSet.setValues(time, trackIds, values);
    };
    animationLibraryNode.beginManipulation = function(trackSetId) {
      m_trackSetId = trackSetId;
      m_trackSet = dgnode.getData('trackSet', trackSetId);
      
      var undoManager = scene.getManager('UndoManager');
      if(undoManager){
        // A new transaction may have already been opened if the 
        // event has already been recieved from a different source. 
        if(!m_newUndoTransaction){
          if(!undoManager.undoTransactionOpen()){
            undoManager.openUndoTransaction();
            m_newUndoTransaction = true;
          }
          var newTrackSet, prevTrackSet = animationLibraryNode.pub.getTrackSet(m_trackSetId);
          undoManager.addAction({
            onClose: function() {
              newTrackSet = m_trackSet;//animationLibraryNode.pub.getTrackSet(m_trackId);
            },
            onUndo: function() {
              m_trackSet = prevTrackSet;
              dgnode.setData('trackSet', m_trackSetId, prevTrackSet);
              animationLibraryNode.pub.fireEvent('keyframetrackchanged', { trackSetId: m_trackSetId });
            },
            onRedo: function() {
              m_trackSet = newTrackSet;
              dgnode.setData('trackSet', m_trackSetId, newTrackSet);
              animationLibraryNode.pub.fireEvent('keyframetrackchanged', { trackSetId: m_trackSetId });
            }
          });
        }
      }
    }
    animationLibraryNode.endManipulation = function() {
      dgnode.setData('trackSet', m_trackSetId, m_trackSet);
      
      var undoManager = scene.getManager('UndoManager');
      if(undoManager && m_newUndoTransaction){
        undoManager.closeUndoTransaction();
        m_newUndoTransaction = false;
      }
      animationLibraryNode.pub.fireEvent('keyframetrackchanged', {});
    }
    
    var paramsdgnode;
    animationLibraryNode.pub.bindToRig = function(rigNode, trackSetId){
      if (!rigNode.isTypeOf('CharacterRig')) {
        throw ('Incorrect type. Must be a CharacterRig');
      }
      
      rigNode = scene.getPrivateInterface(rigNode);
        
      if(!paramsdgnode){
        paramsdgnode = animationLibraryNode.constructDGNode('ParamsDGNode');
        
        dgnode.setDependency(paramsdgnode, 'params');
        paramsdgnode.addMember('boundTrack', 'Integer', trackSetId);
      }else{
        paramsdgnode.setData('boundTrack', 0, trackSetId);
      }
    //  dgnode.setData('bindings', trackSetId, trackBindings);
      
      animationLibraryNode.pub.plotKeyframes = function(trackSetId, timeRange, sampleFrequency){
        var variablesNode = scene.getPrivateInterface(rigNode.pub.getVariablesNode());
        dgnode.setDependency(variablesNode.getDGNode(), 'variables');
        dgnode.setDependency(scene.getGlobalsNode(), 'globals');
        
        paramsdgnode.setData('boundTrack', 0, trackSetId);
        dgnode.bindings.append(scene.constructOperator({
          operatorName: 'keyCurvesFromVariables',
          srcFile: 'FABRIC_ROOT/SG/KL/keyCurvesFromVariables.kl',
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
    //  return trackBindings;
    }
        
    animationLibraryNode.getEvaluateCurveOperator = function() {
      return scene.constructOperator({
          operatorName: 'evaluate'+options.keyframetype+'Curve',
          srcFile: 'FABRIC_ROOT/SG/KL/evaluateKeyframeAnimationTrackSet.kl',
          preProcessorDefinitions: {
            KEYFRAMETYPE: animationLibraryNode.pub.getKeyframeType(),
            TRACKTYPE: animationLibraryNode.getTrackType(),
            TRACKSETTYPE: animationLibraryNode.getTrackSetType(),
            KEYFRAME_EVALUATEDTYPE: animationLibraryNode.pub.getValueType()
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
      options = options ? options : {};
      var curveEditorWindow = window.open(
        FABRIC.processURL('FABRIC_ROOT/SG/CurveEditor/CurveEditor.html') + '?id=' + scene.pub.getContextId(),
        'Fabric Curve Editor:' + (options.name ? options.name : animationLibraryNode.pub.getName()),
        'status=1,resizable=1,width='+window.innerWidth+',height='+(window.innerHeight * 0.6)
      );
      curveEditorWindow.animationLibraryNode = animationLibraryNode.pub;
      curveEditorWindow.trackSetId = options.trackSetId!=undefined ? options.trackSetId : 0;
      curveEditorWindow.drawKeys = options.drawKeys!=undefined ? options.drawKeys : true;
      curveEditorWindow.trackFilters = options.trackFilters!=undefined ? options.trackFilters : [];
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
        nodeData.trackSets.push(dgnode.getData('trackSet', i));
        nodeData.bindings.push(dgnode.getData('bindings', i));
      }
    };
    animationLibraryNode.readData = function(sceneDeserializer, nodeData) {
      parentReadData(sceneDeserializer, nodeData);
      dgnode.setCount(nodeData.numTracks);
      for(var i=0; i<nodeData.numTracks; i++){
        dgnode.setData('trackSet', i, nodeData.trackSets[i]);
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
          srcFile:'FABRIC_ROOT/SG/KL/incrementControllerLocalTime.kl',
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

    parametersdgnode.addMember('trackSetId', 'Integer', options.trackSetId);
    parametersdgnode.addMember('timeRange', 'Vec2', options.timeRange);
    parametersdgnode.addMember('segmentCount', 'Size', options.segmentCount);

    dgnode.setDependency(parametersdgnode, 'parameters');
    
    trackDisplayNode.addMemberInterface(parametersdgnode, 'trackSetId', true);
    trackDisplayNode.addMemberInterface(parametersdgnode, 'timeRange', true);
    trackDisplayNode.addMemberInterface(parametersdgnode, 'segmentCount', true);

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
    trackDisplayNode.pub.getCurveData = function(trackIndex) {
      dgnode.evaluate();
      return dgnode.getData('values', trackIndex);
    }

    if (options.animationTrackNode) {
      trackDisplayNode.pub.setAnimationLibraryNode(options.animationTrackNode);
    }
    if (options.animationLibraryNode) {
      trackDisplayNode.pub.setAnimationLibraryNode(options.animationLibraryNode, options.trackSetId);
    }
    return trackDisplayNode;
  }});

});
