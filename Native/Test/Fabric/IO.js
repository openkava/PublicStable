FC = createFabricClient();
FABRIC = FC.wrapFabricClient(FC);

node = FABRIC.DependencyGraph.createNode("node");
node.addMember("resource", "FabricResource");
node.addMember("resource2", "FabricResource");

var path = FABRIC.IO.queryUserFileAndFolderHandle(FABRIC.IO.forOpenWithWriteAccess, "dummy title", "txt", "testfile");
print(path.fileName);

var path2 = FABRIC.IO.queryUserFileAndFolderHandle(FABRIC.IO.forOpen, "dummy title", "txt", "testfile");

try
{
  FABRIC.IO.putTextFile("dummy", "pathShouldNotBeString");
}
catch(e)
{
  print(e);
}

try {
  FABRIC.IO.putTextFile("dummy", path2);
}
catch (e) {
  print("ErrorCantWriteIfOpenMode");//Can't print 'e' since it contains random bytes
}

var path3 = {
  folderHandle: "badHandle",
  fileName: "dummy.txt"
  };

try {
  var badPath = FABRIC.IO.getTextFile(path3);
}
catch (e) {
  print(e);
}

var path4 = {
  folderHandle: path.folderHandle,
  subFolders: ["SubDir"],
  fileName: "testfile2.txt"
  };

try {
  var badPath = FABRIC.IO.getTextFile(path4);
}
catch (e) {
  print("DirNotFound");//Can't print 'e' since it contains random bytes
}

FABRIC.IO.putTextFile("FileBody", path4);
var body = FABRIC.IO.getTextFile(path4);
print(body);

FABRIC.IO.putUserTextFile("FileBody2", "dummyTitle", "txt", "default");
var body2 = FABRIC.IO.getUserTextFile("dummyTitle.txt", "*"); //Extension::IOManager hardcoded to get file "default.txt"
print(body2);

node.getResourceFromUserFile("resource", "", "txt"); //Extension::IOManager hardcoded to get file "default.txt"
printDeep(node.getData("resource"));
node.putResourceToUserFile("resource", "", "txt", "testfile3");
var path5 = {
  folderHandle: path.folderHandle,
  fileName: "testfile3.txt"
  };

var body3 = FABRIC.IO.getTextFile(path5);
print(body3);

var path6 = {
  folderHandle: path.folderHandle,
  subFolders: ["SubDir"],
  fileName: "testfile4.txt"
  };

FABRIC.IO.putTextFile("FileBody3", path6);
node.getResourceFromFile("resource", path6);
printDeep(node.getData("resource"));

var path7 = {
  folderHandle: path.folderHandle,
  subFolders: ["SubDir"],
  fileName: "testfile5.txt"
  };
node.putResourceToFile("resource", path7);
var body4 = FABRIC.IO.getTextFile(path7);
print(body4);

FABRIC.flush();
FC.dispose();
