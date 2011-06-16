
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

// NOTE: The following primitives are examples of procedural geometry generation.
// Geometry data is stored in 2 separate nodes that contain data of different
// dimensions. uniforms and attributes. Currently, we can only (legally) write to a
// single nodes data (the self node), and we cannot resize a node during the execution
// of an operator.
// We handle the resizing issue here by spliting the generation operators into 2 ops
// the first of which computes the new size and resizes the attributes nodes. The 2nd
// op then generates the data. Ideally, we will be able to resize the attributes
// node from within an operator writing to its data. This will be achievable once
// we have implimented the concept of sub-graphs.
// For all triangle primitives, we are also calculating an index array, and this
// member is stored on the unfiorms node. We currently do this by writing to 2 nodes
// from the same operator (uniforms and attributes). This solution is termporary,
// and will be resolved elegantly with the graph nesting solution mentioned above.

FABRIC.SceneGraph.registerNodeType('LineVector',
  function(options, scene) {
    scene.assignDefaults(options, {
        from: FABRIC.RT.vec3(0, 0, 0),
        to: FABRIC.RT.vec3(1, 0, 0),
        width: 1,
        arrowhead: false
      });

    var lineSegmentNode = scene.constructNode('Lines', options);
    lineSegmentNode.pub.loadGeometryData({
        positions: [options.from, options.to],
        indices: [0, 1]
      });
    return lineSegmentNode;
  });


FABRIC.SceneGraph.registerNodeType('Cross',
  function(options, scene) {
    scene.assignDefaults(options, {
      size: 1.0
    });

    var crossNode = scene.constructNode('Lines', options);
    crossNode.pub.addUniformValue('crosssize', 'Scalar', options.size);
    crossNode.getAttributesDGNode().setCount(6);
    crossNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'generateCross',
        srcFile: '../../../SceneGraph/Resources//KL/generateCross.kl',
        entryFunctionName: 'generateCross',
        parameterBinding: [
          'self.positions[]',
          'uniforms.indices',
          'uniforms.crosssize'
        ]
      })
    ]);
    return crossNode;
  });


FABRIC.SceneGraph.registerNodeType('Rectangle',
  function(options, scene) {
    scene.assignDefaults(options, {
      length: 4.0,
      width: 2.5
    });

    var rectangleNode = scene.constructNode('Lines', options);
    rectangleNode.pub.addUniformValue('length', 'Scalar', options.length);
    rectangleNode.pub.addUniformValue('width', 'Scalar', options.width);
    rectangleNode.getAttributesDGNode().setCount(8);
    rectangleNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'generateRectangle',
        srcFile: '../../../SceneGraph/Resources//KL/generateRectangle.kl',
        entryFunctionName: 'generateRectangle',
        parameterBinding: [
          'self.positions[]',
          'uniforms.indices',
          'uniforms.length',
          'uniforms.width'
        ]
      })
    ]);

    return rectangleNode;
  });


FABRIC.SceneGraph.registerNodeType('BoundingBox',
  function(options, scene) {
    scene.assignDefaults(options, {
      bboxmin: FABRIC.RT.vec3(-2, -3, -4),
      bboxmax: FABRIC.RT.vec3(4, 3, 2)
    });

    var boundingBoxNode = scene.constructNode('Lines', options);
    boundingBoxNode.pub.addUniformValue('bboxmin', 'Vec3', options.bboxmin);
    boundingBoxNode.pub.addUniformValue('bboxmax', 'Vec3', options.bboxmax);
    boundingBoxNode.getAttributesDGNode().setCount(8);
    boundingBoxNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'generateBoundingBox',
        srcFile: '../../../SceneGraph/Resources//KL/generateBoundingBox.kl',
        entryFunctionName: 'generateBoundingBox',
        parameterBinding: [
          'self.positions[]',
          'uniforms.indices',
          'uniforms.bboxmin',
          'uniforms.bboxmax'
        ]
      })
    ]);

    return boundingBoxNode;
  });


FABRIC.SceneGraph.registerNodeType('Grid',
  function(options, scene) {
    scene.assignDefaults(options, {
      size_x: 10.0,
      size_y: 10.0,
      size_z: 10.0,
      sections_x: 10,
      sections_y: 1,
      sections_z: 10
    });

    var gridNode = scene.constructNode('Lines', options);
    gridNode.pub.addUniformValue('size_x', 'Scalar', options.size_x);
    gridNode.pub.addUniformValue('size_y', 'Scalar', options.size_y);
    gridNode.pub.addUniformValue('size_z', 'Scalar', options.size_z);
    gridNode.pub.addUniformValue('sections_x', 'Integer', options.sections_x);
    gridNode.pub.addUniformValue('sections_y', 'Integer', options.sections_y);
    gridNode.pub.addUniformValue('sections_z', 'Integer', options.sections_z);
    gridNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'setGridVertexCount',
        srcFile: '../../../SceneGraph/Resources//KL/generateGrid.kl',
        entryFunctionName: 'setGridVertexCount',
        parameterBinding: [
          'uniforms.sections_x',
          'uniforms.sections_y',
          'uniforms.sections_z',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generateGrid',
        srcFile: '../../../SceneGraph/Resources//KL/generateGrid.kl',
        entryFunctionName: 'generateGrid',
        parameterBinding: [
          'self.positions[]',
          'uniforms.indices',
          'uniforms.size_x',
          'uniforms.size_y',
          'uniforms.size_z',
          'uniforms.sections_x',
          'uniforms.sections_y',
          'uniforms.sections_z'
        ]
      })
    ]);

    return gridNode;
  });


FABRIC.SceneGraph.registerNodeType('Axes',
  function(options, scene) {
    scene.assignDefaults(options, {
        size: 1.0
      });

    var axesNode = scene.constructNode('Lines', options);
    axesNode.pub.addUniformValue('size', 'Scalar', options.size);
    axesNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'setAxesVertexCount',
        srcFile: '../../../SceneGraph/Resources//KL/generateAxes.kl',
        entryFunctionName: 'setAxesVertexCount',
        parameterBinding: [
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generateAxes',
        srcFile: '../../../SceneGraph/Resources//KL/generateAxes.kl',
        entryFunctionName: 'generateAxes',
        parameterBinding: [
          'self.positions[]',
          'uniforms.indices',
          'uniforms.size'
        ]
      })
    ]);
    return axesNode;
  });


FABRIC.SceneGraph.registerNodeType('Circle',
  function(options, scene) {
    scene.assignDefaults(options, {
        radius: 10,
        arcAngle: 6.28318,
        numSegments: 24
      });

    var circleNode = scene.constructNode('Lines', options);
    circleNode.pub.addUniformValue('radius', 'Scalar', options.radius);
    circleNode.pub.addUniformValue('arcAngle', 'Scalar', options.arcAngle);
    circleNode.pub.addUniformValue('numSegments', 'Integer', options.numSegments);

    // getters and setters
    var uniforms = circleNode.getUniformsDGNode();
    scene.addMemberInterface(circleNode, uniforms, 'radius', true);
    scene.addMemberInterface(circleNode, uniforms, 'arcAngle', true);
    scene.addMemberInterface(circleNode, uniforms, 'numSegments', true);

    circleNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'setCircleVertexCount',
        srcFile: '../../../SceneGraph/Resources//KL/generateCircle.kl',
        entryFunctionName: 'setCircleVertexCount',
        parameterBinding: [
          'uniforms.numSegments',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generateCircle',
        srcFile: '../../../SceneGraph/Resources//KL/generateCircle.kl',
        entryFunctionName: 'generateCircle',
        parameterBinding: [
          'self.positions[]',
          'uniforms.indices',
          'uniforms.radius',
          'uniforms.arcAngle',
          'uniforms.numSegments'
        ]
      })
    ]);
    return circleNode;
  });


FABRIC.SceneGraph.registerNodeType('Plane',
  function(options, scene) {
    scene.assignDefaults(options, {
        length: 10,
        width: 10,
        lengthSections: 2,
        widthSections: 2
      });

    options.uvSets = 1;

    var planeNode = scene.constructNode('Triangles', options);
    planeNode.pub.addUniformValue('length', 'Scalar', options.length);
    planeNode.pub.addUniformValue('width', 'Scalar', options.width);
    planeNode.pub.addUniformValue('lengthSections', 'Integer', options.lengthSections);
    planeNode.pub.addUniformValue('widthSections', 'Integer', options.widthSections);

    // getters and setters
    var uniforms = planeNode.getUniformsDGNode();
    scene.addMemberInterface(planeNode, uniforms, 'length', true);
    scene.addMemberInterface(planeNode, uniforms, 'width', true);
    scene.addMemberInterface(planeNode, uniforms, 'lengthSections', true);
    scene.addMemberInterface(planeNode, uniforms, 'widthSections', true);

    planeNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'setPlaneVertexCount',
        srcFile: '../../../SceneGraph/Resources//KL/generatePlane.kl',
        entryFunctionName: 'setPlaneVertexCount',
        parameterBinding: [
          'uniforms.lengthSections',
          'uniforms.widthSections',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generatePlane',
        srcFile: '../../../SceneGraph/Resources//KL/generatePlane.kl',
        entryFunctionName: 'generatePlane',
        parameterBinding: [
          'uniforms.indices',
          'uniforms.length',
          'uniforms.width',
          'uniforms.lengthSections',
          'uniforms.widthSections',

          'self.positions[]',
          'self.normals[]',
          'self.uvs0[]'
        ]
      })
    ]);
    return planeNode;
  });


FABRIC.SceneGraph.registerNodeType('Cuboid',
  function(options, scene) {
    scene.assignDefaults(options, {
        length: 10,
        width: 10,
        height: 10,
        size: undefined
      });

    if (typeof options.size === 'number') {
      options.length = options.height = options.width = options.size;
    }

    var cubeNode = scene.constructNode('Triangles', options);
    cubeNode.pub.addUniformValue('length', 'Scalar', options.length);
    cubeNode.pub.addUniformValue('width', 'Scalar', options.width);
    cubeNode.pub.addUniformValue('height', 'Scalar', options.height);
    cubeNode.getAttributesDGNode().setCount(24);

    // getters and setters
    var uniforms = cubeNode.getUniformsDGNode();
    scene.addMemberInterface(cubeNode, uniforms, 'length', true);
    scene.addMemberInterface(cubeNode, uniforms, 'width', true);
    scene.addMemberInterface(cubeNode, uniforms, 'height', true);

    cubeNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'generateCuboid',
        srcFile: '../../../SceneGraph/Resources//KL/generateCuboid.kl',
        entryFunctionName: 'generateCuboid',
        parameterBinding: [
          'self.positions[]',
          'self.normals[]',
          'uniforms.indices',
          'uniforms.length',
          'uniforms.height',
          'uniforms.width'
        ]
      })
    ]);
    return cubeNode;
  });


FABRIC.SceneGraph.registerNodeType('Sphere',
  function(options, scene) {
    scene.assignDefaults(options, {
        radius: 5.0,
        detail: 5
      });

    options.uvSets = 1;

    var sphereNode = scene.constructNode('Triangles', options);
    sphereNode.pub.addUniformValue('radius', 'Scalar', options.radius);
    sphereNode.pub.addUniformValue('detail', 'Integer', options.detail);

    // getters and setters
    var uniforms = sphereNode.getUniformsDGNode();
    scene.addMemberInterface(sphereNode, uniforms, 'radius', true);
    scene.addMemberInterface(sphereNode, uniforms, 'detail', true);

    sphereNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'setSphereVertexCount',
        srcFile: '../../../SceneGraph/Resources//KL/generateSphere.kl',
        entryFunctionName: 'setSphereVertexCount',
        parameterBinding: [
          'uniforms.detail',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generateSphere',
        srcFile: '../../../SceneGraph/Resources//KL/generateSphere.kl',
        entryFunctionName: 'generateSphere',
        parameterBinding: [
          'uniforms.radius',
          'uniforms.detail',

          'uniforms.indices',
          'self.positions[]',
          'self.normals[]',
          'self.uvs0[]'
        ]
      })
    ]);
    return sphereNode;
  });


FABRIC.SceneGraph.registerNodeType('Torus',
  function(options, scene) {
    scene.assignDefaults(options, {
        inner_radius: 2.0,
        outer_radius: 5.0,
        detail: 5
      });

    var torusNode = scene.constructNode('Triangles', options);
    torusNode.pub.addUniformValue('outer_radius', 'Scalar', options.inner_radius);
    torusNode.pub.addUniformValue('inner_radius', 'Scalar', options.outer_radius);
    torusNode.pub.addUniformValue('detail', 'Integer', options.detail);

    // getters and setters
    var uniforms = torusNode.getUniformsDGNode();
    scene.addMemberInterface(torusNode, uniforms, 'outer_radius', true);
    scene.addMemberInterface(torusNode, uniforms, 'inner_radius', true);
    scene.addMemberInterface(torusNode, uniforms, 'detail', true);

    torusNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'setTorusVertexCount',
        srcFile: '../../../SceneGraph/Resources//KL/generateTorus.kl',
        entryFunctionName: 'setTorusVertexCount',
        parameterBinding: [
          'uniforms.inner_radius',
          'uniforms.outer_radius',
          'uniforms.detail',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generateTorus',
        srcFile: '../../../SceneGraph/Resources//KL/generateTorus.kl',
        entryFunctionName: 'generateTorus',
        parameterBinding: [
          'uniforms.inner_radius',
          'uniforms.outer_radius',
          'uniforms.detail',

          'uniforms.indices',
          'self.positions[]',
          'self.normals[]'
        ]
      })
    ]);
    return torusNode;
  });


FABRIC.SceneGraph.registerNodeType('Cone',
  function(options, scene) {
    scene.assignDefaults(options, {
        radius: 2.0,
        height: 5.0,
        cap: true,
        detail: 5
      });

    var coneNode = scene.constructNode('Triangles', options);
    coneNode.pub.addUniformValue('radius', 'Scalar', options.radius);
    coneNode.pub.addUniformValue('height', 'Scalar', options.height);
    coneNode.pub.addUniformValue('cap', 'Boolean', options.cap);
    coneNode.pub.addUniformValue('detail', 'Integer', options.detail);

    // getters and setters
    var uniforms = coneNode.getUniformsDGNode();
    scene.addMemberInterface(coneNode, uniforms, 'radius', true);
    scene.addMemberInterface(coneNode, uniforms, 'height', true);
    scene.addMemberInterface(coneNode, uniforms, 'cap', true);
    scene.addMemberInterface(coneNode, uniforms, 'detail', true);

    coneNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'setConeVertexCount',
        srcFile: '../../../SceneGraph/Resources//KL/generateCone.kl',
        entryFunctionName: 'setConeVertexCount',
        parameterBinding: [
          'uniforms.cap',
          'uniforms.detail',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generateCone',
        srcFile: '../../../SceneGraph/Resources//KL/generateCone.kl',
        entryFunctionName: 'generateCone',
        parameterBinding: [
          'uniforms.radius',
          'uniforms.height',
          'uniforms.cap',
          'uniforms.detail',

          'uniforms.indices',
          'self.positions[]',
          'self.normals[]'
        ]
      })
    ]);
    return coneNode;
  });


FABRIC.SceneGraph.registerNodeType('Cylinder',
  function(options, scene) {
    scene.assignDefaults(options, {
        radius: 2.0,
        height: 5.0,
        caps: true,
        detail: 5
      });

    var cylinderNode = scene.constructNode('Triangles', options);
    cylinderNode.pub.addUniformValue('radius', 'Scalar', options.radius);
    cylinderNode.pub.addUniformValue('height', 'Scalar', options.height);
    cylinderNode.pub.addUniformValue('caps', 'Boolean', options.caps);
    cylinderNode.pub.addUniformValue('detail', 'Integer', options.detail);

    // getters and setters
    var uniforms = cylinderNode.getUniformsDGNode();
    scene.addMemberInterface(cylinderNode, uniforms, 'radius', true);
    scene.addMemberInterface(cylinderNode, uniforms, 'height', true);
    scene.addMemberInterface(cylinderNode, uniforms, 'cap', true);
    scene.addMemberInterface(cylinderNode, uniforms, 'detail', true);

    cylinderNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'setCylinderVertexCount',
        srcFile: '../../../SceneGraph/Resources//KL/generateCylinder.kl',
        entryFunctionName: 'setCylinderVertexCount',
        parameterBinding: [
          'uniforms.caps',
          'uniforms.detail',
          'self.newCount'
        ]
      }),
      scene.constructOperator({
        operatorName: 'generateCylinder',
        srcFile: '../../../SceneGraph/Resources//KL/generateCylinder.kl',
        entryFunctionName: 'generateCylinder',
        parameterBinding: [
          'uniforms.radius',
          'uniforms.height',
          'uniforms.caps',
          'uniforms.detail',

          'uniforms.indices',
          'self.positions[]',
          'self.normals[]'
        ]
      })
    ]);

    return cylinderNode;
  });


FABRIC.SceneGraph.registerNodeType('Teapot',
  function(options, scene) {
    scene.assignDefaults(options, {
        size: 5.0,
        detail: 5
      });

    var teapotNode = scene.constructNode('Triangles', options);
    teapotNode.pub.addUniformValue('size', 'Scalar', options.size);
    teapotNode.pub.addUniformValue('detail', 'Integer', options.detail);

    // getters and setters
    var uniforms = teapotNode.getUniformsDGNode();
    scene.addMemberInterface(teapotNode, uniforms, 'size', true);
    scene.addMemberInterface(teapotNode, uniforms, 'detail', true);

    teapotNode.setGeneratorOps([
      scene.constructOperator({
        operatorName: 'generateTeapot',
        srcFile: '../../../SceneGraph/Resources//KL/generateTeapot.kl',
        entryFunctionName: 'generateTeapot',
        parameterBinding: [
          'uniforms.size',
          'uniforms.detail',

          'uniforms.indices',
          'self.positions[]',
          'self.normals[]'
        ]
      })
    ]);
    return teapotNode;
  });




