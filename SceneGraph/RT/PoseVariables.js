
FABRIC.RT.PoseVariables = function() {
  this.scalarValues = [];
  this.vec3Values = [];
  this.quatValues = [];
  this.xfoValues = [];
};


FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('PoseVariables', {
    members: {
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


FABRIC.RT.PoseParameterBinding = function(source, target) {
  this.source = (source != undefined) ? source : -1;
  this.target = (target != undefined) ? target : -1;
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('PoseParameterBinding', {
    members: {
      source: 'Integer',
      target: 'Integer'
    },
    constructor: FABRIC.RT.PoseParameterBinding
  });
});

