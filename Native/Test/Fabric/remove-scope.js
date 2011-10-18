FC = createFabricClient();
F = FC.wrapFabricClient(FC);
//F = require('Fabric').createClient();

node = F.DG.createNode('node');
eventHandler = F.DG.createEventHandler('eventHandler');
eventHandler.setScope('node', node);
print(eventHandler.getScopes());
try {
  eventHandler.removeScope('notANode');
  F.flush();
}
catch (e) {
  print("eventHandler.removeScope('notANode'): " + e);
}
eventHandler.removeScope('node');
print(eventHandler.getScopes());

F.flush();
