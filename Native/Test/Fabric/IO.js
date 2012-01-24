FABRIC = require('Fabric').createClient();

node = FABRIC.DependencyGraph.createNode("node");
node.addMember("resource", "FabricResource");
node.addMember("resource2", "FabricResource");

var fileAndFolderHandles = FABRIC.IO.queryUserFileAndFolderHandle(FABRIC.IO.forOpenWithWriteAccess, "dummy title", "txt", "testfile");
console.log(path.fileName);

var fileAndFolderHandles = FABRIC.IO.queryUserFileAndFolderHandle(FABRIC.IO.forOpen, "dummy title", "txt", "testfile");

try
{
  FABRIC.IO.putTextFileContent("dummy", "badHandle");
}
catch(e)
{
  console.log(e);
}

try {
  FABRIC.IO.putTextFile("dummy", fileAndFolderHandles.file);
}
catch (e) {
  console.log("ErrorCantWriteIfOpenMode");//Can't console.log 'e' since it contains random bytes
}

var path4 = FABRIC.IO.buildFileHandleFromRelativePath(fileAndFolderHandles.folder + '/SubDir/testfile2.txt');

try {
  var badPath = FABRIC.IO.getTextFileContent(path4);
}
catch (e) {
  console.log(path4);
  }

FABRIC.IO.putTextFileContent(path4,"FileBody");
var body = FABRIC.IO.getTextFileContent(path4);
console.log(body);

FABRIC.IO.putTextFileContent(path4," append text",true);
var body = FABRIC.IO.getTextFileContent(path4);
console.log(body);

FABRIC.IO.getFileInfo(path4);
var fileInfo = FABRIC.IO.getTextFileContent(path4);
console.log(fileInfo);

node.setData('url',0,path4);
console.log(JSON.stringify(node.getData("resource")));
var path5 = FABRIC.IO.buildFileHandleFromRelativePath(fileAndFolderHandles.folder + '/testfile3.txt');
node.putResourceToFile(path5);

var body3 = FABRIC.IO.getTextFileContent(path5);
console.log(body3);

FABRIC.close();
