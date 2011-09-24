FC = createFabricClient();
F = FC.wrapFabricClient(FC);

  var dgnode1 = F.DG.createNode("myNode1");
  dgnode1.addMember('a','Size');
  dgnode1.setCount(32);
  
  var operatorInit = F.DG.createOperator("initiate");
  operatorInit.setSourceCode(
    'operator initiate(in Size index, io Size a) {\n'+
    '  report "Setting index " + index;\n'+
    '  a = index;\n'+
    '}\n');
  operatorInit.setEntryFunctionName('initiate');
  operatorInit.setMainThreadOnly(true);
  if (operatorInit.getErrors().length > 0) {
    if (operatorInit.getDiagnostics().length > 0)
      printDeep(opreatorInit.getDiagnostics());
  }

  var bindingInit = F.DG.createBinding();
  bindingInit.setOperator(operatorInit);
  bindingInit.setParameterLayout([
    'self.index',
    'self.a'
  ]);

  dgnode1.bindings.append(bindingInit);

  var reportOp = F.DG.createOperator('reportOp');
  reportOp.setSourceCode("\
operator reportValues(io Size a<>) {\n\
  report a;\n\
}\n\
");
  reportOp.setEntryFunctionName('reportValues');

  var reportBindings = F.DG.createBinding();
  reportBindings.setOperator(reportOp);
  reportBindings.setParameterLayout([
    'node.a<>'
  ]);
  
  var eh = F.DG.createEventHandler("eh");
  eh.setScope('node', dgnode1);
  eh.preDescendBindings.append(reportBindings);
  
  var e = F.DG.createEvent('e');
  e.appendEventHandler(eh);
  e.fire();

F.flush();
FC.dispose();
