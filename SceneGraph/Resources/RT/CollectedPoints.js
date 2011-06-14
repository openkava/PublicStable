/**
 * Constructor for a collected points object.
 * @constructor
 */
FABRIC.Math.CollectedPoints = function() {
  this.points = [];
  this.distances = [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('CollectedPoints', {
    members: {
      points: 'Integer[]', distances: 'Scalar[]'
    },
    constructor: FABRIC.Math.CollectedPoints
  });
});
