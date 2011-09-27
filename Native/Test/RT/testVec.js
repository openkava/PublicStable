



FC = createFabricClient();
F = FC.wrapFabricClient(FC);

load( "unitTestUtils.js.inc" );
load( "../../../Web/SceneGraph/RT/Math.js" );
Math.verboseLogFunction = print;

var vecXCode = loadTestFile( 'Vec' );

var dimSpecificCodePrefix = [];
dimSpecificCodePrefix[2] =
  'v1.set(1.0,4.0);\n' +
  'v2.set(2.0,1.0);';
dimSpecificCodePrefix[3] =
  'v1.set(1.0,2.0,3.0);\n' +
  'v2.set(2.0,2.0,1.0);';
dimSpecificCodePrefix[4] =
  'v1.set(1.0,2.0,3.0,4.0);\n' +
  'v2.set(2.0,2.0,1.0,1.0);';

var dimSpecificCodeTests = [];
dimSpecificCodeTests[2] =
  's1 = v1.cross(v2);\n' +
  'v1.appendScalarResult(tests, results, \'cross\', s1);';
dimSpecificCodeTests[3] =
  'v3 = v1.cross(v2);\n' +
  'v3.appendResult(tests, results, \'cross\');';
dimSpecificCodeTests[4] = '';

for( var dim = 2; dim <= 4; ++dim ) {
  var type = 'Vec' + dim;
  print( '****** ' + type + ' Tests ******' );
  appendKLOpAdaptors(type, [ '+', '+=', '-', '-=', '*', '*=', ['*','Scalar'], ['*=','Scalar'], '/', '/=', ['/','Scalar'], ['/=','Scalar'] ] );
  loadType( type );
  defineInPlaceOpAdaptors(type, [ '+=', '-=', '*=', [ '*=', 'Scalar' ], '/=', [ '/=', 'Scalar' ] ] );

  runTests( type, [[type,'v1'], [type,'v2'], [type,'v3'], ['Scalar','s1']], dimSpecificCodePrefix[dim] + vecXCode + dimSpecificCodeTests[dim] );
}

F.flush();
FC.dispose();
