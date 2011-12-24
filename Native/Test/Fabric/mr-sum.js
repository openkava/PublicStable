FC = require("Fabric").createClient();

cv = FC.MR.createConstValue("Size", 100);

ago = FC.KLC.createArrayGeneratorOperator("foo.kl", "operator foo(Size index, io Float64 output) { output = Float64(index+1); }", "foo");
ag = FC.MR.createArrayGenerator(cv, ago);

mo = FC.KLC.createArrayMapOperator("map.kl", "operator map(Float64 input, io Float64 output, Size index, Size count, Scalar multiplier) { output = input * multiplier; }", "map");
sv = FC.MR.createConstValue("Scalar", 3.14);
m = FC.MR.createArrayMap(ag, mo, sv);

ro = FC.KLC.createReduceOperator("reduce.kl", "operator reduce(Float64 input, io Float64 result, Size index, Size count, Scalar multiplier) { result += input * multiplier; }", "reduce");
sv = FC.MR.createConstValue("Scalar", 3.14);
r = FC.MR.createReduce(m, ro, sv);

console.log(r.produce());

FC.close();
