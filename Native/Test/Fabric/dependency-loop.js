FC = createFabricClient();
FABRIC = FC.wrapFabricClient(FC);

node1 = FABRIC.DependencyGraph.createNode("one");
node2 = FABRIC.DependencyGraph.createNode("two");

try {
  node1.addDependency( node1, "reflex" );
  FABRIC.flush();
}
catch (e) {
  print( e );
}

try {
  node1.addDependency( node2, "one" );
  node2.addDependency( node1, "two" );
  FABRIC.flush();
}
catch (e) {
  print( e );
}

FABRIC.flush();
FC.dispose();
