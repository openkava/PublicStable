var subMenu = chrome.contextMenus.create({
    "title": "FABRIC"
});
var item = chrome.contextMenus.create({
    "title": "Debugger",
    "parentId": subMenu,
    "onclick": function(info, tab){

        chrome.tabs.getSelected(null, function(tab) {
          chrome.tabs.sendRequest(tab.id, {message: "onFabricOpenDebugger"}, function(response) {});
        });
    }
});
