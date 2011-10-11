
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//


FABRIC.RT.PoseParameters = function(time, value) {
  this.scalarValues = [];
  this.vec3Values = [];
  this.quatValues = [];
  this.xfoValues = [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('PoseParameters', {
    members: {
      scalarValues: 'Scalar[]',
      vec3Values: 'Vec3[]',
      quatValues: 'Quat[]',
      xfoValues: 'Xfo[]'
    },
    constructor: FABRIC.RT.PoseParameters,
    kBindings: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/PoseParameters.kl')
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

