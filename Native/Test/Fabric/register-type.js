FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

var Vec2 = function( x, y ) {
  if ( typeof x === "number" && typeof y === "number" ) {
    this.x = x;
    this.y = y;
  }
  else if ( x === undefined && y === undefined ) {
    this.x = 0;
    this.y = 0;
  }
  else throw "new Vec2: invalid arguments";
};

Vec2.prototype = {
  sum: function() {
    return this.x + this.y;
  }
};

var desc = {
  members: { x:'Scalar', y:'Scalar' },
  constructor: Vec2,
  kBindings: "\
function Vec2(Scalar x, Scalar y)\n\
{\n\
  self.x = x;\n\
  self.y = y;\n\
}\n\
"
};

FABRIC.RegisteredTypesManager.registerType( 'Vec2', desc );
printDeep(FABRIC.RT.getRegisteredTypes()['Vec2']);

var node = FABRIC.DependencyGraph.createNode("foo");
node.addMember( 'vec2', 'Vec2' );
node.setData( 'vec2', 0, new Vec2( 5.6, 4.3 ) );
var data = node.getData("vec2", 0);
print( JSON.stringify(data) );
print(data.sum());

var op = FABRIC.DG.createOperator("op");
op.setSourceCode("operator entry( io Vec2 vec2 ) { vec2 = Vec2(8.9, 2.3); }");
op.setEntryFunctionName("entry");

var binding = FABRIC.DG.createBinding();
binding.setOperator(op);
binding.setParameterLayout(["self.vec2"]);

node.bindings.append(binding);
node.evaluate();
var data = node.getData("vec2", 0);
print( JSON.stringify(data) );
print(data.sum());

var ComplexType = function() {
  var vec2 = new Vec2(3.14, 2.71);
  var a2_vec2 = [vec2,vec2];
  var a_a2_vec2 = [a2_vec2,a2_vec2,a2_vec2];
  var a2_a_a2_vec2 = [a_a2_vec2,a_a2_vec2];
  var a_a2_a_a2_vec2 = [a2_a_a2_vec2];
  this.member = a_a2_a_a2_vec2;
};
var complexTypeDesc = {
  members: { member: 'Vec2[][2][][2]' },
  constructor: ComplexType,
};
FABRIC.RT.registerType('ComplexType', complexTypeDesc);
printDeep(FABRIC.RT.getRegisteredTypes()['ComplexType']);
node.addMember( 'ct', 'ComplexType' );
printDeep( node.getData( 'ct', 0 ) );

FABRIC.flush();
FC.dispose();
