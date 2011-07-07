FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

var node = FABRIC.DG.createNode("node");
node.addMember("foo","Scalar",3.141);
print(node.getData("foo", 0));
node.setData("foo", 0, 2.718);
print(node.getData("foo", 0));
node.setCount(2);
print(node.getData("foo", 1));

FABRIC.flush();
FC.dispose();
