
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//


FABRIC.RT.KeyframeTrack = function(name, color, keys) {
  this.name = name ? name : "";
  this.color = color ? color : FABRIC.RT.rgb(1.0, 0.0, 0.0);
  this.keys = keys ? keys : [];
};

FABRIC.RT.KeyframeTrackSet = function( name ) {
  this.name = name ? name : 'animationTrack';
  this.timeRange = new FABRIC.RT.Vec2(0,0);
  this.tracks = [];
};

FABRIC.RT.KeyframeTrackSet.prototype = {
  addScalarTrack: function(name, color){
    this.tracks.push(new FABRIC.RT.KeyframeTrack(name, color));
    return this.tracks.length - 1;
  },
  addXfoTrack: function(name, storeEulerAngles){
    var binding = [];
    this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'tr.x', FABRIC.RT.Color.red));      binding.push(this.tracks.length - 1);
    this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'tr.y', FABRIC.RT.Color.green));    binding.push(this.tracks.length - 1);
    this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'tr.z', FABRIC.RT.Color.blue));     binding.push(this.tracks.length - 1);
    
    if(storeEulerAngles == true){
      this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'euler.x', FABRIC.RT.Color.red));   binding.push(this.tracks.length - 1);
      this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'euler.x', FABRIC.RT.Color.green)); binding.push(this.tracks.length - 1);
      this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'euler.x', FABRIC.RT.Color.blue));  binding.push(this.tracks.length - 1);
    }
    else{
      this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'ori.v.x', FABRIC.RT.Color.red));   binding.push(this.tracks.length - 1);
      this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'ori.v.x', FABRIC.RT.Color.green)); binding.push(this.tracks.length - 1);
      this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'ori.v.x', FABRIC.RT.Color.blue));  binding.push(this.tracks.length - 1);
      this.tracks.push(new FABRIC.RT.KeyframeTrack(name+'ori.w',   FABRIC.RT.Color.cyan));  binding.push(this.tracks.length - 1);
    }
    return binding;
  }
};



