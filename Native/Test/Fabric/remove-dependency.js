F = require('Fabric').createClient();

parentNode = F.DG.createNode('parentNode');
childNode = F.DG.createNode('childNode');
childNode.setDependency(parentNode, 'parent');
console.log(childNode.getDependencies());
try {
  childNode.removeDependency('notAParent');
  F.flush();
}
catch (e) {
  console.log("childNode.removeDependency('notAParent'): " + e);
}
childNode.removeDependency('parent');
console.log(childNode.getDependencies());

F.flush();
