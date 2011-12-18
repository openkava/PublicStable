FC = require("Fabric").createClient();

cv = FC.MR.createConstValue("Size", 10000);

ago = FC.KLC.createArrayGeneratorOperator("foo.kl", "operator foo(Size index, io Float64 output) { output = Float64(index+1); }", "foo");
ag = FC.MR.createArrayGenerator(cv, ago);

mo = FC.KLC.createMapOperator("bar.kl", "operator bar(Size index, Float64 input, io Float64 output) { output = 1.0 / (input * input); }", "bar");
m = FC.MR.createMap(ag, mo);

ro = FC.KLC.createReduceOperator("reduce.kl", "operator reduce(Size index, Float64 input, io Float64 result) { result += input; }", "reduce");
r = FC.MR.createReduce(m, ro);

console.log(r.produce());

FC.close();
