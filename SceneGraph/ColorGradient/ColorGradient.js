
var constructColorGradient = function(domRootID, colorKeyTrackNode, options){
  
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
  gradientRect.attr('stroke-width', 2);

  var gradientKeyData = colorKeyTrackNode.getTrackKeys();
  var selectedKeyIndex = -1;
  var addKey = function(index, param, color){
    var gradientKey = gradient.addKey(param, color);
    var keyGroupNode = keysHolderGroup.createGroup()
      .translate(param * windowWidth, 0)
      .selectable({ multiSelectable: false })
      .addOnSelectCallback(
        function(evt) {
          keyRect.attr('stroke', "white");
          keyRect.attr('stroke-width', 3);
          gradientWidget.fireEvent('keyclicked', {
            key: gradientKey,
            keycolor: color
          });
          selectedKeyIndex = index;
        })
      .addOnDeselectCallback(
        function(evt) {
          keyRect.attr('stroke', "black");
          keyRect.attr('stroke-width', 1);
        })
      .draggable({ mouseButton: 0, axis: 'X', containment:gradientRect })
      .addOnDragCallback(
        function(evt) {
          var time = evt.localPos.x / windowWidth;
          gradientKey.setParam(time);
          gradientKeyData[index].time = time;
          colorKeyTrackNode.setTrackKeys(0, gradientKeyData);
          gradientWidget.fireEvent('gradientchanged');
        });
    
    var keyRect = keyGroupNode.createRect().translate(-2, 0).size(6, windowHeight * 0.4 ).stroke("black").attr('fill', color.toHex());
    if(index==selectedKeyIndex){
      keyRect.attr('stroke', "white");
      keyRect.attr('stroke-width', 3);
    }
    keyRect.attr('stroke-width', 1);
    var gradientKeySetColor = gradientKey.setColor;
    gradientKey.setColor = function(col){
      gradientKeySetColor(col);
      keyRect.attr('fill', col.toHex());
      gradientKeyData[index].value = col;
      colorKeyTrackNode.setTrackKeys(0, gradientKeyData);
      
      gradientWidget.fireEvent('gradientchanged');
    }
  }
  
  var displayGradient = function(){
    keysHolderGroup.removeAllChildren();
    gradient.clearKeys();
    for(var i=0; i<gradientKeyData.length; i++){
      addKey(i, gradientKeyData[i].time, gradientKeyData[i].value);
    }
  }
  gradientRect.elem.addEventListener('mousedown', function(evt) {
    var newKeyT = evt.offsetX/windowWidth;
    console.log("add key:"+ newKeyT);
    
    var insertId = 0;
    for(var i=0; i<gradientKeyData.length; i++){
      if(gradientKeyData[i].time > newKeyT){
        var newKeyVal;
        if(i>0){
          var interp = (newKeyT - gradientKeyData[i-1].time) / (gradientKeyData[i].time - gradientKeyData[i-1].time);
          newKeyVal = gradientKeyData[i-1].value.linearInterpolate(gradientKeyData[i].value, interp);
        }else{
          newKeyVal = gradientKeyData[i];
        }
        gradientKeyData.splice(i, 0, FABRIC.Animation.colorKeyframe(newKeyT, newKeyVal));
        selectedKeyIndex = i;
        break;
      }
    }
    if(i==gradientKeyData.length){
      selectedKeyIndex = gradientKeyData.length;
      gradientKeyData.append(FABRIC.Animation.colorKeyframe(newKeyT, gradientKeyData[gradientKeyData.length-1].value));
    }
    colorKeyTrackNode.setTrackKeys(0, gradientKeyData);
    displayGradient();
  });
  
  displayGradient();
  
  
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
