import fabric

client = fabric.createClient()

print(
  client.MR.createValueGenerator(
    client.KLC.createValueGeneratorOperator(
      "xfo.kl", "operator xfo(io Integer value) { value = 121; }", "xfo"
      )
    ).produce()
  )

client.close()
