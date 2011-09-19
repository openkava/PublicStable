FC = createFabricClient();
FABRIC = FC.wrapFabricClient(FC);

opOne = FABRIC.DependencyGraph.createOperator( "opOne" );
opOne.setEntryFunctionName('entry');
opOne.setSourceCode("operator entry( io Scalar input, io Scalar output ) { output = 2 * input; }");

bindingOne = FABRIC.DependencyGraph.createBinding();
bindingOne.setOperator( opOne );
bindingOne.setParameterLayout( [ "self.input", "self.output" ] );

opTwo = FABRIC.DependencyGraph.createOperator( "opTwo" );
opTwo.setEntryFunctionName('entry');
opTwo.setSourceCode("operator entry( io Scalar input, io Scalar output ) { output = 3 * input; }");

bindingTwo = FABRIC.DependencyGraph.createBinding();
bindingTwo.setOperator( opTwo );
bindingTwo.setParameterLayout( [ "self.input", "self.output" ] );

node = FABRIC.DependencyGraph.createNode( "parent" );
node.addMember( "input", "Scalar" );
node.addMember( "output", "Scalar" );
node.setCount( 2 );
node.setData( 'input', 0, 3 );
node.setData( 'input', 1, 7 );
node.setData( 'output', 0, 0 );
node.setData( 'output', 1, 0 );
node.evaluate();
printDeep(node.getBulkData());
print("bindings.length = " + node.bindings.getLength());

node.setData( 'output', 0, 0 );
node.setData( 'output', 1, 0 );
node.bindings.append( bindingOne );
node.evaluate();
printDeep(node.getBulkData());
print("bindings.length = " + node.bindings.getLength());

node.setData( 'output', 0, 0 );
node.setData( 'output', 1, 0 );
node.bindings.remove(0);
node.evaluate();
printDeep(node.getBulkData());
print("bindings.length = " + node.bindings.getLength());

node.setData( 'output', 0, 0 );
node.setData( 'output', 1, 0 );
node.bindings.append(bindingTwo);
node.evaluate();
printDeep(node.getBulkData());
print("bindings.length = " + node.bindings.getLength());

node.setData( 'output', 0, 0 );
node.setData( 'output', 1, 0 );
node.bindings.remove(0);
node.evaluate();
printDeep(node.getBulkData());
print("bindings.length = " + node.bindings.getLength());

FABRIC.flush();
FC.dispose();
