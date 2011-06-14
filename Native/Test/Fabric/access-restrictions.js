
FABRIC = wrapFabricClient(createFabricClient());

op = FABRIC.DependencyGraph.createOperator("foo");
op.setEntryFunctionName("entry");
op.setSourceCode("\
operator entry( io Integer element, io Integer slice[] )\n\
{\n\
}\n\
");

binding = FABRIC.DG.createBinding();
binding.setOperator(op);
binding.setParameterLayout(["self.x", "self.x[]"]);

node = FABRIC.DependencyGraph.createNode( "bar" );
node.addMember( "x", "Integer" );
node.setCount( 2 );
node.bindings.append(binding);
var errors = node.getErrors();
if ( errors.length > 0 ) {
  for ( var i in errors )
    print( errors[i] );
}
else {
  node.evaluate();
}
