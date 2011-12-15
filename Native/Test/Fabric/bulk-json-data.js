FABRIC = require('Fabric').createClient();

node = FABRIC.DependencyGraph.createNode("foo");
node.addMember( "foo", "Integer", 17 );
node.addMember( "bar", "String", "red" );
node.addMember( "baz", "Scalar[]", [3.141] );
console.log(node.getBulkDataJSON());
node.setBulkDataJSON(JSON.stringify({foo:[42],bar:["fred"],baz:[[4.5,3.6]]}));
console.log(node.getBulkDataJSON());
node.setCount(2);
console.log(node.getBulkDataJSON());

FABRIC.flush();