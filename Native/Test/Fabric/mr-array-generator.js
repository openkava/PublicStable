FC = require("Fabric").createClient();

cv = FC.MR.createConstValue("Size", 10);

ago = FC.KLC.createArrayGeneratorOperator("foo.kl", "operator foo(Size index, io Scalar output) { output = Scalar(index); }", "foo");
ag = FC.MR.createArrayGenerator(cv, ago);

mo = FC.KLC.createMapOperator("bar.kl", "operator bar(Scalar input, io Scalar output) { output = input * input; }", "bar");
m = FC.MR.createMap(ag, mo);

var count = m.getCount();
console.log("count = " + count);
for (var i=0; i<count; ++i)
  console.log("produce("+i+") = "+m.produce(i));

FC.close();
