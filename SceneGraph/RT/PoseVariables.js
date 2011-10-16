


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
    constructor: FABRIC.RT.KeyframeTrackBindings
  });
});


FABRIC.RT.PoseVariables = function() {
  this.scalarValues = [];
  this.vec3Values = [];
  this.quatValues = [];
  this.xfoValues = [];
};


FABRIC.RT.PoseVariables.prototype = {
  addVariable: function(type, value) {
    var id = -1;
    switch(type){
      case 'Scalar':
        id = this.scalarValues.length;
        this.scalarValues.push( value ? value : 0.0);
        break;
      case 'Vec3':
        id = this.vec3Values.length;
        this.vec3Values.push( value ? value : new FABRIC.RT.Vec3());
        break;
      case 'Quat':
        id = this.quatValues.length;
        this.quatValues.push( value ? value : new FABRIC.RT.Quat());
        break;
      case 'Xfo':
        id = this.xfoValues.length;
        this.xfoValues.push( value ? value : new FABRIC.RT.Xfo());
        break;
      case 'Xfo[]':
        id = [];
        for(var i=0; i<value.length; i++){
          id.push(this.xfoValues.length);
          this.xfoValues.push( value[i] );
        }
        break;
      default:
        throw "unsupported Type:" + type;
    }
    return id;
  }
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
