FC = createFabricClient();
FABRIC = wrapFabricClient(FC);

load( "UnitTest.js.inc" );

var Vec3 = function( x, y, z ) {
  if ( typeof x === "number" && typeof y === "number" && typeof z === "number" ) {
    this.x = x;
    this.y = y;
    this.z = z;
  }
  else if ( x === undefined && y === undefined && z === undefined ) {
    this.x = 0;
    this.y = 0;
    this.z = 0;
  }
  else throw "new Vec3: invalid arguments";
};

var desc = {
  members: { x:'Scalar', y:'Scalar', z:'Scalar' },
  constructor: Vec3,
  kBindings: "\
function Vec3( Scalar x, Scalar y, Scalar z )\n\
{\n\
  this.x = x;\n\
  this.y = y;\n\
  this.z = z;\n\
}\n\
"
};

FABRIC.RegisteredTypesManager.registerType( 'Vec3', desc );

ut = new FABRIC.UnitTest;
ut.test("Selectors (Vec3)", function() {
	var event, eventHandler, node;
	ut.test("Create event, eventHandler, node", function() {
		event = FABRIC.DependencyGraph.createEvent("UnitTests.Node.selectors.event");
		eventHandler = FABRIC.DependencyGraph.createEventHandler("UnitTests.Node.selectors.eventHandler");
		event.appendEventHandler( eventHandler );
		node = FABRIC.DependencyGraph.createNode( "UnitTests.Node.selectors.node" );
		eventHandler.addScope( 'self', node );
	});

	var nodeOne, operatorOne, bindingOne, eventHandlerOne;
	ut.test("Create nodeOne, operatorOne, bindingOne, eventHandlerOne", function() {
		ut.test("Create nodeOne", function() {
			nodeOne = FABRIC.DependencyGraph.createNode( "UnitTests.Node.selectors.nodeOne" );
		});
		ut.test("Create operatorOne", function() {
			operatorOne = FABRIC.DependencyGraph.createOperator( "UnitTests.Node.selectors.operatorOne" );
			operatorOne.setEntryFunctionName('test');
			operatorOne.setSourceCode("operator test( io Boolean select, io Vec3 value ) { select = true; value.x = 7; }");
		});
		ut.test("Create bindingOne", function() {
			bindingOne = FABRIC.DependencyGraph.createBinding();
			bindingOne.setOperator( operatorOne );
			bindingOne.setParameterLayout( [] );
		});
		ut.test("Create eventHandlerOne", function() {
			eventHandlerOne = FABRIC.DependencyGraph.createEventHandler("UnitTests.Node.selectors.eventHandlerOne");
			eventHandlerOne.addScope( 'self', nodeOne );
			eventHandlerOne.setSelector( 'self', bindingOne );
			eventHandler.appendChildEventHandler( eventHandlerOne );
		});
	});
	
	var nodeTwo = FABRIC.DependencyGraph.createNode( "UnitTests.Node.selectors.nodeTwo" );
	var operatorTwo = FABRIC.DependencyGraph.createOperator( "UnitTests.Node.selectors.operatorTwo" );
	operatorTwo.setEntryFunctionName('test');
	operatorTwo.setSourceCode("operator test( io Boolean select, io Vec3 value ) { value.x = 4; }");
	var bindingTwo = FABRIC.DependencyGraph.createBinding();
	bindingTwo.setOperator( operatorTwo );
	bindingTwo.setParameterLayout( [] );
	var eventHandlerTwo = FABRIC.DependencyGraph.createEventHandler("UnitTests.Node.selectors.eventHandlerTwo");
	eventHandlerTwo.addScope( 'self', nodeTwo );
	eventHandlerTwo.setSelector( 'self', bindingTwo );
	eventHandler.appendChildEventHandler( eventHandlerTwo );

	var result = event.select( 'Vec3' );
	ut.expect( "Correct number of results", result.length, 1 );
	ut.expect( "Correct result value", result[0].value.x, 7 );
});

FABRIC.flush();
FC.dispose();
