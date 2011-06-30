FC = createFabricClient();
F = wrapFabricClient(FC);

eh = F.DG.createEventHandler("event handler");
print(eh.getName());
print(eh.getType());

var mapNamedObjectsToNames = function (namedObjects) {
  var result = [];
  for (var i=0; i<namedObjects.length; ++i)
    result.push(namedObjects[i].getName());
  return result;
};


printDeep(mapNamedObjectsToNames(eh.getChildEventHandlers()));
var childEventHandler = F.DG.createEventHandler("child event handler");
eh.appendChildEventHandler(childEventHandler);
printDeep(mapNamedObjectsToNames(eh.getChildEventHandlers()));
eh.appendChildEventHandler(F.DG.createEventHandler("another child event handler"));
printDeep(mapNamedObjectsToNames(eh.getChildEventHandlers()));
eh.removeChildEventHandler(childEventHandler);
printDeep(mapNamedObjectsToNames(eh.getChildEventHandlers()));

F.flush();
FC.dispose();
