FC = createFabricClient();
F = FC.wrapFabricClient(FC);
//F = require('Fabric').createClient();

parentNode = F.DG.createNode('parentNode');
childNode = F.DG.createNode('childNode');
childNode.setDependency(parentNode, 'parent');
print(childNode.getDependencies());
try {
  childNode.removeDependency('notAParent');
  F.flush();
}
catch (e) {
  print("childNode.removeDependency('notAParent'): " + e);
}
childNode.removeDependency('parent');
print(childNode.getDependencies());

F.flush();
