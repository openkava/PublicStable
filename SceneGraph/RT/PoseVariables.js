
FABRIC.RT.PoseVariables = function() {
    this.v = new FABRIC.RT.Vec3(0.0,0.0,0.0);
    this.w = 1.0;
  this.scalarValues = [];
  this.vec3Values = [];
  this.quatValues = [];
  this.xfoValues = [];
};


FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('PoseVariables', {
    members: {
      v: 'Vec3', w: 'Scalar',
      scalarValues: 'Scalar[]',
      vec3Values: 'Vec3[]',
      quatValues: 'Quat[]',
      xfoValues: 'Xfo[]'
    },
    constructor: FABRIC.RT.PoseVariables,
    klBindings: {
      filename: 'PoseVariables.kl',
      sourceCode: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/PoseVariables.kl')
    }
  });
});
