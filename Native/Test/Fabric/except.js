FC = createFabricClient();
F = wrapFabricClient(FC);

o = F.DG.createOperator("op");
o.setEntryFunctionName("entry");
o.setSourceCode("\
operator entry()\n\
{\n\
  report 'before test exception';\n\
  String string = 'test exception';\n\
  throw(string);\n\
  report 'after test exception';\n\
}\n\
");

binding = F.DependencyGraph.createBinding();
binding.setOperator( o );
binding.setParameterLayout([]);

eh = F.DG.createEventHandler("eh");
eh.preDescendBindings.append( binding );

e = F.DG.createEvent("e");
e.appendEventHandler( eh );
e.fire();

F.flush();
FC.dispose();
