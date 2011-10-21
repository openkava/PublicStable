



FC = createFabricClient();
F = FC.wrapFabricClient(FC);

load( "unitTestUtils.js.inc" );
load( "../../../Web/SceneGraph/RT/Math.js" );
Math.verboseLogFunction = print;

var testCode = loadTestFile( 'Vec' );

var dimSpecificCodePrefix = [];
dimSpecificCodePrefix[2] =
  'v1.set(1.0,4.0);\n' +
  'v2.set(2.0,1.0);\n';
dimSpecificCodePrefix[3] =
  'v1.set(1.0,2.0,3.0);\n' +
  'v2.set(2.0,2.0,1.0);\n';
dimSpecificCodePrefix[4] =
  'v1.set(1.0,2.0,3.0,4.0);\n' +
  'v2.set(2.0,2.0,1.0,1.0);\n';

var dimSpecificCodeTests = [];
dimSpecificCodeTests[2] =
  's1 = v1.cross(v2);\n' +
  'v1.appendScalarResult(tests, results, \'cross\', s1);';
dimSpecificCodeTests[3] =
  'res = v1.cross(v2);\n' +
  'res.appendResult(tests, results, \'cross\');';
dimSpecificCodeTests[4] = '';

for( var dim = 2; dim <= 4; ++dim ) {
  var type = 'Vec' + dim;
  print( '****** ' + type + ' Tests ******' );
  appendKLOpAdaptors(type, [ '+', '+=', '-', '-=', '*', '*=', ['*','Scalar'], ['*=','Scalar'], '/', '/=', ['/','Scalar'], ['/=','Scalar'] ] );
  loadType( type );
  defineInPlaceOpAdaptors(type, [ '+=', '-=', '*=', [ '*=', 'Scalar' ], '/=', [ '/=', 'Scalar' ] ] );

  runTests( type, [[type,'v1'], [type,'v2'], [type,'res'], ['Scalar','s1']], dimSpecificCodePrefix[dim] + testCode + dimSpecificCodeTests[dim] );
}

F.flush();
FC.dispose();
