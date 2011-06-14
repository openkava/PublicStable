
FABRIC = wrapFabricClient(createFabricClient());

op = FABRIC.DependencyGraph.createOperator("op");
op.setEntryFunctionName("load");
op.setSourceCode('\
struct Color\n\
{\n\
  Scalar r;\n\
  Scalar g;\n\
  Scalar b;\n\
  Scalar a;\n\
};\n\
\n\
operator load( io String url, io String mimeType, io Data hdrData, io Size hdrDataSize )\n\
{\n\
  report "Loaded " + url + " (mime type " + mimeType + ")";\n\
  report "HDR data size is "+hdrDataSize;\n\
  Size imageWidth, imageHeight;\n\
  Color imagePixels[];\n\
  FabricHDRDecode( hdrData, hdrDataSize, imageWidth, imageHeight, imagePixels );\n\
  report "Image dimensions are "+imageWidth+" by "+imageHeight;\n\
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

rle = FABRIC.DependencyGraph.createResourceLoadEvent("rle", "file:sample.hdr");
rle.appendEventHandler(eh);
rle.start();
