FC = require('Fabric').createClient();

ag = FC.MR.createArrayGenerator(
  FC.MR.createConstValue("Size", 100),
  FC.KLC.createArrayGeneratorOperator(
    "gen.kl",
    "operator gen(Size index, io String output) { output = String(index+1); }",
    "gen"
    )
  );

console.log(
  FC.MR.createReduce(ag,
    FC.KLC.createReduceOperator(
      "reduce.kl",
      "operator reduce(String input, io String output) { if (output.length > 1) output += '-'; output += input; }",
      "reduce"
      )
    ).produce().length
  );

FC.close();
