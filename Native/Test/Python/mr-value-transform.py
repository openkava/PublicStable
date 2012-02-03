import fabric

client = fabric.createClient()

print(
  client.MR.createValueTransform(
    client.MR.createConstValue('Integer', -42),
    client.KLC.createValueTransformOperator(
      "xfo.kl", "operator xfo(io Integer value) { value *= -2; }", "xfo"
      )
    ).produce()
  )

client.close()
