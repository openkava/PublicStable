FC = createFabricClient();
FABRIC = FC.wrapFabricClient(FC);
load( "UnitTest.js.inc" );
ut = new FABRIC.UnitTest;
ut.test( "Graph Compilation", function() {
  var event;
  ut.expectSuccess( "create event", function() {
    event = FABRIC.DependencyGraph.createEvent("bar");
  } );
  ut.expectSuccess( "fire event", function() {
    event.fire();
  } );
  ut.expect( "event has no errors", event.getErrors().length, 0 );
  
  var eventHandler;
  ut.expectSuccess( "create and add event handler", function() {
    eventHandler = FABRIC.DependencyGraph.createEventHandler("baz");
    event.appendEventHandler( eventHandler );
  } );
  ut.expectSuccess( "fire event", function() {
    event.fire();
  } );
  ut.expect( "event has no errors", event.getErrors().length, 0 );
  ut.expect( "eventHandler has no errors", eventHandler.getErrors().length, 0 );
  
  var node;
  ut.expectSuccess( "create node and set event handler", function() {
    node = FABRIC.DependencyGraph.createNode("foo");
    eventHandler.setScope( "self", node );
  } );
  ut.expectSuccess( "fire event", function() {
    event.fire();
  } );
  ut.expect( "event has no errors", event.getErrors().length, 0 );
  ut.expect( "eventHandler has no errors", eventHandler.getErrors().length, 0 );
  ut.expect( "node has no errors", node.getErrors().length, 0 );
  
  var op, binding;
  ut.expectSuccess( "create and push operator", function() {
    op = FABRIC.DependencyGraph.createOperator("fooTwo");
    binding = FABRIC.DependencyGraph.createBinding();
    binding.setOperator( op );
    binding.setParameterLayout( [ "self.foo" ] );
    node.bindings.append( binding );
  } );
  ut.expectException( "fire event fails", function() {
    event.fire();
  } );
  ut.expect( "event has no errors", event.getErrors().length, 0 );
  ut.expect( "eventHandler has no errors", eventHandler.getErrors().length, 0 );
  ut.expect( "node has one error", node.getErrors().length, 1 );
 
  ut.expectSuccess( "load operator bytecode", function() {
    op.setEntryFunctionName("foo");
    op.setSourceCode("operator foo( io Integer bar ) { bar = 7; }");
  } );
  ut.expectException( "fire event fails", function() {
    event.fire();
  } );
  ut.expect( "event has no errors", event.getErrors().length, 0 );
  ut.expect( "eventHandler has no errors", eventHandler.getErrors().length, 0 );
  ut.expect( "node has one error", node.getErrors().length, 1 );
  
  ut.expectSuccess( "add node member", function() {
    node.addMember( "foo", "Integer" );
  } );
  ut.expectSuccess( "fire event", function() {
    event.fire();
  } );
  ut.expect( "event has no errors", event.getErrors().length, 0 );
  ut.expect( "eventHandler has no errors", eventHandler.getErrors().length, 0 );
  ut.expect( "node has no errors", node.getErrors().length, 0 );
  
  ut.expect( "node.getData('foo') == 7", node.getData('foo'), 7 );
  
  ut.expectSuccess( "remove node member", function() {
    node.removeMember( "foo" );
  } );
  ut.expectException( "fire event fails", function() {
    event.fire();
  } );
  ut.expect( "event has no errors", event.getErrors().length, 0 );
  ut.expect( "eventHandler has no errors", eventHandler.getErrors().length, 0 );
  ut.expect( "node has one error", node.getErrors().length, 1 );
} );
FABRIC.flush();
FC.dispose();
