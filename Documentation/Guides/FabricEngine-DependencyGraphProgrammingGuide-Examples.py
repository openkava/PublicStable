# Obtaining the FABRIC object
import fabric
FABRIC = fabric.createClient()

# Registered types
class Vec3():
  def __init__( self, x = None, y = None, z = None ):
    if type( x ) is float and type( y ) is float and type( z ) is float:
      self.x = x
      self.y = y
      self.z = z
    elif x is None and y is None and z is None:
      self.x = 0
      self.y = 0
      self.z = 0
    else:
      raise Exception( 'Vec3: invalid arguments' )

FABRIC.RT.registerType('Vec3', {
  'members': [{'x': 'Scalar'}, {'y': 'Scalar'}, {'z': 'Scalar'}],
  'constructor': Vec3,
  'klBindings': {
    'filename': "(inline)",
    'sourceCode': """
// Construct a Vec3 from three Scalars
function Vec3(Scalar x, Scalar y, Scalar z) {
  this.x = x;
  this.y = y;
  this.z = z;
}
// Add two Vec3s
function Vec3 + (Vec3 a, Vec3 b) {
  return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
"""
  }
})
FABRIC.RT.getRegisteredTypes()['Vec3']

# Node creation
node = FABRIC.DG.createNode('verticies')

# Getting a Node's name
node.getName()

# Adding and getting members
node.addMember("position", "Vec3", Vec3(0.0, 0.0, 0.0))
node.getMembers()['position']

# getData and setData
node.getData('position', 0)
node.setData('position', 0, Vec3(1.0, 2.0, 3.0))
node.getData('position', 0)

# Node slice counts
node.getCount()
node.getData('position', 1)
node.setCount(2)
node.getCount()
node.getData('position', 1)

# Node dependencies
anotherNode = FABRIC.DG.createNode("originalVertices")
node.setDependency(anotherNode, "original")
node.getDependencies()

# Node evaluation
op = FABRIC.DG.createOperator("offsetPosition")
op.setEntryFunctionName("offset")
op.setSourceCode("require Vec3; operator offset(io Vec3 position, io Vec3 newPosition) { newPosition = position + Vec3(1.0,1.0,1.0); }")
binding = FABRIC.DG.createBinding()
binding.setOperator(op)
binding.setParameterLayout(["self.position", "self.newPosition"])
node.addMember("newPosition", "Vec3")
node.bindings.append(binding)
node.getData("position", 0)
node.getData( "newPosition", 0 );
node.evaluate();
node.getData( "newPosition", 0 );

# Event creation
event = FABRIC.DG.createEvent("anEvent")
event.getName()

# Creating EventHandlers and appending them to Events
eventHandler = FABRIC.DG.createEventHandler("trivialEventHandler")
event.appendEventHandler(eventHandler)
event.getEventHandlers()

# Operator creation
op = FABRIC.DG.createOperator("doSomething")

# Setting operator source code
op.setSourceCode("operator entry( io Scalar result, in Size index, in Size count ) { result = 3.14 }")
op.getDiagnostics()
op.setSourceCode("operator entry( io Scalar result, in Size index, in Size count ) { result = 3.14; }")
op.getDiagnostics()

# Setting the operator entry point
op.setEntryFunctionName('entry')
op.getEntryFunctionName()
