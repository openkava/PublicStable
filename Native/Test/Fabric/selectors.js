FC = createFabricClient();
FABRIC = FC.wrapFabricClient(FC);
load( "UnitTest.js.inc" );
ut = new FABRIC.UnitTest;
        ut.test( "Selectors", function() {
          var event = FABRIC.DependencyGraph.createEvent("UnitTests.Node.selectors.event");
          var eventHandler = FABRIC.DependencyGraph.createEventHandler("UnitTests.Node.selectors.eventHandler");
          event.appendEventHandler( eventHandler );
          var node = FABRIC.DependencyGraph.createNode( "UnitTests.Node.selectors.node" );
          eventHandler.setScope( 'self', node );
          
          var nodeOne = FABRIC.DependencyGraph.createNode( "UnitTests.Node.selectors.nodeOne" );
          var operatorOne = FABRIC.DependencyGraph.createOperator( "UnitTests.Node.selectors.operatorOne" );
          operatorOne.setEntryFunctionName('test');
          operatorOne.setSourceCode("operator test( io Boolean select, io Scalar value ) { select = true; value = 7; }");
          var bindingOne = FABRIC.DependencyGraph.createBinding();
          bindingOne.setOperator( operatorOne );
          bindingOne.setParameterLayout( [] );
          var eventHandlerOne = FABRIC.DependencyGraph.createEventHandler("UnitTests.Node.selectors.eventHandlerOne");
          eventHandlerOne.setScope( 'self', nodeOne );
          eventHandlerOne.setSelector( 'self', bindingOne );
          eventHandler.appendChildEventHandler( eventHandlerOne );
          
          var nodeTwo = FABRIC.DependencyGraph.createNode( "UnitTests.Node.selectors.nodeTwo" );
          var operatorTwo = FABRIC.DependencyGraph.createOperator( "UnitTests.Node.selectors.operatorTwo" );
          operatorTwo.setEntryFunctionName('test');
          operatorTwo.setSourceCode("operator test( io Boolean select, io Scalar value ) { value = 4; }");
          var bindingTwo = FABRIC.DependencyGraph.createBinding();
          bindingTwo.setOperator( operatorTwo );
          bindingTwo.setParameterLayout( [] );
          var eventHandlerTwo = FABRIC.DependencyGraph.createEventHandler("UnitTests.Node.selectors.eventHandlerTwo");
          eventHandlerTwo.setScope( 'self', nodeTwo );
          eventHandlerTwo.setSelector( 'self', bindingTwo );
          eventHandler.appendChildEventHandler( eventHandlerTwo );
          
          var result = event.select( 'Scalar' );
          ut.expect( "Correct number of results", result.length, 1 );
          ut.expect( "Correct result value", result[0].value, 7 );
        } );
FABRIC.flush();
FC.dispose();
