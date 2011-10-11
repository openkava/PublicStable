
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.RT.KeyframeTrackBindings = function() {
  this.scalarBindings = [];
  this.vec3Bindings = [];
  this.quatBindings = [];
  this.xfoBindings = [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('KeyframeTrackBindings', {
    members: {
      scalarBindings: 'Integer[]',
      vec3Bindings: 'Integer[][]',
      quatBindings: 'Integer[][]',
      xfoBindings: 'Integer[][]'
    },
    constructor: FABRIC.RT.KeyframeTrackBindings
  });
});
