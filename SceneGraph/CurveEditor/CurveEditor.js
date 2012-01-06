
var constructCurveEditor = function(domRootID, animationLibraryNode, options){
  
  var keyColor = FABRIC.rgb(.0, .0, .0);
  
  options = options ? options : {};
  options.timeStripe = options.timeStripe!=undefined ? options.timeStripe : true;
  options.draggable = options.draggable!=undefined ? options.draggable : true;
  options.zoomable = options.zoomable!=undefined ? options.zoomable : true;
  options.drawKeys = options.drawKeys!=undefined ? options.drawKeys : true; 
  trackSetId =  options.trackSetId!=undefined ? options.trackSetId : 0;
  trackFilters = options.trackFilters!=undefined ? options.trackFilters : [];
  
  var timeRange = options.timeRange!=undefined ? options.timeRange : new FABRIC.Vec2(0, 100);
  var valueRange = options.valueRange!=undefined ? options.valueRange : new FABRIC.Vec2(0, 1);
  var fitEditorToKeyRanges = options.fitEditorToKeyRanges!=undefined ? options.fitEditorToKeyRanges : true;
  var displayTrackNames = options.displayTrackNames!=undefined ? options.displayTrackNames : true;
  
  var rootDomNode = document.getElementById(domRootID);
  var windowWidth = rootDomNode.clientWidth;
  var windowHeight = rootDomNode.clientHeight;
  var isBezier = animationLibraryNode.isTypeOf('BezierKeyAnimationLibrary');
  
  var svgRoot = FABRIC.createSVGRootElem(domRootID);
  if(options.volumerenderdemohack){
    svgRoot.attr('style', "position:relative; top:-"+windowHeight+"px;z-index:0");
  }

  var graphBGRect = svgRoot.createRect().size(windowWidth, windowHeight).color(FABRIC.rgb(0.5, 0.5, 0.5));
  var graphCenterGroup = svgRoot.createGroup().id('graphCenterGroup').translate(0, windowHeight * 0.5);
  var curvesHolderGroup = graphCenterGroup.createGroup().id('curvesHolderGroup');
  svgRoot.svgRoot = svgRoot;
  svgRoot.state = 'Normal';
  
  var containmentRect;
  if(!fitEditorToKeyRanges){
    containmentRect = graphCenterGroup.createRect().size(windowWidth, windowHeight).translate(0, windowHeight * -0.5);;
    containmentRect.attr('fill', 'none');
    containmentRect.attr('stroke', "black");
    containmentRect.attr('stroke-width', 2);
  }
  ///////////////
  tracksData = animationLibraryNode.getTrackSet(trackSetId);
  var trackCount = tracksData.tracks.length;
  var yRange = new FABRIC.Vec2(0, 0);
  var curvesData = [];
  var trackGroups = [];
  var trackDisplayed = [];
  
  var trackDisplayNode = scene.getSceneGraphNode(animationLibraryNode.getName()+'TrackDisplay');
  if(!trackDisplayNode){
    trackDisplayNode = scene.constructNode('TrackDisplay', {
      name:animationLibraryNode.getName()+'TrackDisplay',
      animationLibraryNode: animationLibraryNode,
      trackSetId: trackSetId
    });
  }
  
  
  var screenXfo = {
    tr: new FABRIC.Vec2(0, 0),
    sc: new FABRIC.Vec2(1, 1),
    fitToScreen:function(){
      if(fitEditorToKeyRanges){
        timeRange = new FABRIC.Vec2(0, 0);
        for (var i = 0; i < trackCount; i++) {
          if(!trackDisplayed[i]) continue;
          var keys = tracksData.tracks[i].keys;
          if(keys.length < 2){
            continue;
          }
          if (timeRange.x > keys[0].time) {
            timeRange.x = keys[0].time;
          }
          if (timeRange.y < keys[keys.length - 1].time) {
            timeRange.y = keys[keys.length - 1].time;
          }
        }
        trackDisplayNode.setTimeRange(timeRange);
      }
      var getCurveYRange = function(curveData) {
        for (var i = 1; i < curveData.length; i++) {
          if (yRange.x > curveData[i]) {
            yRange.x = curveData[i];
          }
          if (yRange.y < curveData[i]) {
            yRange.y = curveData[i];
          }
        }
      }
      
      curvesData = trackDisplayNode.getCurveData();
      if(!fitEditorToKeyRanges){
        yRange = valueRange;
      }
      else{
        yRange = new FABRIC.Vec2(0, 0);
        for (var i = 0; i < trackCount; i++) {
          if(!trackDisplayed[i]) continue;
          getCurveYRange(curvesData.values[i]);
        }
      }
      var vscale;
      if(!fitEditorToKeyRanges){
        vscale = -(windowHeight) / (yRange.y - yRange.x);
      }else{
        vscale = -(windowHeight - 40) / (yRange.y - yRange.x);
      }
      
      var hscale = windowWidth;
      if(timeRange.y > timeRange.x){
        hscale = windowWidth/(timeRange.y - timeRange.x);
      }
      this.sc = new FABRIC.Vec2(hscale, vscale);
      this.tr = new FABRIC.Vec2(-timeRange.x, (yRange.y + yRange.x) * -0.5);
    },
    update: function(){
      this.sc.x = windowWidth / (timeRange.y - timeRange.x);
    },
    toScreenSpace: function(val, local){
      if(local){
        return val.multiply(this.sc);
      }else{
        return val.add(this.tr).multiply(this.sc);
      }
    },
    toGraphSpace: function(val, local){
      if(local){
        return val.multiply(this.sc.inverse());
      }else{
        return val.multiply(this.sc.inverse()).subtract(this.tr);
      }
    }
  }
  
  var drawTrackCurves;
  var buildCurveEditor = function(){
    
    if(displayTrackNames){
      ///////////////
      // Build the tree
      var tree = {};
      trackDisplayed = [];
      var currDomElement = $('#keyframeTracks');
      currDomElement.empty();
      for (var i = 0; i < trackCount; i++) {
        var trackData = tracksData.tracks[i];
        
        if(!trackGroups[i]){
          trackGroups[i] = curvesHolderGroup.createGroup().id(tracksData.tracks[i].name);
        }
        if(trackFilters.length > 0){
          trackDisplayed[i] = true;
          for (var j = 0; j < trackFilters.length; j++) {
            if(trackData.name.indexOf(trackFilters[j])===-1){
              trackDisplayed[i] = false;
            }
          }
          if(!trackDisplayed[i]){
            continue;
          }
        }
        
        var namePath = trackData.name.split('.');
        var currBranch = tree;
        for (var j = 0; j < namePath.length; j++) {
          if(!currBranch[namePath[j]]){
            if(j<(namePath.length-1)){
              currBranch[namePath[j]] = {};
            }else{
              // set the track id as the leaf value
              currBranch[namePath[j]] = i;
            }
          }
          currBranch = currBranch[namePath[j]];
        }
      }
      var trackId = 0;
      var buildTree = function(el, branchName, treedata){
        var li = $(document.createElement('li'));
        var checkbox = $(document.createElement('input')).attr('type', "checkbox");
        li.append(checkbox);
        li.append(branchName);
        el.append(li);
        if(typeof treedata == 'number'){
          checkbox.attr('id', tracksData.tracks[trackId].name);
          checkbox.attr('checked', true);
          checkbox.data('displayed', true);
          checkbox.data('trackId', trackId)
          var trackData = tracksData.tracks[trackId];
          
          trackId++;
        }else{
          var ul = $(document.createElement('ul'));
          li.append(ul);
          for(var subBranchName in treedata){
            buildTree(ul, subBranchName, treedata[subBranchName]);
          }
        }
      }
      for(var branchName in tree){
        buildTree($('#keyframeTracks'), branchName, tree[branchName]);
      }
    }
    else{
      trackDisplayed.length = trackCount;
      for (var j = 0; j < trackCount; j++) {
        trackDisplayed[trackId] = true;
      }
    }
  
    if(!fitEditorToKeyRanges){
      yRange = valueRange;
    }
  
    screenXfo.fitToScreen();
  
    var drawTrackCurve = function(trackId) {
      var trackData = tracksData.tracks[trackId];
      var trackGroup = trackGroups[trackId];
      trackGroup.removeAllChildren();
      var path = trackGroup.createPath().addClass('CurvePath').stroke(trackData.color);
      var setPathCurveValues = function(curveData) {
        try{
          var pathValues;
          if(timeRange.y > timeRange.x){
            var val = screenXfo.toScreenSpace(new FABRIC.Vec2(0, curveData[0]));
            pathValues = ['M', val.x, val.y, 'L'];
            for (var i = 0; i < curveData.length; i++) {
              var t = ((i/curveData.length)*(timeRange.y - timeRange.x))+timeRange.x;
              val = screenXfo.toScreenSpace(new FABRIC.Vec2(t, curveData[i]));
              pathValues[(i * 2) + 4] = val.x;
              pathValues[(i * 2) + 5] = val.y;
            }
          }
          else{
            var val1 = screenXfo.toScreenSpace(new FABRIC.Vec2(0, curveData[0]));
            var val2 = screenXfo.toScreenSpace(new FABRIC.Vec2(1, curveData[0]));
            pathValues = ['M', val1.x, val1.y, 'L', val2.x, val2.y];
          }
          path.attr('d', pathValues.join(' '));
        }
        catch(e){
          console.warn("Bug: the core is sometimes returning null values e.g.:"+JSON.stringify(curveData));
        }
      }
      setPathCurveValues(curvesData.values[trackId]);
  
      var updateCurve = function() {
        curvesData = trackDisplayNode.getCurveData();
        setPathCurveValues(curvesData.values[trackId]);
      }
      var updateDrawnKeys = function(){
        for(var i=0; i<trackData.keys.length; i++){
          var key = trackData.keys[i];
          var keySsVal = screenXfo.toScreenSpace(new FABRIC.Vec2(key.time, key.value));
          drawnKeys[i].key.translate(keySsVal);
          if(drawnKeys[i].intan){
            var inTanSsVal = screenXfo.toScreenSpace(key.intangent, true);
            drawnKeys[i].intan.translate(inTanSsVal.x - 3, inTanSsVal.y - 3)
            drawnKeys[i].intanline.attr('d', 'M 0 0 L ' + inTanSsVal.x + ' ' + inTanSsVal.y);
          }
          if(drawnKeys[i].outtan){
            var outTanSsVal = screenXfo.toScreenSpace(key.outtangent, true);
            drawnKeys[i].outtan.translate(outTanSsVal.x - 3, outTanSsVal.y - 3)
            drawnKeys[i].outtanline.attr('d', 'M 0 0 L ' + outTanSsVal.x + ' ' + outTanSsVal.y);
          }
        }
      }
      var drawnKeys = [];
      var drawKey = function(keyIndex, keyData) {
        
        if(isBezier ? ((keyData.time + keyData.intangent.x) < timeRange.y &&
           (keyData.time + keyData.outtangent.x) >= timeRange.x) :
           keyData.time <= timeRange.y && keyData.time >= timeRange.x ){
          ///////////////////////////////////////////////
          // Key
          var keySsVal = screenXfo.toScreenSpace(new FABRIC.Vec2(keyData.time, keyData.value));
          // When keyframes are manipulated, we can maintain the tangent lengths
          // by storing the relative lengths, and gradients. Then, during manipulation
          // we maintain these relative lengths and gradients. This simplifies the
          // keyframes, by moving the complexity into the manipulation system.
          var prevKey = trackData.keys[keyIndex-1],
              nextKey = trackData.keys[keyIndex+1],
              tangentNormalizedValues = [];
          var keyGroupNode = trackGroup.createGroup().translate(keySsVal)
            .draggable({ mouseButton: 0, containment:containmentRect })
            .addOnDragBeginCallback(
              function(evt){
                var deltat, i=0;
                if(keyIndex > 0){
                  deltat = keyData.time - prevKey.time;
                  if(isBezier) {
                    tangentNormalizedValues[i++] = prevKey.outtangent.x / deltat;
                    tangentNormalizedValues[i++] = prevKey.outtangent.y / prevKey.outtangent.x;
                    tangentNormalizedValues[i++] = keyData.intangent.x / deltat;
                    tangentNormalizedValues[i++] = keyData.intangent.y / keyData.intangent.x;
                  }
                }
                if(keyIndex < trackData.keys.length-1){;
                  deltat = nextKey.time - keyData.time;
                  if(isBezier) {
                    tangentNormalizedValues[i++] = keyData.outtangent.x / deltat;
                    tangentNormalizedValues[i++] = keyData.outtangent.y / keyData.outtangent.x;
                    tangentNormalizedValues[i++] = nextKey.intangent.x / deltat;
                    tangentNormalizedValues[i++] = nextKey.intangent.y / nextKey.intangent.x;
                  }
                }
            })
            .addOnDragCallback(
              function(evt) {
                var keyGsVal = screenXfo.toGraphSpace(evt.localPos);
                tracksData.tracks[trackId].moveKey(keyIndex, keyGsVal.x, keyGsVal.y);
                animationLibraryNode.setTrackSet(tracksData, trackSetId);
                updateCurve();
                updateDrawnKeys();
                scene.redrawAllViewports();
              });
          var keyNode = keyGroupNode.createRect().translate(-4, -4).size(8, 8).stroke(keyColor);
          drawnKeys[keyIndex] = { key: keyGroupNode };
          
          if(isBezier) {
            ///////////////////////////////////////////////
            // In Tangent
            var inTanSsVal = screenXfo.toScreenSpace(keyData.intangent, true);
            var inTanLineNode = keyGroupNode.createPath().addClass('KeyframeHandle');
            inTanLineNode.attr('d', 'M 0 0 L ' + inTanSsVal.x + ' ' + inTanSsVal.y);
            var inTanNode = keyGroupNode.createRect()
                                                .translate(inTanSsVal.x - 3, inTanSsVal.y - 3)
                                                .size(6, 6).stroke(keyColor);
            inTanNode.draggable({ mouseButton: 0 })
              .addOnDragBeginCallback(
                function(evt){
                })
              .addOnDragCallback(
                function(evt) {
                  keyData.intangent = screenXfo.toGraphSpace(evt.localPos, true);
                  // Clamp the handle lengths > 0 && < deltat
                  if(!evt.altKey && keyIndex > 0){
                    var deltat = prevKey.time - keyData.time;
                    keyData.intangent.x = keyData.intangent.x < deltat ? deltat : keyData.intangent.x;
                  }
                  keyData.intangent.x = keyData.intangent.x > -0.1 ? -0.1 : keyData.intangent.x;
                  
                  if(!evt.altKey && keyIndex < trackData.keys.length-1){
                    keyData.outtangent.y = (keyData.intangent.y / keyData.intangent.x) * keyData.outtangent.x;
                  }
                  tracksData.tracks[trackId].keys[keyIndex] = keyData;
                  animationLibraryNode.setTrackSet(tracksData, trackSetId);
                  updateCurve();
                  updateDrawnKeys();
                  scene.redrawAllViewports();
                });
              drawnKeys[keyIndex].intan = inTanNode;
              drawnKeys[keyIndex].intanline = inTanLineNode;
                
            keyData.setInTan = function(intangent, setKeyData){
              if(inTanNode){
                var inTanSsVal = screenXfo.toScreenSpace(intangent, true);
                inTanNode.translate(inTanSsVal.x - 3, inTanSsVal.y - 3)
                inTanLineNode.attr('d', 'M 0 0 L ' + inTanSsVal.x + ' ' + inTanSsVal.y);
              }
              if(setKeyData){
                keyData.intangent = intangent;
                tracksData.tracks[trackId].keys[keyIndex] = keyData;
                animationLibraryNode.setTrackSet(tracksData, trackSetId);
              }
            }
            ///////////////////////////////////////////////
            // Out Tangent
            var outTanSsVal = screenXfo.toScreenSpace(keyData.outtangent, true);
            var outTanLineNode = keyGroupNode.createPath().addClass('KeyframeHandle');
            outTanLineNode.attr('d', 'M 0 0 L ' + outTanSsVal.x + ' ' + outTanSsVal.y);
            var outTanNode = keyGroupNode.createRect()
                                                 .translate(outTanSsVal.x - 3, outTanSsVal.y - 3)
                                                 .size(6, 6).stroke(keyColor);
    
            outTanNode.draggable({ mouseButton: 0 })
              .addOnDragBeginCallback(
                function(evt){
                })
              .addOnDragCallback(
                function(evt) {
                  keyData.outtangent = screenXfo.toGraphSpace(evt.localPos, true);
                  
                  // Clamp the handle lengths > 0 && < deltat
                 if(keyIndex < trackData.keys.length-1){
                    var deltat = nextKey.time - keyData.time;
                    keyData.outtangent.x = keyData.outtangent.x > deltat ? deltat : keyData.outtangent.x;
                  }
                  keyData.outtangent.x = keyData.outtangent.x < 0.1 ? 0.1 : keyData.outtangent.x;
                  
                  if(!evt.altKey && keyIndex < trackData.keys.length-1){
                    keyData.intangent.y = (keyData.outtangent.y / keyData.outtangent.x) * keyData.intangent.x;
                  }
                  
                  tracksData.tracks[trackId].keys[keyIndex] = keyData;
                  animationLibraryNode.setTrackSet(tracksData, trackSetId);
                  updateCurve();
                  updateDrawnKeys();
                  scene.redrawAllViewports();
                });
            drawnKeys[keyIndex].outtan = outTanNode;
            drawnKeys[keyIndex].outtanline = outTanLineNode;
          }
        }
      }
      if (options.drawKeys) {
        var trackData = tracksData.tracks[trackId];
        for (var i = 0; i < trackData.keys.length; i++) {
          drawKey(i, trackData.keys[i]);
        }
      }
    }
      
    drawTrackCurves = function(){
      for (var i = 0; i < trackCount; i++) {
        if(trackDisplayed[i]){
          drawTrackCurve(i);
        }else if(trackGroups[i]){
          trackGroups[i].removeAllChildren();
        }
      }
    }
    
    var clearTrackCurves = function(){
      for (var i = 0; i < trackCount; i++) {
        trackGroups[i].removeAllChildren();
      }
    }
      
    drawTrackCurves();
    
    if(displayTrackNames){
      $('#keyframeTracks').click( function(){
        $(this).find("input[type='checkbox']").each(function(index, el){
          if($(el).attr('id') != undefined){
            var trackId = $(el).data('trackId');
            if(!$(el).is(':checked')){
              // remove the curve from display
              if($(el).data('displayed')){
                trackGroups[trackId].removeAllChildren();
                $(el).data('displayed', false);
              }
            }else{
              if(!$(el).data('displayed')){
                drawTrackCurve(trackId);
                $(el).data('displayed', true);
              }
            }
            trackDisplayed[trackId] = $(el).is(':checked');
          }
        });
      });
    }
    
  }
  buildCurveEditor();
  
  var setCurveFilters = function(filters){
    trackFilters = filters;
    tracksData = animationLibraryNode.getTrackSet(trackSetId);
    buildCurveEditor();
  }
  
  if(options.timeStripe){
    var timeStripeGroupNode = graphCenterGroup.createGroup()
      .translate((scene.animation.getTime() + screenXfo.tr.x)  * screenXfo.sc.x, windowHeight * -0.5)
      .draggable({ mouseButton: 0, axis: 'X' })
      .addOnDragCallback(
        function(evt) {
          scene.animation.setTime((evt.localPos.x / screenXfo.sc.x) - screenXfo.tr.x);
        });
    timeStripeGroupNode.createRect().size(10, windowHeight).addClass('EventCatcher');
    var timeStripe = timeStripeGroupNode.createPath().addClass('TimeStripePath')
        .attr('d', 'M 5 0 L 5 ' + windowHeight);
    
    var updateTimeStripe = function(){
      timeStripeGroupNode.translate((scene.animation.getTime() + screenXfo.tr.x)  * screenXfo.sc.x, windowHeight * -0.5);
    }
    scene.addEventListener('timechanged', updateTimeStripe);
  }
  
  var updateTimeRange = function(){
    trackDisplayNode.setTimeRange(timeRange);
    curvesData = trackDisplayNode.getCurveData();
    
    if(options.timeStripe){
      updateTimeStripe();
    }
    drawTrackCurves();
  }
  
  var fitCurveEditorToWindow = function(){
    var newWindowWidth = $('#viewer').width();
    var newWindowHeight = $('#viewer').height();
    
    // Occasionaly when the window is opened, it has a negative width and
    // then we get sent a resize event. Here, we re-fit the curve to the screen.
    var invalidWindowWidth = (windowWidth < 0);
    windowWidth = newWindowWidth;
    windowHeight = newWindowHeight;
    if(invalidWindowWidth){
      screenXfo.fitToScreen();
    }
    graphCenterGroup.translate(0, windowHeight * 0.5);
    graphBGRect.size(windowWidth, windowHeight);
    timeStripe.attr('d', 'M 5 0 L 5 ' + windowHeight);
    updateTimeRange();
  }
  
  if(options.draggable){
    var dragStartTimeRange;
    var dragStartOffset;
    graphBGRect.draggable( {mouseButton:0, delegateTranslateObj:false, highlight:false } )
      .addOnDragBeginCallback(
        function(evt){
          dragStartTimeRange = timeRange.clone();
          dragStartOffset = screenXfo.tr.clone();
      })
      .addOnDragCallback(
        function(evt){
          timeRange.x = dragStartTimeRange.x - (evt.dragVec.x / screenXfo.sc.x);
          timeRange.y = dragStartTimeRange.y - (evt.dragVec.x / screenXfo.sc.x);
          screenXfo.tr = dragStartOffset.add(evt.dragVec.multiply(screenXfo.sc.inverse()));
          updateTimeRange();
        });
  }
  
  if(options.zoomable){
    var mouseWheelZoomFn = function(evt) {
      if(evt.detail != undefined) evt.wheelDelta = evt.detail * -50;
      var zoomDist = evt.wheelDelta * 0.5;
      timeRange.x += zoomDist / screenXfo.sc.x;
      timeRange.y -= zoomDist / screenXfo.sc.x;
      yRange.x -= zoomDist / (screenXfo.sc.y * (windowWidth/windowHeight));
      yRange.y += zoomDist / (screenXfo.sc.y * (windowWidth/windowHeight));
      // recompute the screen fitting. TODO: this code shouldn't be int he event handler
      screenXfo.sc.x = windowWidth/(timeRange.y - timeRange.x);
      screenXfo.sc.y = -(windowHeight - 40) / (yRange.y - yRange.x)
      screenXfo.tr.x = -timeRange.x;
    // screenXfo.tr.y = (yRange.y + yRange.x) * -0.5;  
      updateTimeRange();
    }
    
    rootDomNode.addEventListener('mouseover', function(){
      document.addEventListener('mousewheel', mouseWheelZoomFn, false);
      document.addEventListener('DOMMouseScroll', mouseWheelZoomFn, false);
    });
    
    rootDomNode.addEventListener('mouseout', function(){
      document.removeEventListener('mousewheel', mouseWheelZoomFn, false);
      document.removeEventListener('DOMMouseScroll', mouseWheelZoomFn, false);
    });
  }
  
  var updateGraph = function(){
    tracksData = animationLibraryNode.getTrackSet(trackSetId);
    updateTimeRange();
  }
  
  var updateGraphEventFn = function(evt){
    updateGraph();
  };
  animationLibraryNode.addEventListener('valuechanged', updateGraphEventFn);
  window.onunload = function(){
    animationLibraryNode.removeEventListener('valuechanged', updateGraphEventFn);
    if(updateTimeStripe){
      scene.removeEventListener('timechanged', updateTimeStripe);
    }
  };
  
  var resizeIntervalId;
  
  return {
    resize: function(){
      window.clearTimeout( resizeIntervalId );
      resizeIntervalId = window.setTimeout( function () {
          fitCurveEditorToWindow();
        }, 50 );
    },
    redraw: function(){
      updateGraph();
    },
    setKeyDisplayToggle: function(val){
      clearTrackCurves();
      options.drawKeys = val;
      drawTrackCurves();
    },
    setCurveFilters: function(filters){
      setCurveFilters(filters);
    }
  }
};
