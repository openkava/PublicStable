#
#  Copyright 2010-2012 Fabric Engine Inc. All rights reserved.
#

import fabric
fabricClient = fabric.createClient()

op = fabricClient.DependencyGraph.createOperator("op")
op.setEntryFunctionName("load")
op.setSourceCode('\
require FabricHDR;\n\
\n\
struct Color\n\
{\n\
  Scalar r;\n\
  Scalar g;\n\
  Scalar b;\n\
  Scalar a;\n\
};\n\
\n\
operator load( io String url, io FabricResource resource )\n\
{\n\
  report "Loaded " + url + " (mime type " + resource.mimeType + ")";\n\
  report "HDR data size is " + resource.data.dataSize();\n\
  Size imageWidth, imageHeight;\n\
  Color imagePixels[];\n\
  FabricHDRDecode( resource.data.data(), resource.data.dataSize(), imageWidth, imageHeight, imagePixels );\n\
  report "Image dimensions are "+imageWidth+" by "+imageHeight;\n\
  report "Image pixels size is "+imagePixels.size;\n\
}\n\
')

binding = fabricClient.DG.createBinding()
binding.setOperator(op)
binding.setParameterLayout([
  "loadnode.url",
  "loadnode.resource"
])

rlnode = fabricClient.DependencyGraph.createResourceLoadNode("rlnode")
rlnode.setData("url", 0, "testfile://sample.hdr")

node = fabricClient.DependencyGraph.createNode("node")
node.setDependency(rlnode, "loadnode")
node.bindings.append(binding)
node.evaluate()

fabricClient.close()
