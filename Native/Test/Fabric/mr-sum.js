FC = require("Fabric").createClient();

cv = FC.MR.createConstValue("Size", 100);

ago = FC.KLC.createArrayGeneratorOperator("foo.kl", "operator foo(Size index, io Float64 output) { report index; output = Float64(index+1); }", "foo");
ag = FC.MR.createArrayGenerator(cv, ago);

ro = FC.KLC.createReduceOperator("reduce.kl", "operator reduce(Float64 input, io Float64 result, Size index, Size count, Scalar multiplier) { report 'index=' + index + ' count=' + count + ' multiplier=' + multiplier; result += input * multiplier; }", "reduce");
sv = FC.MR.createConstValue("Scalar", 3.14);
r = FC.MR.createReduce(ag, ro, sv);

console.log(r.produce());

FC.close();
