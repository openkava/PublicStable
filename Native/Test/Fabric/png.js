FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

op = FABRIC.DependencyGraph.createOperator("op");
op.setEntryFunctionName("load");
op.setSourceCode('\
use FabricPNG;\n\
\n\
struct RGBA\n\
{\n\
  Byte r;\n\
  Byte g;\n\
  Byte b;\n\
  Byte a;\n\
};\n\
\n\
operator load( io String url, io FabricResource resource )\n\
{\n\
  report "Loaded " + url + " (mime type " + resource.mimeType + ")";\n\
  report "PNG data size is " + resource.dataSize;\n\
  Size imageWidth, imageHeight;\n\
  RGBA imagePixels[];\n\
  FabricPNGDecode( resource.data, resource.dataSize, imageWidth, imageHeight, imagePixels );\n\
  report "Image dimentions are "+imageWidth+" by "+imageHeight;\n\
  report "Image pixels size is "+imagePixels.size;\n\
}\n\
');

binding = FABRIC.DG.createBinding();
binding.setOperator(op);
binding.setParameterLayout([
  "loadnode.url",
  "loadnode.resource"
]);

rlnode = FABRIC.DependencyGraph.createResourceLoadNode("rlnode");
rlnode.setData("url", 0, "file:tomatoes_960_640.png");

node = FABRIC.DependencyGraph.createNode("node");
node.addDependency(rlnode, "loadnode");
node.bindings.append(binding);
node.evaluate();

FABRIC.flush();
FC.dispose();
