FC = createFabricClient();
F = FC.wrapFabricClient(FC);

o = F.DG.createOperator("op");
o.setSourceCode("operator entry() { report 'Hello'; }");
o.setEntryFunctionName("entry");
print(o.getSourceCode());
print(o.getEntryFunctionName());

F.flush();
FC.dispose();
