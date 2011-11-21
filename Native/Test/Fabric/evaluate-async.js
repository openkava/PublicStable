FABRIC = require('Fabric').createClient();

o = FABRIC.DG.createOperator("o");
o.setSourceCode("inline", "operator entry(in Size index, io Scalar result) { for (Size i=0; i<1000; ++i) result = log(1+index+i); }");
o.setEntryFunctionName("entry");

b = FABRIC.DG.createBinding("b");
b.setOperator(o);
b.setParameterLayout(["self.index","self.a"]);

n = FABRIC.DG.createNode("n");
n.addMember("a", "Scalar");
n.bindings.append(b);
n.setCount(10000);
n.evaluateAsync(function () {
  console.log("done!");
});

FABRIC.flush();
