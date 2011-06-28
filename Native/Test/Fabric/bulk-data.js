FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

node = FABRIC.DependencyGraph.createNode("foo");
node.addMember( "foo", "Integer", 17 );
node.addMember( "bar", "String", "red" );
node.addMember( "baz", "Scalar[]", [3.141] );
printDeep(node.getBulkData());
printDeep(node.getSliceBulkData(0));
node.setBulkData({foo:[42],bar:["fred"],baz:[[4.5,3.6]]});
printDeep(node.getBulkData());
printDeep(node.getSliceBulkData(0));
node.setCount(2);
printDeep(node.getBulkData());
printDeep(node.getSliceBulkData(0));
printDeep(node.getSliceBulkData(1));
printDeep(node.getSlicesBulkData([1,0,1]));
node.setSliceBulkData(1, {foo:18,bar:"hello",baz:[]});
printDeep(node.getBulkData());
printDeep(node.getSliceBulkData(0));
printDeep(node.getSliceBulkData(1));
printDeep(node.getSlicesBulkData([1,0,1]));

FABRIC.flush();
FC.dispose();
