FC = createFabricClient();
FABRIC = FC.wrapFabricClient(FC);

for( var i = 1; i < 32; i++ )
{
  var klCode = "operator foo( ";
  for( var j = 0; j < i; j++ )
  {
    if( j > 0 ) klCode += ", ";
    klCode += "io Integer v" + j;
  }
  klCode += " ) { ";
  for( var j = 0; j < i; j++ )
    klCode += "v" + j + " -= " + (j+1) + "; ";
  klCode += " }";
  
  op = FABRIC.DependencyGraph.createOperator("foo_"+i);
  op.setEntryFunctionName("foo");
  op.setSourceCode(klCode);

  node = FABRIC.DependencyGraph.createNode("bar_"+i);

  var layout = [];
  for( var j = 0; j < i; j++ )
  {
    layout.push( "self.p" + j );
    node.addMember( "p" + j, "Integer" );
	node.setData( "p" + j, 0, j+1 );
  }

	binding = FABRIC.DG.createBinding();
	binding.setOperator(op);
	binding.setParameterLayout(layout);
  
  node.bindings.append(binding);
  node.evaluate();
  
  for( var j = 0; j < i; j++ )
  {
    if( node.getData( "p" + j, 0 ) != 0 )
      print( "Parameter " + j + " fails when evaluating " + i + " params" );
  }
}

FABRIC.flush();
FC.dispose();
