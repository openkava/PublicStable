
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * A function to create a linear key frame object.
 * @constructor
 * @param {number} time The time of the keyframe.
 * @param {number} value The value of the keyframe.
 */
FABRIC.RT.LinearKeyframe = function(time, value) {
  this.time = typeof time === 'number' ? time : 0;
  this.value = typeof value === 'number' ? value : 0;
};

FABRIC.RT.LinearKeyframe.prototype = {
  // This method enables an animation evaluator to know what kind of data
  // track full of these data types should evaluate to.
  get valueType() {
    return 'Scalar';
  },
  getType: function() {
    return 'FABRIC.RT.LinearKeyframe';
  }
};

/**
 * overloaded constructor function for LinearKeyframe
 * @param {number} value The value of the keyframe.
 * @param {number} time The time of the keyframe.
 * @return {object} The linear key frame object.
 */
FABRIC.RT.linearKeyframe = function(value, time) {
  return new FABRIC.RT.LinearKeyframe(value, time);
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('LinearKeyframe', {
    members: {
      time: 'Scalar', value: 'Scalar'
    },
    constructor: FABRIC.RT.LinearKeyframe,
    klBindings: {
      filename: 'LinearKeyframe.kl',
      sourceCode: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/LinearKeyframe.kl')
    }
  });
});

if(!FABRIC.RT.KeyframeTrack){
  throw("please include the SceneGraph/RT/KeyframeTrack.js file before this one.");
}

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('LinearKeyframeTrack', {
    members: {
      name: 'String',
      color: 'Color',
      keys: 'LinearKeyframe[]'
    },
    constructor: FABRIC.RT.KeyframeTrack,
    klBindings: {
      filename: 'FABRIC_ROOT/SceneGraph/RT/KeyframeTrack.kl',
      sourceCode: FABRIC.preProcessCode(
        FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/KeyframeTrack.kl'), {
          KEYFRAMEDATATYPE:'Scalar',
          KEYFRAMETYPE: 'LinearKeyframe',
          KEYFRAMEDATADEFAULTVALUE: '0.0'
        })
    }
  });
});

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('LinearKeyframeTrackSet', {
    members: {
      name: 'String',
      timeRange: 'Vec2',
      tracks: 'LinearKeyframeTrack[]'
    },
    constructor: FABRIC.RT.KeyframeTrackSet,
    klBindings: {
      filename: 'FABRIC_ROOT/SceneGraph/RT/KeyframeTrack.kl',
      sourceCode: FABRIC.preProcessCode(
        FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/KeyframeTrack.kl'), {
          KEYFRAMEDATATYPE: 'Scalar',
          KEYFRAMETYPE: 'LinearKeyframe',
          KEYFRAMEDATADEFAULTVALUE: '0.0'
        }
      )
    }
  });
});


