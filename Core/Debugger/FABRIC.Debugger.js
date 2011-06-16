
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

$(document).ready(function() {
  
  var contextID = document.location.search.split('?id=')[1];
  var context = FABRIC.createContext({ contextID:contextID  });

  // Horizontal splitter, nested in the right pane of the vertical splitter.
  $("#LeftPane").splitter({
    splitHorizontal: true,
    resizeTo: window,
    cookie: "FABRIC.Debugger.HorizontalSplit",
  });
  // Vertical splitter. Set min/max/starting sizes for the left pane.
  $("#MySplitter").splitter({
        splitVertical: true,
        outline: true,
        resizeTo: window,
    cookie: "FABRIC.Debugger.VerticalSplit"
    });
  
  // init the lined text area
  $(".lined").linedtextarea();

  // decide if we should use cookies to restore settings
  // for now we are only doing this if the context is the
  // same as before
  var oldContext = $.cookie("FABRIC.Debugger.ContextID");
  var useCookies = true;
  if(oldContext){
    useCookies = (context.contextID == oldContext);
  }
  $.cookie("FABRIC.Debugger.ContextID",context.contextID);
  
  
  var allNamedObjects = context.DG.getAllNamedObjects();
  
  
  var eventsMap = {};
  var eventHandlersMap = {};
  var nodesMap = {};
  for(var i in allNamedObjects){
    if(!allNamedObjects[ i ].getType){
      continue;
    }
    var type = allNamedObjects[ i ].getType();
    if(type === "Event"){
      eventsMap[i] = allNamedObjects[ i ];
    }
    else if(type === "EventHandler"){
      eventHandlersMap[i] = allNamedObjects[ i ];
    }
    if(type === "Node"){
      nodesMap[i] = allNamedObjects[ i ];
    }
  }

    //------------------------------------------------------------
    // Events
  var events = [];
  for(var i in eventsMap){
    events.push(eventsMap[ i ]);
  }
  
  var filteredEvents = [];
  var eventName = [];
  var eventNameToID = {};
  for(var i=0;i<events.length;i++){
    if(eventNameToID[events[i].getName()] == undefined){
      eventName.push(events[i].getName());
      eventNameToID[events[i].getName()] = filteredEvents.length;
      filteredEvents.push(events[i]);
    }
  }
  events = filteredEvents;
    var getEvents = function(){
        return events;
    }
    var getEvent = function(eventID){
        return getEvents()[eventID];
    }
  
    //------------------------------------------------------------
    // Event Handlers
  var eventHandlers = [];
  for(var i in eventHandlersMap){
    eventHandlers.push(eventHandlersMap[ i ]);
  }
  
  var filteredEventHandlers = [];
  var eventHandlerName = [];
  var eventHandlerNameToID = {};
  for(var i=0;i<eventHandlers.length;i++){
    if(eventHandlerNameToID[eventHandlers[i].getName()] == undefined){
      eventHandlerName.push(eventHandlers[i].getName());
      eventHandlerNameToID[eventHandlers[i].getName()] = filteredEventHandlers.length;
      filteredEventHandlers.push(eventHandlers[i]);
    }
  }
  eventHandlers = filteredEventHandlers;
  
  // let's sort the event handlers
  // for this I will base it on the event handler order
  var eventHandlerDone = {};
  var parentEventHandlers = {};
/*  for(var i=0;i<eventHandlerName.length;i++){
    eventHandlerDone[eventHandlerName[i]] = false;
    var result = [];
    var parents = eventHandlers[i].getParentEventHandlers();
    for(var j=0;j<parents.length;j++){
      result.push(parents[j].name);
    }
    parentEventHandlers[eventHandlerName[i]] = result;
  }
*/
  for(var i=0;i<eventHandlerName.length;i++){
    eventHandlerDone[eventHandlerName[i]] = false;
    var children = eventHandlers[i].getChildEventHandlers();
    for(var j=0;j<children.length;j++){
      var childName = children[j].getName();
      if(!parentEventHandlers[ childName ]){
        parentEventHandlers[ childName ] = [];
      }
      parentEventHandlers[ childName ].push(eventHandlers[i].getName());
    }
  }


  var sortedEventHandlers = [];
  var sortedEventHandlerName = [];
  while(sortedEventHandlers.length < filteredEventHandlers.length){
    // find the next event handler that has either NO parent
    // or all of them are used
    for(var i=0;i<filteredEventHandlers.length;i++){
      if(eventHandlerDone[eventHandlerName[i]]){
        continue;
      }
      var parents = parentEventHandlers[eventHandlerName[i]];
      var useIt = (!parents || parents.length == 0);
      if(!useIt){
        var foundOnlyDone = true;
        for(var j=0;j<parents.length;j++){
          if(!eventHandlerDone[parents[j]]){
            foundOnlyDone = false;
            break;
          }
        }
        useIt = foundOnlyDone;
      }
      if(useIt){
        eventHandlerNameToID[eventHandlerName[i]] = sortedEventHandlers.length;
        sortedEventHandlers.push(filteredEventHandlers[i]);
        sortedEventHandlerName.push(eventHandlerName[i]);
        eventHandlerDone[eventHandlerName[i]] = true;
        break;
      }
    }
  }
  eventHandlers = sortedEventHandlers;
  eventHandlerName = sortedEventHandlerName;
  
  // cache the bindings
  var eventHandlerBindings = {};
  for(var i=0;i<eventHandlers.length;i++){
    var bindings = eventHandlers[i].getScopes();
    var result = [];
    for(var bindingName in bindings){
      result.push(bindings[bindingName].getName());
    }
    eventHandlerBindings[eventHandlers[i].getName()] = result;
  }
  
    var getEventHandlers = function(){
        return eventHandlers;
    }
    var getEventHandler = function(eventHandlerID){
        return getEventHandlers()[eventHandlerID];
    }

    var getParentEventHandler = function(name){
        return parentEventHandlers[ name ] ? parentEventHandlers[ name ] : [];
    }

    //------------------------------------------------------------
  // NODES
    var nodes = [];
  for(var i in nodesMap){
    nodes.push(nodesMap[ i ]);
  }
  
  var filteredNodes = [];
  var nodeName = [];
  var nodeNameToID = {};
  for(var i=0;i<nodes.length;i++){
    if(nodeNameToID[nodes[i].getName()] == undefined){
      nodeName.push(nodes[i].getName());
      nodeNameToID[nodes[i].getName()] = filteredNodes.length;
      filteredNodes.push(nodes[i]);
    }
  }
  nodes = filteredNodes;
    var nodeDepths = [];
  var nodeRow = [];
  for(var i=0;i<filteredNodes.length;i++)
    nodeRow.push(0);
    var nodeCollumns = [];
  
  var nodeDependencies = [];
  var nodeDependents = [];
  for(var i=0;i<nodes.length;i++){
    var node = nodes[i];
    var dependencies = node.getDependencies();
    var results = [];
    for(var j in dependencies){
      results.push(nodeNameToID[dependencies[j].getName()]);
      
      var childID = nodeNameToID[ dependencies[j].getName() ]
      if(!nodeDependents[ childID ]){
        nodeDependents[ childID ] = [];
      }
      nodeDependents[ childID ].push(i);
      
    }
    nodeDependencies.push(results);
    
  //  var dependents = node.getDependents();
  //  results = [];
  //  for(var j in dependents){
  //    results.push(nodeNameToID[dependents[j].getName()]);
  //  }
  //  nodeDependents.push(results);
  }
  
    var getNodeDependencies = function(nodeID){
    return nodeDependencies[nodeID] ?  nodeDependencies[nodeID] : [];
    }
    var getNodeDependents = function(nodeID){
    return nodeDependents[nodeID] ?  nodeDependents[nodeID] : [];
    }
  
  // TODO
  // furthermore we should use the fabric object without
  // a window, as stated by peter in the email. I should
  // save the content to a text file on /development.

    //------------------------------------------------------------
    // Prepare data for the UI
    var setNodeGraphDepth = function(nodeID,depth){
        var node = nodes[nodeID];
        if(nodeDepths[nodeID] < depth){
            nodeDepths[nodeID] = depth;
    }
    var name = nodeName[nodeID];
        
        // now call this for all dependent nodes
        var dependencies = getNodeDependencies(nodeID);
        for(var i=0;i<dependencies.length;i++){
            setNodeGraphDepth(dependencies[i],nodeDepths[nodeID]+1);
        }
    }
    
    //------------------------------------------------------------
    // Execute the Setup Code
    while(nodeDepths.length<nodes.length){
        nodeDepths.push(0);
  }
    var names = "";
    // first init the depth to 2 (left of this are events and handlers)
    for(var i=0;i<nodes.length;i++){
        setNodeGraphDepth(i,2);
  }
    // now init the collumns
    for(var i=0;i<nodes.length;i++){
    while(nodeCollumns.length<=nodeDepths[i]){
      nodeCollumns.push([]);
    }
    nodeRow[i] = nodeCollumns[nodeDepths[i]].length;
        nodeCollumns[nodeDepths[i]].push(i);
    }
  
  // now, let's sort the first collumn (based on the event handlers!)
  if(nodeCollumns.length > 2){
    // let's have a look at the first row,
    // and use a bubble sort to move the nodes up and down
    // based on the index of the eventhandler
    var sortedNodes = [];
    for(var i=0;i<nodeCollumns[2].length;i++){
      var entry = {};
      entry.i = i;
      entry.node = nodeCollumns[2][i];
      entry.name = nodeName[entry.node];
      entry.id = -1;
      for(var j=0;j<eventHandlerName.length;j++){
        var bindings = eventHandlerBindings[eventHandlerName[j]];
        for(var k=0;k<bindings.length;k++){
          if(bindings[k] == entry.name){
            entry.id = j;
            break;
          }
        }
        if(entry.id != -1){
          break;
        }
      }
      sortedNodes.push(entry);
    }
    sortedNodes.sort(function(a,b){
      return a.id - b.id;
    });
    // now output the data again to the nodeRow and collumn
    for(var i=0;i<sortedNodes.length;i++){
      nodeRow[sortedNodes[i].node] = i;
      nodeCollumns[2][i] = sortedNodes[i].node;
    }
  }
  
  // for all of the other collumns
  for(var k=3;k<nodeCollumns.length;k++){
    // let's have a look at the first row,
    // and use a bubble sort to move the nodes up and down
    // based on the index of the eventhandler
    var sortedNodes = [];
    for(var i=0;i<nodeCollumns[k].length;i++){
      var entry = {};
      entry.i = i;
      entry.node = nodeCollumns[k][i];
      entry.name = nodeName[entry.node];
      entry.id = 10000;
      var deps = getNodeDependents(entry.node);
      for(var j=0;j<deps.length;j++){
        var id = nodeRow[deps[j]];
        if(id < entry.id)
          entry.id = id;
      }
      sortedNodes.push(entry);
    }
    sortedNodes.sort(function(a,b){
      return a.id - b.id;
    });
    // now output the data again to the nodeRow and collumn
    for(var i=0;i<sortedNodes.length;i++){
      nodeRow[sortedNodes[i].node] = i;
      nodeCollumns[k][i] = sortedNodes[i].node;
    }
  }
  
  // create the SVG root
  var svgRoot = FABRIC.createSVGRootElem("TopPane");
  var graphCenterGroup = svgRoot.createGroup().id("graphCenterGroup");
  var graphSclGroup = graphCenterGroup.createGroup().id("graphSclGroup");//.scale(controller.getGraphDefZoom());
  var graphPosGroup = graphSclGroup.createGroup().id("graphPosGroup");//.translate(controller.getGraphDefPan());
  var graphHolderGroup = graphPosGroup.createGroup().id("graphHolderGroup");
  svgRoot.edgeHolderGroup = graphHolderGroup.createGroup().id("edgeHolderGroup");
  svgRoot.nodeHolderGroup = graphHolderGroup.createGroup().id("nodeHolderGroup");
  svgRoot.svgRoot = svgRoot;
  svgRoot.state = "Normal";
  svgRoot
    .draggable({mouseButton:0, delegateTranslateObj:graphPosGroup, highlight:false })
    .addOnDragEndCallback(
      function(evt){
        try{
          // store the graph pos in a cookie
          var pos = graphPosGroup.translate();
          $.cookie("FABRIC.Debugger.TopPane.Translate", pos.x + ":" + pos.y, { expires: 7 });
        }catch(e){}
      })
    .zoomable({delegateZoomObj:graphSclGroup })
    .addOnZoomCallback(
      function(evt){
        // store the graph zoom in a cookie
        $.cookie("FABRIC.Debugger.TopPane.Zoom", evt.zoomValue, { expires: 7 });
      });
    
  // members, for the UI
  var UIMembers = {};
  UIMembers.nodeKind = "";
  UIMembers.nodeName = "";
  UIMembers.operatorCookie = true;
  
  var clearAllViews = function(){
    $("#OperatorDropDown").html("").attr("disabled",true);
    $("#OperatorEntryFunction").attr("value","").attr("disabled",true);
    $("#OperatorParameterLayout").html("");
    $("#OperatorCode").html("").attr("disabled",true);
    $("#RightPaneTitle").html("");
    $("#NodeName").html("").attr("disabled",true);
    $("#NodeListATitle").html("");
    $("#NodeListA").html("").attr("disabled",true);
    $("#NodeListAButtons").html("").attr("disabled",true);
    $("#NodeListBTitle").html("");
    $("#NodeListB").html("").attr("disabled",true);
    $("#NodeListBButtons").html("").attr("disabled",true);
    $("#NodeListCTitle").html("");
    $("#NodeListC").html("").attr("disabled",true);
    $("#NodeListCButtons").html("").attr("disabled",true);
    $("#NodeListDTitle").html("");
    $("#NodeListD").html("").attr("disabled",true);
    $("#NodeListDButtons").html("").attr("disabled",true);
  }
  
  var onNodeSelected = function(nodeID){
    var node = nodes[nodeID];
    if(!node){
      return;
    }
    UIMembers.nodeKind = "DependencyNode";
    UIMembers.nodeName = node.getName();
    UIMembers.listData = {};
    UIMembers.listData.members = [];
    for(var memberName in node.getMembers()){
      UIMembers.listData.members.push(memberName);
    }
    UIMembers.listData.bindings = node.bindings;
    UIMembers.operatorName = UIMembers.listData.bindings.length > 0 ? UIMembers.listData.bindings.get(0).getOperator().getName() : "";
    
    // store all relevant data in cookies
    $.cookie("FABRIC.Debugger.UIMembers.nodeKind",UIMembers.nodeKind,{ expires: 7 });
    $.cookie("FABRIC.Debugger.UIMembers.nodeName",UIMembers.nodeName,{ expires: 7 });
    
    // now put the data into fields
    $("#RightPaneTitle").html("Dependency Node");
    $("#NodeName").attr("value",UIMembers.nodeName);
    
    $("#NodeListATitle").html("Members");
    var html = "";
    for(var i=0;i<UIMembers.listData.members.length;i++){
      html += "<li>"+UIMembers.listData.members[i]+"</li>";
    }
    $("#NodeListA").html(html);

    // populate the second list, should be the depends!
    $("#NodeListBTitle").html("Outputs");
    var html = "";
    var deps = getNodeDependents(nodeID);
    for(var i=0;i<deps.length;i++){
      html += "<li><a href=\"javascript:$.cookie('FABRIC.Debugger.UIMembers.nodeName','"+nodeName[deps[i]]+"');history.go(0);\">"+nodeName[deps[i]]+"</a></li>";
    }
    $("#NodeListB").html(html);
    
    // populate the third list, should be the dependencies!
    $("#NodeListCTitle").html("Inputs");
    var html = "";
    var deps = getNodeDependencies(nodeID);
    for(var i=0;i<deps.length;i++){
      if(deps[i]!=undefined){
        html += "<li><a href=\"javascript:$.cookie('FABRIC.Debugger.UIMembers.nodeName','"+nodeName[deps[i]]+"');history.go(0);\">"+nodeName[deps[i]]+"</a></li>";
      }
    }
    $("#NodeListC").html(html);

    // populate the fouth list, should be the dependencies!
    $("#NodeListDTitle").html("Operators");
    var html = "";
    for(var i=0;i<UIMembers.listData.bindings.length;i++){
      var operatorName = UIMembers.listData.bindings.get(i).getOperator().getName();
      html += "<li><a href=\"javascript:$.cookie('FABRIC.Debugger.UIMembers.operatorName','"+operatorName+"');history.go(0);\">"+operatorName+"</a></li>";
    }
    $("#NodeListD").html(html);
    
    // populate the operator view
    html = "";
    var binding;
    for(var i=0;i<UIMembers.listData.bindings.length;i++){
      binding = UIMembers.listData.bindings.get(i);
      html += "<option"+(i==0?" select":"")+">"+binding.getOperator().getName()+"</option>";
    }
    // add a callback to the operator dropdown
    var onOperatorSelected = function(){
      var operatorName = UIMembers.operatorName;
      if(!UIMembers.operatorCookie){
        operatorName = $("#OperatorDropDown").val();
        UIMembers.operatorName = operatorName;
      }
        
      var operatorBinding = false;
      for(var i=0;i<UIMembers.listData.bindings.length;i++){
        if(UIMembers.listData.bindings.get(i).getOperator().getName() == operatorName){
          operatorBinding = UIMembers.listData.bindings.get(i);
          break;
        }
      }
      if(!operator){
        $("#OperatorEntryFunction").attr("value","").attr("disabled",true);
        $("#OperatorParameterLayout").html("").attr("disabled",true);
        $("#OperatorSourceCode").val("").attr("disabled",true);
        return;
      }
      
      var operator = operatorBinding.getOperator();
      $("#OperatorEntryFunction")
        .attr("value",operator.entryFunctionName ? operator.entryFunctionName : "")
        .attr("disabled",false)
        .unbind("change")
        .change(function(){
          console.log("Debugger: entryFunctionName changed!" + $("#OperatorEntryFunction").val());
        });
        
      var html = "";
      var parameters = [];
      try{
        parameters = operatorBinding.getParameterLayout();
      }
      catch(e){}
      for(var i=0;i<parameters.length;i++){
        html += "<li>"+parameters[i]+"</li>"; // eventually add buttons here
      }
      $("#OperatorParameterLayout")
        .html(html)
        .attr("disabled",true);
        
      $("#OperatorSourceCode")
        .val(operator.sourceCode ? operator.sourceCode : "")
        .attr("disabled",false)
        .unbind("change")
        .change(function(){
          var code = $("#OperatorSourceCode").val();
          var operator = false;
          var recompiled = false;
          for(var i=0;i<UIMembers.listData.bindings.length;i++){
            if(UIMembers.listData.bindings.get(i).getOperator().getName() == UIMembers.operatorName){
              operator = UIMembers.listData.bindings.get(i).getOperator();
              break;
            }
          }
          if(operator){
            if(operator.sourceCode != code){
              operator.sourceCode = code;
              recompiled = true;
            }

            // check for errors
            var html = "";
            for(var i=0;i<operator.diagnostics.length;i++){
              if(operator.diagnostics[i].level=="warning"){
                html += "<div class='sourceCode-warning'>";
              }
              else{
                html += "<div class='sourceCode-error'>";
              }
              html += "Line "+operator.diagnostics[i].line+": "+operator.diagnostics[i].desc + "</div>";
            }
          }
          if(recompiled){
            console.log("Debugger: Operator "+UIMembers.operatorName+" recompiled!");
          }
          $("#OperatorErrors").html(html);
        });
        
      $("#OperatorSourceCode").trigger("change");
        
      // store the cookie!
      if(!UIMembers.operatorCookie){
        $.cookie("FABRIC.Debugger.UIMembers.operatorName",operatorName);
      }
      return;
    }

    $("#OperatorDropDown")
      .html(html)
      .attr("disabled",false)
      .unbind("change")
      .change(onOperatorSelected);
    onOperatorSelected();
  };

  var onNodeClicked = function(evt){
    evt.preventDefault();
    evt.stopPropagation();
    clearAllViews();
    onNodeSelected(evt.currentTarget.nodeID);
  }

  var onEventSelected = function(eventID){
    var event = events[eventID];
    if(!event){
      return;
    }
    UIMembers.nodeKind = "Event";
    UIMembers.nodeName = event.getName();
    UIMembers.listData = {};
    UIMembers.listData.handlers = event.getEventHandlers();
    
    // store all relevant data in cookies
    $.cookie("FABRIC.Debugger.UIMembers.nodeKind",UIMembers.nodeKind,{ expires: 7 });
    $.cookie("FABRIC.Debugger.UIMembers.nodeName",UIMembers.nodeName,{ expires: 7 });
    
    // now put the data into fields
    $("#RightPaneTitle").html("Event Node");
    $("#NodeName").attr("value",UIMembers.nodeName);
    
    // populate the first list, should be the depends!
    $("#NodeListATitle").html("Handlers");
    var html = "";
    for(var i=0;i<UIMembers.listData.handlers.length;i++){
      html += "<li><a href=\"javascript:$.cookie('FABRIC.Debugger.UIMembers.nodeKind','EventHandler');$.cookie('FABRIC.Debugger.UIMembers.nodeName','"+UIMembers.listData.handlers[i].getName()+"');history.go(0);\">"+UIMembers.listData.handlers[i].getName()+"</a></li>";
    }
    $("#NodeListA").html(html);
  };

  var onEventClicked = function(evt){
    evt.preventDefault();
    evt.stopPropagation();
    clearAllViews();

    onEventSelected(evt.currentTarget.eventID);
  }
  
  var onEventHandlerSelected = function(eventHandlerID){
    var eventHandler = eventHandlers[eventHandlerID];
    if(!eventHandler){
      return;
    }
    UIMembers.nodeKind = "EventHandler";
    UIMembers.nodeName = eventHandler.getName();
    UIMembers.listData = {};
    UIMembers.listData.preoperators = eventHandler.preDescendBindings;
    if(!UIMembers.listData.preoperators){
      UIMembers.listData.preoperators = [];
    }
    UIMembers.listData.postoperators = eventHandler.postDescendBindings;
    if(!UIMembers.listData.postoperators){
      UIMembers.listData.postoperators = [];
    }
    UIMembers.operatorName = UIMembers.listData.preoperators.length > 0 ? UIMembers.listData.preoperators.get(0).getName() : "";
    if(UIMembers.operatorName == ""){
      UIMembers.operatorName = UIMembers.listData.postoperators.length > 0 ? UIMembers.listData.postoperators.get(0).getName() : "";
    }
    
    // store all relevant data in cookies
    $.cookie("FABRIC.Debugger.UIMembers.nodeKind",UIMembers.nodeKind,{ expires: 7 });
    $.cookie("FABRIC.Debugger.UIMembers.nodeName",UIMembers.nodeName,{ expires: 7 });
    
    // now put the data into fields
    $("#RightPaneTitle").html("Event Handler Node");
    $("#NodeName").attr("value",UIMembers.nodeName);
    
    $("#NodeListATitle").html("Parents");
    var html = "";
    var parents = getParentEventHandler(eventHandler.getName());
    for(var i=0;i<parents.length;i++){
      html += "<li><a href=\"javascript:$.cookie('FABRIC.Debugger.UIMembers.nodeName','"+parents[i].getName()+"');history.go(0);\">"+parents[i].getName()+"</a></li>";
    }
    $("#NodeListA").html(html);

    $("#NodeListBTitle").html("Children");
    var html = "";
    var children = eventHandler.getChildEventHandlers();
    for(var i=0;i<children.length;i++)
      html += "<li><a href=\"javascript:$.cookie('FABRIC.Debugger.UIMembers.nodeName','"+children[i].getName()+"');history.go(0);\">"+children[i].getName()+"</a></li>";
    $("#NodeListB").html(html);
    
    // populate the third list, should be the dependencies!
    $("#NodeListCTitle").html("Pre-Ops");
    var html = "";
    for(var i=0;i<UIMembers.listData.preoperators.length;i++){
      var op = UIMembers.listData.preoperators.get(i);
      html += "<li><a href=\"javascript:$.cookie('FABRIC.Debugger.UIMembers.operatorName','"+op.getOperator().getName()+"');history.go(0);\">"+op.getOperator().getName()+"</a></li>";
    }
    $("#NodeListC").html(html);

    // populate the third list, should be the dependencies!
    $("#NodeListDTitle").html("Post-Ops");
    var html = "";
    for(var i=0;i<UIMembers.listData.postoperators.length;i++){
      var op = UIMembers.listData.postoperators.get(i);
      html += "<li><a href=\"javascript:$.cookie('FABRIC.Debugger.UIMembers.operatorName','"+op.getOperator().getName()+"');history.go(0);\">"+op.getOperator().getName()+"</a></li>";
    }
    $("#NodeListD").html(html);
    
    // populate the operator view
    html = "";
    if(UIMembers.listData.preoperators.length){
      html += "<option disabled>Pre-descend operators</option>";
      for(var i=0;i<UIMembers.listData.preoperators.length;i++){
        var name = UIMembers.listData.preoperators.get(i).getOperator().getName();
        html += "<option value='" + name + "'"+(i==0?" select":"")+">&nbsp;&nbsp;"+name+"</option>";
      }
    }
    if(UIMembers.listData.postoperators.length){
      html += "<option disabled>Post-descend operators</option>";
      for(var i=0;i<UIMembers.listData.postoperators.length;i++){
        var name = UIMembers.listData.postoperators.get(i).getOperator().getName();
        html += "<option value='" + name + "'"+(i==0?" select":"")+">&nbsp;&nbsp;"+name+"</option>";
      }
    }
    if(UIMembers.listData.postoperators.length == 0 && UIMembers.listData.postoperators.length){
      html += "<option disabled>No operators</option>";
    }
      
    // add a callback to the operator dropdown
    var onOperatorSelected = function(){
      var operatorName = UIMembers.operatorName;
      if(!UIMembers.operatorCookie){
        operatorName = $("#OperatorDropDown").val();
        UIMembers.operatorName = operatorName;
      }
        
      var operatorBinding = false;
      for(var i=0;i<UIMembers.listData.preoperators.length;i++){
        if(UIMembers.listData.preoperators.get(i).getName() == operatorName){
          operatorBinding = UIMembers.listData.preoperators.get(i);
          break;
        }
      }
      if(!operatorBinding){
        for(var i=0;i<UIMembers.listData.postoperators.length;i++){
          if(UIMembers.listData.postoperators.get(i).getName() == operatorName){
            operatorBinding = UIMembers.listData.postoperators.get(i);
            break;
          }
        }
      }
      if(!operatorBinding){
        $("#OperatorEntryFunction").attr("value","").attr("disabled",true);
        $("#OperatorParameterLayout").html("").attr("disabled",true);
        $("#OperatorSourceCode").val("").attr("disabled",true);
        return;
      }

      $("#OperatorEntryFunction")
        .attr("value",operator.entryFunctionName ? operator.entryFunctionName : "")
        .attr("disabled",true)
        .unbind("change")
        .change(function(){
          console.log("Debugger: entryFunctionName changed!");
        });
        
      var html = "";
      var parameters = [];
      try{
        parameters = operatorBinding.getParameterLayout();
      }
      catch(e){}
      for(var i=0;i<parameters.length;i++){
        html += "<li>"+parameters[i]+"</li>"; // eventually add buttons here
      }
      $("#OperatorParameterLayout")
        .html(html)
        .attr("disabled",true);
      
      var operator = operatorBinding.getOperator();
      
      $("#OperatorSourceCode")
        .val(operator.sourceCode ? operator.sourceCode : "")
        .attr("disabled",false)
        .unbind("change")
        .change(function(){
          var code = $("#OperatorSourceCode").val();
          var operator = false;
          var recompiled = false;
          if(operator){
            if(operator.sourceCode != code){
              operator.sourceCode = code;
              recompiled = true;
            }

            // check for errors
            var html = "";
            for(var i=0;i<operator.diagnostics.length;i++){
              if(operator.diagnostics[i].level=="warning"){
                html += "<div class='sourceCode-warning'>";
              }
              else{
                html += "<div class='sourceCode-error'>";
              }
              html += "Line "+operator.diagnostics[i].line+": "+operator.diagnostics[i].desc + "</div>";
            }
          }
          if(recompiled){
            console.log("Debugger: Operator "+UIMembers.operatorName+" recompiled!");
          }
          $("#OperatorErrors").html(html);
        });
        
      // store the cookie!
      if(!UIMembers.operatorCookie){
        $.cookie("FABRIC.Debugger.UIMembers.operatorName",operatorName);
      }
      return;
    }

    $("#OperatorDropDown")
      .html(html)
      .attr("disabled",false)
      .unbind("change")
      .change(onOperatorSelected);
    onOperatorSelected();
  };
  
  var onEventHandlerClicked = function(evt){
    evt.preventDefault();
    evt.stopPropagation();
    clearAllViews();
    onEventHandlerSelected(evt.currentTarget.eventHandlerID);
  }

  // check if we have to select a node again!
  if($.cookie("FABRIC.Debugger.UIMembers.nodeKind") != undefined && useCookies){
    var nodeKind = $.cookie("FABRIC.Debugger.UIMembers.nodeKind");
    if(nodeKind == "DependencyNode"){
      //try to find the right node
      var thisNodeName = $.cookie("FABRIC.Debugger.UIMembers.nodeName");
      for(var i=0;i<nodeName.length;i++){
        if(nodeName[i] == thisNodeName){
          // select the node
          onNodeSelected(i);
          break;
        }
      }
    }
    else if(nodeKind == "EventHandler"){
      //try to find the right node
      var thisHandlerName = $.cookie("FABRIC.Debugger.UIMembers.nodeName");
      for(var i=0;i<eventHandlerName.length;i++){
        if(eventHandlerName[i] == thisHandlerName){
          // select the node
          onEventHandlerSelected(i);
          break;
        }
      }
    }
    else if(nodeKind == "Event"){
      //try to find the right node
      var thisEventName = $.cookie("FABRIC.Debugger.UIMembers.nodeName");
      for(var i=0;i<eventName.length;i++){
        if(eventName[i] == thisEventName){
          // select the node
          onEventSelected(i);
          break;
        }
      }
    }
    else{
      throw("Unexpected nodeKind");
    }
  }
  
  // let's see if we should select an operator as well!
  if($.cookie("FABRIC.Debugger.UIMembers.operatorName") != undefined && useCookies){
    UIMembers.operatorName = $.cookie("FABRIC.Debugger.UIMembers.operatorName");
    UIMembers.operatorCookie = true;
    $("#OperatorDropDown").val(UIMembers.operatorName);
    $("#OperatorDropDown").trigger('change');
  }
  UIMembers.operatorCookie = false;
  
  // now walk all of the collumns
  var nbCollumns = nodeCollumns.length;
  var widthStep = 200;
  var heightStep = 50;

  // now create all of the events
  var x = 10 + 0 * widthStep;
  var nbRows = events.length;
  var eventGroups = [];
  for(var i=0;i<nbRows;i++)
    eventGroups.push(undefined);
  for(var eventID=0;eventID<nbRows;eventID++){
    var y = 10 + eventID * heightStep;
    
    var eventGroup = graphHolderGroup.createNode({
      text : eventName[eventID],
      color:FABRIC.RT.rgb255(90,200,90),
      height: 15,
      createEditButton: false,
      draggable: false,
      shadow: FABRIC.RT.vec2(0,0)
      });
    eventGroup.translate(x,y);
    eventGroup.elem.eventID = eventID;
    eventGroups[eventID] = eventGroup;
    
    eventGroup.addOutPort({  
        text:"",
        color: FABRIC.RT.rgb255(0,0,0),
        direction:"Source",
        allowMultipleConnections:true
    })
    
    // on click handler
    eventGroup.elem.addEventListener("mousedown", onEventClicked, false);
  }
  
  // now create all of the eventhandlers
  x = 10 + widthStep;
  var nbRows = eventHandlers.length;
  var eventHandlerGroups = [];
  for(var i=0;i<nbRows;i++){
    eventHandlerGroups.push(undefined);
  }
  for(var eventHandlerID=0;eventHandlerID<nbRows;eventHandlerID++){
    var y = 10 + eventHandlerID * heightStep;
    
    var name = eventHandlerName[eventHandlerID];
    var eventHandlerGroup = graphHolderGroup.createNode({
      text : name,
      color:FABRIC.RT.rgb255(200,90,90),
      height: 15,
      createEditButton: false,
      draggable: false,
      shadow: FABRIC.RT.vec2(0,0)
      });
    eventHandlerGroup.translate(x,y);
    eventHandlerGroup.elem.eventHandlerID = eventHandlerID;
    eventHandlerGroups[eventHandlerID] = eventHandlerGroup;
    
    eventHandlerGroup.addInPort({  
        text:"",
        color: FABRIC.RT.rgb255(0,0,0),
        direction:"Target",
        allowMultipleConnections:true
    });
    if(parentEventHandlers[name] || eventHandlerBindings[name].length > 0){
      eventHandlerGroup.addOutPort({  
          text:"",
          color: FABRIC.RT.rgb255(0,0,0),
          direction:"Source",
          allowMultipleConnections:true
      });
    }
    
    // on click handler
    eventHandlerGroup.elem.addEventListener("mousedown", onEventHandlerClicked, false);
  }
  
  var nodeGroups = [];
  for(var i=0;i<nodes.length;i++){
    nodeGroups.push(undefined);
  }
  for(var collumnID=0;collumnID<nbCollumns;collumnID++){
    var x = 10 + collumnID * widthStep;

    var nbRows = nodeCollumns[collumnID].length;
    for(var rowID=0;rowID<nbRows;rowID++){
      var nodeID = nodeCollumns[collumnID][rowID];
      var y = 10 + rowID * heightStep;
      
      var nodeGroup = graphHolderGroup.createNode({
        text : nodeName[nodeID],
        color:FABRIC.RT.rgb255(90,90,200),
        height: 15,
        createEditButton: false,
        draggable: false,
        shadow: FABRIC.RT.vec2(0,0)
        });
      nodeGroup.translate(x,y);
      nodeGroup.elem.nodeID = nodeID;
      nodeGroups[nodeID] = nodeGroup;
      
      // if we have dependencies
      if(getNodeDependents(nodeID).length > 0){
        nodeGroup.addInPort({
            text:"",
            color: FABRIC.RT.rgb255(0,0,0),
            direction:"Target",
            allowMultipleConnections:true
        })
      }
      if(getNodeDependencies(nodeID).length>0){
        nodeGroup.addOutPort({  
            text:"",
            color: FABRIC.RT.rgb255(0,0,0),
            direction:"Source",
            allowMultipleConnections:true
        })
      }
      
      // on click handler
      nodeGroup.elem.addEventListener("mousedown", onNodeClicked, false);
    }
  }
  
  // now loop again over the nodes and create the connections!
  for(var i=0;i<nodeGroups.length;i++){
    var nodeGroup = nodeGroups[i];
    var dependencies = getNodeDependencies(i);
    for(var j=0;j<dependencies.length;j++){
      var otherNodeGroup = nodeGroups[dependencies[j]];
      if(!otherNodeGroup.getInPort(0)){
        otherNodeGroup.addInPort({  
            text:"",
            color: FABRIC.RT.rgb255(0,0,0),
            direction:"Target",
            allowMultipleConnections:true
        });
      }
      if(!nodeGroup.getOutPort(0)){
        nodeGroup.addOutPort({  
            text:"",
            color: FABRIC.RT.rgb255(0,0,0),
            direction:"Source",
            allowMultipleConnections:true
        });
      }
      nodeGroup.getOutPort(0).connectTo(otherNodeGroup.getInPort(0),{ fireCallbacks: false,connectable:false });
    }
  }

  // now loop again over the eventhandlers and create the connections!
  for(var i=0;i<eventHandlers.length;i++)
  {
    var eventHandlerGroup = eventHandlerGroups[i];
    var parents = getParentEventHandler(eventHandlerName[i]);
    for(var j=0;j<parents.length;j++){
      var id = eventHandlerNameToID[parents[j]];
      var otherEventHandlerGroup = eventHandlerGroups[id];
      if(!otherEventHandlerGroup.getOutPort(0)){
        otherEventHandlerGroup.addOutPort({  
            text:"",
            color: FABRIC.RT.rgb255(0,0,0),
            direction:"Source",
            allowMultipleConnections:true
        });
      }
      otherEventHandlerGroup.getOutPort(0).connectTo(eventHandlerGroup.getInPort(0),{ fireCallbacks: false,connectable:false });
    }
    var bindings = eventHandlerBindings[eventHandlerName[i]];
    for(var j=0;j<bindings.length;j++){
      var id = nodeNameToID[bindings[j]];
      var nodeGroup = nodeGroups[id];
      if(!eventHandlerGroup.getOutPort(0)){
        eventHandlerGroup.addOutPort({  
            text:"",
            color: FABRIC.RT.rgb255(0,0,0),
            direction:"Source",
            allowMultipleConnections:true
        });
      }
      if(!nodeGroup.getInPort(0)){
        nodeGroup.addInPort({  
            text:"",
            color: FABRIC.RT.rgb255(0,0,0),
            direction:"Target",
            allowMultipleConnections:true
        });
      }
      eventHandlerGroup.getOutPort(0).connectTo(nodeGroup.getInPort(0),{ fireCallbacks: false,connectable:false });
    }
  }

  // now loop again over the events and create the connections!
  for(var i=0;i<events.length;i++){
    var eventGroup = eventGroups[i];
    var handlers = getEvent(i).getEventHandlers();
    for(var j=0;j<handlers.length;j++){
      var id = eventHandlerNameToID[handlers[j].getName()];
      var eventHandlerGroup = eventHandlerGroups[id];
      eventGroup.getOutPort(0).connectTo(eventHandlerGroup.getInPort(0),{ fireCallbacks: false,connectable:false });
    }
  }


  // initialize the transform of the view
  if($.cookie("FABRIC.Debugger.TopPane.Zoom") != undefined && useCookies){
    graphSclGroup.scale(parseFloat($.cookie("FABRIC.Debugger.TopPane.Zoom")));
  }
  else{
    graphSclGroup.scale(1.0);
  }
  if($.cookie("FABRIC.Debugger.TopPane.Translate") != undefined && useCookies){
    var str = $.cookie("FABRIC.Debugger.TopPane.Translate");
    str = str.split(":");
    graphPosGroup.translate(new FABRIC.RT.Vec2(parseInt(str[0]),parseInt(str[1])));
  }
  else{
    graphPosGroup.translate(new FABRIC.RT.Vec2(0,0));
  }
});
