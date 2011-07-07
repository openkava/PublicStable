FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

op = FABRIC.DependencyGraph.createOperator("op");
op.setEntryFunctionName("load");
op.setSourceCode('\
struct RGBA\n\
{\n\
  Byte r;\n\
  Byte g;\n\
  Byte b;\n\
  Byte a;\n\
};\n\
\n\
operator load( io String url, io String mimeType, io Data pngData, io Size pngDataSize )\n\
{\n\
  report "Loaded " + url + " (mime type " + mimeType + ")";\n\
  report "PNG data size is "+pngDataSize;\n\
  Size imageWidth, imageHeight;\n\
  RGBA imagePixels[];\n\
  FabricPNGDecode( pngData, pngDataSize, imageWidth, imageHeight, imagePixels );\n\
  report "Image dimentions are "+imageWidth+" by "+imageHeight;\n\
  report "Image pixels size is "+imagePixels.size;\n\
}\n\
');

binding = FABRIC.DependencyGraph.createBinding();
binding.setOperator( op );
binding.setParameterLayout( [
  "resource.url",
  "resource.mimeType",
  "resource.data",
  "resource.dataSize"
] );

eh = FABRIC.DependencyGraph.createEventHandler("eh");
eh.preDescendBindings.append(binding);

rle = FABRIC.DependencyGraph.createResourceLoadEvent("rle", "file:tomatoes_960_640.png");
rle.appendEventHandler(eh);
rle.start();

FABRIC.flush();
FC.dispose();
