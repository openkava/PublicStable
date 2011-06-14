
F = wrapFabricClient(createFabricClient());

b = F.DG.createBinding();
printDeep(b.getOperator());
printDeep(b.getParameterLayout());

o = F.DG.createOperator("op");
b.setOperator(o);
printDeep(b.getOperator());

b.setParameterLayout(["self.foo"]);
printDeep(b.getParameterLayout());
