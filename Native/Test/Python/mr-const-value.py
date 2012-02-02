import fabric

client = fabric.createClient()

cv = client.MR.createConstValue("Size", 10000)
print(cv.produce())

client.close()

