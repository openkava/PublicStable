
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

FABRIC.SceneGraph.registerNodeType('CameraManipulator', {
  briefDesc: 'The CameraManipulator is a basic tool for controling viewport cameras.',
  detailedDesc: 'The CameraManipulator is a basic tool for controling viewport cameras.' +
                'Camera manipulation is controlled with the mouse. ' +
                'The Left mouse button is used to orbit the camera. ' +
                'The Middle mouse button is used to pan the camera. ' +
                'The Right mouse button is used to zoom the camera. ',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    mouseWheelZoomRate: 'The rate at which the camera will zoom when the mouse wheel it manipulated',
    mouseDragZoomRate: 'The rate at which the camera will zoom when the mouse is dragged with the right mouse button held down',
    orbitRate: 'The rate at which the camera will orbit when the mouse is dragged with the left mouse button held down',
    enabled: 'The manipulator can be disabled, thereby allowing other manipulators to operate.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        mouseWheelZoomRate: 0.3,
        mouseDragZoomRate:0.001,
        orbitRate: 0.005,
        enabled: true
      });

    if (!options.targetNode) {
      throw ('Camera not specified');
    }

    var cameraNode = options.targetNode;
    var cameraManipulatorNode = scene.constructNode('SceneGraphNode', options);
    scene.addEventHandlingFunctions(cameraManipulatorNode);

    var enabled = options.enabled;
    cameraManipulatorNode.pub.enable = function(){
      enabled = true;
    }
    cameraManipulatorNode.pub.disable = function(){
      enabled = false;
    }

    var mouseWheelZoomFn = function(evt) {
      if(!enabled){
        return;
      }
      var zoomDist = cameraNode.getFocalDistance() * options.mouseWheelZoomRate * evt.wheelDelta * -0.001;
      var cameraXfo = cameraNode.getTransformNode().getGlobalXfo();
      var cameraZoom = cameraXfo.ori.getZaxis().multiplyScalar(zoomDist);
      if(!evt.altKey || !cameraNode.getTransformNode().getTarget) {
        cameraXfo.tr = cameraXfo.tr.add(cameraZoom);
        cameraNode.getTransformNode().setGlobalXfo(cameraXfo);
        if (!cameraNode.getTransformNode().getTarget) {
          cameraNode.setFocalDistance(cameraNode.getFocalDistance() - zoomDist);
        }
      } else {
        var targetPos = cameraNode.getTransformNode().getTarget();
        targetPos = targetPos.add(cameraZoom);
        cameraNode.getTransformNode().setTarget(targetPos);
      }
      cameraManipulatorNode.pub.fireEvent('cameraManipulated',{newCameraXfo: cameraXfo});
      evt.viewportNode.redraw();
      evt.stopPropagation();
    }
    cameraNode.addEventListener('mousewheel', mouseWheelZoomFn);

    var cameraPos, cameraTarget, cameraOffset, cameraXfo, upaxis, swaxis, focalDist;
    var mouseDownScreenPos, viewportNode;
    var getCameraValues = function(evt) {
      mouseDownScreenPos = evt.mouseScreenPos;
      viewportNode = evt.viewportNode;
      cameraXfo = evt.cameraNode.getTransformNode().getGlobalXfo();
      cameraPos = cameraXfo.tr;
      swaxis = cameraXfo.ori.getXaxis();
      upaxis = cameraXfo.ori.getYaxis();

      focalDist = evt.cameraNode.getFocalDistance();
      cameraOffset = cameraXfo.ori.getZaxis().multiplyScalar(-focalDist);
      cameraTarget = cameraPos.add(cameraOffset);
    }
    var mouseDownFn = function(evt) {
      if(!enabled){
        return;
      }
      if (evt.button === 0) {
        getCameraValues(evt);
        document.addEventListener('mousemove', dragOrbitFn, false);
        document.addEventListener('mouseup', releaseOrbitFn, false);
      }
      else if (evt.button === 1) {
        getCameraValues(evt);
        document.addEventListener('mousemove', dragPanFn, false);
        document.addEventListener('mouseup', releasePanFn, false);
      }
      else if (evt.button === 2) {
        getCameraValues(evt);
        document.addEventListener('mousemove', dragZoomFn, false);
        document.addEventListener('mouseup', releaseZoomFn, false);
      }
    }
    cameraNode.addEventListener('mousedown', mouseDownFn);

    var dragOrbitFn = function(evt) {
      if(!enabled){
        return;
      }
      var mouseDragScreenDelta = evt.mouseScreenPos.subtract(mouseDownScreenPos);
      var newcameraXfo = cameraXfo.clone();
      var arbit = new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0,1,0), mouseDragScreenDelta.x * -options.orbitRate);
      newcameraXfo.ori = arbit.multiply(newcameraXfo.ori);
      
      var pitch = new FABRIC.RT.Quat().setFromAxisAndAngle(newcameraXfo.ori.getXaxis(), mouseDragScreenDelta.y * -options.orbitRate);
      newcameraXfo.ori = pitch.multiply(newcameraXfo.ori);
      
      var newCameraOffset = arbit.rotateVector(cameraOffset);
      newCameraOffset = pitch.rotateVector(newCameraOffset);
      newcameraXfo.tr = cameraTarget.subtract(newCameraOffset);
      
      cameraNode.getTransformNode().setGlobalXfo(newcameraXfo);
      cameraManipulatorNode.pub.fireEvent('cameraManipulated',{newCameraXfo: newcameraXfo});
      viewportNode.redraw(true);
      evt.stopPropagation();
    }

    var releaseOrbitFn = function(evt) {
      viewportNode.redraw();
      document.removeEventListener('mousemove', dragOrbitFn, false);
      document.removeEventListener('mouseup', releaseOrbitFn, false);
      evt.stopPropagation();
    }
    var dragPanFn = function(evt) {
      if(!enabled){
        return;
      }
      var mouseDragScreenDelta = evt.mouseScreenPos.subtract(mouseDownScreenPos);
      var dragDist = upaxis.multiplyScalar(mouseDragScreenDelta.y)
                           .add(swaxis.multiplyScalar(-mouseDragScreenDelta.x))
                           .multiplyScalar(focalDist * 0.001);
      var newcameraXfo = cameraXfo.clone();
      newcameraXfo.tr = newcameraXfo.tr.add(dragDist);
      cameraNode.getTransformNode().setGlobalXfo(newcameraXfo);
      if (cameraNode.getTransformNode().getTarget) {
        cameraNode.getTransformNode().setTarget(cameraTarget.add(dragDist));
      }
      cameraManipulatorNode.pub.fireEvent('cameraManipulated',{newCameraXfo: cameraXfo});
      viewportNode.redraw(true);
      evt.stopPropagation();
    }
    var releasePanFn = function(evt) {
      viewportNode.redraw();
      document.removeEventListener('mousemove', dragPanFn, false);
      document.removeEventListener('mouseup', releasePanFn, false);
      evt.stopPropagation();
    }


    var dragZoomFn = function(evt) {
      if(!enabled){
        return;
      }
      var mouseDragScreenDelta = evt.mouseScreenPos.x - mouseDownScreenPos.x;
      
      var zoomDist = cameraNode.getFocalDistance() * -options.mouseDragZoomRate * mouseDragScreenDelta;
      var newcameraXfo = cameraXfo.clone();
      var cameraZoom = cameraXfo.ori.getZaxis().multiplyScalar(zoomDist);
      newcameraXfo.tr = newcameraXfo.tr.add(cameraZoom);
      cameraNode.getTransformNode().setGlobalXfo(newcameraXfo);
      if (!cameraNode.getTransformNode().getTarget) {
        cameraNode.setFocalDistance(cameraNode.getFocalDistance() - zoomDist);
      }
      cameraManipulatorNode.pub.fireEvent('cameraManipulated',{newCameraXfo: cameraXfo});
      viewportNode.redraw(true);
      evt.stopPropagation();
    }
    var releaseZoomFn = function(evt) {
      viewportNode.redraw();
      document.removeEventListener('mousemove', dragZoomFn, false);
      document.removeEventListener('mouseup', releaseZoomFn, false);
      evt.stopPropagation();
    }

    return cameraManipulatorNode;
  }});


FABRIC.SceneGraph.registerNodeType('PaintManipulator', {
  briefDesc: 'The PaintManipulator collects points using a brush.',
  detailedDesc: 'Paint manipulators can be used to modify many vertex attributes at once.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    brushSize: 'The radius of the brush',
    brushScaleSpeed: 'The mouse wheeel is used to modify the brush radius, and this paramter controls how fast the bush is resized',
    enabled: 'Toggle controlling whther this manipulator traps events.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      brushSize: 0.1,
      brushScaleSpeed: 1.0,
      enabled:true
      });

    var paintManipulatorNode = scene.constructNode('SceneGraphNode', options),
      paintEventHandler = paintManipulatorNode.constructEventHandlerNode('Paint'),
      paintEvent,
      viewportNode,
      brushPos,
      width,
      height,
      cameraMatrix,
      projectionMatrix,
      aspectRatio,
      paintableNodes = [],
      paintableNodePaintHandlers = [],
      enabled = options.enabled,
      brushSize = options.brushSize;

    scene.addEventHandlingFunctions(paintManipulatorNode);
    
    paintEventHandler.addMember('cameraMatrix', 'Mat44');
    paintEventHandler.addMember('projectionMatrix', 'Mat44');
    paintEventHandler.addMember('aspectRatio', 'Scalar');
    paintEventHandler.addMember('brushPos', 'Vec3');
    
    paintEventHandler.addMember('brushSize', 'Scalar', brushSize);
    paintManipulatorNode.addMemberInterface(paintEventHandler, 'brushSize', true);

    paintEventHandler.setScopeName('paintData');

    // Raycast events are fired from the viewport. As the event
    // propagates down the tree it collects scopes and fires operators.
    // The operators us the collected scopes to calculate the ray.
    paintEvent = paintManipulatorNode.constructEventNode('Event');
    paintEvent.appendEventHandler(paintEventHandler);
    
    var brushMaterial = scene.constructNode('FlatScreenSpaceMaterial', { color: FABRIC.RT.rgb(0.8, 0, 0) });
    var brushShapeTransform = scene.constructNode('Transform', { hierarchical: false, globalXfo: new FABRIC.RT.Xfo({
        ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(1, 0, 0), Math.HALF_PI),
        sc: new FABRIC.RT.Vec3(0, 0, 0)
      }) });
    var brushInstance = scene.constructNode('Instance', {
        transformNode: brushShapeTransform.pub,
        geometryNode: scene.constructNode('Circle', { radius: 1.0 }).pub,
        materialNode: brushMaterial.pub,
        enableDrawing: false
      });

    var collectPoints = function() {
      paintEventHandler.setData('brushPos', brushPos);
      paintEventHandler.setData('cameraMatrix', cameraMatrix);
      paintEventHandler.setData('projectionMatrix', projectionMatrix);
      paintEventHandler.setData('aspectRatio', aspectRatio);
      return paintEvent.select('CollectedPoints');
    };
    
    paintManipulatorNode.pub.setBrushSize = function(val){
      brushSize = val;
      paintEventHandler.setData('brushSize', brushSize);
    }
    paintManipulatorNode.pub.getBrushSize = function(){
      return brushSize;
    }

    var resizePaintBrushFn = function(evt) {
      if(!enabled){
        return;
      }
      brushSize += evt.wheelDelta * 0.0001 * options.brushScaleSpeed;
      brushSize = Math.max(brushSize, 0.01);
      paintEventHandler.setData('brushSize', brushSize);
      evt.stopPropagation();
      evt.viewportNode.redraw();
      
      paintManipulatorNode.pub.fireEvent('brushsizechanged', { brushSize:brushSize });
    }
    scene.pub.addEventListener('mousewheel', resizePaintBrushFn);

    var getOffset = function( el ) {
      var _x = 0;
      var _y = 0;
      while( el && !isNaN( el.offsetLeft ) && !isNaN( el.offsetTop ) ) {
          _x += el.offsetLeft - el.scrollLeft;
          _y += el.offsetTop - el.scrollTop;
          el = el.offsetParent;
      }
      return { top: _y, left: _x };
    }
    var getMousePos = function(evt) {
      if (evt.offsetX) {
        return new FABRIC.RT.Vec2(evt.offsetX, evt.offsetY);
      }
      else if (evt.pageY) {
        var eloffset = getOffset(evt.target);
        return new FABRIC.RT.Vec2(evt.pageX-eloffset.left, evt.pageY-eloffset.top);
      }
      throw("Unsupported Browser");
    }

    var moveBrush = function(evt) {
      var mousepos = getMousePos(evt);
      width = parseInt(evt.target.width, 10);
      height = parseInt(evt.target.height, 10);
      aspectRatio = width / height;
      brushPos = new FABRIC.RT.Vec3(((mousepos.x / width) - 0.5) * 2.0, ((mousepos.y / height) - 0.5) * -2.0, 0);

      brushShapeTransform.pub.setGlobalXfo(new FABRIC.RT.Xfo({
          tr: brushPos,
          ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(1, 0, 0), Math.HALF_PI),
          sc: new FABRIC.RT.Vec3(brushSize / aspectRatio, brushSize, brushSize)
        }));
    }

    var mouseMoveFn = function(evt) {
      if(!enabled){
        return;
      }
      viewportNode = evt.viewportNode;
      moveBrush(evt);
      viewportNode.redraw();
    }
    scene.pub.addEventListener('mousemove', mouseMoveFn);

    var paintFn = function(evt) {
      if(!enabled){
        return;
      }
      moveBrush(evt);
      var points = collectPoints();

      evt.paintData = points.length > 0 ? points[0].value : null;
      paintManipulatorNode.pub.fireEvent('onpaint', evt);
      viewportNode.redraw();
      evt.stopPropagation();
    }

    var mouseDownFn = function(evt) {
      if(!enabled){
        return;
      }
      if (evt.button === 0) {
        // Paint
        cameraMatrix = viewportNode.getCameraNode().getCameraMat44();
        projectionMatrix = viewportNode.getCameraNode().getProjectionMat44();

        document.addEventListener('mousemove', paintFn, false);
        document.addEventListener('mouseup', releasePaintFn, false);

        paintManipulatorNode.pub.fireEvent('onbeginpaint', evt);
        paintFn(evt);
      }
    }
    scene.pub.addEventListener('mousedown', mouseDownFn);

    var releasePaintFn = function(evt) {
      document.removeEventListener('mousemove', paintFn, false);
      document.removeEventListener('mouseup', releasePaintFn, false);
    }
    
    
    paintManipulatorNode.pub.addPaintableNode = function(node) {
      if (!node.isTypeOf || !node.isTypeOf('Instance')) {
        throw ('Incorrect type. Must assign a Instance');
      }

      var paintInstanceEventHandler,
        instanceNode = scene.getPrivateInterface(node),
        geometryNode = scene.getPrivateInterface(instanceNode.pub.getGeometryNode()),
        transformNode = scene.getPrivateInterface(instanceNode.pub.getTransformNode()),
        paintOperator;

      paintInstanceEventHandler = paintManipulatorNode.constructEventHandlerNode('Paint' + node.getName());
      paintInstanceEventHandler.setScope('geometryattributes', geometryNode.getAttributesDGNode());
      paintInstanceEventHandler.setScope('geometryuniforms', geometryNode.getAttributesDGNode());
      paintInstanceEventHandler.setScope('transform', transformNode.getDGNode());
      paintInstanceEventHandler.setScope('instance', instanceNode.getDGNode());

      // The selector will return the node bound with the given binding name.
      var paintingOpDef = options.paintingOpDef;
      if(!paintingOpDef){
        paintingOpDef = {
          operatorName: 'collectPointsInsideBrush',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/collectPointsInsideVolume.kl',
          entryFunctionName: 'collectPointsInsideBrush',
          parameterLayout: [
            'paintData.cameraMatrix',
            'paintData.projectionMatrix',
            'paintData.aspectRatio',
  
            'paintData.brushPos',
            'paintData.brushSize',
  
            'transform.' + instanceNode.pub.getTransformNodeMember(),
            'geometryattributes.positions<>',
            'geometryattributes.normals<>'
          ],
          async: false
        };
      }
      // At this moment, the operators bindings are checked and the binding with the
      // given name is searched for. (instance). This is why the operator must be
      // constructed synchronously.
      paintInstanceEventHandler.setSelector('instance', scene.constructOperator(paintingOpDef));
      
      paintEventHandler.appendChildEventHandler(paintInstanceEventHandler);

      paintableNodes.push(instanceNode);
      paintableNodePaintHandlers.push(paintInstanceEventHandler);
    };

    paintManipulatorNode.pub.enable = function(){
      enabled = true;
      brushInstance.pub.setDrawToggle(true);
      scene.pub.redrawAllViewports();
    }
    paintManipulatorNode.pub.disable = function(){
      enabled = false;
      brushInstance.pub.setDrawToggle(false);
      scene.pub.redrawAllViewports();
    }

    return paintManipulatorNode;
  }});


FABRIC.SceneGraph.registerNodeType('Manipulator', {
  briefDesc: 'The Manipulator is a basic tool for controling 3D objects.',
  detailedDesc: 'The Manipulator is a basic tool for controling 3D objects. ' +
                'The manipulation is implmented per Manipulator type.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    targetNode: 'The node to be manipulated.',
    targetMember: 'The member on the targetNode to be manipulated',
    targetMemberIndex: 'The index of the member\'s element on the targetNode to be manipulated (undefined if not an array)',
    parentNode: 'The parent space for this Manipulator',
    parentMember: 'The member of the parent space of for this Manipulator (xfo member)',
    parentMemberIndex: 'The index of the member\'s element for the parent space of this Manipulator (undefinde if not an array)',
    color: 'The color to use for this Manipulator',
    highlightcolor: 'The highlight color to use for this manipulator.',
    localXfo: 'The local offset to use for this Manipulator.'
  },
  manipulating: false,
  factoryFn: function(options, scene) {
    
    scene.assignDefaults(options, {
        materialNode: undefined,
        color: FABRIC.RT.rgb(0, 0.8, 0, 1),
        highlightcolor: FABRIC.RT.rgb(0.8, 0.8, 0.8, 1)
    });
    
    options.enableRaycasting = true;
    var manipulatorNode = scene.constructNode('Instance', options),
      manipulating = false,
      material = manipulatorNode.pub.getMaterialNode(),
      highlightColor = options.highlightcolor;
      
      
    if(!material){
      material = scene.pub.constructNode('FlatMaterial', {
        color: options.color,
        drawOverlaid: options.drawOverlaid
      });
      manipulatorNode.pub.setMaterialNode(material);
    }
    var color = material.getColor();
      
    var manipulatorGlobals = this;
    var highlightNode = function(){
      material.setColor(highlightColor);
    }
    var unhighlightNode = function(){
      material.setColor(color);
    }
    manipulatorNode.pub.addEventListener('mouseover_geom', function(evt) {
        if(!manipulatorGlobals.manipulating){
          highlightNode();
          manipulatorNode.pub.fireEvent('highlight', evt);
          evt.viewportNode.redraw();
        }
      });
    manipulatorNode.pub.addEventListener('mouseout_geom', function(evt) {
        if(!manipulating){
          unhighlightNode();
          manipulatorNode.pub.fireEvent('unhighlight', evt);
          if(!evt.toNode){
            // if there is a 'toElement' then there will be a 
            // mouseover event. the redraw will occur then.
            evt.viewportNode.redraw();
          }
        }
      });

    var mouseDownScreenPos, mouseDrag, viewportNode;
    var dragFn = function(evt) {
      if(!evt.mouseScreenPos)
        return;
      evt.mouseDownScreenPos = mouseDownScreenPos;
      evt.mouseDragScreenPos = evt.mouseScreenPos;
      evt.mouseDragScreenDelta = evt.mouseDragScreenPos.subtract(mouseDownScreenPos);
      manipulatorNode.pub.fireEvent('drag', evt);
      evt.stopPropagation();
      viewportNode.redraw(true);
    }
    var releaseFn = function(evt) {
      manipulating = false;
      manipulatorGlobals.manipulating = false;
      unhighlightNode();
      manipulatorNode.pub.fireEvent('dragend', evt);
      document.removeEventListener('mousemove', dragFn, false);
      document.removeEventListener('mouseup', releaseFn, false);
      evt.stopPropagation();
      viewportNode.redraw();
    }
    manipulatorNode.pub.addEventListener('mousedown_geom', function(evt) {
        manipulating = true;
        manipulatorGlobals.manipulating = true;
        highlightNode();
        viewportNode = evt.viewportNode;
        evt.mouseDownScreenPos = mouseDownScreenPos = evt.mouseScreenPos;
        manipulatorNode.pub.fireEvent('dragstart', evt);
        document.addEventListener('mousemove', dragFn, false);
        document.addEventListener('mouseup', releaseFn, false);
        evt.stopPropagation();
        viewportNode.redraw();
      });
    
    return manipulatorNode;
  }});


FABRIC.SceneGraph.registerNodeType('XfoManipulator', {
  briefDesc: 'The TransformManipulator is a basic tool for controling 3D objects.',
  detailedDesc: 'The TransformManipulator is a basic tool for controling 3D objects. ' +
                'The manipulation is implmented per Manipulator type.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
  },
  manipulating: false,
  factoryFn: function(options, scene) {
    
    scene.assignDefaults(options, {
        targetNode: undefined,
        targetMember: 'globalXfo',
        localXfo: new FABRIC.RT.Xfo()
      });
    if(!options.targetNode)
      options.targetNode = options.parentNode;
    if(!options.targetNode){
      throw "Target Node not specified";
    }
    if(options.targetNode.isTypeOf("Instance")){
      options.targetNode = options.targetNode.getTransformNode();
    }
    options.raycastOverlaid = options.drawOverlaid;
    
    // get the target node (the one we are manipulating)
    var targetNode = options.targetNode;
    var targetMember = options.targetMember;
    var targetMemberGetter = "get"+targetMember.charAt(0).toUpperCase()+targetMember.slice(1);
    var targetMemberSetter = "set"+targetMember.charAt(0).toUpperCase()+targetMember.slice(1);

    var transformNode = scene.constructNode('Transform', {
      name: options.name + 'Transform',
      hierarchical: true,
      parentTransformNode: targetNode,
      localXfo: options.localXfo
    });
    
    options.transformNode = transformNode;
    var manipulatorNode = scene.constructNode('Manipulator', options );
    
    ///////////////////////////////////////////////////////////
    manipulatorNode.getTargetNode = function() {
      return targetNode;
    };
    manipulatorNode.getTargetMember = function() {
      return targetMember;
    };
    manipulatorNode.getTargetXfo = function() {
      return targetNode[targetMemberGetter]();
    }
    manipulatorNode.setTargetXfo = function(xfo) {
        targetNode[targetMemberSetter](xfo);
        manipulatorNode.pub.fireEvent('targetManipulated',{newTargetXfo: xfo});
    }
    manipulatorNode.setTargetOri = function(ori) {
      var xfo = this.getTargetXfo();
      xfo.ori = ori;
      this.setTargetXfo(xfo);
    }
    
    manipulatorNode.getManipulationSpaceXfo = function() {
      return transformNode.pub.getGlobalXfo();
    }
    
    
    // Thia function is used to find the closest local axis to
    // the given vec. The local axis can then be used in manipulation.
    // Manipulators can then determine the best local axis based on the
    // camera facing direction. 
    manipulatorNode.findClosestLocalAxis = function(vec) {
      var manipulationSpaceOri = this.getManipulationSpaceXfo().ori,
        localX = manipulationSpaceOri.rotateVector(new FABRIC.RT.Vec3(1, 0, 0)),
        localY = manipulationSpaceOri.rotateVector(new FABRIC.RT.Vec3(0, 1, 0)),
        localZ = manipulationSpaceOri.rotateVector(new FABRIC.RT.Vec3(0, 0, 1)),
        deltaX = vec.angleTo(localX),
        deltaY = vec.angleTo(localY),
        deltaZ = vec.angleTo(localZ);

      if (deltaX > Math.HALF_PI) {
        deltaX = Math.PI - deltaX;
        localX = localX.negate();
      }
      if (deltaY > Math.HALF_PI) {
        deltaY = Math.PI - deltaY;
        localY = localY.negate();
      }
      if (deltaZ > Math.HALF_PI) {
        deltaZ = Math.PI - deltaZ;
        localZ = localZ.negate();
      }
      if (deltaX < deltaY && deltaX < deltaZ) {
        return localX;
      }
      else if (deltaY < deltaX && deltaY < deltaZ) {
        return localY;
      }
      return localZ;
    }

    return manipulatorNode;
  }});

FABRIC.SceneGraph.registerNodeType('RotationManipulator', {
  briefDesc: 'The RotationManipulator is a manipulator for 3D rotation.',
  detailedDesc: 'The RotationManipulator is a manipulator for 3D rotation. ' +
                'It is visualized using a Circle primitive node.',
  parentNodeDesc: 'Manipulator',
  optionsDesc: {
    rotateRate: 'The amount of rotation per mouse move unit.',
    radius: 'The radius of the rotation Manipulator.',
    name: 'The name of the Manipulator node.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        rotateRate: 0.35,
        radius: 15,
        name: 'RotationManipulator'
      });
    if(!options.geometryNode){ 
      options.geometryNode = scene.pub.constructNode('Circle', { radius: options.radius });
    }
    var manipulatorNode = scene.constructNode('XfoManipulator', options);

    var viewportNode;
    var dragStartXFo, vec1, angle, ray1, ray2, planePoint, planeNormal, hitPoint1, hitPoint2;
    var dragStartFn = function(evt) {
      viewportNode = evt.viewportNode;
      dragStartXFo = manipulatorNode.getTargetXfo();
      ray1 = evt.rayData;

      planePoint = dragStartXFo.tr;
      planeNormal = manipulatorNode.getManipulationSpaceXfo().ori.getYaxis();
      hitPoint1 = ray1.intersectPlane(planePoint, planeNormal).point;
      angle = 0;
      vec1 = hitPoint1.subtract(planePoint).unit();
    }
    manipulatorNode.pub.addEventListener('dragstart', dragStartFn);

    var dragFn = function(evt) {
      ray2 = viewportNode.calcRayFromMouseEvent(evt);

      hitPoint2 = ray2.intersectPlane(planePoint, planeNormal).point;
      if (!hitPoint2)
        return;

      var vec2 = hitPoint2.subtract(planePoint).unit();
      var deltaAngle = vec1.angleTo(vec2);
      if (vec1.cross(vec2).dot(planeNormal) > 0) {
        angle += deltaAngle;
      }else {
        angle -= deltaAngle;
      }
      vec1 = vec2;
      var dragXfo = dragStartXFo.clone();
      dragXfo.ori = (new FABRIC.RT.Quat().setFromAxisAndAngle(planeNormal, angle)).multiply(dragXfo.ori);
      manipulatorNode.setTargetXfo(dragXfo);
    }
    manipulatorNode.pub.addEventListener('drag', dragFn);

    var dragendFn = function(evt) {
    }
    manipulatorNode.pub.addEventListener('dragend', dragendFn);

    return manipulatorNode;
  }});

FABRIC.SceneGraph.registerNodeType('3AxisRotationManipulator', {
  briefDesc: 'The 3AxisRotationManipulator is a manipulator for 3D rotation along 3 axes.',
  detailedDesc: 'The 3AxisRotationManipulator is a manipulator for 3D rotation along 3 axes.',
  parentNodeDesc: 'Manipulator',
  optionsDesc: {
    xaxis: 'If set to true, we will have a RotationManipulator along the X axis.',
    yaxis: 'If set to true, we will have a RotationManipulator along the Y axis.',
    zaxis: 'If set to true, we will have a RotationManipulator along the Z axis.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        rotateRate: 0.1,
        radius: 15,
        xaxis: true,
        yaxis: true,
        zaxis: true,
        name: 'ThreeAxisRotationManipulator'
      });
    
    var threeAxisRotationManipulator = scene.constructNode('SceneGraphNode', options);
    var name = threeAxisRotationManipulator.pub.getName();

    var circle = scene.pub.constructNode('Circle', { radius: options.radius });

    var xaxisGizmoNode = scene.pub.constructNode('RotationManipulator', scene.assignDefaults(options, {
        name: name + 'XAxis',
        color: FABRIC.RT.rgb(0.8, 0, 0, 1),
        localXfo: new FABRIC.RT.Xfo({ ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0, 0, 1), -Math.HALF_PI) }),
        geometryNode: circle
      }, true));
    var yaxisGizmoNode = scene.pub.constructNode('RotationManipulator', scene.assignDefaults(options, {
        name: name + 'YAxis',
        color: FABRIC.RT.rgb(0, 0.8, 0, 1),
        localXfo: new FABRIC.RT.Xfo(),
        geometryNode: circle
      }, true));
    var zaxisGizmoNode = scene.pub.constructNode('RotationManipulator', scene.assignDefaults(options, {
        name: name + 'ZAxis',
        color: FABRIC.RT.rgb(0, 0, 0.8, 1),
        localXfo: new FABRIC.RT.Xfo({ ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(1, 0, 0), Math.HALF_PI) }),
        geometryNode: circle
      }, true));

    return threeAxisRotationManipulator;
  }});

FABRIC.SceneGraph.registerNodeType('LinearTranslationManipulator', {
  briefDesc: 'The LinearTranslationManipulator is a manipulator for linear translation.',
  detailedDesc: 'The LinearTranslationManipulator is a manipulator for linear translation. It is drawn as an arrow in 3D.',
  parentNodeDesc: 'Manipulator',
  optionsDesc: {
    size: 'The size of the linear translation Manipulator.',
    name: 'The name of the linear translation Manipulator.',
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        size: 20,
        name: 'LinearTranslationManipulator'
      });
    if (!options.geometryNode) {
      options.geometryNode = scene.pub.constructNode('LineVector', { to: new FABRIC.RT.Vec3(0, options.size, 0) });
    }
    var manipulatorNode = scene.constructNode('XfoManipulator', options);

    var viewportNode, dragStartXFo, rotateAxis, ray1, ray2, dragStartPoint, translateAxis;
    var dragStartFn = function(evt) {
      viewportNode = evt.viewportNode;
      dragStartXFo = manipulatorNode.getTargetXfo();
      translateAxis = manipulatorNode.getManipulationSpaceXfo().ori.rotateVector(new FABRIC.RT.Vec3(0, 1, 0));
      ray1 = evt.rayData;
      var intersection = ray1.distanceToLine(dragStartXFo.tr, dragStartXFo.tr.add(translateAxis));
      dragStartPoint = dragStartXFo.tr.add(translateAxis.multiplyScalar(intersection.lineParam));
    }
    manipulatorNode.pub.addEventListener('dragstart', dragStartFn);

    var dragFn = function(evt) {
      ray2 = viewportNode.calcRayFromMouseEvent(evt);
      var intersection = ray2.distanceToLine(dragStartXFo.tr, dragStartXFo.tr.add(translateAxis));
      var dragPoint = dragStartXFo.tr.add(translateAxis.multiplyScalar(intersection.lineParam));
      var distance = dragPoint.subtract(dragStartPoint).dot(translateAxis);
      var dragXfo = dragStartXFo.clone();
      dragXfo.tr = dragXfo.tr.add(translateAxis.multiplyScalar(distance));
      manipulatorNode.setTargetXfo(dragXfo);
    }
    manipulatorNode.pub.addEventListener('drag', dragFn);

    return manipulatorNode;
  }});


FABRIC.SceneGraph.registerNodeType('PlanarTranslationManipulator', {
  briefDesc: 'The PlanarTranslationManipulator is a manipulator for planar translation.',
  detailedDesc: 'The PlanarTranslationManipulator is a manipulator for planar translation. It is drawn as a plane.',
  parentNodeDesc: 'XfoManipulator',
  optionsDesc: {
    size: 'The size of the planar translation Manipulator.',
    name: 'The name of the planar translation Manipulator.',
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        size: 15,
        name: 'PlanarTranslationManipulator'
      });
    if (!options.geometryNode) {
      // Draw a simple triangle on the XY plane
      options.geometryNode = scene.pub.constructNode('Triangles');
      options.geometryNode.loadGeometryData({
          positions: [new FABRIC.RT.Vec3(0, 0, 0), new FABRIC.RT.Vec3(0, 0, options.size),
                      new FABRIC.RT.Vec3(options.size, 0, 0)],
          indices: [0, 1, 2]
        });
    }
    var manipulatorNode = scene.constructNode('XfoManipulator', options);

    var viewportNode;
    var dragStartXFo, vec1, ray1, ray2, planePoint, planeNormal, hitPoint1, hitPoint2;
    var dragStartFn = function(evt) {
      viewportNode = evt.viewportNode;
      dragStartXFo = manipulatorNode.getTargetXfo();
      ray1 = evt.rayData;
      planePoint = dragStartXFo.tr;
      planeNormal = manipulatorNode.getManipulationSpaceXfo().ori.rotateVector(new FABRIC.RT.Vec3(0, 1, 0));
      hitPoint1 = ray1.intersectPlane(planePoint, planeNormal).point;
    }
    manipulatorNode.pub.addEventListener('dragstart', dragStartFn);

    var dragFn = function(evt) {
      ray2 = viewportNode.calcRayFromMouseEvent(evt);
      hitPoint2 = ray2.intersectPlane(planePoint, planeNormal).point;
      if (!hitPoint2)
        return;
      var delta = hitPoint2.subtract(hitPoint1);
      var dragXfo = dragStartXFo.clone();
      dragXfo.tr = dragXfo.tr.add(delta);
      manipulatorNode.setTargetXfo(dragXfo);
    }
    manipulatorNode.pub.addEventListener('drag', dragFn);

    return manipulatorNode;
  }});

FABRIC.SceneGraph.registerNodeType('ScreenTranslationManipulator', {
  briefDesc: 'The ScreenTranslationManipulator is a manipulator for translation in screen space.',
  detailedDesc: 'The ScreenTranslationManipulator is a manipulator for translation in screen space.',
  optionsDesc: {
    radius: 'The size of the screen space translation Manipulator.',
    name: 'The name of the screen space translation Manipulator.',
    drawOverlaid: 'If set to true the Manipulator will be drawn overlaid.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        radius: 0.5,
        name: 'ScreenTranslationManipulator',
        drawOverlaid: true
      });

    if (!options.geometryNode) {
      options.geometryNode = scene.pub.constructNode('Sphere', { radius: options.radius, detail: 8.0 });
    }
    var manipulatorNode = scene.constructNode('XfoManipulator', options);

    var viewportNode;
    var dragStartXFo, vec1, ray1, ray2, planePoint, planeNormal, hitPoint1, hitPoint2;
    var dragStartFn = function(evt) {
      viewportNode = evt.viewportNode;
      dragStartXFo = manipulatorNode.getTargetXfo();
      ray1 = evt.rayData;
      planePoint = dragStartXFo.tr;
      planeNormal = evt.cameraNode.getTransformNode().getGlobalXfo().ori.rotateVector(new FABRIC.RT.Vec3(0, 0, 1));
      hitPoint1 = ray1.intersectPlane(planePoint, planeNormal).point;
    }
    manipulatorNode.pub.addEventListener('dragstart', dragStartFn);

    var dragFn = function(evt) {
      ray2 = viewportNode.calcRayFromMouseEvent(evt);
      hitPoint2 = ray2.intersectPlane(planePoint, planeNormal).point;
      if (!hitPoint2)
        return;
      var delta = hitPoint2.subtract(hitPoint1);
      var dragXfo = dragStartXFo.clone();
      dragXfo.tr = dragXfo.tr.add(delta);
      manipulatorNode.setTargetXfo(dragXfo);
    }
    manipulatorNode.pub.addEventListener('drag', dragFn);

    return manipulatorNode;
  }});

FABRIC.SceneGraph.registerNodeType('3AxisTranslationManipulator', {
  briefDesc: 'The 3AxisTranslationManipulator is a manipulator for linear and planar translation along 3 axes.',
  detailedDesc: 'The 3AxisTranslationManipulator is a manipulator for linear and planar translation along 3 axes. It uses three linear as well as three planar translation Manipulators.',
  parentNodeDesc: 'XfoManipulator',
  optionsDesc: {
    trackRate: 'The rate of translation in relation to mouse move units.',
    size: 'The size of the linear and planar primitives of the Manipulator',
    name: 'The name of the 3 axis translation Manipulator.',
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        trackRate: 0.1,
        size: 25,
        name: 'threeAxisTrans'
      });
    
    var threeAxisTranslationManipulator = scene.constructNode('SceneGraphNode', options);
    var name = threeAxisTranslationManipulator.pub.getName();

    var lineVector = scene.pub.constructNode('LineVector', { to: new FABRIC.RT.Vec3(0, options.size, 0) });
    var arrowHead = scene.pub.constructNode('Cone', { radius: options.size * 0.04, height: options.size * 0.2 });

    scene.pub.constructNode('LinearTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_XAxis',
        color: FABRIC.RT.rgb(0.8, 0, 0, 1),
        localXfo: new FABRIC.RT.Xfo({
          ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0, 0, 1), -Math.HALF_PI)
        }),
        geometryNode: lineVector
      }, true));
    scene.pub.constructNode('LinearTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_XAxisArrowHead',
        color: FABRIC.RT.rgb(0.8, 0, 0, 1),
        localXfo: new FABRIC.RT.Xfo({
          ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0, 0, 1), -Math.HALF_PI),
          tr: new FABRIC.RT.Vec3(options.size, 0, 0)
        }),
        geometryNode: arrowHead
      }, true));

    scene.pub.constructNode('LinearTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_YAxis',
        color: FABRIC.RT.rgb(0, 0.8, 0),
        localXfo: new FABRIC.RT.Xfo(),
        geometryNode: lineVector
      }, true));
    scene.pub.constructNode('LinearTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_YAxisArrowHead',
        color: FABRIC.RT.rgb(0, 0.8, 0),
        localXfo: new FABRIC.RT.Xfo({
          tr: new FABRIC.RT.Vec3(0, options.size, 0)
        }),
        geometryNode: arrowHead
      }, true));

    scene.pub.constructNode('LinearTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_ZAxis',
        color: FABRIC.RT.rgb(0, 0, 0.8),
        localXfo: new FABRIC.RT.Xfo({
          ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(1, 0, 0), Math.HALF_PI)
        }),
        geometryNode: lineVector
      }, true));
    scene.pub.constructNode('LinearTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_ZAxisArrowHead',
        color: FABRIC.RT.rgb(0, 0, 0.8),
        localXfo: new FABRIC.RT.Xfo({
          ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(1, 0, 0), Math.HALF_PI),
          tr: new FABRIC.RT.Vec3(0, 0, options.size)
        }),
        geometryNode: arrowHead
      }, true));

    var drawTriangle = scene.pub.constructNode('Triangles');
    drawTriangle.loadGeometryData({
        positions: [new FABRIC.RT.Vec3(0, 0, 0), new FABRIC.RT.Vec3(0, 0, options.size * 0.6),
                    new FABRIC.RT.Vec3(options.size * 0.6, 0, 0), new FABRIC.RT.Vec3(0, 0, 0),
                    new FABRIC.RT.Vec3(0, 0, options.size * 0.6), new FABRIC.RT.Vec3(options.size * 0.6, 0, 0)],
        indices: [0, 1, 2, 3, 5, 4]
      });

    scene.pub.constructNode('PlanarTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_YZPlane',
        color: FABRIC.RT.rgb(0.8, 0, 0, 1),
        localXfo: new FABRIC.RT.Xfo({
          ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(0, 0, 1), Math.HALF_PI)
        }),
        geometryNode: drawTriangle
      }, true));
    scene.pub.constructNode('PlanarTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_XZPlane',
        color: FABRIC.RT.rgb(0, 0.8, 0, 1),
        localXfo: new FABRIC.RT.Xfo(),
        geometryNode: drawTriangle
      }, true));
    scene.pub.constructNode('PlanarTranslationManipulator', scene.assignDefaults(options, {
        name: name + '_XYPlane',
        color: FABRIC.RT.rgb(0, 0, 0.8, 1),
        localXfo: new FABRIC.RT.Xfo({
          ori: new FABRIC.RT.Quat().setFromAxisAndAngle(new FABRIC.RT.Vec3(1, 0, 0), -Math.HALF_PI)
        }),
        geometryNode: drawTriangle
      }, true));

    return threeAxisTranslationManipulator;
  }});


FABRIC.SceneGraph.registerNodeType('PivotRotationManipulator', {
  briefDesc: 'The PivotRotationManipulator is a manipulator for rotation based on a 3D pivot.',
  detailedDesc: 'The PivotRotationManipulator is a manipulator for rotation based on a 3D pivot. It is drawn as a circle.',
  parentNodeDesc: 'XfoManipulator',
  optionsDesc: {
    rotateRate: 'The rate of rotation in relation to mouse move units.',
    radius: 'The radius of the circle primitive of the Manipulator',
    name: 'The name of the pivot rotation Manipulator.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        rotateRate: 0.35,
        radius: 15,
        name: 'RotationManipulator'
      });

    if(!options.geometryNode){
      options.geometryNode = 
        scene.pub.constructNode('Circle', { radius: options.radius });
    }

    var manipulatorNode = scene.constructNode('XfoManipulator', options);

    var viewportNode;
    var dragStartXFo, vec1, ray1, ray2, planePoint, planeNormal, hitPoint1;
    var pivotPoint, hitPoint2, localAxis, globalAxis;
    var dragStartFn = function(evt) {
      viewportNode = evt.viewportNode;
      dragStartXFo = manipulatorNode.getTargetXfo();
      ray1 = evt.rayData;

      planePoint = dragStartXFo.tr;
      planeNormal = ray1.direction.negate();
      localAxis = manipulatorNode.findClosestLocalAxis(ray1.direction);
    }
    manipulatorNode.pub.addEventListener('dragstart', dragStartFn);

    var dragFn = function(evt) {
      var vec2,
        angle,
        movement,
        dragXfo,
        normal;

      ray2 = viewportNode.calcRayFromMouseEvent(evt);
      if (evt.ctrlKey) {
        normal = planeNormal;
        hitPoint1 = ray1.intersectPlane(planePoint, planeNormal).point;
        hitPoint2 = ray2.intersectPlane(planePoint, planeNormal).point;
        pivotPoint = planePoint.add(planePoint.subtract(hitPoint1));
        vec1 = hitPoint1.subtract(pivotPoint).unit();
      }
      else {
        normal = localAxis;
        hitPoint1 = ray1.intersectPlane(planePoint, localAxis).point;
        hitPoint2 = ray2.intersectPlane(planePoint, localAxis).point;
        pivotPoint = planePoint.add(planePoint.subtract(hitPoint1));
        vec1 = hitPoint1.subtract(pivotPoint).unit();
      }
      if (!hitPoint2)
        return;

      vec2 = hitPoint2.subtract(pivotPoint).unit();
      angle = vec1.angleTo(vec2);

      if (evt.shiftKey) {
        angle = Math.round(angle / Math.degToRad(5.0)) * Math.degToRad(5.0);
      }

      movement = vec2.subtract(vec1).unit().multiplyScalar(Math.sin(angle * 0.5) * options.radius * 2.0);
      if (vec1.cross(vec2).dot(normal) < 0) {
        angle = -angle;
        movement.negate();
      }

      dragXfo = dragStartXFo.clone();
      dragXfo.ori = new FABRIC.RT.Quat().setFromAxisAndAngle(normal, angle).multiply(dragXfo.ori);
      dragXfo.tr = dragXfo.tr.add(movement);
      manipulatorNode.setTargetXfo(dragXfo);
    }
    manipulatorNode.pub.addEventListener('drag', dragFn);

    var dragendFn = function(evt) {
    }
    manipulatorNode.pub.addEventListener('dragend', dragendFn);

    return manipulatorNode;
  }});

FABRIC.SceneGraph.registerNodeType('BoneManipulator', {
  briefDesc: 'The BoneManipulator is a manipulator for the rotation of Bone primitive.',
  detailedDesc: 'The BoneManipulator is a manipulator for the rotation of Bone primitive.',
  parentNodeDesc: 'XfoManipulator',
  optionsDesc: {
    name: 'The name of the BoneManipulator',
    parentManipulator: 'The parent bone Manipulator. None if undefined.',
    childManipulator: 'The child bone Manipulator. None if undefined.',
    length: 'The length of the bone Manipulator',
    boneVector: 'The directino of the bone\'s alignment. Typically along the X axis.'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        name: 'BoneManipulator',
        parentManipulator: undefined,
        childManipulator: undefined,
        length: 35,
        boneVector: new FABRIC.RT.Vec3(1, 0, 0)
      });
    
    options.geometryNode = scene.pub.constructNode('LineVector', { to: options.boneVector.multiplyScalar(options.length) });
    options.compensation = false;
    
    var manipulatorNode = scene.constructNode('XfoManipulator', options),
      parentManipulator = options.parentManipulator,
      childManipulator = options.childManipulator,
      viewportNode,
      dragStartXFo, vec1, ray1, ray2, planePoint, planeNormal, hitPoint1, hitPoint2, localAxis, globalAxis,
      currDragHandlerFn, dragXfo;

    var dragStartFn = function(evt) {
      viewportNode = evt.viewportNode;
      dragStartXFo = manipulatorNode.getTargetXfo();
      ray1 = evt.rayData;

      planePoint = dragStartXFo.tr;
      planeNormal = ray1.direction.negate();
      localAxis = manipulatorNode.findClosestLocalAxis(ray1.direction);

      vec1 = manipulatorNode.getManipulationSpaceXfo().ori.rotateVector(options.boneVector);
      currDragHandlerFn = dragHandlerFn;

      if (parentManipulator) {
        parentManipulator.beginManipulation(manipulatorNode);
      }
      if (childManipulator) {
        childManipulator.beginManipulation(manipulatorNode);
      }
    };

    var dragHandlerFn = function(evt, ray) {
      var dragVec,
        vec2,
        angle,
        movement,
        parentXfo,
        dragXfo,
        normal,
        dist,
        lengthRatio = 0,
        translationRatio = 0;
      ray2 = ray;
      if (evt.ctrlKey) {
        normal = localAxis;
      }
      else {
        normal = planeNormal;
      }
      hitPoint1 = ray1.intersectPlane(planePoint, normal).point;
      hitPoint2 = ray2.intersectPlane(planePoint, normal).point;
      
      dragVec = hitPoint2.subtract(hitPoint1);
      vec1 = normal.cross(vec1.cross(normal)).unit();
      vec2 = hitPoint2.subtract(planePoint);
      
      dragXfo = dragStartXFo.clone();
      
      if(parentManipulator && childManipulator){
        parentXfo = parentManipulator.counterRotateParent(dragVec);
        dragXfo.tr = parentXfo.tr;
      }else{
        parentXfo = manipulatorNode.getParentXfo();
        // The root of the chain can be translated.
      //  dragXfo.tr = dragXfo.tr.add(dragVec);
      }

      var angle1 = -(vec1.negate().angleTo(dragXfo.tr.subtract(planePoint.add(vec1.multiplyScalar(options.length)))));
      var angle2 = vec1.angleTo(vec2);
      
      if(childManipulator){
        angle = angle1;
      }else{
        angle = angle2;
      }
      if (evt.shiftKey) {
        angle = Math.round(angle / Math.degToRad(5.0)) * Math.degToRad(5.0);
      }
      if (vec1.cross(vec2).dot(normal) < 0) {
        angle = -angle;
      }
      
      dragXfo.ori = dragXfo.ori.multiply(new FABRIC.RT.Quat().setFromAxisAndAngle(normal, angle));
      
      if(parentManipulator){
        manipulatorNode.setTargetOri(dragXfo.ori.multiply(parentXfo.ori.inverse()));
      }else{
        manipulatorNode.setTargetXfo(dragXfo);
      }
      
      if (childManipulator) {
        movement = dragXfo.tr.add(dragXfo.ori.rotateVector(options.boneVector).multiplyScalar(options.length)).subtract(
              dragStartXFo.tr.add(dragStartXFo.ori.rotateVector(options.boneVector).multiplyScalar(options.length)));
        childManipulator.counterRotateChild(movement, dragXfo);
      }
    };
    
    var dragFn = function(evt) {
      currDragHandlerFn(evt, viewportNode.calcRayFromMouseEvent(evt));
    };

    var dragendFn = function(evt) {
    };

    // Private
    manipulatorNode.beginManipulation = function(chainedManipulator) {
      dragStartXFo = manipulatorNode.getTargetXfo();
      vec1 = this.getManipulationSpaceXfo().ori.rotateVector(options.boneVector);

      if (parentManipulator && parentManipulator !== chainedManipulator) {
        parentManipulator.beginManipulation(manipulatorNode);
      }
      if (childManipulator && childManipulator !== chainedManipulator) {
        childManipulator.beginManipulation(manipulatorNode);
      }
    };
    
    manipulatorNode.counterRotateParent = function(movement) {
      var dragXfo = dragStartXFo.clone(),
        newVec1 = movement.add(vec1.multiplyScalar(options.length)),
        newVec1Length = newVec1.length(),
        counterRotation = new FABRIC.RT.Quat().setFrom2Vectors(vec1, newVec1.multiplyScalar(1.0 / newVec1Length));

      dragXfo.ori = dragXfo.ori.multiply(counterRotation);
      this.setTargetOri(dragXfo.ori);
      dragXfo.tr = dragXfo.tr.add(dragXfo.ori.rotateVector(options.boneVector).multiplyScalar(options.length));
      return dragXfo;
    };
    manipulatorNode.counterRotateChild = function(movement, parentXfo) {
      var dragXfo = dragStartXFo.clone(),
        newVec1 = movement.add(vec1.multiplyScalar(-options.length)),
        newVec1Length = newVec1.length(),
        counterRotation = new FABRIC.RT.Quat().setFrom2Vectors(vec1.negate(), newVec1.multiplyScalar(1.0 / newVec1Length));

      dragXfo.tr = dragXfo.tr.add(movement);
      dragXfo.ori = dragXfo.ori.multiply(counterRotation);
      this.setTargetOri(dragXfo.ori.multiply(parentXfo.ori.inverse()));
     // this.setTargetOri(parentXfo.inverse().multiply(dragXfo).ori);

      if (childManipulator) {
        childManipulator.counterRotateChild(newVec1.multiplyScalar((newVec1Length - options.length) / newVec1Length), dragXfo);
      }
    };
    
    manipulatorNode.getDragHandlerFn = function() {
      return dragHandlerFn;
    };

    manipulatorNode.pub.addEventListener('dragstart', dragStartFn);
    manipulatorNode.pub.addEventListener('drag', dragFn);
    manipulatorNode.pub.addEventListener('dragend', dragendFn);
    
    manipulatorNode.setParentManipulatorNode = function(node) {
      if (!node.isTypeOf('BoneManipulator')) {
        throw ('Incorrect type assignment. Must assign a BoneManipulator');
      }
      parentManipulator = scene.getPrivateInterface(node);
    };
    manipulatorNode.setChildManipulatorNode = function(node) {
      if (!node.isTypeOf('BoneManipulator')) {
        throw ('Incorrect type assignment. Must assign a BoneManipulator');
      }
      childManipulator = scene.getPrivateInterface(node);
    };
    manipulatorNode.getBoneLength = function() {
      return options.length;
    }

    return manipulatorNode;
  }});



