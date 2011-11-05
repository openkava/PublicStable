
var constructHistogram = function(domRootID, options){
  
  var keyColor = FABRIC.rgb(.0, .0, .0);
  
  options = options ? options : {};
  options.draggable = options.draggable!=undefined ? options.draggable : true;
  options.zoomable = options.zoomable!=undefined ? options.zoomable : true;
  options.rangeX = options.rangeX!=undefined ? options.rangeX : new FABRIC.RT.Vec2(0, 100);
  
  var rootDomNode = document.getElementById(domRootID);
  var windowWidth = rootDomNode.clientWidth;
  var windowHeight = rootDomNode.clientHeight;
  
  var svgRoot = FABRIC.createSVGRootElem(domRootID);
//  svgRoot.attr('style', "position:relative; top:-"+windowHeight+"px;z-index:0");
  
  var polygon = svgRoot.createPolygon();

  polygon.attr('stroke', "black");
  polygon.attr('stroke-width', 0.5);
  polygon.attr('fill', "#33CC00");

  var generateGraph = function(histogramData){
    polygon.addPoint(0, windowHeight);
    polygon.addPoint(0, windowHeight*0.9);
    var numSamples = windowWidth;
    for(var i=0; i<numSamples; i++){
      var x = Math.round((i/numSamples)*windowWidth);
      var y = (1.0 - histogramData[Math.round((i/numSamples)*histogramData.length)]) * (windowHeight*0.9);
      polygon.addPoint(x, y);
    };
    polygon.addPoint(windowWidth, windowHeight*0.9);
    polygon.addPoint(windowWidth, windowHeight);
    polygon.updateShape();
  }
  var histogramWidget = {
    setData:function(data){
      generateGraph(data);
    }
  };
  return histogramWidget;  
};
