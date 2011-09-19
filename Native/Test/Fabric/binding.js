FC = createFabricClient();
F = FC.wrapFabricClient(FC);

b = F.DG.createBinding();
printDeep(b.getOperator());
printDeep(b.getParameterLayout());

o = F.DG.createOperator("op");
b.setOperator(o);
printDeep(b.getOperator().getName());

b.setParameterLayout(["self.foo"]);
printDeep(b.getParameterLayout());

F.flush();
FC.dispose();
