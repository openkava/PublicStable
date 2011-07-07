FC = createFabricClient();
F = wrapFabricClient(FC);

o = F.DG.createOperator("op");
o.setEntryFunctionName("entry");
o.setSourceCode("\
operator entry( io String string )\n\
{\n\
  string = 'foobar';\n\
}\n\
");
F.flush();

binding = F.DependencyGraph.createBinding();
binding.setOperator( o );
binding.setParameterLayout( [ "self.string" ] );
F.flush();

eh = F.DG.createEventHandler("eh");
eh.addMember( "string", "String" );
eh.preDescendBindings.append( binding );
F.flush();

e = F.DG.createEvent("e");
e.appendEventHandler( eh );
e.fire();
print( eh.getData( "string" ) );

F.flush();
FC.dispose();
