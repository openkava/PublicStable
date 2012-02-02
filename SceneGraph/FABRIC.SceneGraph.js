
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * The scene graph is the core implementation of Fabric's
 * node graph in javascript.
 */
FABRIC.SceneGraph = {
  managerDescriptions: {},
  nodeDescriptions: {},
  assetLoaders: {},
  registerManagerType: function(type, description) {
    if (this.managerDescriptions[type]) {
      throw ('Manager Constructor already Registered:' + type);
    }else {
        // Commented out till we can finish the documentation.
  //    if (!nodeDescription.briefDesc || !nodeDescription.detailedDesc)
  //      console.log('WARNING: Node Constructor "'+type+'" does not provide a proper description.');
      if(!description.briefDesc) description.briefDesc = '';
      if(!description.detailedDesc) description.detailedDesc = '';
      if(!description.optionsDesc) description.optionsDesc = {};
      if(!description.parentNodeDesc) description.parentNodeDesc = '';
      this.managerDescriptions[type] = description;
    }
  },
  registerNodeType: function(type, description) {
    if (this.nodeDescriptions[type]) {
      throw ('Node Constructor already Registered:' + type);
    }else {
      if (!description.factoryFn)
        throw ('Node Constructor "'+type+'" does not implement the factoryFn');
        // Commented out till we can finish the documentation.
  //    if (!nodeDescription.briefDesc || !nodeDescription.detailedDesc)
  //      console.log('WARNING: Node Constructor "'+type+'" does not provide a proper description.');
      if(!description.briefDesc) description.briefDesc = '';
      if(!description.detailedDesc) description.detailedDesc = '';
      if(!description.optionsDesc) description.optionsDesc = {};
      if(!description.parentNodeDesc) description.parentNodeDesc = '';
      this.nodeDescriptions[type] = description;
    }
  },
  help: function(type) {
    var result = {};
    if (!type) {
      for (type in this.nodeDescriptions) {
        result[type] = {};
        result[type].brief = this.nodeDescriptions[type].briefDesc;
        result[type].detailed = this.nodeDescriptions[type].detailedDesc;
        result[type].parentDesc = this.nodeDescriptions[type].parentNodeDesc;
        result[type].optionsDesc = this.nodeDescriptions[type].optionsDesc;
      }
    } else {
      if (!this.nodeDescriptions[type])
        throw ('Node Constructor "'+type+'" is not registered!');
      result.brief = this.nodeDescriptions[type].briefDesc;
      result.detailed = this.nodeDescriptions[type].detailedDesc;
      if(this.nodeDescriptions[this.nodeDescriptions[type].parentNodeDesc]){
        result.parentDesc = this.help(this.nodeDescriptions[type].parentNodeDesc);
      }
      result.optionsDesc = this.nodeDescriptions[type].optionsDesc;
    }
    return result;
  },
  registerParser: function(ext, parserFn) {
    ext = ext.toLocaleLowerCase();
    if (!this.assetLoaders[ext]) {
      this.assetLoaders[ext] = function(scene, assetFile, options, callback) {
        if (!options) options = {};
        var pathArray = assetFile.split('/');
        var fileName = pathArray.pop();
        options.baseName = fileName.split('.')[0];
        options.basePath = pathArray.join('/');
        return parserFn(scene, assetFile, options, callback);
      }
    }
  },
  createScene: function(sceneOptions) {
    
    if(!sceneOptions)sceneOptions = {};
    
    var context = FABRIC.createContext({'contextID':sceneOptions.contextID});
    
    var assignDefaults = function(options, defaults, force) {
      if (!options) options = {};
      for (var i in defaults) {
        if (options[i] === undefined || force) options[i] = defaults[i];
      }
      return options;
    };
    
    sceneOptions = assignDefaults(sceneOptions, {
        constructGlobalsNode: true,
        preDraw: true,
        postDraw: true,
        constructAnimationInterface: true,
        timeStep: 1/50 /* 50 fps */
      });
    
    // first let's create the basic scene object
    // we will have a private (complete) as well as
    // a public interface, which we will return from this
    // function
    var scene = {
      pub: {
      }
    };
    
    // [pzion 20110711] This is a bit of a hack: we populate the *global*
    // OpenGL constants structure if it doesn't alrady exist.
    if (!FABRIC.SceneGraph.OpenGLConstants)
      FABRIC.SceneGraph.OpenGLConstants = JSON.parse(context.EX.getLoadedExts().FabricOGL.jsConstants);

    // EAch Viewport creates a new fabricwindow which is the origin of
    // window redraw events.
    var viewports = [];

    ///////////////////////////////////////////////////////////////////
    // Maps
    var operatorStore = {};
    var sceneGraphNodes = {};
    var shaderNodeStore = {};

    ///////////////////////////////////////////////////////////////////
    // Profiling
    var Timer = function(name) {
      this.name = name;
      this.count = 0;
      this.time = 0.0;
      this.childTimers = {};
      this.start = function() {
        this.pushtime = (new Date).getTime();
      }
      this.stop = function() {
        this.count += 1;
        var bracket = (new Date).getTime() - this.pushtime;
        this.time += bracket;
        if (!profiles[this.name]) {
          profiles[this.name] = { time: bracket, count: 1 };
        }else {
          profiles[this.name].time += bracket;
          profiles[this.name].count += 1;
        }
      }
      this.start();
    }
    var timerTree, currTimer;
    currTimer = new Timer('createScene');
    timerTree = currTimer;
    var timerStack = [];
    var profiles = {};

    var pushTimer = function(name) {
      var timer = currTimer.childTimers[name];
      if (timer) {
        timer.start();
      }else {
        timer = new Timer(name);
        currTimer.childTimers[name] = timer;
      }
      currTimer = timer;
      timerStack.push(timer);
    }
    var popTimer = function(name) {
      var timer = timerStack.pop();
      timer.stop();
      if (timerStack.length > 0) {
        currTimer = timerStack[timerStack.length - 1];
      }else {
        currTimer = timerTree;
      }
    }

    /////////////////////////////////////////////////////////////////////
    // The private scene object is passed into the constructor and can augment
    // objects with extra functionality.

    // Use the following methods with *extreme* caution. please...
    scene.getContext = function() {
      return context;
    };
    scene.pub.getContextId = function() {
      return context.getContextID();
    };
    scene.bindViewportToWindow = function(element, viewPort, options) {
      var fabricwindow = context.createWindow(element, options);
      // [pzion 20110326] Add a context menu item for any windows
      // in the context that pops up a Fabric debugger for the context
      // Note: PT 19-09-11
      // This feature has only been working on OsX, and so we are diabling
      // it for the Fabric beta to ensure the developer experience is clear
      // and consistent across platforms. Developers on OsX, feel free to
      // uncomment this code :)
    //  fabricwindow.addPopUpMenuItem(
    //    'display-core-debugger',
    //    'Fabric debugger...',
    //    function(arg) {
    //      scene.pub.displayDebugger();
    //    }
    //  );
      viewports.push(viewPort);
      return fabricwindow;
    };
    scene.getRegisteredTypesManager = function() {
      return context.RegisteredTypesManager;
    };

    ///////////////////////////////////////////////////////////////////
    //  Resource File Loading
    scene.writeResourceFile = function(filepath, fileContents) {
      throw ' FS has been depreciated ';
    };
    scene.readResourceFile = function(filepath) {
      throw ' FS has been depreciated ';
    };
    scene.assignDefaults = assignDefaults;
    
    scene.cloneObj = function(obj, assignedValues) {
      var i, clonedobj = {};
      for (i in obj) {
        clonedobj[i] = obj[i];
      }
      if(assignedValues){
        for (i in assignedValues) {
          clonedobj[i] = assignedValues[i];
        }
      }
      return clonedobj;
    };
    
    scene.loadResourceURL = function(url, mimeType, callback) {
      return FABRIC.loadResourceURL(url, mimeType, callback);
    };
    //////////////////////////////////////////////////
    // Timers.
    scene.pushTimer = function(name) {
      pushTimer(name);
    };
    scene.popTimer = function(name) {
      popTimer(name);
    };
    //////////////////////////////////////////////////
    // Constructors for the CORE
    scene.constructEventNode = function(name) {
      return context.DependencyGraph.createEvent(name);
    };
    scene.constructEventHandlerNode = function(name) {
      return context.DependencyGraph.createEventHandler(name);
    };
    scene.constructResourceLoadNode = function(name) {
      return context.DependencyGraph.createResourceLoadNode(name);
    };
    scene.constructDependencyGraphNode = function(name, isResourceLoad) {
      return context.DependencyGraph.createNode(name);
    };
    
    scene.constructManager = function(type, options) {
      if (!FABRIC.SceneGraph.managerDescriptions[type]) {
        throw ('Manager Constructor not Registered:' + type);
      }
      options = (options ? options : {});
      var managerNode = FABRIC.SceneGraph.managerDescriptions[type].factoryFn(options, scene);
      if (!managerNode) {
        throw (' Factory function method must return an object');
      }
      var parentTypeOfFn = managerNode.pub.isTypeOf;
      managerNode.pub.isTypeOf = function(classname) {
        if (classname == type) {
          return true;
        }else if (parentTypeOfFn !== undefined) {
          return parentTypeOfFn(classname);
        }else {
          return false;
        }
      }
      return managerNode;
    };
    
    scene.constructNode = function(type, options) {
      if (!FABRIC.SceneGraph.nodeDescriptions[type]) {
        throw ('Node Constructor not Registered:' + type);
      }
      options = (options ? options : {});
      if(!options.type) options.type = type;
      var sceneGraphNode = FABRIC.SceneGraph.nodeDescriptions[type].factoryFn(options, scene);
      if (!sceneGraphNode) {
        throw (' Factory function method must return an object');
      }
      var parentTypeOfFn = sceneGraphNode.pub.isTypeOf;
      sceneGraphNode.pub.isTypeOf = function(classname) {
        if (classname == type) {
          return true;
        }else if (parentTypeOfFn !== undefined) {
          return parentTypeOfFn(classname);
        }else {
          return false;
        }
      }
      return sceneGraphNode;
    };
    
    scene.renameNode = function(privateNode, newname) {
      if (!sceneGraphNodes[privateNode.pub.getName()]) {
        throw ('SceneGraphNode "' + privateNode.pub.getName() + '" does not exist!');
      }
      if (sceneGraphNodes[newname]) {
        throw ('Name "' + newname + '" is already taken');
      }
      delete sceneGraphNodes[privateNode.pub.getName()];
      sceneGraphNodes[newname] = privateNode;
    }
    scene.getPrivateInterface = function(publicNode) {
      if (publicNode.pub && publicNode.pub.getName) {
        return publicNode;
      }
      if (!publicNode.getName) {
        throw ('Given object is not a valid public interface.');
      }
      if (!sceneGraphNodes[publicNode.getName()]) {
        throw ('SceneGraphNode "' + publicNode.getName() + '" does not exist!');
      }
      return sceneGraphNodes[publicNode.getName()];
    };
    scene.getPublicInterface = function(privateNode) {
      if (!privateNode.pub) {
        throw ('Given object does not have a public interface.');
      }
      return privateNode.pub;
    };
    scene.constructShaderNode = function(shaderType, options) {
      if (shaderNodeStore[shaderType]) {
        return shaderNodeStore[shaderType];
      }
      var shader = this.constructNode('Shader', options);
      shaderNodeStore[shaderType] = shader;
      return shader;
    };
    scene.constructOperator = function(operatorDef) {
      
      var constructBinding = function(operator) {
        var binding = context.DG.createBinding();
        binding.setOperator(operator);
        binding.setParameterLayout(operatorDef.parameterLayout ? operatorDef.parameterLayout : []);
        return binding;
      }
      
      var uid = operatorDef.operatorName;
      if (operatorStore[uid]) {
        // If this operator has already been constructed,
        // reuse the exiting one. returne a unique binding.
        return constructBinding(operatorStore[uid]);
      }
      
      var operator = context.DG.createOperator(uid);
      var filename;
      operatorStore[uid] = operator;
      
      ///////////////////////////////////////////////////
      // Construct the operator
      var configureOperator = function(code) {
        var descDiags;
        
        if (operatorDef.mainThreadOnly){
          operator.setMainThreadOnly(true);
        }
  
        descDiags = function(fullCode, diags) {
          var fullCodeLines = fullCode.split('\n');
          var desc = 'Error compiling operator: ' + operatorDef.operatorName + ' in '+filename+'\n';
          if (operatorDef.srcFile) desc += 'File:' + operatorDef.srcFile + '\n';
          for (var i = 0; i < diags.length; ++i) {
            if (i == 16) {
              desc += '(' + (diags.length - i) + ' more diagnostic(s) omitted)\n';
              break;
            }
            desc += diags[i].line + ':' + diags[i].column + ': ' + diags[i].level + ': ' + diags[i].desc + '\n';
            var line = diags[i].line - 1;
            for (var j = line - 6; j <= line + 6; ++j) {
              if (j >= 0 && j < fullCodeLines.length) {
                if (j == line)
                  desc += '>>>\t';
                else
                  desc += '\t';
                desc += '' + fullCodeLines[j] + '\n';
              }
            }
          }
          return desc;
        };
  
        operator.setEntryFunctionName(operatorDef.entryFunctionName);
        try {
          operator.setSourceCode(filename, code);
        }
        catch (e) {
          var message = 'Error compiling operator: ' + operatorDef.operatorName + '\n';
          if (operatorDef.srcFile) message += ' File:' + operatorDef.srcFile + '\n';
          else message += ' Code:' + operatorDef.srcCode + '\n';
          throw (message + e);
        }
        var diagnostics = operator.getDiagnostics();
        if (diagnostics.length > 0) {
          delete operatorStore[uid];
          console.error(descDiags(operator.getSourceCode(), diagnostics));
        }
      }

      if (operatorDef.srcFile) {
        filename = operatorDef.srcFile;
        if(operatorDef.async === false){
          var code = FABRIC.loadResourceURL(operatorDef.srcFile, 'text/plain');
          code = FABRIC.preProcessCode(code, operatorDef.preProcessorDefinitions);
          configureOperator(code);
        }else{
          FABRIC.loadResourceURL(operatorDef.srcFile, 'text/plain', function(code){
            code = FABRIC.preProcessCode(code, operatorDef.preProcessorDefinitions);
            configureOperator(code);
          });
        }
      }
      else if (operatorDef.srcCode) {
        filename = operatorDef.operatorName;
        // Fake an asynchronous operator construction so that we don't block waiting
        // for the operator compilation.
        if(operatorDef.async === false){
          var code = FABRIC.preProcessCode(operatorDef.srcCode, operatorDef.preProcessorDefinitions);
          configureOperator(code);
        }else{
          FABRIC.createAsyncTask(function(){
            var code = FABRIC.preProcessCode(operatorDef.srcCode, operatorDef.preProcessorDefinitions);
            configureOperator(code);
          });
        }
      }else{
        throw "Invalid operator definition. Either a source file must be specified, or source code.";
      }
      return constructBinding(operator);
    };
    //////////////////////////////////////////////////////////
    // Accessors
    var globalsNode;
    scene.getGlobalsNode = function() {
      return globalsNode;
    };
    scene.getScenePreRedrawEventHandler = function() {
      return preDrawEventHandler;
    };
    scene.getSceneRedrawEventHandler = function() {
      return beginDrawEventHandler;
    };
    scene.getSceneRedrawOpaqueObjectsEventHandler = function() {
      return beginDrawOpaqueObjectsEventHandler;
    };
    scene.getSceneRedrawTransparentObjectsEventHandler = function() {
      return beginDrawTransparentObjectsEventHandler;
    };
    scene.getSceneRedrawOverlayObjectsEventHandler = function() {
      return beginDrawOverlayObjectsEventHandler;
    };
    
    scene.getScenePostRedrawEventHandler = function() {
      return postDrawEventHandler;
    };
    scene.getSceneRaycastEventHandler = function() {
      return sceneRaycastEventHandler;
    };
    //////////////////////////////////////////////////////////
    // Shadow Mapping
    scene.registerShadowCastingLightSourceHandler = function(lightSourceRenderhandler) {
      propagateRenderShadowMapEvent.appendChildEventHandler(lightSourceRenderhandler);
      lightSourceRenderhandler.appendChildEventHandler(beginRenderShadowMap);
    };
    scene.getBeginRenderShadowMapEventHandler = function() {
      return beginRenderShadowMap;
    };
    scene.addEventHandlingFunctions = function(obj) {
      // We store a map of arrays of event listener functions.
      var eventListeners = {};
      obj.pub.addEventListener = function(type, fn) {
        if(!fn) throw "Listener Function not provided";
        if (!eventListeners[type]) {
          eventListeners[type] = [];
        }
        eventListeners[type].push(fn);
      };
      obj.pub.removeEventListener = function(type, fn) {
        if(!fn) throw "Listener Function not provided";
        if (eventListeners[type]) {
          var id = eventListeners[type].indexOf(fn);
          if (id !== -1) {
            eventListeners[type].splice(id, 1);
          }
        }
      };
      obj.pub.fireEvent = function(type, evt) {
        for (i in eventListeners[type]) {
          if (eventListeners[type].hasOwnProperty(i)) {
            eventListeners[type][i].call(this, evt);
          }
        }
      }
      return obj;
    };

    /////////////////////////////////////////////////////////////////////

    scene.pub.displayDebugger = function() {
      FABRIC.displayDebugger(context);
    };
    scene.getSceneGraphNodes = function(name) {
      return sceneGraphNodes;
    };
    scene.pub.getSceneGraphNode = function(name) {
      if(sceneGraphNodes[name]){
        return sceneGraphNodes[name].pub;
      }
      return undefined;
    };
    scene.setSceneGraphNode = function(name,node) {
      return sceneGraphNodes[name] = node;
    };
    scene.pub.getRootTransformNode = function() {
      return rootNode;
    };
    scene.pub.constructManager = function(type, options) {
      return scene.constructManager(type, options).pub;
    };
    scene.pub.constructNode = function(type, options) {
      return scene.constructNode(type, options).pub;
    };
    // TODO: Remove the following 2 functoins and test.
    scene.pub.readResourceFile = function(filepath) {
      return scene.readResourceFile(filepath);
    };
    scene.pub.writeResourceFile = function(filepath, fileContents) {
      scene.writeResourceFile(filepath, fileContents);
    };
    scene.pub.getGlobalsNode = function() {
      return globalsNode;
    };
    scene.pub.importAssetFile = function(file, options, callback) {
      var ext = file.split('.').pop().toLocaleLowerCase();
      if (FABRIC.SceneGraph.assetLoaders[ext]) {
        var assets = FABRIC.SceneGraph.assetLoaders[ext](scene.pub, file, options, callback);
        return assets;
      }
      else {
        throw ('Missing Resource Loader for :' + ext);
      }
    };
    scene.pub.IO = context.IO;
    scene.pub.redrawAllViewports = function(force) {
      for (var i=0; i<viewports.length; i++) {
        viewports[i].pub.redraw(force);
      }
    };
    scene.pub.getErrors = function() {
      var errors = [];
      var allNamedObjects = context.DG.getAllNamedObjects();
      for (i in allNamedObjects) {
        if (allNamedObjects.hasOwnProperty(i)) {
          var dgErrors = allNamedObjects[i].getErrors();
          if(!dgErrors){
            console.warn( i + " does not return an errors array");
            continue;
          }
          if (dgErrors.length > 0) {
            errors.push(i + ':' + JSON.stringify(dgErrors));
          }
        }
      }
      return errors;
    };
    scene.pub.getTimerProfiles = function() {
      if (timerStack.length > 0) {
        throw 'Timer Stack not balanced';
      }
      timerTree.stop();
      var timerProfiles = 'Scene Construction Profiles \n';
      timerProfiles += 'SceneConstructTime:' + timerTree.time / 1000 + ' seconds\n';

      var logTimer = function(timer, parentTimer, indent) {
        var str = indent + (
          timer.name + ' count:' + timer.count + ' time:' + timer.time / 1000 +
          ' percentage:' + ((timer.time / parentTimer.time) * 100).toFixed(2) + '%\n');
        for (i in timer.childTimers) {
          str += logTimer(timer.childTimers[i], timer, indent + '\t');
        }
        return str;
      }
      var str = logTimer(timerTree, timerTree, '');
      str += '----------------------------------\n';
      for (i in profiles) {
        str += (i + ' count:' + profiles[i].count + ' time:' + profiles[i].time / 1000 +
          ' costpercall:' + ((profiles[i].time / profiles[i].count) / 1000).toFixed(4) +
          ' percentage:' + ((profiles[i].time / timerTree.time) * 100).toFixed(2) + '%\n');
      }
      return str;
    };

    scene.addEventHandlingFunctions(scene);
    
    window.addEventListener('unload', function(){
      scene.pub.fireEvent('unloading');
    }, false);

    ///////////////////////////////////////////////////////////////////
    // Create the root transform node.
    // All transformation nodes must have a parent...
    var rootNode = scene.pub.constructNode('Transform', { name: 'RootTransfrom', hierarchical: false });

    // and the shaders will be left connected to this node. Multiple
    // cameras can render the scene by connecting to this node.
    var preDrawEventHandler, postDrawEventHandler;
    var propagateRenderShadowMapEvent, beginRenderShadowMap, shadowMapMaterial;
    if(sceneOptions.preDraw){
      preDrawEventHandler = scene.constructEventHandlerNode('Scene_PreDraw');
      propagateRenderShadowMapEvent = scene.constructEventHandlerNode('PropagateRenderShadowMapEvent');
      
      ///////////////////////////////////////////////////////////////////
      // Shadowcasting Lightsource <-> SceneGraph draw event handler firewall
      beginRenderShadowMap = scene.constructEventHandlerNode('BeginRenderShadowMap');
      
        
      // The 'Parent' node is the first child of the scene redraw event.
      // This means that the event traversial will propagate down the
      // shadow casting graph, before propagating down the render graph.
      preDrawEventHandler.appendChildEventHandler(propagateRenderShadowMapEvent);
    }
    if(sceneOptions.postDraw){
      postDrawEventHandler = scene.constructEventHandlerNode('Scene_PostDraw');
    }

    ///////////////////////////////////////////////////////////////////
    // All scene draw event handlers(shaders) are attached to this handler.
    var beginDrawEventHandler = scene.constructEventHandlerNode('Scene_Draw');
    
    var beginDrawOpaqueObjectsEventHandler = scene.constructEventHandlerNode('Scene_DrawOpaqueObjects');
    beginDrawEventHandler.appendChildEventHandler(beginDrawOpaqueObjectsEventHandler);
    
    // Transparent objects are always drawn after opaque objects
    var beginDrawTransparentObjectsEventHandler = scene.constructEventHandlerNode('Scene_DrawTransparentObjects');
    beginDrawEventHandler.appendChildEventHandler(beginDrawTransparentObjectsEventHandler);
    
    // Overlay objects are always drawn after everything else
    var beginDrawOverlayObjectsEventHandler = scene.constructEventHandlerNode('Scene_DrawOverlaybjects');
    beginDrawEventHandler.appendChildEventHandler(beginDrawOverlayObjectsEventHandler);
    
    ///////////////////////////////////////////////////////////////////
    // Window <-> SceneGraph raycast event handler firewall
    var sceneRaycastEventHandler = scene.constructEventHandlerNode('Scene_raycast');

    if (sceneOptions.constructGlobalsNode) {
      globalsNode = scene.constructDependencyGraphNode('Scene_globals');

      if (sceneOptions.constructAnimationInterface) {
        // All time values are in seconds. 
        globalsNode.addMember('time', 'Scalar', 0);
        globalsNode.addMember('time_prevupdate', 'Scalar', 0);
        globalsNode.addMember('timestep', 'Scalar', 0);
        globalsNode.bindings.append(scene.constructOperator( {
          operatorName:'calcTimeStep',
          srcCode:'operator calcTimeStep(io Scalar t, io Scalar prev_t, io Scalar delta_t){ delta_t = t - prev_t; prev_t = t; }',
          entryFunctionName:'calcTimeStep',
          parameterLayout: [
            'self.time',
            'self.time_prevupdate',
            'self.timestep'
          ]
        }));
        
        var isPlaying = false, time = 0;
        var setTime = function(t, redraw) {
          time = Math.round(t/sceneOptions.timeStep) * sceneOptions.timeStep;
          globalsNode.setData('time', 0, time);
          scene.pub.fireEvent('timechanged', { time: time, playing: isPlaying });
          if(redraw !== false){
            scene.pub.redrawAllViewports(true);
          }
        }
        var timeStepMS = sceneOptions.timeStep * 1000.0;
        var prevTime, prevFrameDuration = 0;
      //  var frameStartTime, frameRate = 0;
        var advanceTime = function() {
          var t = time + sceneOptions.timeStep;
          
          // The computer will attempt to play back
          // at exactly the given frame rate. If the
          // frame rate cannot be achieved it plays 
          // as fast as possible. The time step as
          // used throughout the graph will always 
          // be fixed at the given rate.
          var currTime = (new Date).getTime();
          var prevFrameDuration = (currTime - prevTime);
          
          // Measuring the frame time using JavaScript gives
          // garbage results, and I'm not sure why. 
        //  frameRate = (currTime - frameStartTime);
        //  console.log("frameRate:"+frameRate);
        //  frameStartTime = currTime;
          if(prevFrameDuration < timeStepMS){
            var delay = (timeStepMS - prevFrameDuration);
            setTimeout(function(){
                prevTime = currTime + delay;
                setTime(t);
              },
              delay
            );
          }else{
            prevTime = currTime;
            setTime(t);
          }
        }
        /////////////////////////////////////////////////////////
        // Animation Interface
        scene.pub.animation = {
          setTime:function(t, redraw) {
            setTime(t, redraw);
          },
          getTime:function() {
            return time;
          },
          getTimeStep:function() {
            return sceneOptions.timeStep;
          },
          play: function() {
            prevTime = (new Date).getTime();
            isPlaying = true;
            // Note: this is a big ugly hack to work around the fact that
            // we have zero or more windows. What happens when we have
            // multiple viewports? Should the 'play' controls be moved to
            // Viewport?
            prevTime = (new Date).getTime();
            scene.getContext().VP.viewPort.setRedrawFinishedCallback(advanceTime);
            scene.getContext().VP.viewPort.needsRedraw();
          },
          pause: function() {
            isPlaying = false;
            scene.getContext().VP.viewPort.setRedrawFinishedCallback(null);
          },
          reset: function() {
            isPlaying = false;
            scene.getContext().VP.viewPort.setRedrawFinishedCallback(null);
            setTime(0, true);
            globalsNode.setData('time_prevupdate', 0, 0);
          },
          step: function() {
            advanceTime();
          }
        };
        scene.isPlaying = function(){
          return isPlaying;
        }
      }
    }

    // only return the public interface to the outside world
    return scene.pub;
  }
};

FABRIC.SceneGraph.registerNodeType('SceneGraphNode', {
  briefDesc: 'The base class for the SceneGraphNodes.',
  detailedDesc: 'The SceneGraphNode is a basic wrapper for Fabric\'s DGNode. Each SceneGraphNode can '+
                'contain several DGNodes. Furthermore the SceneGraphNode implements the concept of a '+
                'private and public interface. All methods which are public (outside of the scope of) '+
                'a constructor) are added to the "pub" member of the SceneGraphNode, whereas private '+
                'methods (inside of the scope of a constructor) are attached to the SceneGraphNode '+
                'object itself.',
  factoryFn: function(options, scene) {

    var dgnodes = {};
    var eventnodes = {};
    var eventhandlernodes = {};
    var memberInterfaces = {};
    var nodeReferenceInterfaces = {};
    var nodeReferences = {};

    var capitalizeFirstLetter = function(str) {
      return str[0].toUpperCase() + str.substr(1);
    };
    
    // ensure the name is unique
    var name, type = options.type;
    
    var sceneGraphNode = {
      pub: {
        getName: function() {
          return name;
        },
        setName: function(newname) {
          if (scene.pub.getSceneGraphNode(newname)) {
            var prefix = 1;
            while (scene.pub.getSceneGraphNode(newname + prefix)) {
              prefix++;
            }
            newname = newname + prefix;
          }
          if(name){
            scene.renameNode(sceneGraphNode, newname);
          }
          name = newname;
        },
        getType: function() {
          return type;
        }
      },
      addMemberInterface : function(corenode, memberName, defineSetter) {
        var getterName = 'get' + capitalizeFirstLetter(memberName);
        var getterFn = function(sliceIndex){
          return corenode.getData(memberName, sliceIndex);
        }
        sceneGraphNode.pub[getterName] = getterFn;
        if(defineSetter===true){
          var setterName = 'set' + capitalizeFirstLetter(memberName);
          var setterFn = function(value, sliceIndex){
            var prevalue = corenode.getData(memberName, sliceIndex?sliceIndex:0);
            corenode.setData(memberName, sliceIndex?sliceIndex:0, value);
            
            scene.pub.fireEvent('valuechanged', {
              sgnode: sceneGraphNode.pub,
              newvalue: value,
              prevalue: prevalue,
              sliceIndex: sliceIndex,
              getterFn: getterFn,
              setterFn: setterFn
            });
          }
          sceneGraphNode.pub[setterName] = setterFn;
          memberInterfaces[memberName] = { getterFn:getterFn, setterFn:setterFn };
        }
      },
      addReferenceInterface : function(referenceName, typeConstraint, setterCallback) {
        var getterName = 'get' + capitalizeFirstLetter(referenceName) + 'Node';
        var setterName = 'set' + capitalizeFirstLetter(referenceName) + 'Node';
        var getterFn = function(){
          return nodeReferences[referenceName];
        }
        var typeConstraints = typeConstraint.split('|');
        var setterFn = function(node, option){
          if (node){
            var matchesType = false;
            for(var i=0; !matchesType && i<typeConstraints.length; i++){
              if(node.isTypeOf(typeConstraints[i])) {
                matchesType = true;
              }
            }
            if(!matchesType){
              throw ('Incorrect type assignment. Must assign a '+typeConstraint);
            }
          }
          var prevnode = nodeReferences[referenceName];
          nodeReferences[referenceName] = node;
          // a reference can be removed by passing in undefined for the value.
          // the setterFn must handle undefined as a value.
          setterCallback(node ? scene.getPrivateInterface(node) : undefined, option);
          
          scene.pub.fireEvent('referenceassigned', {
            sgnode: sceneGraphNode.pub,
            newnode: node,
            prevnode: prevnode,
            getterFn: getterFn,
            setterFn: setterFn
          });
          return sceneGraphNode.pub;
        }
        sceneGraphNode.pub[getterName] = getterFn;
        sceneGraphNode.pub[setterName] = setterFn;
        nodeReferenceInterfaces[referenceName] = { getterFn:getterFn, setterFn:setterFn };
        return sceneGraphNode.pub[setterName];
      },
      addReferenceListInterface : function(referenceName, typeConstraint, adderCallback, removeCallback) {
        nodeReferences[referenceName] = [];
        var getterName = 'get' + capitalizeFirstLetter(referenceName) + 'Node';
        var adderName = 'add' + capitalizeFirstLetter(referenceName) + 'Node';
        var removerName = 'remove' + capitalizeFirstLetter(referenceName) + 'Node';
        var getterFn = function(index){
          return nodeReferences[referenceName][index ? index : 0];
        }
        var typeConstraints = typeConstraint.split('|');
        var adderFn = function(node){
          if (node){
            for(var i=0; i<typeConstraints.length; i++){
              if(!node.isTypeOf(typeConstraints[i])) {
                throw ('Incorrect type assignment. Must assign a '+typeConstraint);
              }
            }
          }
          var index = nodeReferences[referenceName].indexOf(node);
          if(index !== -1) return sceneGraphNode.pub;
          nodeReferences[referenceName].push(node);
          adderCallback(scene.getPrivateInterface(node));
          
          scene.pub.fireEvent('referenceadded', {
            sgnode: sceneGraphNode.pub,
            newnode: node,
            getterFn: getterFn,
            adderFn: adderFn,
            removerFn: removerFn
          });
          return sceneGraphNode.pub;
        }
        var removerFn = function(val){
          var index = -1;
          if(typeof val == 'number'){
            index = val;
          }else{
            index = nodeReferences[referenceName].indexOf(val);
          }
          if (index === -1) {
            throw ( typeConstraint + ' not assigned');
          }
          var node = nodeReferences[referenceName][index];
          removeCallback(scene.getPrivateInterface(node), index);
          nodeReferences[referenceName].splice(index, 1);
          
          scene.pub.fireEvent('referenceremoved', {
            sgnode: sceneGraphNode.pub,
            prevnode: node,
            getterFn: getterFn,
            adderFn: adderFn,
            removerFn: removerFn
          });
          return sceneGraphNode.pub;
        }
        sceneGraphNode.pub[getterName] = getterFn;
        sceneGraphNode.pub[adderName] = adderFn;
        sceneGraphNode.pub[removerName] = removerFn;
        nodeReferenceInterfaces[referenceName] = { getterFn:getterFn, adderFn:adderFn, removerFn:removerFn };
        return sceneGraphNode.pub[adderName];
      },
      constructDGNode: function(dgnodename, isResourceLoad) {
        if(dgnodes[dgnodename]){
          throw "SceneGraphNode already has a " + dgnodename;
        }
        var dgnode;
        if(isResourceLoad){
          dgnode = scene.constructResourceLoadNode(name + '_' + dgnodename);
        }
        else{
          dgnode = scene.constructDependencyGraphNode(name + '_' + dgnodename);
        }
        dgnode.sceneGraphNode = sceneGraphNode;
        sceneGraphNode['get' + dgnodename] = function() {
          return dgnode;
        };
        dgnodes[dgnodename] = dgnode;
        return dgnode;
      },
      constructResourceLoadNode: function(dgnodename) {
        return sceneGraphNode.constructDGNode(dgnodename, true);
      },
      getDGNodes: function() {
        return dgnodes;
      },
      constructEventHandlerNode: function(ehname) {
        var eventhandlernode = scene.constructEventHandlerNode(name + '_' + ehname);
        eventhandlernode.sceneGraphNode = sceneGraphNode;
        sceneGraphNode['get' + ehname + 'EventHandler'] = function() {
          return eventhandlernode;
        };
        eventhandlernodes[ehname] = eventhandlernode;
        return eventhandlernode;
      },
      constructEventNode: function(eventname) {
        eventnode = scene.constructEventNode(name + '_' + eventname);
        eventnode.sceneGraphNode = sceneGraphNode;
        eventnodes[eventname] = eventnode;
        return eventnode;
      },
      addDependencies: function(sceneSerializer) {
        for(var referenceName in nodeReferences){
          if(nodeReferences[referenceName].constructor.name == 'Array'){
            for(var i=0; i<nodeReferences[referenceName].length; i++){
              sceneSerializer.addNode(nodeReferences[referenceName][i]);
            }
          }
          else{
            sceneSerializer.addNode(nodeReferences[referenceName]);
          } 
        }
      },
      writeData: function(sceneSerializer, constructionOptions, nodeData) {
        constructionOptions.name = name;
        
        for(var referenceName in nodeReferences){
          if(nodeReferences[referenceName].constructor.name == 'Array'){
            nodeData[referenceName] =  [];
            for(var i=0; i<nodeReferences[referenceName].length; i++){
              nodeData[referenceName].push(nodeReferences[referenceName][i].getName());
            }
          }
          else {
            nodeData[referenceName] = nodeReferences[referenceName].getName();
          }
        }
        for(var memberName in memberInterfaces){
          if(!nodeData[memberName]){
            nodeData[memberName] = memberInterfaces[memberName].getterFn();
          }
        }
      },
      readData: function(sceneDeserializer, nodeData) {
        for(var referenceName in nodeReferences){
          if(nodeData[referenceName]){
            if(nodeData[referenceName].constructor.name == 'Array'){
              for(var i=0; i<nodeReferences[referenceName].length; i++){
                var dgnode = sceneDeserializer.getNode(nodeData[referenceName][i]);
                if(dgnode){
                  nodeReferenceInterfaces[referenceName].adderFn(dgnode);
                }
              }
            }
            else if(typeof nodeData[referenceName] == 'string'){
              var dgnode = sceneDeserializer.getNode(nodeData[referenceName]);
              if(dgnode){
                nodeReferenceInterfaces[referenceName].setterFn(dgnode);
              }
            }
          }
        }
        for(var memberName in memberInterfaces){
          if(nodeData[memberName]){
            memberInterfaces[memberName].setterFn(nodeData[memberName]);
          }
        }
      }
    }
    
    sceneGraphNode.pub.setName(options.name ? options.name : options.type);

    // store it to the map
    scene.setSceneGraphNode(name, sceneGraphNode);
    
    return sceneGraphNode;
  }});

FABRIC.SceneGraph.registerNodeType('Viewport', {
  briefDesc: 'The Viewport node implements the basic OpenGL canvas.',
  detailedDesc: 'Utilizing a redraw eventhandler, the ViewPort node offers a powerful OpenGL canvas, ' +
                'which is connected to an embed element inside the DOM. The most important parameter of '+
                'the ViewPort node\'s options is the windowElement, the ID of the HTML element to append '+
                'the viewport to.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    windowElement: 'The HTML element to attach the viewport to.',
    cameraNode: 'The cameraNode to use for this viewport',
    enableMouseEvents: 'Set to true this ensures to create the mouse down, up and move events.',
    enableRaycasting: 'Set to true this enables raycasting for selection of 3D objects.',
    mouseUpEvents: 'Set to true this enables the mouse up event',
    mouseMoveEvents: 'Set to true this enables the mouse move event',
    backgroundColor: 'The background color of the viewport used for glClearColor',
    postProcessEffect: 'An optional PostProcessEffect node to be used after drawing the viewport, undefined if None.',
    rayIntersectionThreshold: 'The treshold of raycast intersections, typicall below 1.0'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
        windowElement: undefined,
        cameraNode: undefined,
        enableMouseEvents: true,
        enableRaycasting: false,
        mouseUpEvents: true,
        mouseMoveEvents: true,
        backgroundColor: FABRIC.RT.rgb(0.5, 0.5, 0.5),
        postProcessEffect: undefined,
        rayIntersectionThreshold: 0.1,
        checkOpenGL2Support: true
      });

    if (!options.windowElement) {
      throw ('Must provide a window to this constructor');
    }

    var cameraNode = undefined, fabricwindow;
    var raycastingEnabled = false;
    var loading = true;
    var windowElement = options.windowElement;
    var viewportNode = scene.constructNode('SceneGraphNode', options),
      dgnode = viewportNode.constructDGNode('DGNode'),
      redrawEventHandler = viewportNode.constructEventHandlerNode('Redraw');
      
    dgnode.addMember('backgroundColor', 'Color', options.backgroundColor);

    redrawEventHandler.setScope('viewPort', dgnode);

    redrawEventHandler.preDescendBindings.append(scene.constructOperator({
          operatorName: 'viewPortBeginRender',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/viewPortBeginRender.kl',
          entryFunctionName: 'viewPortBeginRender',
          parameterLayout: [
            'window.width',
            'window.height',
            'viewPort.backgroundColor'
          ]
        }));


    var fabricwindow = scene.bindViewportToWindow(windowElement, viewportNode);
    
    var initialLoad = true;
    var visible = false;
    var startLoadMode = function() {
      fabricwindow.hide();
      FABRIC.appendOnResolveAsyncTaskCallback(function(label, countRemaining){
        if(countRemaining===0){

          if(initialLoad) {
            initialLoad = false;
            loading = false;
            redrawEventHandler.setScope('window', fabricwindow.windowNode);
            if(scene.getScenePreRedrawEventHandler()){
              fabricwindow.redrawEvent.appendEventHandler(scene.getScenePreRedrawEventHandler());
            }
            fabricwindow.redrawEvent.appendEventHandler(redrawEventHandler);
            if(scene.getScenePostRedrawEventHandler()){
              fabricwindow.redrawEvent.appendEventHandler(scene.getScenePostRedrawEventHandler());
            }
            if(raycastingEnabled){
              // the sceneRaycastEventHandler propogates the event throughtout the scene.
              viewPortRaycastEventHandler.appendChildEventHandler(scene.getSceneRaycastEventHandler());
            }
        
            // These functions cannot be called during the initial construction of the
            // graph because they rely on an OpenGL context being set up, and this occurs
            // during the 1st redraw.
            viewportNode.pub.getOpenGLVersion = fabricwindow.getOpenGLVersion;
            viewportNode.pub.getGlewSupported = fabricwindow.getGlewSupported;
            viewportNode.pub.show = function(){ fabricwindow.show(); visible = true; };
            viewportNode.pub.hide = function(){ fabricwindow.hide(); visible = false; };

            viewportNode.pub.getWidth = function(){ return fabricwindow.windowNode.getData('width'); };
            viewportNode.pub.getHeight = function(){ return fabricwindow.windowNode.getData('height'); };
            viewportNode.pub.getGlewSupported = fabricwindow.getGlewSupported;
          }

          viewportNode.pub.show();
          return true;
        }
      });
    };
    startLoadMode();
    
    var propagationRedrawEventHandler = viewportNode.constructEventHandlerNode('DrawPropagation');
    redrawEventHandler.appendChildEventHandler(propagationRedrawEventHandler);

    // Texture Stub for loading Background textures.
    var backgroundTextureNode, textureStub;
    textureStub = viewportNode.constructEventHandlerNode('BackgroundTextureStub');
    propagationRedrawEventHandler.appendChildEventHandler(textureStub);

    var postProcessEffects = [];

    ///////////////////////////////////////////////////////////////////
    // Raycasting
    var viewPortRaycastEvent, viewPortRaycastEventHandler, viewPortRayCastDgNode;
    var raycastingConstructed = false;

    viewportNode.pub.enableRaycasting = function() {
      if( !raycastingEnabled && scene.getSceneRaycastEventHandler() ) {
        raycastingEnabled = true;
        if( !raycastingConstructed ) {
          raycastingConstructed = true;
          viewPortRayCastDgNode = viewportNode.constructDGNode('RayCastDgNodeDGNode');
          viewPortRayCastDgNode.addMember('x', 'Integer');
          viewPortRayCastDgNode.addMember('y', 'Integer');
          viewPortRayCastDgNode.addMember('ray', 'Ray');
          viewPortRayCastDgNode.addMember('threshold', 'Scalar', options.rayIntersectionThreshold);
          viewPortRayCastDgNode.setDependency(fabricwindow.windowNode, 'window');

          // this operator calculates the rayOri and rayDir from the scopes collected so far.
          // The scopes should be the window, viewport, camera and projection.
          viewPortRayCastDgNode.bindings.append(scene.constructOperator({
            operatorName: 'ViewportRaycast',
            srcFile: 'FABRIC_ROOT/SceneGraph/KL/viewPortUpdateRayCast.kl',
            entryFunctionName: 'viewPortUpdateRayCast',
            parameterLayout: [
              'camera.cameraMat44',
              'camera.projectionMat44',
              'window.width',
              'window.height',
              'self.x',
              'self.y',
              'self.ray'
            ]
          }));

          viewPortRaycastEventHandler = viewportNode.constructEventHandlerNode('Raycast');
          viewPortRaycastEventHandler.setScope('raycastData', viewPortRayCastDgNode);
          viewPortRaycastEvent = viewportNode.constructEventNode('RaycastEvent');
          viewPortRaycastEvent.setSelectType('RayIntersection');

          // Raycast events are fired from the viewport. As the event
          // propagates down the tree it collects scopes and fires operators.
          // The operators us the collected scopes to calculate the ray.
          viewPortRaycastEvent.appendEventHandler(viewPortRaycastEventHandler);
          
          // During load we do not connect up the event tree,
          // the registered callback will make the connection
          if( !loading )
            viewPortRaycastEventHandler.appendChildEventHandler(scene.getSceneRaycastEventHandler());
        }
      }
    };

    viewportNode.pub.disableRaycasting = function() {
      if( raycastingEnabled ) {
        raycastingEnabled = false;
      }
    };

    var getElementCoords = function(evt) {
      var browserZoom = fabricwindow.windowNode.getData('width') / evt.target.clientWidth;
      if (evt.offsetX != undefined) {
        // Webkit
        return new FABRIC.RT.Vec2(Math.floor(evt.offsetX*browserZoom), Math.floor(evt.offsetY*browserZoom));
      }
      else if (evt.layerX != undefined) {
        // Firefox
        return new FABRIC.RT.Vec2(Math.floor(evt.layerX*browserZoom), Math.floor(evt.layerY*browserZoom));
      }
      throw("Unsupported Browser");
    }
    
    viewportNode.getElementCoords = function(evt) {
      return getElementCoords(evt);
    };
    
    viewportNode.getWindowElement = function() {
      return windowElement;
    };
    viewportNode.getFabricWindowObject = function() {
      return fabricwindow;
    };
    
    viewportNode.addMemberInterface(dgnode, 'backgroundColor', true);
    viewportNode.addReferenceInterface('Camera', 'Camera',
      function(nodePrivate){
      // remove the child event handler first
        if(cameraNode != undefined) {
          propagationRedrawEventHandler.removeChildEventHandler(cameraNode.getRedrawEventHandler());
        }
        cameraNode = nodePrivate;
        propagationRedrawEventHandler.appendChildEventHandler(cameraNode.getRedrawEventHandler());
        if (viewPortRayCastDgNode) {
          viewPortRayCastDgNode.setDependency(cameraNode.getDGNode(), 'camera');
        }
      });
    
    
    viewportNode.addReferenceInterface('BackgroundTexture', 'Image2D',
      function(nodePrivate){
        if (textureStub.postDescendBindings.getLength() == 0) {
          textureStub.setScopeName('textureStub');
          textureStub.addMember('textureUnit', 'Integer', 0);
          textureStub.addMember('program', 'Integer', 0);
          textureStub.postDescendBindings.append(
            scene.constructOperator({
                operatorName: 'drawTextureFullScreen',
                srcFile: 'FABRIC_ROOT/SceneGraph/KL/drawTexture.kl',
                entryFunctionName: 'drawTextureFullScreen',
                parameterLayout: [
                  'self.textureUnit',
                  'self.program'
                ]
              }
           ));
        }
        if (backgroundTextureNode) {
          textureStub.removeChildEventHandler(backgroundTextureNode.getRedrawEventHandler());
        }
        backgroundTextureNode = nodePrivate;
        textureStub.appendChildEventHandler(backgroundTextureNode.getRedrawEventHandler());
      });
    viewportNode.addReferenceListInterface('PostProcessEffect', 'PostProcessEffect',
      function(nodePrivate, index){
        var parentEventHandler;
        if (postProcessEffects.length > 0) {
          parentEventHandler = postProcessEffects[postProcessEffects.length - 1].getRedrawEventHandler();
        }
        else {
          parentEventHandler = redrawEventHandler;
        }
        parentEventHandler.removeChildEventHandler(propagationRedrawEventHandler);
        parentEventHandler.appendChildEventHandler(nodePrivate.getRedrawEventHandler());
  
        nodePrivate.getRedrawEventHandler().appendChildEventHandler(propagationRedrawEventHandler);
        postProcessEffects.push(nodePrivate);
      },
      function(nodePrivate, index) {
        var parentEventHandler, childEventHandler;
        postProcessEffects.splice(index, 1);
        if(filterIndex < postProcessEffects.length){
          childEventHandler = postProcessEffects[filterIndex].getRedrawEventHandler();
        }
        else{
          childEventHandler = propagationRedrawEventHandler;
        }
        nodePrivate.getRedrawEventHandler().removeChildEventHandler(childEventHandler);
        
        if (filterIndex > 0) {
          parentEventHandler = postProcessEffects[filterIndex - 1].getRedrawEventHandler();
        }
        else {
          parentEventHandler = redrawEventHandler;
        }
        parentEventHandler.removeChildEventHandler(nodePrivate.getRedrawEventHandler());
        if (filterIndex < postProcessEffects.length) {
          parentEventHandler.appendChildEventHandler(postProcessEffects[filterIndex].getRedrawEventHandler());
        }
        else {
          parentEventHandler.appendChildEventHandler(propagationRedrawEventHandler);
        }
      });
    
    viewportNode.pub.rayCast = function(evt, options) {
      var result = {
        rayData: undefined
      };
      options = scene.assignDefaults(options, {
          returnOnlyClosestNode: true
        });
      if( raycastingEnabled ) {
        var elementCoords = getElementCoords(evt);
        viewPortRayCastDgNode.setData('x', elementCoords.x);
        viewPortRayCastDgNode.setData('y', elementCoords.y);
        var nodes = viewPortRaycastEvent.select();
        result.rayData = viewPortRayCastDgNode.getData('ray');

        if (options.returnOnlyClosestNode) {
          for (var i = 0; i < nodes.length; i++) {
            if (!result.closestNode || nodes[i].value.distance < result.closestNode.value.distance) {
              result.closestNode = nodes[i];
            }
          }
        }else {
          result.nodes = nodes;
        }
      }
      return result;
    };
    
    viewportNode.pub.calcRayFromMouseEvent = function(evt) {
      var elementCoords = getElementCoords(evt);
      viewPortRayCastDgNode.setData('x', elementCoords.x);
      viewPortRayCastDgNode.setData('y', elementCoords.y);
      viewPortRayCastDgNode.evaluate();
      var ray = viewPortRayCastDgNode.getData('ray');
      return ray;
    };
    
    viewportNode.pub.redraw = function(force) {
      if(!visible){
        return;
      }
      if(scene.isPlaying()){
        if(force){
          fabricwindow.needsRedraw();
        }
      }else{
        // If we give the browser a millisecond pause, then the redraw will
        // occur. Otherwist this message gets lost, causing a blank screen when
        // demos load. 
        setTimeout(function(){
          fabricwindow.needsRedraw();
        }, 1);
      }
    };
    
    viewportNode.pub.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      nodeData.camera = cameraNode.getName();
    };
    
    viewportNode.pub.readData = function(sceneDeserializer, nodeData) {
      if (nodeData.camera) {
        this.setCameraNode(sceneDeserializer.getNode(nodeData.camera));
      }
    };
    
    viewportNode.pub.getFPS = function() {
      // TODO: once we have support for multiple viewports, we should
      // re-write this function.
      return scene.getContext().VP.viewPort.getFPS();
    };
    
    if (options.enableRaycasting)
      viewportNode.pub.enableRaycasting();

    if (options.postProcessEffect) {
      viewportNode.pub.addPostProcessEffectShader(options.postProcessEffect);
    }

    if (options.enableMouseEvents) {
      ///////////////////////////////////////////////////////////////////
      // Add Mouse Handling Events
      var mouseOverNode;
      var mouseOverNodeData;
      var propagateEvent = true;
      var bindEventProperties = function(evt) {
        evt.mouseScreenPos  = getElementCoords(evt);
        evt.scene = scene.pub;
        evt.viewportNode = viewportNode.pub;
        if (cameraNode) {
          evt.cameraNode = cameraNode.pub;
        }
        propagateEvent = true;
        var stopPropagation = evt.stopPropagation;
        evt.stopPropagation = function() {
          propagateEvent = false;
          stopPropagation.call(evt);
        }
      }

      var fireEvent = function(name, evt, targetNode) {
        bindEventProperties(evt);
        if (cameraNode) {
          cameraNode.pub.fireEvent(name, evt);
        }
        if (propagateEvent) viewportNode.pub.fireEvent(name, evt);
        if (propagateEvent) scene.pub.fireEvent(name, evt);
      }

      // The mouse has interacted in some way with a geometry.
      // fire an event for objects to handle thier own interaction.
      var fireGeomEvent = function(name, evt, targetNode) {
        bindEventProperties(evt);
        evt.targetNode = targetNode.pub;
        evt.targetNode.fireEvent(name, evt);
        evt.viewportNode = viewportNode;
        if (propagateEvent) cameraNode.pub.fireEvent(name, evt);
        if (propagateEvent) viewportNode.pub.fireEvent(name, evt);
        if (propagateEvent) scene.pub.fireEvent(name, evt);
      }

      var mouseMoveTime;
      var mouseMoveFn = function(evt) {
        mouseMoveTime = (new Date).getTime();
        propagateEvent = true;
        if (cameraNode && viewPortRayCastDgNode && options.mouseMoveEvents) {
          var raycastResult = viewportNode.pub.rayCast(evt);
          if (raycastResult.closestNode) {
            var hitNode = raycastResult.closestNode.node.sceneGraphNode;
            evt.rayData = raycastResult.rayData;
            evt.hitData = raycastResult.closestNode.value;
            if (mouseOverNode == undefined ||
                mouseOverNode.pub.getName() !== hitNode.pub.getName()) {
              if (mouseOverNode) {
                evt.toNode = hitNode;
                evt.hitData = mouseOverNodeData;
                fireGeomEvent('mouseout_geom', evt, mouseOverNode);
              }
              evt.fromElement = evt.relatedTarget = mouseOverNode;
              fireGeomEvent('mouseover_geom', evt, hitNode);
              mouseOverNode = hitNode;
              mouseOverNodeData = evt.hitData;
            }else {
              fireGeomEvent('mousemove_geom', evt, hitNode);
            }
          }
          else {
            if (mouseOverNode) {
              evt.hitData = mouseOverNodeData;
              fireGeomEvent('mouseout_geom', evt, mouseOverNode);
              mouseOverNode = undefined;
            }
          }
        }
        if(propagateEvent){
          fireEvent('mousemove', evt);
        }
      };
      var mouseDownTime;
      var mouseDownFn = function(evt) {
        if(((new Date).getTime() - mouseDownTime) < 200){
          // generate a double-click if the mouse goes down 2x in less than 200ms.
          mouseDblClickFn(evt);
          return;
        }
        propagateEvent = true;
        if (cameraNode && viewPortRayCastDgNode) {
          var raycastResult = viewportNode.pub.rayCast(evt);
          if (raycastResult.closestNode) {
            var hitNode = raycastResult.closestNode.node.sceneGraphNode;
            evt.rayData = raycastResult.rayData;
            evt.hitData = raycastResult.closestNode.value;
            fireGeomEvent('mousedown_geom', evt, hitNode);
          }
        }
        if(propagateEvent){
          fireEvent('mousedown', evt);
        }
        mouseDownTime = (new Date).getTime();
      };

      var mouseUpFn = function(evt) {
        propagateEvent = true;
        if (cameraNode && viewPortRayCastDgNode && options.mouseUpEvents) {
          var raycastResult = viewportNode.pub.rayCast(evt);
          if (raycastResult.closestNode) {
            var hitNode = raycastResult.closestNode.node.sceneGraphNode;
            evt.rayData = raycastResult.rayData;
            evt.hitData = raycastResult;
            fireGeomEvent('mouseup_geom', evt, hitNode);
          }
        }
        if(propagateEvent){
          fireEvent('mouseup', evt);
        }
      };
      
      var mouseDblClickFn = function(evt) {
        propagateEvent = true;
        if (cameraNode && viewPortRayCastDgNode) {
          var raycastResult = viewportNode.pub.rayCast(evt);
          if (raycastResult.closestNode) {
            var hitNode = raycastResult.closestNode.node.sceneGraphNode;
            evt.rayData = raycastResult.rayData;
            evt.hitData = raycastResult.closestNode.value;
            fireGeomEvent('mousedblclick_geom', evt, hitNode);
          }
        }
      };


      // In cases wehre mouse events cost a lot, we can restrict firing to mouse down and moue up.
      windowElement.addEventListener('mousemove', mouseMoveFn, false);
      windowElement.addEventListener('mousedown', mouseDownFn, false);
      windowElement.addEventListener('mouseup', mouseUpFn, false);
      
      scene.pub.addEventListener('beginmanipulation', function(evt){
        // During manipulation we disable raycasting
        viewportNode.pub.disableRaycasting();
      });
      scene.pub.addEventListener('endmanipulation', function(evt){
        viewportNode.pub.enableRaycasting();
      });

      // Mouse Wheel event trapping.
      // Mouse wheel events are sent to the document, not the element,
      // so here we catch mouse wheel events only when the mouse goes over the element.
      // TODO: Fix Safari mouse wheel events..
      var mouseWheelActivated = false;
      var activateMousewheelFn = function(evt) {
        if(mouseWheelActivated){
          return;
        }
        var mousewheelFn = function(evt) {
          if(evt.detail) evt.wheelDelta = evt.detail * -50;
          fireEvent('mousewheel', evt);
        }
        document.addEventListener('mousewheel', mousewheelFn, false);
        document.addEventListener('DOMMouseScroll', mousewheelFn, false);
        var deactivateMousewheelFn = function(evt) {
          windowElement.removeEventListener('mouseout', deactivateMousewheelFn, false);
          document.removeEventListener('mousewheel', mousewheelFn, false);
          mouseWheelActivated = false;
        }
        windowElement.addEventListener('mouseout', deactivateMousewheelFn, false);
        mouseWheelActivated = true;
      }
      windowElement.addEventListener('mousemove', activateMousewheelFn, false);
      scene.addEventHandlingFunctions(viewportNode);
    }

    if (options.cameraNode) {
      viewportNode.pub.setCameraNode(options.cameraNode);
    }

    return viewportNode;
  }});

FABRIC.SceneGraph.registerNodeType('ResourceLoad', {
  briefDesc: 'The ResourceLoad node implements the loading of a resource from an URL.',
  detailedDesc: 'Based on is \'url\' member, the ResourceLoad node will asynchronously load the associated ' +
                'resource to its \'resource\' member. Until the data is loaded, resource.dataSize will be zero. ' +
                'Once the data is loaded, JS callbacks will be fired; you can register those by calling the ' +
                '\'addOnLoadSuccessCallback\' and \'addOnLoadFailureCallback\' member function. Unless ' + 
                '\'option.redrawOnLoad\' is set to false, the loading will automatically trigger a redraw. ' +
                'Note that operators can dynamically modify the URL.',
  parentNodeDesc: 'SceneGraphNode',
  optionsDesc: {
    blockRedrawingTillResourceIsLoaded: 'If set to true redrawing will be blocked until the resource is loaded.',
    redrawOnLoad: 'If set to true, the viewport will fire a redraw once the resource has been loaded.',
    storeDataAsFile: 'If set to true, the resource data will be stored as a file, with handle = resource.dataExternalLocation'
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      blockRedrawingTillResourceIsLoaded:true,
      redrawOnLoad: true,
      storeDataAsFile: false,
      url: undefined
    });
    
    var fileName = options.url.split('/').pop();
    var baseName = fileName.split('.')[0];
    if(!options.name){
      options.name = baseName;
    }

    var onloadSuccessCallbacks = [];
    var onloadProgressCallbacks = [];
    var onloadFailureCallbacks = [];
    var lastLoadCallbackURL = '';

    var resourceLoadNode = scene.constructNode('SceneGraphNode', options);
    var dgnode;
      //TODO: this is probably obselete with the new storeDataAsFile option???
    {
    dgnode = resourceLoadNode.constructResourceLoadNode('DGLoadNode');
    if( options.storeDataAsFile === true )
      dgnode.setData( 'storeDataAsFile', 0, true );
    }

    resourceLoadNode.addMemberInterface(dgnode, 'url');
    
    var remainingTaskWeight = 1.0;
    var incrementLoadProgressBar;
    if(options.blockRedrawingTillResourceIsLoaded && options.url && !options.localPath){
      incrementLoadProgressBar = FABRIC.addAsyncTask("Loading: "+ options.url, remainingTaskWeight);
    }
    
    var onLoadCallbackFunction = function(callbacks) {
      var i;
      for (i = 0; i < callbacks.length; i++) {
        callbacks[i](resourceLoadNode.pub);
      }
      callbacks.length = 0;
      
      if(incrementLoadProgressBar){
        incrementLoadProgressBar(true, -remainingTaskWeight);
        incrementLoadProgressBar = undefined;
      }
      else if (options.redrawOnLoad) {
        scene.pub.redrawAllViewports();
      }
    }
    
    var onLoadSuccessCallbackFunction = function(node) {
      lastLoadCallbackURL = resourceLoadNode.pub.getUrl();
      onLoadCallbackFunction(onloadSuccessCallbacks);
    }
    
    var onLoadProgressCallbackFunction = function(node, progress) {
      prevRemainingTaskWeight = remainingTaskWeight;
      //TaskWeight = 1 + size/100KB
      remainingTaskWeight = 1.0 + (progress.total - progress.received) / 100000;
      if(incrementLoadProgressBar)
        incrementLoadProgressBar(false, remainingTaskWeight-prevRemainingTaskWeight);

      for (var i = 0; i < onloadProgressCallbacks.length; i++) {
        onloadProgressCallbacks[i](resourceLoadNode.pub, progress);
      }
    }
    
    var onLoadFailureCallbackFunction = function(node) {
      onLoadCallbackFunction(onloadFailureCallbacks);
    }

    if(!options.localPath) {
      dgnode.addOnLoadSuccessCallback(onLoadSuccessCallbackFunction);
      dgnode.addOnLoadProgressCallback(onLoadProgressCallbackFunction);
      dgnode.addOnLoadFailureCallback(onLoadFailureCallbackFunction);
    }

    resourceLoadNode.pub.isLoaded = function() {
      return lastLoadCallbackURL !== '' && lastLoadCallbackURL === resourceLoadNode.pub.getUrl();
    }

    resourceLoadNode.pub.addOnLoadProgressCallback = function(callback) {
      //It is possible that a resourceLoadNode actually loads multiple resources in a sequence;
      //make sure the callback is only fired when the 'next' resource is loaded.
      if (resourceLoadNode.pub.isLoaded()) {
        callback.call();
      } else {
        onloadProgressCallbacks.push(callback);
      }
    };

    resourceLoadNode.pub.addOnLoadSuccessCallback = function(callback) {
      //It is possible that a resourceLoadNode actually loads multiple resources in a sequence;
      //make sure the callback is only fired when the 'next' resource is loaded.
      if (resourceLoadNode.pub.isLoaded()) {
        callback.call(); //Already loaded. Todo: we don't keep track of success/failure state, which is wrong.
      } else {
        onloadSuccessCallbacks.push(callback);
      }
    };

    resourceLoadNode.pub.addOnLoadFailureCallback = function(callback) {
      onloadFailureCallbacks.push(callback);
    };
    
    resourceLoadNode.pub.setUrl = function(url, forceLoad) {
      if(options.localPath) {
        dgnode.setData('url', 0, url);
        dgnode.bindings.append(scene.constructOperator({
          operatorName: 'loadStorageResource',
          parameterLayout: [
            'self.resource',
            'self.url'
          ],
          entryFunctionName: 'loadStorageResource',
          srcFile: 'FABRIC_ROOT/SceneGraph/KL/localStorage.kl',
          async: false
        }));
        dgnode.evaluate();
        return;
      } else {
        if(url !== '' && url !== dgnode.getData('url') && incrementLoadProgressBar === undefined && options.blockRedrawingTillResourceIsLoaded){
          incrementLoadProgressBar = FABRIC.addAsyncTask("Loading: "+ options.url, remainingTaskWeight);
        }
        dgnode.setData('url', 0, url);
      }
      if(forceLoad!= false){
        dgnode.evaluate();
      }
    };

    var parentWriteData = resourceLoadNode.writeData;
    resourceLoadNode.writeData = function(sceneSerializer, constructionOptions, nodeData) {
      constructionOptions.url = options.url;
      parentWriteData(sceneSerializer, constructionOptions, nodeData);
    };
    
    scene.addEventHandlingFunctions(resourceLoadNode);

    if (options.url) {
      // check if the url has a handle
      if(options.url.folderHandle) {
        dgnode.getResourceFromFile('resource', options.url);
      } else {
        resourceLoadNode.pub.setUrl(options.url);
      }
    }

    return resourceLoadNode;
  }});

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


