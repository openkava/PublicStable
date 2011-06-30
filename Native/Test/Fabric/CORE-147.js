FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

op = FABRIC.DependencyGraph.createOperator( "op" );
op.setEntryFunctionName('entry');
op.setSourceCode("operator entry( io Scalar input ) { String foo; report foo; }");

binding = FABRIC.DependencyGraph.createBinding();
binding.setOperator( op );
binding.setParameterLayout( [ "self.input" ] );

node = FABRIC.DependencyGraph.createNode( "node" );
node.addMember( "input", "Scalar" );
node.bindings.append( binding );
node.setData( "input", 17 );
node.evaluate();

FC.dispose();
