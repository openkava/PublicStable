



FABRIC.RT.KeyframeTrackBinding = function(varId, trackIds) {
  this.varId = varId != undefined ? varId : -1;
  this.trackIds = trackIds != undefined ? trackIds : [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('KeyframeTrackBinding', {
    members: {
      varId: 'Integer',
      trackIds: 'Integer[]'
    },
    constructor: FABRIC.RT.KeyframeTrackBinding
  });
});


FABRIC.RT.KeyframeTrackBindings = function() {
  this.scalarBindings = [];
  this.vec3Bindings = [];
  this.quatBindings = [];
  this.xfoBindings = [];
};


FABRIC.RT.KeyframeTrackBindings.prototype = {
  addScalarBinding: function(varId, trackId){
    this.scalarBindings.push(new FABRIC.RT.KeyframeTrackBinding(varId, [trackId]));
  },
  addXfoBinding: function(varId, trackIds){
    this.xfoBindings.push(new FABRIC.RT.KeyframeTrackBinding(varId, trackIds));
  }
};


FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('KeyframeTrackBindings', {
    members: {
      scalarBindings: 'KeyframeTrackBinding[]',
      vec3Bindings: 'KeyframeTrackBinding[]',
      quatBindings: 'KeyframeTrackBinding[]',
      xfoBindings: 'KeyframeTrackBinding[]'
    },
    constructor: FABRIC.RT.KeyframeTrackBindings,
    klBindings: {
      filename: 'KeyframeTrackBindings.kl',
      sourceCode: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/KeyframeTrackBindings.kl')
    }
  });
});

