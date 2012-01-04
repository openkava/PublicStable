
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * A function to create a bezier key frame object.
 * @constructor
 * @param {number} time The time of the keyframe.
 * @param {number} value The value of the keyframe.
 * @param {number} intan The input tangent of the keyframe.
 * @param {number} outtan The output tangent of the keyframe.
 */
FABRIC.RT.BezierKeyframe = function(time, value, intan, outtan) {
  this.time = typeof time === 'number' ? time : 0;
  this.value = typeof value === 'number' ? value : 0;
  this.intangent = (intan &&
    intan.getType &&
    intan.getType() === 'FABRIC.RT.Vec2') ? intan : new FABRIC.RT.Vec2(-0.333, 0);
  this.outtangent = (outtan &&
    outtan.getType &&
    outtan.getType() === 'FABRIC.RT.Vec2') ? outtan : new FABRIC.RT.Vec2(0.333, 0);
};

FABRIC.RT.BezierKeyframe.prototype = {
  // This method enables an animation evaluator to know what kind of data
  // track full of these data types should evaluate to.
  get valueType() {
    return 'Scalar';
  },
  getType: function() {
    return 'FABRIC.RT.BezierKeyframe';
  }
};

/**
 * overloaded constructor function for BezierKeyFrame
 * @param {number} value The value of the keyframe.
 * @param {number} time The time of the keyframe.
 * @param {number} intan The input tangent of the keyframe.
 * @param {number} outtan The output tangent of the keyframe.
 * @return {object} The bezier key frame object.
 */

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('BezierKeyframe', {
    members: {
      time: 'Scalar', value: 'Scalar', intangent: 'Vec2', outtangent: 'Vec2'
    },
    constructor: FABRIC.RT.BezierKeyframe,
    klBindings: {
      filename: 'BezierKeyframe.kl',
      sourceCode: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/BezierKeyframe.kl')
    }
  });
});


if(!FABRIC.RT.KeyframeTrack){
  throw("please include the SceneGraph/RT/KeyframeTrack.js file before this one.");
}

FABRIC.RT.BezierKeyframeTrack = function(name, color, keys) {
  this.name = name ? name : "bezierAnimationTrack";
  this.color = color ? color : FABRIC.RT.rgb(1.0, 0.0, 0.0);
  this.keys = keys ? keys : [];
};

FABRIC.RT.BezierKeyframeTrack.prototype = {
  __proto__: FABRIC.RT.KeyframeTrack.prototype,
  newKey: function(value, time, intan, outtan) {
    return new FABRIC.RT.BezierKeyframe(value, time, intan, outtan);
  },
  setValue: function(time, value) {
    var numKeys = this.keys.length;
    if (numKeys == 0 || time > this.keys[numKeys - 1].time) {
      this.keys.push(this.newKey(time, value));
    }
    else {
      var keyIndex = -1;
      for (var i = 0; i < numKeys; i++) {
        if (this.keys[i].time >= time) {
          keyIndex = i;
          break;
        }
      }
      var key, prevkey, nextkey;
      if(this.keys[i].time === time){
        key = this.keys[keyIndex];
        key.value = value;
        /*
        this.keys[keyIndex].value = value;
        if(intan)
          this.keys[keyIndex].intangent = intan;
        if(outtan)
          this.keys[keyIndex].outtangent = outtan;
        */
      }
      else{
        key = new this.newKey(time, value);
        this.keys.splice(keyIndex, 0, key);
        numKeys = this.keys.length;
      //  this.keys.splice(keyIndex, 0, new this.newKey(time, value, intan, outtan));
      }
      
      var intan, outtan, intdelta, outtdelta, inGradient = 0.0, outGradient = 0.0;
      if(keyIndex > 0){
        prevkey = this.keys[keyIndex-1];
        intdelta = (key.time - prevkey.time);
        inGradient = (key.value - prevkey.value)/intdelta;
      }
      if(keyIndex < numKeys-1){
        nextkey = this.keys[keyIndex+1];
        outtdelta = (nextkey.time - key.time);
        outGradient = (nextkey.value - key.value)/outtdelta;
      }
      var gradient = 0.0;
      if(Math.sign(inGradient) === Math.sign(outGradient)){
      //  gradient = (Math.abs(inGradient) < Math.abs(outGradient) ? inGradient : outGradient);
        gradient = (inGradient + outGradient) * 0.5;
        if(prevkey){
          if(Math.abs(key.value - prevkey.value) < Math.abs(gradient * intdelta * 0.5)){
            gradient = (key.value - prevkey.value)/(intdelta * 0.5);
          }
        }
        if(nextkey){
          if(Math.abs(nextkey.value - key.value) < Math.abs(gradient * outtdelta * 0.5)){
            gradient = (nextkey.value - key.value)/(outtdelta * 0.5);
          }
        }
      }
      if(prevkey){
        key.intangent.set(intdelta * -0.33, gradient * intdelta * -0.33);
        
        var prevKeyInGradient = 0.0;
        var prevKeyGradient = 0.0;
        if(keyIndex > 1){
          prevKeyInGradient = (this.keys[keyIndex-2].value - prevkey.value)/
                              (this.keys[keyIndex-2].time - prevkey.time);
          prevKeyGradient = (Math.abs(prevKeyInGradient) < Math.abs(gradient) ? prevKeyInGradient : gradient);
        }
        prevkey.outtangent.set(intdelta * 0.33, prevKeyGradient * 0.33);
      }
      if(keyIndex < numKeys-1){
        key.outtangent.set(outtdelta * 0.33, gradient * outtdelta * 0.33);
        
        var nextKeyOutGradient = 0.0;
        var nextKeyGradient = 0.0;
        if(keyIndex < numKeys-2){
          nextKeyOutGradient = (nextkey.value - this.keys[keyIndex+2].value)/
                               (nextkey.time - this.keys[keyIndex+2].time);
          nextKeyGradient = (Math.abs(nextKeyOutGradient) < Math.abs(gradient) ? nextKeyOutGradient : gradient);
        }
        nextkey.intangent.set(outtdelta * -0.33, nextKeyGradient * -0.33);
      }
    }
  }
}

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('BezierKeyframeTrack', {
    members: {
      name: 'String',
      color: 'Color',
      keys: 'BezierKeyframe[]'
    },
    constructor: FABRIC.RT.BezierKeyframeTrack,
    klBindings: {
      filename: 'FABRIC_ROOT/SceneGraph/RT/KeyframeTrack.kl',
      sourceCode: FABRIC.preProcessCode(
        FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/KeyframeTrack.kl'), {
          KEYFRAMEDATATYPE:'Scalar',
          KEYFRAMETYPE: 'BezierKeyframe',
          KEYFRAMEDATADEFAULTVALUE: '0.0'
        })
    }
  });
});

FABRIC.RT.BezierKeyframeTrackSet = function(name) {
  this.name = name ? name : 'bezierAnimationTrackSet';
  this.timeRange = new FABRIC.RT.Vec2(0,0);
  this.tracks = [];
};

FABRIC.RT.BezierKeyframeTrackSet.prototype = {
  __proto__: FABRIC.RT.KeyframeTrackSet.prototype,
  newTrack: function(name, color, keys) {
    return new FABRIC.RT.BezierKeyframeTrack(name, color, keys);
  }
}


FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('BezierKeyframeTrackSet', {
    members: {
      name: 'String',
      timeRange: 'Vec2',
      tracks: 'BezierKeyframeTrack[]'
    },
    constructor: FABRIC.RT.BezierKeyframeTrackSet,
    klBindings: {
      filename: 'FABRIC_ROOT/SceneGraph/RT/KeyframeTrack.kl',
      sourceCode: FABRIC.preProcessCode(
        FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/KeyframeTrack.kl'), {
          KEYFRAMEDATATYPE:'Scalar',
          KEYFRAMETYPE: 'BezierKeyframe',
          KEYFRAMEDATADEFAULTVALUE: '0.0'
        })
    }
  });
});
