F = require('Fabric').createClient();

o = F.DG.createOperator("op");
o.setSourceCode("operator entry() { report 'Hello'; }");
o.setEntryFunctionName("entry");
console.log(o.getSourceCode());
console.log(o.getEntryFunctionName());

F.flush();
