
FABRIC = wrapFabricClient(createFabricClient());
load( "UnitTest.js.inc" );
ut = new FABRIC.UnitTest;
          ut.test( "addDependency/getDependencies/getDependents", function() {
            var parent = FABRIC.DependencyGraph.createNode( "UnitTests.Node.Dependencies.Parent" );
            
            var child = FABRIC.DependencyGraph.createNode( "UnitTests.Node.Dependencies.Child" );
            
            ut.expectException( "parentName must be non-empty", function() {
              child.addDependency( parent, "" );
            } );
            ut.expectException( "parentName must not be 'self'", function() {
              child.addDependency( parent, "self" );
            } );
            ut.expectSuccess( "addDependency works", function() {
              child.addDependency( parent, "parent" );
            } );
            ut.expectSuccess( "addDependency works the second time with same name", function() {
              child.addDependency( parent, "parent" );
            } );
            ut.expectSuccess( "addDependency works with same node but different name", function() {
              child.addDependency( parent, "parentAgain" );
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
