FC = createFabricClient();
F = wrapFabricClient(FC);

var mapNamedObjectsToNames = function (namedObjects) {
  var result = [];
  for (var i=0; i<namedObjects.length; ++i)
    result.push(namedObjects[i].getName());
  return result;
};

e = F.DG.createEvent("event");
print(e.getName());
print(e.getType());

printDeep(mapNamedObjectsToNames(e.getEventHandlers()));
eh = F.DG.createEventHandler("eventHandler");
e.appendEventHandler(eh);
printDeep(mapNamedObjectsToNames(e.getEventHandlers()));

print(e.getDidFireCallback());
e.setDidFireCallback(function () {
  print(this.getName() + " fired");
});
print(e.getDidFireCallback());
e.fire();

FC.dispose();
