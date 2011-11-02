
//
// Copyright 2010-2011Fabric Technologies Inc. All rights reserved.
//
FABRIC.RT.KinectCamera = function(options) {
  if(!options)
    options = {};
  this.localData = null;
  this.initiated = false;
  this.tiltAngle = 0;
  this.width = 320;
  this.height = 240;
  this.supportsColor = options.supportsColor == undefined ? true : options.supportsColor;
  this.supportsDepth = options.supportsDepth == undefined ? false : options.supportsDepth;
  this.supportsSkeleton = options.supportsSkeleton == undefined ? false : options.supportsSkeleton;
  this.colorData = [];
  this.playerData = [];
  this.depthData = [];
  this.skeletonData = [];
};

FABRIC.SceneGraph.registerNodeType('KinectCamera', {
  briefDesc: 'The KinectCamera represents a Microsoft Kinect Camera.',
  detailedDesc: 'The KinectCamera represents a Microsoft Kinect Camera, providing access to its rgba and depth channel, as well as NUI skeleton data.',
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      supportsColor: true,
      supportsDepth: false,
      supportsSkeleton: false,
      tiltAngle: 0,
      depthMin: 800,
      depthMax: 1500
    });
    
    // create the bullet node
    var kinectNode = scene.constructNode('SceneGraphNode', {name: 'KinectCameraNode'});
    var dgnode = kinectNode.constructDGNode('DGNode');
    dgnode.addMember('camera', 'KinectCamera', new FABRIC.RT.KinectCamera(options));
    dgnode.addMember('tiltAngle', 'Integer', options.tiltAngle);
    dgnode.addMember('depthMin', 'Integer', options.depthMin);
    dgnode.addMember('depthMax', 'Integer', options.depthMax);
    kinectNode.addMemberInterface(dgnode, 'camera', false);
    kinectNode.addMemberInterface(dgnode, 'tiltAngle', true);
    kinectNode.addMemberInterface(dgnode, 'depthMin', true);
    kinectNode.addMemberInterface(dgnode, 'depthMax', true);

    // create init operator
    dgnode.bindings.append(scene.constructOperator({
      operatorName: 'initKinectCamera',
      parameterLayout: [
        'self.camera'
      ],
      entryFunctionName: 'initKinectCamera',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/kinect.kl'
    }));

    // create tilt operator
    dgnode.bindings.append(scene.constructOperator({
      operatorName: 'tiltKinectCamera',
      parameterLayout: [
        'self.camera',
        'self.tiltAngle'
      ],
      entryFunctionName: 'tiltKinectCamera',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/kinect.kl'
    }));
    
    // see if we need to support color
    if(options.supportsColor) {
      
      kinectNode.pub.constructColorTextureNode = function() {
        var textureNode = scene.constructNode('Image', {
          createResourceLoadNode: false,
          createDgNode: true,
          width: 640,
          height: 480,
          forceRefresh: true
        });
        var textureDGNode = textureNode.getDGNode();
        textureDGNode.setDependency(scene.getGlobalsNode(), 'globals');
        textureDGNode.setDependency(dgnode,'kinect');

        // create init operator
        textureDGNode.bindings.append(scene.constructOperator({
          operatorName: 'getKinectCameraColorPixels',
          parameterLayout: [
            'kinect.camera',
            'self.pixels',
            'self.width',
            'self.height',
            'globals.time'
          ],
          entryFunctionName: 'getKinectCameraColorPixels',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/kinect.kl'
        }));
        
        return textureNode.pub;
      };
    }
    
    // see if we need to support depth
    if(options.supportsDepth) {
      
      kinectNode.pub.constructDepthTextureNode = function() {
        var textureNode = scene.constructNode('Image', {
          createResourceLoadNode: false,
          createDgNode: true,
          width: 320,
          height: 240,
          forceRefresh: true
        });
        var textureDGNode = textureNode.getDGNode();
        textureDGNode.setDependency(scene.getGlobalsNode(), 'globals');
        textureDGNode.setDependency(dgnode,'kinect');

        // create init operator
        textureDGNode.bindings.append(scene.constructOperator({
          operatorName: 'getKinectCameraDepthAsColorPixels',
          parameterLayout: [
            'kinect.camera',
            'self.pixels',
            'self.width',
            'self.height',
            'kinect.depthMin',
            'kinect.depthMax',
            'globals.time'
          ],
          entryFunctionName: 'getKinectCameraDepthAsColorPixels',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/kinect.kl'
        }));

        return textureNode.pub;
      };
    }
    
    return kinectNode;
  }});
