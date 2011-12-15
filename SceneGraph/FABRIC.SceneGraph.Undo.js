

//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * The UndoManager provides undo / redo functionality to the scene graph.
 */
FABRIC.SceneGraph.registerManagerType('UndoManager', {
  briefDesc: 'The UndoManager manages performed tasks.',
  detailedDesc: 'The UndoManager manages performed tasks and is abled to undo resp. redo them.',
  parentManagerDesc: '',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      maxTasks: 50
    });

    var blockTasks = false;
    var undoTasks = [];
    var redoTasks = [];
    
    var undoManager = {
      pub:{
        getTasks: function() {
          return undoTasks;
        }
      }
    };
    
    // add a task to the stack
    undoManager.pub.addTask = function(taskOptions) {
      if(blockTasks)
        return;
      scene.assignDefaults(taskOptions, {
        name: 'Task',
        onDo: undefined,
        onUndo: undefined,
        additionCallBacks: undefined
      });
      if(!taskOptions.onDo)
        throw('taskOptions.onDo is not specified!');
      if(!taskOptions.onUndo)
        throw('taskOptions.onUndo is not specified!');
        
      var task = {
        name: taskOptions.name,
        onDo: taskOptions.onDo,
        onUndo: taskOptions.onUndo
      };
      
      if(taskOptions.additionCallBacks) {
        for(var key in taskOptions.additionCallBacks)
          task[key] = taskOptions.additionCallBacks;
      }
      if(undoTasks.length >= options.maxTasks)
        undoTasks.splice(0,1 + undoTasks.length - options.maxTasks);
      undoTasks.push(task);
      redoTasks = [];
      undoManager.pub.fireEvent('taskAdded', { task: task });
      return task;
    };
    
    // add a task to the stack, and also do it
    undoManager.pub.doTask = function(taskOptions) {
      if(blockTasks)
        return;
      var task = undoManager.pub.addTask(taskOptions);
      blockTasks = true;
      task.onDo();
      blockTasks = false;
      undoManager.pub.fireEvent('taskDone',{task: task});
    }

    // undo a task from the stack
    undoManager.pub.undo = function() {
      if(undoTasks.length == 0)
        return;
      var task = undoTasks.pop();
      blockTasks = true;
      task.onUndo();
      blockTasks = false;
      redoTasks.push(task);
      undoManager.pub.fireEvent('taskUndone',{task: task});
    }

    // redo a task from the stack
    undoManager.pub.redo = function() {
      if(redoTasks.length == 0)
        return;
      var task = redoTasks.pop();
      blockTasks = true;
      task.onDo();
      blockTasks = false;
      undoTasks.push(task);
      undoManager.pub.fireEvent('taskRedone',{task: task});
    }

    // do a typical set data task
    undoManager.pub.setDataTask = function(taskOptions) {
      scene.assignDefaults(taskOptions, {
        node: undefined,
        member: undefined,
        value: undefined,
        name: undefined,
        getter: undefined,
        setter: undefined,
        prevValue: undefined,
        doTask: true
      });
      
      // check and extend the options
      if(taskOptions.node == undefined)
        throw('taskOptions.node is not specified!');
      if(taskOptions.member == undefined)
        throw('taskOptions.member is not specified!');
      if(taskOptions.value == undefined)
        throw('taskOptions.value is not specified!');
      var capitalizedMember = taskOptions.member[0].toUpperCase() + taskOptions.member.substr(1);
      if(!taskOptions.name)
        taskOptions.name = "set"+capitalizedMember;
      if(!taskOptions.getter)
        taskOptions.getter = "get"+capitalizedMember;
      if(!taskOptions.setter)
        taskOptions.setter = "set"+capitalizedMember;

      // get the elemental variables
      // and setup the onDo and onUndo functions
      var node = taskOptions.node;
      var value = taskOptions.value;
      var prevValue = taskOptions.prevValue;
      taskOptions.onDo = function() {
        if(prevValue == undefined)
          prevValue = node[taskOptions.getter]();
        node[taskOptions.setter](value);
      }
      taskOptions.onUndo = function() {
        node[taskOptions.setter](prevValue);
      }
      taskOptions.additionCallBacks = {
        getPrevValue: function () {
          return prevValue;
        },
        getValue: function () {
          return value;
        }
      };
      
      if(taskOptions.doTask) {
        // if we are blocking tasks, simply execute,
        // otherwise, stack it
        if(blockTasks)
          taskOptions.onDo();
        else
          undoManager.pub.doTask(taskOptions);
      } else if(!blockTasks) {
        undoManager.pub.addTask(taskOptions);
      }
    }

    scene.addEventHandlingFunctions(undoManager);
    
    return undoManager;
  }});
