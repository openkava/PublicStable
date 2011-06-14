/**
 * Constructor for a bounding box object.
 * @constructor
 */
FABRIC.Math.BoundingBox = function() {
  this.min = FABRIC.Math.vec3();
  this.max = FABRIC.Math.vec3();
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('BoundingBox', {
    members: {
      min: 'Vec3', max: 'Vec3'
    },
    constructor: FABRIC.Math.BoundingBox
  });
});
