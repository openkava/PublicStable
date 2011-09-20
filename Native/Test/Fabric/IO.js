FC = createFabricClient();
FABRIC = FC.wrapFabricClient(FC);

node = FABRIC.DependencyGraph.createNode("node");
node.addMember("resource", "FabricResource");
node.addMember("resource2", "FabricResource");

var path = FABRIC.IO.queryUserFileAndFolderHandle(false, "dummy title", "txt", "testfile");
printDeep(path[1]);

try
{
  FABRIC.IO.putTextFile("dummy", "pathShouldNotBeString");
}
catch(e)
{
  print(e);
}

var path2 = ["badHandle", "dummy.txt"];
try {
  var badPath = FABRIC.IO.getTextFile(path2);
}
catch (e) {
  print(e);
}

var path3 = [path[0], "SubDir", "testfile2.txt"];
try {
  var badPath = FABRIC.IO.getTextFile(path3);
}
catch (e) {
  print("DirNotFound");//Don't print 'e': contains a print of path3, which is random
}

FABRIC.IO.putTextFile("FileBody", path3);
var body = FABRIC.IO.getTextFile(path3);
print(body);

FABRIC.IO.putUserTextFile("FileBody2", "dummyTitle", "txt", "default");
var body2 = FABRIC.IO.getUserTextFile("dummyTitle.txt", "*"); //Extension::IOManager hardcoded to get file "default.txt"
print(body2);

node.getResourceFromUserFile("resource", "", "txt"); //Extension::IOManager hardcoded to get file "default.txt"
printDeep(node.getData("resource"));
node.putResourceToUserFile("resource", "", "txt", "testfile3");
var path4 = [path[0], "testfile3.txt"];
var body3 = FABRIC.IO.getTextFile(path4);
print(body3);

var path5 = [path[0], "SubDir", "testfile4.txt"];
FABRIC.IO.putTextFile("FileBody3", path5);
node.getResourceFromFile("resource", path5);
printDeep(node.getData("resource"));

var path6 = [path[0], "SubDir", "testfile5.txt"];
node.putResourceToFile("resource", path5);
var body4 = FABRIC.IO.getTextFile(path5);
print(body4);

FABRIC.flush();
FC.dispose();
