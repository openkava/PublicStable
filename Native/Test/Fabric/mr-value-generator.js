FC = require('Fabric').createClient();

console.log(
  FC.MR.createValueGenerator(
    FC.KLC.createValueGeneratorOperator(
      "xfo.kl", "operator xfo(io Integer value) { value = 121; }", "xfo"
      )
    ).produce()
  );

FC.close();
