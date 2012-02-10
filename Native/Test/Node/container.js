FABRIC = require('Fabric').createClient();

node = FABRIC.DependencyGraph.createResourceLoadNode("testDGNode");
node.addMember("intMember", "Integer");
node.addMember("stringMember", "String");

op = FABRIC.DependencyGraph.createOperator("op");
op.setEntryFunctionName("op");
op.setSourceCode('\
operator op(\n\
  io Container c,\n\
  io Integer i<>,\n\
  io String s<>\n\
  )\n\
{\n\
  report "Container string: " + c + " Count: " + c.getCount() + " Is valid: " + Boolean(c);\n\
  Container otherC;\n\
  report "Uninitialized Container string: " + otherC + " Is valid: " + Boolean(otherC);\n\
  report "Member sizes: " + i.size;\n\
  report "Members: " + i + " " + s;\n\
  otherC = c;\n\
  otherC.setCount(Size(3));\n\
  report "Container string: " + otherC + " Count: " + otherC.getCount() + " Is valid: " + Boolean(otherC);\n\
  report "To fix: i.size is wrong! LLVM doesn\'t know about aliasing!";\n\
  i[2] = 1;\n\
  s[2] = "test";\n\
  report "Member sizes: " + s.size;\n\
  report "Members: " + i + " " + s;\n\
  //No test accessing uninitialized Container\n\
  Container bad;\n\
  report bad.getCount();\n\
}\n\
');
if (op.getDiagnostics().length > 0 ) {
  console.log(JSON.stringify(op.getDiagnostics()));
}
opBinding = FABRIC.DG.createBinding();
opBinding.setOperator(op);
opBinding.setParameterLayout([
  "self",
  "self.intMember<>",
  "self.stringMember<>"
]);
node.bindings.append(opBinding);
try
{
  node.evaluate();
}
catch(e)
{
  console.log("Runtime eval error (uninit container access): " + e);
}

//Error test: we are not allowed to have io Container along with members elements
badOp = FABRIC.DependencyGraph.createOperator("badOp");
badOp.setEntryFunctionName("badOp");
badOp.setSourceCode('\
operator badOp(\n\
  io Container c,\n\
  io Integer i\n\
  )\n\
{\n\
}\n\
');
if (badOp.getDiagnostics().length > 0 ) {
  console.log(JSON.stringify(badOp.getDiagnostics()));
}
badOpBinding = FABRIC.DG.createBinding();
badOpBinding.setOperator(badOp);
badOpBinding.setParameterLayout([
  "self",
  "self.intMember"
]);
node.bindings.append(badOpBinding);
if ( node.getErrors().length > 0 )
  console.log("Bad bindings error: " + JSON.stringify((node.getErrors())));

FABRIC.close();
