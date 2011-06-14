
// Note: the ids defined in this table are used in the KL/GLSL code.
/**
 * The basic module for lights, defining the types for now.
 */
FABRIC.SceneGraph.Lights = {
  types: {
    'PointLight': 0,
    'DirectionalLight': 1,
    'SpotLight': 1
  }
};

FABRIC.SceneGraph.registerNodeType('Light',
  function(options, scene) {
    scene.assignDefaults(options, {
        color: FABRIC.Math.rgb(1.0, 1.0, 1.0)
      });

    if (options.lightType == undefined) {
      throw ': Lights must define a type';
    }

    options.dgnodenames.push('DGNode');
    var lightNode = scene.constructNode('SceneGraphNode', options);

    var redrawEventHandler;
    var dgnode = lightNode.getDGNode();
    dgnode.addMember('type', 'Integer', options.lightType);
    dgnode.addMember('color', 'Color', options.color);

    scene.addMemberInterface(lightNode, dgnode, 'color', true);

    lightNode.getRedrawEventHandler = function() {
      if (redrawEventHandler) {
        return redrawEventHandler;
      }
      redrawEventHandler = scene.constructEventHandlerNode(options.name + '_render');
      redrawEventHandler.addScope('light', dgnode);

      var operators = redrawEventHandler.preDescendBindings;

      operators.append(scene.constructOperator({
        operatorName: 'loadLight',
        srcFile: '../../../SceneGraph/Resources//KL/lights.kl',
        preProcessorDefinitions: {
          LIGHTTYPE_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightType'].id,
          LIGHTCOLOR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightColor'].id,
          LIGHTPOS_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightPosition'].id,
          LIGHTDIR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightDir'].id,
          LIGHTCOSCUTOFF_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightCosCutoff'].id,
          LIGHTVIEWMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightShadowMapMatrix'].id
        },
        entryFunctionName: 'loadLight',
        parameterBinding: [
          'shader.uniformValues',
          'light.type',
          'light.color'
        ]
      }));

      return redrawEventHandler;
    };

    // the operator stack functions enable the ligth properties to be animated.
    scene.addMemberAndOperatorStackFunctions(lightNode, dgnode);

    return lightNode;
  });


FABRIC.SceneGraph.registerNodeType('PointLight',
  function(options, scene) {
    scene.assignDefaults(options, {
        position: FABRIC.Math.vec3(420.0, 1000.0, 600.0)
      });
    options.lightType = FABRIC.SceneGraph.Lights.types.PointLight;
    var pointLightNode = scene.constructNode('Light', options);
    var dgnode = pointLightNode.getDGNode();

    dgnode.addMember('position', 'Vec3', options.position);
    scene.addMemberInterface(pointLightNode, dgnode, 'position', true);

    var redrawEventHandlerConfigured = false;
    var parentGetRedrawEventHandler = pointLightNode.getRedrawEventHandler;
    pointLightNode.getRedrawEventHandler = function() {
      var redrawEventHandler = parentGetRedrawEventHandler();
      if (redrawEventHandlerConfigured) {
        return redrawEventHandler;
      }

      redrawEventHandler.preDescendBindings.append(scene.constructOperator({
        operatorName: 'loadLight',
        srcFile: '../../../SceneGraph/Resources//KL/lights.kl',
        preProcessorDefinitions: {
          LIGHTTYPE_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightType'].id,
          LIGHTCOLOR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightColor'].id,
          LIGHTPOS_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightPosition'].id,
          LIGHTDIR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightDir'].id,
          LIGHTCOSCUTOFF_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightCosCutoff'].id,
          LIGHTVIEWMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightShadowMapMatrix'].id
        },
        entryFunctionName: 'loadLight',
        parameterBinding: [
          'shader.uniformValues',
          'light.type',
          'light.color'
        ]
      }));

      redrawEventHandlerConfigured = true;
      return redrawEventHandler;
    };


    if (options.display === true) {
      // TODO: lights should probably be bound to transforms,
      // rather than just having position values.
      scene.pub.constructNode('Instance', {
        transformNode: scene.pub.createNode('Transform', {
          hierarchical: false,
          globalXfo: FABRIC.Math.xfo({ tr: options.position })
        }),
        geometryNode: scene.pub.createNode('Circle', {
          radius: 7
        }),
        materialNode: scene.pub.createNode('FlatMaterial', {
          color: FABRIC.Math.rgb(0.2, 0.5, 0.8)
        })
      });

    }

    return pointLightNode;
  });

FABRIC.SceneGraph.registerNodeType('DirectionalLight',
  function(options, scene) {
    scene.assignDefaults(options, {
        position: FABRIC.Math.vec3(420.0, 1000.0, 600.0),
        direction: FABRIC.Math.vec3(0.0, 0.737, 0.737)
      });

    options.lightType = FABRIC.SceneGraph.Lights.types.DirectionalLight;
    var directionalLightNode = scene.constructNode('Light', options);
    var dgnode = directionalLightNode.getDGNode();

    dgnode.addMember('direction', 'Vec3', options.direction);
    scene.addMemberInterface(directionalLightNode, dgnode, 'direction', true);

    dgnode.addMember('position', 'Vec3', options.position);
    scene.addMemberInterface(directionalLightNode, dgnode, 'direction', true);

    var redrawEventHandlerConfigured = false;
    var parentGetRedrawEventHandler = directionalLightNode.getRedrawEventHandler;
    directionalLightNode.getRedrawEventHandler = function() {
      var redrawEventHandler = parentGetRedrawEventHandler();
      if (redrawEventHandlerConfigured) {
        return redrawEventHandler;
      }

      redrawEventHandler.preDescendBindings.append(scene.constructOperator({
        operatorName: 'loadDirectionalLight',
        srcFile: '../../../SceneGraph/Resources//KL/lights.kl',
        preProcessorDefinitions: {
          LIGHTTYPE_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightType'].id,
          LIGHTCOLOR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightColor'].id,
          LIGHTPOS_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightPosition'].id,
          LIGHTDIR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightDir'].id,
          LIGHTCOSCUTOFF_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightCosCutoff'].id,
          LIGHTVIEWMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightShadowMapMatrix'].id
        },
        entryFunctionName: 'loadDirectionalLight',
        parameterBinding: [
          'shader.uniformValues',
          'light.position',
          'light.dir',
          'camera.cameraMat44'
        ]
      }));

      redrawEventHandlerConfigured = true;
      return redrawEventHandler;
    };

    return directionalLightNode;
  });


FABRIC.SceneGraph.registerNodeType('SpotLight',
  function(options, scene) {
    scene.assignDefaults(options, {
        position: FABRIC.Math.vec3(100.0, 100.0, 100.0),
        target: FABRIC.Math.vec3(0.0, 0.0, 0.0),
        coneAngle: 60 * FABRIC.Math.degToRad,
        nearDistance: 1,
        farDistance: 1000,
        color: FABRIC.Math.rgb(1.0, 1.0, 1.0),
        castShadows: true,
        resolution: 1024
      });

    options.lightType = FABRIC.SceneGraph.Lights.types.SpotLight;
    var spotLightNode = scene.constructNode('Light', options);
    var dgnode = spotLightNode.getDGNode();

    dgnode.addMember('position', 'Vec3', options.position);
    scene.addMemberInterface(spotLightNode, dgnode, 'position', true);

    dgnode.addMember('target', 'Vec3', options.target);
    scene.addMemberInterface(spotLightNode, dgnode, 'target', true);

    dgnode.addMember('coneAngle', 'Scalar', options.coneAngle);
    scene.addMemberInterface(spotLightNode, dgnode, 'coneAngle', true);

    var redrawEventHandlerConfigured = false;
    var parentGetRedrawEventHandler = spotLightNode.getRedrawEventHandler;
    spotLightNode.getRedrawEventHandler = function() {
      var redrawEventHandler = parentGetRedrawEventHandler();
      if (redrawEventHandlerConfigured) {
        return redrawEventHandler;
      }

      redrawEventHandler.preDescendBindings.append(
        scene.constructOperator({
          operatorName: 'loadSpotLight',
          srcFile: '../../../SceneGraph/Resources//KL/lights.kl',
          preProcessorDefinitions: {
          LIGHTTYPE_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightType'].id,
          LIGHTCOLOR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightColor'].id,
          LIGHTPOS_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightPosition'].id,
          LIGHTDIR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightDir'].id,
          LIGHTCOSCUTOFF_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightCosCutoff'].id,
          LIGHTVIEWMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightShadowMapMatrix'].id
          },
          entryFunctionName: 'loadSpotLight',
          parameterBinding: [
            'shader.uniformValues',
            'light.position',
            'light.target',
            'light.coneAngle',
            'camera.cameraMat44'
          ]
        }));

      if (options.castShadows) {
        redrawEventHandler.addMember('shadowMap', 'Integer', 0);

        redrawEventHandler.preDescendBindings.append(
          scene.constructOperator({
              operatorName: 'loadLightMatrixUniform',
              srcFile: '../../../SceneGraph/Resources//KL/lights.kl',
              preProcessorDefinitions: {
                LIGHTTYPE_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightType'].id,
                LIGHTCOLOR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightColor'].id,
                LIGHTPOS_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightPosition'].id,
                LIGHTDIR_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightDir'].id,
                LIGHTCOSCUTOFF_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightCosCutoff'].id,
                LIGHTVIEWMATRIX_ATTRIBUTE_ID: FABRIC.shaderAttributeTable['lightShadowMapMatrix'].id
              },
              entryFunctionName: 'loadLightMatrixUniform',
              parameterBinding: [
                'shader.uniformValues',
                'light.shadowMat44',
                'camera.cameraMat44'
              ]
            }));

        redrawEventHandler.preDescendBindings.append(
          scene.constructOperator({
              operatorName: 'bindShadowMapBufferOp',
              srcFile: '../../../SceneGraph/Resources//KL/shadowMaps.kl',
              entryFunctionName: 'bindShadowMapBuffer',
              parameterBinding: [
                'self.shadowMap',
                'light.depthTextureID'
              ]
            }));

        // Debug shadowmap?
    /*    if(true)
        {
          redrawEventHandler.preDescendBindings.append(
            scene.constructOperator({
                operatorName:"debugShadowMapBuffer",
                srcFile:"../../../SceneGraph/Resources/KL/shadowMaps.kl",
                entryFunctionName:"debugShadowMapBuffer",
                parameterBinding:[
                  "light.colorTextureID"
                ]
              }));
        }
    */ }

      redrawEventHandlerConfigured = true;
      return redrawEventHandler;
    };

    if (options.castShadows) {

      var shadowRenderEventHandler = scene.constructEventHandlerNode(options.name + '_renderDepthBuffer');

      dgnode.addMember('cameraMat44', 'Mat44');
      // Projection Values
      dgnode.addMember('nearDistance', 'Scalar', options.nearDistance);
      dgnode.addMember('farDistance', 'Scalar', options.farDistance);
      dgnode.addMember('projectionMat44', 'Mat44');
      dgnode.addMember('shadowMat44', 'Mat44');

      dgnode.addMember('shadowmapsize', 'Integer', options.resolution);
      dgnode.addMember('shadowFBO', 'Integer', 0);
      dgnode.addMember('prevFBO', 'Integer', 0);
      dgnode.addMember('depthTextureID', 'Integer', 0);
      dgnode.addMember('colorTextureID', 'Integer', 0);

      // TODO: I think that the light projections matrices aren't correct.
      // the shadow map is clipped by the cone angle. The projection matrix,
      // should encapsulate the cone. This error can be seens clearly in the
      // Shadow Casting lights demo.
      dgnode.bindings.append(scene.constructOperator({
          operatorName: 'calcLightProjectionMatricies',
          srcFile: '../../../SceneGraph/Resources//KL/shadowMaps.kl',
          entryFunctionName: 'calcLightProjectionMatricies',
          parameterBinding: [
            'self.position',
            'self.target',
            'self.nearDistance',
            'self.farDistance',
            'self.coneAngle',
            'self.cameraMat44',
            'self.projectionMat44',
            'self.shadowMat44'
          ]
        }));

      shadowRenderEventHandler.addScope('light', dgnode);
      shadowRenderEventHandler.addScope('camera', dgnode);

      shadowRenderEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'genAndBindShadowMapFBO',
          srcFile: '../../../SceneGraph/Resources//KL/shadowMaps.kl',
          entryFunctionName: 'genAndBindShadowMapFBO',
          parameterBinding: [
            'light.shadowFBO',
            'light.prevFBO',
            'light.depthTextureID',
            'light.colorTextureID',
            'light.shadowmapsize'
          ]
        }));

      shadowRenderEventHandler.postDescendBindings.append(scene.constructOperator({
          operatorName: 'unbindFBO',
          srcFile: '../../../SceneGraph/Resources//KL/shadowMaps.kl',
          entryFunctionName: 'unbindFBO',
          parameterBinding: [
            'light.prevFBO'
          ]
        }));

      scene.registerShadowCastingLightSourceHandler(shadowRenderEventHandler);

    }

    // the operator stack functions enable the ligth properties to be animated.
    scene.addMemberAndOperatorStackFunctions(spotLightNode, dgnode);


    if (options.display === true) {
      // TODO: lights should probably be bound to transforms,
      // rather than just having position values.
      // tan(theta) = o/a
      // tan(theta)/a = o
      var coneDist = options.position.dist(options.target);
      var coneRadius = Math.tan(options.coneAngle * FABRIC.Math.degToRad * 0.5) * coneDist;
      var lightMaterial = scene.pub.constructNode('FlatMaterial', { color: FABRIC.Math.rgb(1.0, 0.7, 0.4) });
      var crossGeometry = scene.pub.constructNode('Cross', { size: coneDist * 0.05 });

      scene.pub.constructNode('Instance', {
        transformNode: scene.pub.constructNode('Transform', {
          hierarchical: false,
          globalXfo: FABRIC.Math.xfo({ tr: options.position })
        }),
        geometryNode: crossGeometry,
        materialNode: lightMaterial
      });
      scene.pub.constructNode('Instance', {
        transformNode: scene.pub.constructNode('Transform', {
          hierarchical: false,
          globalXfo: FABRIC.Math.xfo({ tr: options.target })
        }),
        geometryNode: crossGeometry,
        materialNode: lightMaterial
      });
      scene.pub.constructNode('Instance', {
        transformNode: scene.pub.constructNode('Transform', {
          hierarchical: false,
          globalXfo: FABRIC.Math.xfo({ tr: options.target })
        }),
        geometryNode: scene.pub.constructNode('Circle', {
          radius: coneRadius
        }),
        materialNode: lightMaterial
      });
    }

    return spotLightNode;
  });


