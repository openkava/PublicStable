
FABRIC = wrapFabricClient(createFabricClient());

node = FABRIC.DependencyGraph.createNode("foo");
node.addMember( "foo", "Integer[5]" );
node.setData( "foo", [6,4,7,2,1] );
print( node.getDataSize( "foo", 0 ) );
print( node.getDataElement( "foo", 0, 3 ) );
