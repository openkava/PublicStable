chrome.extension.onRequest.addListener(
  function(request, sender, sendResponse) {
    if(request.message == "onFabricOpenDebugger")
    {
        var customEvent = document.createEvent('Event');
        customEvent.initEvent('onFabricOpenDebugger', true, true);
        window.dispatchEvent(customEvent);
        
        //window.postMessage("onFabricOpenDebugger",document.URL)
    }
    sendResponse({}); // snub them.
  });
