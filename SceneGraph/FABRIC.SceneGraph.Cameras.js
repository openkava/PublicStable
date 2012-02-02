
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.define(["SceneGraph/FABRIC.SceneGraph",
               "SceneGraph/FABRIC.SceneGraph.Kinematics",
               "SceneGraph/RT/Color"], function() {


FABRIC.SceneGraph.registerNodeType('Camera', {
  briefDesc: 'The Camera node implements an OpenGL camera for the ViewPort node.',
  detailedDesc: 'The Camera node uses a redraw event handler to draw the camera projection to '+
                'the OpenGL canvas.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    nearDistance: 'The near clipping distance for the camera.',
    farDistance: 'The far clipping distance for the camera.',
    fovY: 'The vertical (Y) field of view angle for this camera.',
    focalDistance: 'The focal distance for the camera.',
    orthographic: 'Set to true the camera is rendered in orthographic more, otherwise perspective mode is used.',
    transformNode: 'The type of transformNode to use, typically \'Transform\'',
    screenOffset: 'Viewport center offset'
  },
  factoryFn: function(options, scene) {

    scene.assignDefaults(options, {
        nearDistance: 5,
        farDistance: 1000,
        fovY: Math.degToRad(60),
        focalDistance: 160,
        orthographic: false,
        transformNode: 'Transform',
        screenOffset: new FABRIC.RT.Vec2(0.0, 0.0)
      });

    var cameraNode = scene.constructNode('SceneGraphNode', options),
      dgnode = cameraNode.constructDGNode('DGNode'),
      redrawEventHandler = cameraNode.constructEventHandlerNode('Redraw'),
      transformNode,
      transformNodeMember = 'globalXfo';
      
    dgnode.addMember('nearDistance', 'Scalar', options.nearDistance);
    dgnode.addMember('farDistance', 'Scalar', options.farDistance);
    dgnode.addMember('fovY', 'Scalar', options.fovY);
    dgnode.addMember('focalDistance', 'Scalar', options.focalDistance);
    dgnode.addMember('cameraMat44', 'Mat44');
    dgnode.addMember('orthographic', 'Boolean', options.orthographic);
    dgnode.addMember('projectionMat44', 'Mat44');
    dgnode.addMember('screenOffset', 'Vec2', options.screenOffset);

    cameraNode.addMemberInterface(dgnode, 'nearDistance', true);
    cameraNode.addMemberInterface(dgnode, 'farDistance', true);
    cameraNode.addMemberInterface(dgnode, 'fovY', true);
    cameraNode.addMemberInterface(dgnode, 'focalDistance', true);
    cameraNode.addMemberInterface(dgnode, 'orthographic', true);
    cameraNode.addMemberInterface(dgnode, 'screenOffset', true);

    redrawEventHandler.setScope('camera', dgnode);

    redrawEventHandler.preDescendBindings.append(scene.constructOperator({
      operatorName: 'UpdateCameraProjection',
      srcFile: 'FABRIC_ROOT/SceneGraph/KL/updateCameraProjection.kl',
      entryFunctionName: 'updateCameraProjection',
      parameterLayout: [
        'camera.projectionMat44',
        'window.width',
        'window.height',
        'camera.nearDistance',
        'camera.farDistance',
        'camera.fovY',
        'camera.orthographic',
        'camera.screenOffset'
      ]
    }));

    // Now register the camera with the Scene Graph so that
    // It will connect the camera with the scene graph rendered elements.(shaders etc)
    redrawEventHandler.appendChildEventHandler(scene.getSceneRedrawEventHandler());

    // public interface
    cameraNode.pub.getTransformNode = function() {
      return transformNode.pub;
    };
    cameraNode.pub.setTransformNode = function(node, member) {
      if (member) {
        transformNodeMember = member;
      }
      node = scene.getPrivateInterface(node);
      if (!(node.getDGNode() && node.getDGNode().getMembers()[transformNodeMember])) {
        var message = 'Error in Transform node assignement on :' + node.name +
          ' \n member not found :' + transformNodeMember + '\n\n';
        message += 'Members:' + JSON.stringify(node.getDGNode().getMembers());
        throw (message);
      }
      transformNode = node;
      dgnode.setDependency(transformNode.getDGNode(), 'transform');

      dgnode.bindings.append(scene.constructOperator({
        operatorName: 'loadXfo',
        srcCode: 'use Xfo, Mat44; operator loadXfo(io Xfo xfo, io Mat44 mat44){ mat44 = xfo.toMat44().inverse(); }',
        entryFunctionName: 'loadXfo',
        parameterLayout: [
          'transform.' + transformNodeMember,
          'self.cameraMat44'
        ]
      }));
    };
    cameraNode.addMemberInterface(dgnode, 'cameraMat44');
    cameraNode.addMemberInterface(dgnode, 'projectionMat44');
    cameraNode.addMemberInterface(dgnode, 'nearDistance', true);
    cameraNode.addMemberInterface(dgnode, 'farDistance', true);
    cameraNode.addMemberInterface(dgnode, 'fovY', true);
    cameraNode.addMemberInterface(dgnode, 'focalDistance', true);

    scene.addEventHandlingFunctions(cameraNode);
    
    
    var parentWriteData = cameraNode.writeData;
    var parentReadData = cameraNode.readData;
    cameraNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      if(transformNodeMember){
        nodeData.transformNodeMember = transformNodeMember;
      }
      sceneSerializer.addNode(transformNode.pub);
      nodeData.transformNode = transformNode.pub.getName();
      
      nodeData.nearDistance = cameraNode.pub.getNearDistance();
      nodeData.farDistance = cameraNode.pub.getFarDistance();
      nodeData.fovY = cameraNode.pub.getFovY();
      nodeData.focalDistance = cameraNode.pub.getFocalDistance();
      
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
    };
    cameraNode.readData = function(sceneDeserializer, nodeData) {
      cameraNode.pub.setTransformNode(sceneDeserializer.getNode(nodeData.transformNode), nodeData.transformNodeMember);
      
      cameraNode.pub.setNearDistance(nodeData.nearDistance);
      cameraNode.pub.setFarDistance(nodeData.farDistance);
      cameraNode.pub.setFovY(nodeData.fovY);
      cameraNode.pub.setFocalDistance(nodeData.focalDistance);
      
      parentReadData(sceneDeserializer, nodeData);
    };

    if (typeof options.transformNode == 'string') {
      cameraNode.pub.setTransformNode(scene.constructNode(options.transformNode, { hierarchical: false }).pub);
    }else {
      cameraNode.pub.setTransformNode(options.transformNode);
    }
    return cameraNode;
  }});



FABRIC.SceneGraph.registerNodeType('FreeCamera', {
  briefDesc: 'The FreeCamera node implements an OpenGL camera in a free roaming mode.',
  detailedDesc: 'The FreeCamera node uses the Camera node to implement a free roaming camera without a target point.',
  parentNodeDesc: 'Camera',
  optionsDesc: {
    position: 'The position of the free camera (Vec3).',
    orientation: 'The orientation of the free camera (Quaternion).',
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        position: new FABRIC.RT.Vec3(1, 0, 0),
        orientation: new FABRIC.RT.Quat()
      });

    options.transformNode = scene.constructNode('Transform', {
      globalXfo: new FABRIC.RT.Xfo({ tr: options.position, ori: options.orientation })
    });

    var freeCameraNode = scene.constructNode('Camera', options);

    return freeCameraNode;
  }});



FABRIC.SceneGraph.registerNodeType('TargetCamera', {
  briefDesc: 'The TargetCamera node implements an OpenGL camera in using a target point',
  detailedDesc: 'The TargetCamera node uses the Camera node to implement a camera with a target point, similar to the GLUT lookat camera.',
  parentNodeDesc: 'Camera',
  optionsDesc: {
    target: 'The target point of the target camera (Vec3).',
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        target: new FABRIC.RT.Vec3(0, 0, 0)
      });

    if(!options.transformNode) {
      options.transformNode = scene.pub.constructNode('AimTransform', {
        globalXfo: options.globalXfo,
        position: options.position,
        target: options.target
      });
    }

    var targetCameraNode = scene.constructNode('Camera', options);

    targetCameraNode.getDGNode().bindings.append(scene.constructOperator({
      operatorName: 'loadFocalDist',
      srcCode: 'use Xfo, Vec3; operator loadFocalDist(io Xfo xfo, io Vec3 target, io Scalar focalDist){' +
      '  focalDist = xfo.tr.distanceTo(target);' +
      '}',
      entryFunctionName: 'loadFocalDist',
      parameterLayout: [
        'transform.globalXfo',
        'transform.target',
        'self.focalDistance'
      ]
    }));

    return targetCameraNode;
  }});


});

