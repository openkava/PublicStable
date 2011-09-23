FC = createFabricClient();
F = FC.wrapFabricClient(FC);

o = F.DG.createOperator("op");
o.setSourceCode("\
operator entry(io Size a, io String b) {\n\
  report 'Hello ' + a + ' ' + b;\n\
}\n\
");
o.setEntryFunctionName("entry");

b = F.DG.createBinding();
b.setOperator(o);
b.setParameterLayout([
  "self.foo",
  "self.bar"
]);

n = F.DG.createNode("node");
n.addMember("foo", "String");
n.addMember("bar", "Size");
n.bindings.append(b);
printDeep(n.getErrors());

F.flush();
FC.dispose();
