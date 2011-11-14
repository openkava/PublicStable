
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//
FABRIC.SceneGraph.registerNodeType('WebSocketNode', {
  briefDesc: 'The WebSocketNode wraps the HTML5 websocket functionality.',
  detailedDesc: 'The WebSocketNode wraps the HTML5 websocket functionality. It provides access to send and message callbacks.',
  optionsDesc: {
  },
  factoryFn: function(options, scene) {
    scene.assignDefaults(options, {
      serverUrl: undefined,
      serverPort: 8123,
      sendFunctionNames: undefined,
      messageCallBacks: undefined,
      contextID: scene.pub.getContextId()
    });

    var node = scene.constructNode('SceneGraphNode', {name: 'WebSocketNode'});

    // check if we have a serverUrl
    if(options.serverUrl == undefined)
      throw('The serverUrl options HAS to be specified.');
    var serverUrl = 'ws://'+options.serverUrl+':'+options.serverPort;
    
    // setup the web socket
    var messageCallBacks = {};
    var context = options.contextID;
    var connections = {};
    var conn = undefined;
    var isConnected = false;
    if (window["WebSocket"]) {
      conn = new WebSocket(serverUrl);

      conn.onmessage = function(evt) {
        message = JSON.parse(evt.data);
        if(message.context == undefined)
          return;
        if(message.context != context)
          return;
        
        // check if this message contains data
        if(message.data == undefined)
          returnl
        
        // check if we have a callback for this
        if(messageCallBacks[message.action]) {
          messageCallBacks[message.action](message);
        }
        else
          console.log('WebSocket: Received unhandled action message "'+message.action+'"');
      };
      
      // send a message with our context
      conn.onopen = function() {
        conn.send(JSON.stringify({
        'action': 'setContext',
        'context': context
        }));
        isConnected = true;
        
        // if we have an on opencallback
        if(options.onOpenCallBack) {
          options.onOpenCallBack();
        }
      }
    } else {
      throw("This browser doesn't support WebSockets natively.");
    }
  
    // take care of all of the sender functions
    var sendFunctions = {};
    node.pub.addSendFunction = function(actionName)  {
      var actionNameKey = actionName.toUpperCase();
      if(sendFunctions[actionNameKey] != undefined)
        throw('sendFunction "'+actionName+'" already defined.!');
      
      var functionName = 'send'+actionName.substr(0,1).toUpperCase()+actionName.substr(1,10000);
      node.pub[functionName] = function(data,id) {
        if(!isConnected)
          return;
        var message = {};
        message.targetID = id == undefined ? '*' : id;
        message.context = context;
        message.action = actionName;
        message.data = data;
        conn.send(JSON.stringify(message));
      }
      sendFunctions[actionNameKey] = functionName;
    };
    
    node.pub.addMessageCallBack = function(actionName,callBack) {
      if(messageCallBacks[actionName] != undefined)
        throw('messageCallBack "'+actionName+'" already defined.!');
      messageCallBacks[actionName] = callBack;
    };
    
    if(options.sendFunctionNames) {
      for(var i=0; i<options.sendFunctionNames.length; i++)
        node.pub.addSendFunction(options.sendFunctionNames[i]);
    }
    if(options.messageCallBacks) {
      for(var actionName in options.messageCallBacks)
        node.pub.addMessageCallBack(name,options.messageCallBacks[name]);
    }
    
    return node;
  }
});
