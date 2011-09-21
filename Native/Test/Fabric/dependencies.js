FC = createFabricClient();
FABRIC = FC.wrapFabricClient(FC);
load( "UnitTest.js.inc" );
ut = new FABRIC.UnitTest;
          ut.test( "setDependency/getDependencies/getDependents", function() {
            var parent = FABRIC.DependencyGraph.createNode( "UnitTests.Node.Dependencies.Parent" );
            
            var child = FABRIC.DependencyGraph.createNode( "UnitTests.Node.Dependencies.Child" );
            
            ut.expectException( "parentName must be non-empty", function() {
              child.setDependency( parent, "" );
            } );
            ut.expectException( "parentName must not be 'self'", function() {
              child.setDependency( parent, "self" );
            } );
            ut.expectSuccess( "setDependency works", function() {
              child.setDependency( parent, "parent" );
            } );
            ut.expectSuccess( "setDependency works the second time with same name", function() {
              child.setDependency( parent, "parent" );
            } );
            ut.expectSuccess( "setDependency works with same node but different name", function() {
              child.setDependency( parent, "parentAgain" );
            } );
            
            ut.expect( "child.getDependency('parent').eq(parent)", child.getDependency('parent') === parent );
            ut.expect( "child.getDependency('parentAgain').eq(parent)", child.getDependency('parentAgain') === parent );
            ut.expectException( "child.getDependency('children') fails", function() {
              child.getDependency("children");
            } );
            ut.expectException( "parent.getDependency('parent') fails", function() {
              parent.getDependency("parent");
            } );
            ut.expectException( "parent.getDependency('children') fails", function() {
              parent.getDependency("children");
            } );
            
            ut.expect( "child.getDependencies().parent.eq(parent)", child.getDependencies().parent === parent );
            ut.expect( "child.getDependencies().parentAgain.eq(parent)", child.getDependencies().parentAgain === parent );
          } );
FC.dispose();
