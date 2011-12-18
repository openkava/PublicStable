FC = require("Fabric").createClient();

cv = FC.MR.createConstValue("Size", 100);

ago = FC.KLC.createArrayGeneratorOperator("foo.kl", "operator foo(Size index, io Float64 output) { report index; output = Float64(index+1); }", "foo");
ag = FC.MR.createArrayGenerator(cv, ago);

ro = FC.KLC.createReduceOperator("reduce.kl", "operator reduce(Float64 input, io Float64 result, Size index, Size count) { report 'index=' + index + ' count=' + count; result += input; }", "reduce");
r = FC.MR.createReduce(ag, ro);

console.log(r.produce());

FC.close();
