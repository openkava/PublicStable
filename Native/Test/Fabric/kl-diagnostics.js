FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

op = FABRIC.DependencyGraph.createOperator( "op" );
print( op.getDiagnostics().length );

op.setSourceCode("operator entry( io Scalar input, io Scalar output ) { foo; }");
print( op.getDiagnostics().length );
print( op.getDiagnostics()[0].desc );

op.setSourceCode("operator entry( io Scalar input, io Scalar output ) { output = 2 * input; }");
print( op.getDiagnostics().length );

FABRIC.flush();
FC.dispose();
