
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

window.addEventListener("onFabricOpenDebugger", onFabricOpenDebugger, false);
function onFabricOpenDebugger(event)
{
    id = FABRIC.getContextIDs()[0];
    debuggerWindow = window.open("../../../Core/Debugger/FABRIC.Debugger.html?id="+id,"FABRIC Debugger","location=0, toolbar=0, width=1280, height=860");
}
