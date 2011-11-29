

//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * The rselection manager provides selection functionality to the scene graph.
 * additional rendering pipeline and capabilities.
 */
FABRIC.SceneGraph.registerManagerType('SelectionManager', {
  briefDesc: 'The SelectionManager manages selections of objects.',
  detailedDesc: '.',
  parentManagerDesc: '',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      undoManager: undefined
    });

    var selection = [];
    var listeners = [];
    
    var selectionManager = {
      pub:{
        isSelected:function( obj ){ 
          return selection.indexOf(obj) != -1;
        },
        select:function( obj ){
          var prevSelection = selectionManager.pub.getSelectionCopy();
          this.clearSelection(false);
          if(obj.constructor.name == 'Array'){
            for(var i=0; i<obj.length; i++){
              this.addToSelection(obj[i], false);
            }
            selectionManager.pub.fireEvent('selectionChanged', {
              selection: selectionManager.pub.getSelectionCopy(),
              prevSelection: prevSelection
            });
          }
          else{
            this.addToSelection(obj, false);
            selectionManager.pub.fireEvent('selectionChanged', {
              selection: selectionManager.pub.getSelectionCopy(),
              prevSelection: prevSelection
            });
          }
        },
        addToSelection:function(obj, fireevents){
          if(selection.indexOf(obj)!=-1)
            return;
          var prevSelection = selectionManager.pub.getSelectionCopy();
          selection.push(obj);
          if(obj.fireEvent){
            obj.fireEvent('selected');
          }
          if(fireevents!=false){
            selectionManager.pub.fireEvent('selectionChanged', {
              selection: selectionManager.pub.getSelectionCopy(),
              prevSelection: prevSelection
            });
          }
        },
        removeFromSelection:function( obj, firevents ){
          if(selection.length == 0)
            return;
          var id = selection.indexOf(obj);
          if(id != -1){
            var prevSelection = selectionManager.pub.getSelectionCopy();

            // Put the color back. 
            if(obj.fireEvent){
              obj.fireEvent('deselected');
            }
            // Remove from the seleciton array.
            if( selection.length == 1 ){
              selection = []
            }else{
              selection.splice(id,1);
            }
            if(firevents!=false){
              selectionManager.pub.fireEvent('selectionChanged', {
                selection: selectionManager.pub.getSelectionCopy(),
                prevSelection: prevSelection
              });
            }
          }
        },
        clearSelection:function(firevents){ 
          if(selection.length > 0){
            var prevSelection = selectionManager.pub.getSelectionCopy();
            for (var i = selection.length-1; i >=0 ; i--) {
              this.removeFromSelection(selection[i], false);
            }
            if(firevents!=false){
              selectionManager.pub.fireEvent('selectionChanged', {
                selection: selectionManager.pub.getSelectionCopy(),
                prevSelection: prevSelection
              });
            }
          }
        },
        getSelection: function(){
          return selection;
        },
        getSelectionCopy: function(){
          var result = [];
          for(var i=0;i<selection.length;i++)
            result.push(selection[i]);
          return result;
        },
        addListener: function(listener){
          if(!listener.fireEvent){
            throw "listener must support events";
          }
          listeners.push(listener);
        }
      }
    }
    scene.addEventHandlingFunctions(selectionManager);

    // deal with undo
    var undoManager = options.undoManager;
    selectionManager.pub.getUndoManager = function() {
      return undoManager;
    };
    if(undoManager) {
      selectionManager.pub.addEventListener('selectionChanged', function(evt){
        undoManager.addTask({
          name: 'SelectionChanged',
          onDo: function(){
            selectionManager.pub.select(evt.selection);
          },
          onUndo: function(){
            selectionManager.pub.select(evt.prevSelection);
          }
        });
      });
    }
    
    return selectionManager;
  }});



FABRIC.SceneGraph.registerManagerType('ViewportSelectionManager', {
  briefDesc: 'The ViewportSelectorManager manages selections of objects view mouse events from the viewport.',
  detailedDesc: '.',
  parentManagerDesc: '',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      addToSelectionKeyName: 'ctrlKey',
      removeFromSelectionKeyName: 'altKey',
      enabled: true
    });
    
    var selectionManager = scene.constructManager('SelectionManager', options);
    var enabled = options.enabled;
    selectionManager.pub.setEnabled = function(val) {
      enabled = val;
    };
    
    scene.pub.addEventListener('mouseover_geom', function(evt){
      if(!enabled)
        return;
      if(evt.targetNode.fireEvent){
        evt.targetNode.fireEvent('highlight');
        scene.pub.redrawAllViewports();
      }
    });
    scene.pub.addEventListener('mouseout_geom', function(evt){
      if(!enabled)
        return;
      if(evt.targetNode.fireEvent){
        evt.targetNode.fireEvent('unhighlight');
        scene.pub.redrawAllViewports();
      }
    });
    scene.pub.addEventListener('mousedown_geom', function(evt){
      if(!enabled)
        return;
      if(evt[options.addToSelectionKeyName]){
        selectionManager.pub.addToSelection(evt.targetNode);
      }else if(evt[options.removeFromSelectionKeyName]){
        selectionManager.pub.removeFromSelection(evt.targetNode);
      }else{
        selectionManager.pub.select(evt.targetNode);
        evt.stopPropagation();
        scene.pub.redrawAllViewports();
      }
    });
    var mouseDownScreenPos;
    scene.pub.addEventListener('mousedown', function(evt){
      if(!enabled)
        return;
      if(!evt.targetNode){
        mouseDownScreenPos = evt.mouseScreenPos;
      }
    });
    scene.pub.addEventListener('mouseup', function(evt){
      if(!enabled)
        return;
      if(mouseDownScreenPos && evt.mouseScreenPos.subtract(mouseDownScreenPos).length() < 0.01){
        selectionManager.pub.clearSelection();
        scene.pub.redrawAllViewports();
        mouseDownScreenPos = undefined;
      }
    });
    
    return selectionManager;
  }});


/**
 * The rselection manager provides selection functionality to the scene graph.
 * additional rendering pipeline and capabilities.
 */
FABRIC.SceneGraph.registerManagerType('SelectionManipulationManager', {
  briefDesc: 'The SelectionManipulationManager manages manipulatoin of selected of objects.',
  detailedDesc: '.',
  parentManagerDesc: '',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      selectionManager: undefined,
      manipulators: undefined,
      transformGetter: 'getGlobalXfo',
      transformSetter: 'setGlobalXfo',
      screenTranslationRadius: 2.0,
      });
    
    var selectionManager = options.selectionManager;
    if(!selectionManager)
      throw("options.selectionManager is not defined!");
    
    // Construct a 'Transform' node that is used as the parent of the Maipulators.
    // This transform node aligns its self with the average of the selected objects.
    // an event is fired on the node when it is manipulated, and it uses this
    // event to manipulate all the selected objects.
    var groupTransform = scene.constructNode('Transform', {
      name: 'SelectionManipulationManagerGroupTransform',
      hierarchical: false
    } );

    /////
    // Propagate the manipulation to all selected members.
    var dragStartGlobalXfo, dragStartSelGlobalXfos;
    var dragStartFn = function(evt) {
      dragStartGlobalXfo = groupTransform.pub.getGlobalXfo();
      var selection = selectionManager.getSelection();
      dragStartSelGlobalXfos = [];
      for(var i=0; i<selection.length; i++){
        dragStartSelGlobalXfos.push(selection[i].getTransformNode()[options.transformGetter]());
      }
    }
    var dragFn = function(evt) {
    }
    var dragEndFn = function(evt) {
      
      // if we support undo, let's create a task for this
      if(selectionManager.getUndoManager()) {
        
        // store previous and current transforms into arrays
        var selection = selectionManager.getSelectionCopy()
        var previousTransforms = [];
        var currentTransforms = [];
        for(var i=0;i<dragStartSelGlobalXfos.length;i++) {
          previousTransforms.push(dragStartSelGlobalXfos[i].clone());
          currentTransforms.push(selection[i].getTransformNode()[options.transformGetter]());
        }
        selectionManager.getUndoManager().addTask({
          name: "ViewPortManipulationManager",
          onUndo: function (){
            for(var i=0;i<selection.length;i++)
              selection[i].getTransformNode()[options.transformSetter](previousTransforms[i]);
          },
          onDo: function (){
            for(var i=0;i<selection.length;i++)
              selection[i].getTransformNode()[options.transformSetter](currentTransforms[i]);
          },
        });
      }
    }
    
    groupTransform.pub.setGlobalXfo = function(xfo){
      var xform = xfo.multiply(dragStartGlobalXfo.inverse());
      var selection = selectionManager.getSelection();
      for(var i=0; i<selection.length; i++){
        selection[i].getTransformNode()[options.transformSetter]( xform.multiply(dragStartSelGlobalXfos[i]) );
      }
      selectionManager.fireEvent('groupTransformChanged',{selection: selection});
    };

    // Create a list of known operators for each selection count    
    var operatorAssigned = false;
    var generatedOperators = {};
    var generateTransformOperator = function(selection){
      if(generatedOperators[selection.length]){
        return generatedOperators[selection.length];
      }
      var parameterLayout = [
        'self.globalXfo'
      ]
      var srcCodeHeader = 'use Xfo, Mat44;\noperator calcAverageXfo( \n  io Xfo xfo';
      var srcCodeBody = '\n){ \n  xfo.setIdentity();\n  Scalar weight = 1.0/'+selection.length+'.0;\n';
      for(var i=0; i<selection.length; i++){
        srcCodeHeader += ',\n  io Xfo obj' + i;
        srcCodeBody  += '  xfo.tr += obj' + i + '.tr * weight;\n';
        if(selection.length==1){
          srcCodeBody  += '  xfo.ori = obj' + i + '.ori;\n';
        }
        parameterLayout.push('obj' + i +'.globalXfo');
      }
      srcCodeBody += '  \n}';
      
      var operatorName = 'calcAverageXfo' + selection.length;
      generatedOperators[selection.length] = scene.constructOperator( {
          operatorName: operatorName,
          srcCode: (srcCodeHeader + srcCodeBody),
          parameterLayout: parameterLayout,
          entryFunctionName: 'calcAverageXfo',
          async: false
        });
      
      return generatedOperators[selection.length];
    }

    // retrieve the list of manipulators    
    var manipulators = options.manipulators;
    var manipulatorsDrawToggle = false;
    var toggleManipulatorsDisplay = function(drawToggle){
      if(!manipulators)
        return;
      manipulatorsDrawToggle = drawToggle;
      for(var i=0; i<manipulators.length; i++){
        manipulators[i].setDrawToggle(drawToggle);
      }
    }

    var prevSelectionCount = 0;
    selectionManager.addEventListener('selectionChanged', function(evt){
      if(operatorAssigned){
        groupTransform.getDGNode().bindings.remove(0);
        operatorAssigned = false;
      }
      if(evt.selection.length==0){
        toggleManipulatorsDisplay(false);
      }
      else{
        toggleManipulatorsDisplay(true);
        
        for(var i=0; i<evt.selection.length; i++){
          var obj = scene.getPrivateInterface(evt.selection[i].getTransformNode());
          groupTransform.getDGNode().setDependency(obj.getDGNode(), 'obj'+i);
        }
        for(var i=evt.selection.length; i<prevSelectionCount; i++){
          // TODO: uncomment this once we have removeDependency
        //  groupTransform.getDGNode().removeDependency('obj'+i);
        }
        groupTransform.getDGNode().bindings.append(generateTransformOperator(evt.selection));
        prevSelectionCount = evt.selection.length;
        operatorAssigned = true;
      }
    });

    if(!manipulators){
      var manipulator = scene.pub.constructNode('ScreenTranslationManipulator', {
          name: options.name + 'ScreenTranslationManipulator',
          color: FABRIC.RT.rgb(0, 0.8, 0, 1),
          parentNode: groupTransform.pub,
          geometryNode: scene.pub.constructNode('Sphere', {radius: options.screenTranslationRadius, detail: 8.0}),
          drawToggle: false,
          drawOverlaid: true,
        });
      manipulator.addEventListener('dragstart', dragStartFn);
      manipulator.addEventListener('drag', dragFn);
      manipulator.addEventListener('dragend', dragEndFn);
      manipulators = [];
      manipulators.push(manipulator);
    } else {
      for(var i=0;i<manipulators.length;i++) {
        manipulators[i].addEventListener('dragstart', dragStartFn);
        manipulators[i].addEventListener('drag', dragFn);
        manipulators[i].addEventListener('dragend', dragEndFn);
      }
    }
    
    // setup the public interface
    var selectionManipulationManager = {
      pub:{
        getTransformNode : function() {
          return groupTransform.pub;
        },
        addManipulator: function(manipulator) {
          manipulator.addEventListener('dragstart', dragStartFn);
          manipulator.addEventListener('drag', dragFn);
          manipulator.addEventListener('dragend', dragEndFn);
          manipulator.setDrawToggle(manipulatorsDrawToggle);
          manipulators.push(manipulator);
        },
        toggleManipulatorsDisplay: function(value) {
          toggleManipulatorsDisplay(value);
        },
        getManipulatorsDrawToggle: function() {
          return manipulatorsDrawToggle;
        }
      }
    };
    toggleManipulatorsDisplay(false);
    
    selectionManipulationManager.pub.getSelectionManager = function(){
      return selectionManager;
    };
    
    return selectionManipulationManager;
  }});

FABRIC.SceneGraph.registerNodeType('SelectableInstance', {
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      highlightMaterial: undefined,
      selectMaterial: undefined,
      removeMainMaterial: false
    });
    
    var selectableInstance = scene.constructNode('Instance', options);
    var mainMaterialNode = options.materialNode;
    options.removeMainMaterial = mainMaterialNode ? options.removeMainMaterial : false;
    
    var selectable = true;
    selectableInstance.pub.isSelectable = function(){
      return selectable;
    }
    selectableInstance.pub.setSelectable = function(val){
      selectable = val;
    }
    var highlighted = false;
    var selected = false;
    var removedMainMaterial = undefined;
    selectableInstance.pub.addEventListener('highlight', function(evt) {
      if(!selected && selectable){
        if(options.removeMainMaterial) {
          selectableInstance.pub.removeMaterialNode( mainMaterialNode );
          removedMainMaterial = true;
        }
        selectableInstance.pub.setMaterialNode( options.highlightMaterial );
        highlighted = true;
      }
    });
    selectableInstance.pub.addEventListener('unhighlight', function(evt) {
      if(highlighted){
        selectableInstance.pub.removeMaterialNode( options.highlightMaterial );
        if(removedMainMaterial) {
          selectableInstance.pub.setMaterialNode( mainMaterialNode );
          removedMainMaterial = undefined;
        }
        highlighted = false;
      }
    });
    selectableInstance.pub.addEventListener('selected', function(evt) {
      if(!selectable)
        return;
      if(highlighted){
        selectableInstance.pub.removeMaterialNode( options.highlightMaterial );
        highlighted = false;
      } else if(options.removeMainMaterial) {
        selectableInstance.pub.removeMaterialNode( mainMaterialNode );
        removedMainMaterial = true;
      }
      selectableInstance.pub.setMaterialNode( options.selectMaterial );
      selected = true;
    });
    selectableInstance.pub.addEventListener('deselected', function(evt) {
      selectableInstance.pub.removeMaterialNode( options.selectMaterial );
      if(removedMainMaterial) {
        selectableInstance.pub.setMaterialNode( mainMaterialNode );
        removedMainMaterial = undefined;
      }
      selected = false;
    });
    return selectableInstance;
  }});
