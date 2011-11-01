
var constructColorGradient = function(domRootID, keyframeTrackNode, options){
  
  var keyColor = FABRIC.rgb(.0, .0, .0);
  
  options = options ? options : {};
  options.timeStripe = options.timeStripe!=undefined ? options.timeStripe : true;
  options.draggable = options.draggable!=undefined ? options.draggable : true;
  options.zoomable = options.zoomable!=undefined ? options.zoomable : true;
  options.timeRange = options.timeRange!=undefined ? options.timeRange : new FABRIC.RT.Vec2(0, 100);
  
  var rootDomNode = document.getElementById(domRootID);
  var windowWidth = rootDomNode.clientWidth;
  var windowHeight = rootDomNode.clientHeight;
  
  var svgRoot = FABRIC.createSVGRootElem(domRootID);
  var gradient = svgRoot.createLinearGradient().id('grad1');
  var gradientRect = svgRoot.createRect().size(windowWidth, windowHeight).attr('fill', "url(#grad1)");
  var graphCenterGroup = svgRoot.createGroup().id('graphCenterGroup').translate(0, windowHeight * 0.5);
  var keysHolderGroup = graphCenterGroup.createGroup().id('keysHolderGroup');


  gradientRect.attr('stroke', "black");
  gradientRect.attr('stroke-width', 3);


  var addKey = function(param, color){
    var gradientKey = gradient.addKey(param, color);
    var keyGroupNode = keysHolderGroup.createGroup()
      .translate(param * windowWidth, 0)
      .button()
      .addOnMouseDownCallback(
        function(evt) {
          gradientWidget.fireEvent('keyclicked', {
            key: gradientKey,
            keycolor: color
          });
        })
      .draggable({ mouseButton: 0, axis: 'X', containment:gradientRect })
      .addOnDragCallback(
        function(evt) {
          gradientKey.setParam(evt.localPos.x / windowWidth);
        });
    
    var keyRect = keyGroupNode.createRect().translate(-2, windowHeight * -0.45).size(4, windowHeight * 0.9 ).stroke(keyColor).attr('fill', color.toHex());
    
    var gradientKeySetColor = gradientKey.setColor;
    gradientKey.setColor = function(col){
      gradientKeySetColor(col);
      keyRect.attr('fill', col.toHex());
    }
  }
  
  addKey(0, new FABRIC.Color(1, 1, 0.3));
  addKey(0.3, new FABRIC.Color(1, 0.3, 0));
  addKey(0.45, new FABRIC.Color(1, 1, 0.3));
  addKey(0.75, new FABRIC.Color(1,  0.3, 0));
  addKey(1, new FABRIC.Color(1, 1, 0.3));
  
  /*
  
  ///////////////
  var trackDisplayNodes = [];
  var trackCount = keyframeTrackNode.getTrackCount();
  var timeRange = new FABRIC.Vec2(0, 0);
  var yRange = new FABRIC.Vec2(0, 0);
  var curvesData = [];
  var trackCurves = [];
  for (var i = 0; i < trackCount; i++) {
    var trackData = keyframeTrackNode.getTrackData(i);
    trackDisplayNodes[i] = scene.constructNode('TrackDisplay', {
      animationTrackNode: keyframeTrackNode,
      trackIndex: i,
      timeRange: options.timeRange
    });
  }
  
  var screenXfo = {
    tr: new FABRIC.Vec2(0, 0),
    sc: new FABRIC.Vec2(1, 1),
    fitToScreen:function(){
      for (var i = 0; i < trackCount; i++) {
        var keys = keyframeTrackNode.getTrackKeys(i);
        if (timeRange.x > keys[0].time) {
          timeRange.x = keys[0].time;
        }
        if (timeRange.y < keys[keys.length - 1].time) {
          timeRange.y = keys[keys.length - 1].time;
        }
      }
      var getCurveYRange = function(curveData) {
        for (var i = 1; i < curveData.time.length; i++) {
          if (yRange.x > curveData.value[i]) {
            yRange.x = curveData.value[i];
          }
          if (yRange.y < curveData.value[i]) {
            yRange.y = curveData.value[i];
          }
        }
      }
      for (var i = 0; i < trackCount; i++) {
        trackDisplayNodes[i].setTimeRange(timeRange);
        curvesData[i] = trackDisplayNodes[i].getCurveData();
        if(i==0){
          yRange = new FABRIC.Vec2(curvesData[0].value[0], curvesData[0].value[0]);
        }
        getCurveYRange(curvesData[i]);
      }
      this.sc = new FABRIC.Vec2(windowWidth/(timeRange.y - timeRange.x), -(windowHeight - 40) / (yRange.y - yRange.x));
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
  
  
  var updateTimeRange = function(){
    for (var i = 0; i < trackCount; i++) {
      trackDisplayNodes[i].setTimeRange(timeRange);
      curvesData[i] = trackDisplayNodes[i].getCurveData();
    }
    if(options.timeStripe){
      updateGraphTimeStripe();
    }
    keysHolderGroup.removeAllChildren();
    drawTrackCurves();
  }
  
  var fitCurveEditorToWindow = function(){
    var newWindowWidth = $('#viewer').width();
    var newWindowHeight = $('#viewer').height();
    
    timeRange.y += ( newWindowWidth - windowWidth ) / screenXfo.sc.x;
    windowWidth = newWindowWidth;
    windowHeight = newWindowHeight;
    graphCenterGroup.translate(0, windowHeight * 0.5);
    graphBGRect.size(windowWidth, windowHeight);
    timeStripe.attr('d', 'M 5 0 L 5 ' + windowHeight);
    
    updateTimeRange();
  }
  
  if(options.draggable){
    var dragStartTimeRange;
    var dragStartOffset;
    graphBGRect.draggable( {mouseButton:1, delegateTranslateObj:false, highlight:false } )
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
  */
  
  // We store a map of arrays of event listener functions.
  var eventListeners = {};

  var gradientWidget = {
    resize: function(){
      fitCurveEditorToWindow();
    },
    
    addEventListener: function(type, fn) {
      if (!eventListeners[type]) {
        eventListeners[type] = [];
      }
      eventListeners[type].push(fn);
    },
    removeEventListener: function(type, fn) {
      if (eventListeners[type]) {
        var id = eventListeners[type].indexOf(fn);
        if (id !== -1) {
          eventListeners[type].splice(id, 1);
        }
      }
    },
    fireEvent: function(type, evt) {
      for (i in eventListeners[type]) {
        if (eventListeners[type].hasOwnProperty(i)) {
          eventListeners[type][i].call(this, evt);
        }
      }
    }
  };
  return gradientWidget;  
};
