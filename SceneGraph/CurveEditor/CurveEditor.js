
var constructCurveEditor = function(domRootID, animationLibraryNode, options){
  
  var keyColor = FABRIC.rgb(.0, .0, .0);
  
  options = options ? options : {};
  options.timeStripe = options.timeStripe!=undefined ? options.timeStripe : true;
  options.draggable = options.draggable!=undefined ? options.draggable : true;
  options.zoomable = options.zoomable!=undefined ? options.zoomable : true;
  trackSetId =  options.trackSetId!=undefined ? options.trackSetId : 0;
  
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

  var graphBGRect = svgRoot.createRect().size(windowWidth, windowHeight).addClass('EventCatcher');
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
  trackDisplayNode = scene.constructNode('TrackDisplay', {
      animationLibraryNode: animationLibraryNode,
      trackSetId: trackSetId
    });
  
  if(displayTrackNames){
    ///////////////
    // Build the tree
    var tree = {};
    var currDomElement = $('#keyframeTracks');
    for (var i = 0; i < trackCount; i++) {
      var trackData = tracksData.tracks[i];
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
    var trackDisplayed = [];
    var buildTree = function(el, branchName, treedata){
      var li = $(document.createElement('li'));
      var checkbox = $(document.createElement('input')).attr('type', "checkbox");
      li.append(checkbox);
      li.append(branchName);
      el.append(li);
      if(typeof treedata == 'number'){
        var trackId = treedata;
        checkbox.attr('id', tracksData.tracks[trackId].name);
        checkbox.attr('checked', true);
        checkbox.data('displayed', true);
        checkbox.data('trackId', trackId)
        var trackData = tracksData.tracks[trackId];
        
        trackGroups[trackId] = curvesHolderGroup.createGroup().id('curveGroup');
        trackDisplayed[trackId] = true;
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

  var screenXfo = {
    tr: new FABRIC.Vec2(0, 0),
    sc: new FABRIC.Vec2(1, 1),
    fitToScreen:function(){
      if(fitEditorToKeyRanges){
        timeRange = new FABRIC.Vec2(0, 0);
        for (var i = 0; i < trackCount; i++) {
          var keys = tracksData.tracks[i].keys;
          if(keys.length <= 1){
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
      if(!fitEditorToKeyRanges){
        this.sc = new FABRIC.Vec2(windowWidth/(timeRange.y - timeRange.x), -(windowHeight) / (yRange.y - yRange.x));
      }else{
        this.sc = new FABRIC.Vec2(windowWidth/(timeRange.y - timeRange.x), -(windowHeight - 40) / (yRange.y - yRange.x));
      }
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
  screenXfo.fitToScreen();

  var drawTrackCurve = function(trackId) {
    var trackData = tracksData.tracks[trackId];
    var trackGroup = trackGroups[trackId];
    trackGroup.removeAllChildren();
    var path = trackGroup.createPath().addClass('CurvePath').stroke(trackData.color);
    var setPathCurveValues = function(curveData) {
      var val = screenXfo.toScreenSpace(new FABRIC.Vec2(0, curveData[0]))
      var pathValues = ['M', val.x, val.y, 'L', 100, 100, 300, 200];
      try{
      for (var i = 0; i < curveData.length; i++) {
        var t = ((i/curveData.length)*(timeRange.y - timeRange.x))+timeRange.x;
        val = screenXfo.toScreenSpace(new FABRIC.Vec2(t, curveData[i]));
        pathValues[(i * 2) + 4] = val.x;
        pathValues[(i * 2) + 5] = val.y;
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
              keyData.time = keyGsVal.x;
              keyData.value = keyGsVal.y;
              var deltat, i=0;
              if(keyIndex > 0){
                deltat = keyData.time - prevKey.time;
                if(isBezier) {
                  prevKey.outtangent.x = deltat * tangentNormalizedValues[i++];
                  prevKey.outtangent.y = prevKey.outtangent.x * tangentNormalizedValues[i++];
                  keyData.intangent.x = deltat * tangentNormalizedValues[i++];
                  keyData.intangent.y = keyData.intangent.x * tangentNormalizedValues[i++];
                  prevKey.setOutTan(prevKey.outtangent, true);
                  keyData.setInTan(keyData.intangent);
                }
              }
              if(keyIndex < trackData.keys.length-1){
                deltat = nextKey.time - keyData.time;
                if(isBezier) {
                  keyData.outtangent.x = deltat * tangentNormalizedValues[i++];
                  keyData.outtangent.y = keyData.outtangent.x * tangentNormalizedValues[i++];
                  nextKey.intangent.x = deltat * tangentNormalizedValues[i++];
                  nextKey.intangent.y = nextKey.intangent.x * tangentNormalizedValues[i++];
                  keyData.setOutTan(keyData.outtangent);
                  nextKey.setInTan(nextKey.intangent, true);
                }
              }
              tracksData.tracks[trackId].keys[keyIndex] = keyData;
              animationLibraryNode.setTrackSet(tracksData, trackSetId);
              updateCurve();
              scene.redrawAllViewports();
            });
        var keyNode = keyGroupNode.createRect().translate(-4, -4).size(8, 8).stroke(keyColor);
        
        if(isBezier) {
          ///////////////////////////////////////////////
          // In Tangent
          if(keyData.time > timeRange.x){
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
                    keyData.setOutTan(keyData.outtangent);
                  }
                  keyData.setInTan(keyData.intangent);
                  tracksData.tracks[trackId].keys[keyIndex] = keyData;
                  animationLibraryNode.setTrackSet(tracksData, trackSetId);
                  updateCurve();
                  scene.redrawAllViewports();
                });
          }
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
          if(keyData.time < timeRange.y){
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
                    keyData.setInTan(keyData.intangent);
                  }
                  keyData.setOutTan(keyData.outtangent);
                  
                  tracksData.tracks[trackId].keys[keyIndex] = keyData;
                  animationLibraryNode.setTrackSet(tracksData, trackSetId);
                  updateCurve();
                  scene.redrawAllViewports();
                });
          }
          keyData.setOutTan = function(outtangent, setKeyData){
            if(outTanNode){
              var outTanSsVal = screenXfo.toScreenSpace(outtangent, true);
              outTanNode.translate(outTanSsVal.x - 3, outTanSsVal.y - 3)
              outTanLineNode.attr('d', 'M 0 0 L ' + outTanSsVal.x + ' ' + outTanSsVal.y);
            }
            if(setKeyData){
              keyData.outtangent = outtangent;
              
              tracksData.tracks[trackId].keys[keyIndex] = keyData;
              animationLibraryNode.setTrackSet(tracksData, trackSetId);
            }
          }
        }
      }
    }
    if (window.drawKeys !== false) {
      var trackData = tracksData.tracks[trackId];
      for (var i = 0; i < trackData.keys.length; i++) {
        drawKey(i, trackData.keys[i]);
      }
    }
  }
    
  var drawTrackCurves = function(){
    for (var i = 0; i < trackCount; i++) {
      // Note: this array of checkboxes is a bit hackey.
      // I couldn't get the CSS selectors to work with the 'id' I have used.
      // (Probably bcause the id contains '.')
      if(trackDisplayed[i]){
        drawTrackCurve(i);
      }
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
  
    window.updateGraphTimeStripe = function() {
      timeStripeGroupNode.translate((scene.animation.getTime() + screenXfo.tr.x)  * screenXfo.sc.x, windowHeight * -0.5);
    }
  }
  
  var updateTimeRange = function(){
    trackDisplayNode.setTimeRange(timeRange);
    curvesData = trackDisplayNode.getCurveData();
    
    if(options.timeStripe){
      updateGraphTimeStripe();
    }
    drawTrackCurves();
  }
  
  var fitCurveEditorToWindow = function(){
    var newWindowWidth = $('#viewer').width();
    var newWindowHeight = $('#viewer').height();
    
    // Occasionaly when the window is opened, it has a negative width and
    // then we get sent a resize event. Here, we re-fit the curve to the screen.
    var validWindowWidth = (windowWidth < 0);
    if(validWindowWidth){
      timeRange.y += ( newWindowWidth - windowWidth ) / screenXfo.sc.x;
    }
    windowWidth = newWindowWidth;
    windowHeight = newWindowHeight;
    if(!validWindowWidth){
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
      var zoomDist = evt.wheelDelta * 0.1;
      timeRange.x += zoomDist / screenXfo.sc.x;
      timeRange.y -= zoomDist / screenXfo.sc.x;
      yRange.x -= zoomDist / screenXfo.sc.y;
      yRange.y += zoomDist / screenXfo.sc.y;
      screenXfo.sc.x = windowWidth/(timeRange.y - timeRange.x);
      screenXfo.sc.y = -(windowHeight - 40) / (yRange.y - yRange.x)
      screenXfo.tr.x = -timeRange.x;
    // screenXfo.tr.y = (yRange.y + yRange.x) * -0.5;  
      updateTimeRange();
    }
    
    rootDomNode.addEventListener('mouseover', function(){
      document.addEventListener('mousewheel', mouseWheelZoomFn, false);
    });
    
    rootDomNode.addEventListener('mouseout', function(){
      document.removeEventListener('mousewheel', mouseWheelZoomFn, false);
    });
  }
  
  return {
    resize: function(){
      fitCurveEditorToWindow();
    },
    redraw: function(){
      tracksData = animationLibraryNode.getTrackSet(trackSetId);
      updateTimeRange();
    }
  }
};
