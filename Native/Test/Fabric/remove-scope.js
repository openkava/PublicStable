F = require('Fabric').createClient();

node = F.DG.createNode('node');
eventHandler = F.DG.createEventHandler('eventHandler');
eventHandler.setScope('node', node);
console.log(eventHandler.getScopes());
try {
  eventHandler.removeScope('notANode');
  F.flush();
}
catch (e) {
  console.log("eventHandler.removeScope('notANode'): " + e);
}
eventHandler.removeScope('node');
console.log(eventHandler.getScopes());

F.flush();
