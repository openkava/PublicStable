
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * The basic Animation module.
 */
FABRIC.Animation = FABRIC.Animation ? FABRIC.Animation : {};

/**
 * A function to create a linear key frame object.
 * @constructor
 * @param {number} time The time of the keyframe.
 * @param {number} value The value of the keyframe.
 */
FABRIC.Animation.ColorKeyframe = function(time, value) {
  this.time = typeof time === 'number' ? time : 0;
  this.value = value != undefined ? value : FABRIC.RT.rgb(1,0,0);
};

FABRIC.Animation.ColorKeyframe.prototype = {
  // This method enables an animation evaluator to know what kind of data
  // track full of these data types should evaluate to.
  get valueType() {
    return 'Color';
  },
  getType: function() {
    return 'FABRIC.Animation.ColorKeyframe';
  }
};

/**
 * overloaded constructor function for ColorKeyframe
 * @param {number} value The value of the keyframe.
 * @param {number} time The time of the keyframe.
 * @return {object} The linear key frame object.
 */
FABRIC.Animation.colorKeyframe = function(value, time) {
  return new FABRIC.Animation.ColorKeyframe(value, time);
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('ColorKeyframe', {
    members: {
      time: 'Scalar', value: 'Color'
    },
    constructor: FABRIC.Animation.ColorKeyframe,
    klBindings: {
      filename: 'ColorKeyframe.kl',
      sourceCode: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/ColorKeyframe.kl')
    }
  });
});
