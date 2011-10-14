FC = createFabricClient();
F = FC.wrapFabricClient(FC);
n = F.DG.createNode("n");
n.addMember("foo","String[String]");
printDeep(n.getData("foo",0));
n.setData("foo",0,{x:"bar"});
printDeep(n.getData("foo",0));

