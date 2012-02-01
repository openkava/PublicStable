
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.define(["SceneGraph/RT/Vec3"], function() {
  
/**
 * Constructor for a bounding box object.
 * @constructor
 */
FABRIC.RT.BoundingBox = function() {
  this.min = new FABRIC.RT.Vec3();
  this.max = new FABRIC.RT.Vec3();
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('BoundingBox', {
    members: {
      min: 'Vec3', max: 'Vec3'
    },
    constructor: FABRIC.RT.BoundingBox
  });
});

});