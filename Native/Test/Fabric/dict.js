FC = createFabricClient();
F = FC.wrapFabricClient(FC);
n = F.DG.createNode("n");

n.addMember("foo","String[String]");
a = n.getData("foo", 0);
printDeep(a);
a.x = "bar";
n.setData("foo",0,a);
a = n.getData("foo", 0);
printDeep(a);
a.x = "baz";
n.setData("foo",0,a);
a = n.getData("foo", 0);
printDeep(a);

n.addMember("bar","Scalar[Scalar]");
a = n.getData("bar", 0);
printDeep(a);
a[2.4] = 3.14;
a[7.9] = 2.71;
n.setData("bar", 0, a );
a = n.getData("bar", 0);
printDeep(a);
