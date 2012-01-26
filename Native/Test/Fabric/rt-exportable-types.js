FC = require("Fabric").createClient();

var node = FC.DG.createNode("foo");
try {
  node.addMember("bar", "Data");
  FC.flush();
}
catch (e) {
  console.log("node.addMember: " + e);
}

FC.close();
