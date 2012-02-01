import fabric
fabricClient = fabric.createClient()

op = fabricClient.DependencyGraph.createOperator( "op" )
op.setEntryFunctionName('entry')
op.setSourceCode("operator entry( io Scalar input ) { String foo; report foo; }")

binding = fabricClient.DependencyGraph.createBinding()
binding.setOperator( op )
binding.setParameterLayout( [ "self.input" ] )

node = fabricClient.DependencyGraph.createNode( "node" )
node.addMember( "input", "Scalar" )
node.bindings.append( binding )
node.setData( "input", 17 )
node.evaluate()

fabricClient.close()
