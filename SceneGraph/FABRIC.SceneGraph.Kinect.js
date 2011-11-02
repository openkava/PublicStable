
//
// Copyright 2010-2011Fabric Technologies Inc. All rights reserved.
//
FABRIC.RT.KinectCamera = function(options) {
  if(!options)
    options = {};
  this.localData = null;
  this.initiated = false;
  this.tiltAngle = 0;
  this.width = 640;
  this.height = 480;
  this.supportsColor = options.supportsColor == undefined ? true : options.supportsColor;
  this.supportsDepth = options.supportsDepth == undefined ? false : options.supportsDepth;
  this.supportsSkeleton = options.supportsSkeleton == undefined ? false : options.supportsSkeleton;
  this.colorData = [];
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
      tiltAngle: 0
    });
    
    // create the bullet node
    var kinectNode = scene.constructNode('SceneGraphNode', {name: 'KinectCameraNode'});
    var dgnode = kinectNode.constructDGNode('DGNode');
    dgnode.addMember('camera', 'KinectCamera', new FABRIC.RT.KinectCamera(options));
    dgnode.addMember('tiltAngle', 'Integer', options.tiltAngle);
    kinectNode.addMemberInterface(dgnode, 'camera', false);
    kinectNode.addMemberInterface(dgnode, 'tiltAngle', true);

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
      
      kinectNode.pub.connectToTexture = function(textureNode) {
        var textureDGNode = scene.getPrivateInterface(textureNode).getDGNode();
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
      };
    }
    
    return kinectNode;
  }});
