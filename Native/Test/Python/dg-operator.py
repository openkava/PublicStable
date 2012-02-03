import fabric
F = fabric.createClient()

o = F.DG.createOperator("op")
o.setSourceCode("operator entry() { report 'Hello'; }")
o.setEntryFunctionName("entry")
print(o.getSourceCode())
print(o.getEntryFunctionName())

F.close()
