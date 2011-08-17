FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

var MyStruct = function( i, s ) {
  if ( typeof i === "number" && typeof s === "number" ) {
    this.i = i;
    this.s = s;
  }
  else if ( i === undefined && s === undefined ) {
    this.i = 0;
    this.s = 0.0;
  }
  else throw "invalid use of constructor";
};
myStructDesc = {
  members: {
    i: "Integer",
    s: "Scalar"
  },
  constructor: MyStruct
};
FABRIC.RegisteredTypesManager.registerType( "MyStruct", myStructDesc );

op = FABRIC.DependencyGraph.createOperator( "op" );
op.setEntryFunctionName("entry");
op.setSourceCode("\n\
operator entry( io MyStruct arg<>[] )\n\
{\n\
  report 'parent data: ' + arg;\n\
}\n\
");
var diagnostics = op.getDiagnostics();
if ( diagnostics.length > 0 ) {
  for ( var i in diagnostics ) {
    print( diagnostics[i].line + ": " + diagnostics[i].desc );
  }
  print( "Full code:" );
  print( op.getFullSourceCode() );
}
else {
  parentNode = FABRIC.DependencyGraph.createNode( "parentNode" );
  parentNode.addMember( "msa", "MyStruct[]" );
  parentNode.setCount( 2 );
  parentNode.setData( "msa", 0, [new MyStruct( 42, 3.141 ), new MyStruct( 64, 5.67 ) ] );
  parentNode.setData( "msa", 1, [new MyStruct( 7, 2.718 )] );
  printDeep( parentNode.getData("msa", 0) );
  printDeep( parentNode.getData("msa", 1) );

	var binding = FABRIC.DG.createBinding();
	binding.setOperator(op);
	binding.setParameterLayout(["parent.msa[]"]);

  node = FABRIC.DependencyGraph.createNode( "node" );
  node.addDependency( parentNode, "parent" );
  node.bindings.append(binding);
	var errors = node.getErrors();
  if ( errors.length > 0 ) {
    for ( var i in errors ) {
      print( errors[i] );
    }
  }
  else {
    node.evaluate();
  }
}

FABRIC.flush();
FC.dispose();
