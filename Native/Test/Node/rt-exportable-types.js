FC = require("Fabric").createClient();

var node = FC.DG.createNode("foo");
node.addMember("bar", "ValueProducer<Integer>");
try {
  console.log(node.getData("bar"));
  FC.flush();
}
catch (e) {
  console.log("node.getData('bar'): " + e);
}

FC.close();
