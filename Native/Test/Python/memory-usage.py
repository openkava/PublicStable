import fabric
F = fabric.createClient()

n = F.DG.createNode("foo")
n.addMember("bar", "String[]")
print(F.getMemoryUsage())

F.close()
