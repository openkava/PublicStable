
F = wrapFabricClient(createFabricClient());

var mapNamedObjectsToNames = function (namedObjects) {
  var result = {};
  for (var i in namedObjects)
    result[i] = namedObjects[i].getName();
  return result;
};

node = F.DG.createNode("node");
print(node.getType());
node.addMember("foo", "String");
printDeep(node.getMembers());
node.setCount(2);
print(node.getCount());
node.setData("foo", 0, "fred");
node.setData("foo", 1, "joe");
print(node.getData("foo", 0));
print(node.getData("foo", 1));

node2 = F.DG.createNode("node2");
node.addDependency( node2, "parent" );
printDeep(mapNamedObjectsToNames(node.getDependencies()));
