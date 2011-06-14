
FABRIC = wrapFabricClient(createFabricClient());

node = FABRIC.DependencyGraph.createNode("foo");
node.addMember( "foo", "Integer[]" );
node.setData( "foo", 0, [1,6,7,9,8] );
print( node.getDataSize( "foo", 0 ) );
print( node.getDataElement( "foo", 0, 3 ) );
