
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//


FABRIC.RT.TrackSetController = function( name ) {
  this.activeTrackSet = -1;
  this.time = -1;
  this.currKeys = [];
  this.bindings = new FABRIC.RT.KeyframeTrackBindings();
  this.trackWeight = 0.0;
};


FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('TrackSetController', {
    members: {
      activeTrackSet: 'Integer',
      time: 'Scalar',
      currKeys: 'Integer[]',
      bindings: 'KeyframeTrackBindings',
      trackWeight: 'Scalar'
    },
    constructor: FABRIC.RT.TrackSetController
  });
});
