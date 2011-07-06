FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

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
operator load( io String url, io String mimeType, io Data exrData, io Size exrDataSize )\n\
{\n\
  report "Loaded " + url + " (mime type " + mimeType + ")";\n\
  report "EXR data size is "+exrDataSize;\n\
  Size imageWidth, imageHeight;\n\
  Color imagePixels[];\n\
  FabricEXRDecode( exrData, exrDataSize, imageWidth, imageHeight, imagePixels );\n\
  report "Image dimensions are "+imageWidth+" by "+imageHeight;\n\
  report "Image pixels size is "+imagePixels.size;\n\
}\n\
');

binding = FABRIC.DG.createBinding();
binding.setOperator(op);
binding.setParameterLayout([
  "resource.url",
  "resource.mimeType",
  "resource.data",
  "resource.dataSize"
]);

eh = FABRIC.DependencyGraph.createEventHandler("eh");
eh.preDescendBindings.append(binding);

rle = FABRIC.DependencyGraph.createResourceLoadEvent("exr", "file:sample.exr");
rle.appendEventHandler(eh);
rle.start();

FABRIC.flush();
FC.dispose();
