



FC = createFabricClient();
F = FC.wrapFabricClient(FC);

load( "unitTestUtils.js.inc" );
load( "../../../Web/SceneGraph/RT/Math.js" );
Math.verboseLogFunction = print;

var testCode = loadTestFile( 'Mat' );

var dimSpecificCodePrefix = [];
dimSpecificCodePrefix[2] =
  'm1.set(1.0,3.0, 4.0,5.0);\n' +
  'XtoY.set(0.0,-1.0, 1.0,0.0);\n' +
  'YtoMinusY.set(1.0,0.0, 0.0,-1.0);\n' +
  'xAxis.set(1.0,0.0);\n' +
  'yAxis.set(0.0,1.0);\n';
dimSpecificCodePrefix[3] =
  'm1.set(1.0,2.0,3.0, 4.0,5.0,6.0, 7.0,8.0,9.0);\n' +
  'XtoY.set(0.0,-1.0,0.0, 1.0,0.0,0.0, 0.0,0.0,1.0);\n' +
  'YtoMinusY.set(1.0,0.0,0.0, 0.0,-1.0,0.0, 0.0,0.0,1.0);\n' +
  'xAxis.set(1.0,0.0,0.0);\n' +
  'yAxis.set(0.0,1.0,0.0);\n';
dimSpecificCodePrefix[4] =
  'm1.set(1.0,2.0,3.0,4.0, 5.0,6.0,7.0,8.0, 9.0,10.0,11.0,12.0, 13.0,14.0,15.0,16.0);\n' +
  'XtoY.set(0.0,-1.0,0.0,0.0, 1.0,0.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0);\n' +
  'YtoMinusY.set(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0);\n' +
  'xAxis.set(1.0,0.0,0.0,0.0);\n' +
  'yAxis.set(0.0,1.0,0.0,0.0);\n';

var dimSpecificCodeTests = [];
dimSpecificCodeTests[2] = '';
dimSpecificCodeTests[3] = '';
dimSpecificCodeTests[4] = 
    'v1.set(1.0,2.0,3.0,1.0);\n' +
    'v1 = m1.multiplyVector(v1);\n' +
    'v1.appendResult(tests, results, \'multiplyVec4\');\n' +
    'v2.set(1.0,2.0,3.0);\n' +
    'v2 = m1.multiplyVector(v2);\n' +
    'v2.appendResult(tests, results, \'multiplyVec3\');\n' +
    'm1.upperLeft().appendResult(tests, results, \'upperLeft\');\n';

for( var dim = 2; dim <= 4; ++dim ) {
  var type = ('Mat' + dim) + dim;
  var vecType = 'Vec' + dim;
  loadType(vecType);
  print( '****** ' + type + ' Tests ******' );
  appendKLOpAdaptors(type, [ '+', '+=', '-', '-=', '*', '*=', ['*','Scalar'], ['*=','Scalar'], ['/','Scalar'], ['/=','Scalar'] ] );
  //Add special adaptor for function VecX MatX.multiplyVecX( in VecX other)
  appendToKLCode(type, "\nfunction " + vecType + ' ' + type + ".multiplyVector( in " + vecType + " other ) {\n  return this * other;\n}\n\n");
  var vecType2 = vecType;
  if(dim == 4) {
    vecType2 = 'Vec3';
    appendToKLCode(type, "\nfunction " + vecType2 + ' ' + type + ".multiplyVector( in " + vecType2 + " other ) {\n  return this * other;\n}\n\n");
  }
  loadType( type );
  defineInPlaceOpAdaptors(type, [ '+=', '-=', '*=', [ '*=', 'Scalar' ], [ '/=', 'Scalar' ] ] );

  runTests( type, [[type,'m1'], [type,'XtoY'], [type,'YtoMinusY'], [type,'res'], ['Scalar','s1'], [vecType,'v1'], [vecType2,'v2'], [vecType,'xAxis'], [vecType,'yAxis']], dimSpecificCodePrefix[dim] + testCode + dimSpecificCodeTests[dim] );
}

F.flush();
FC.dispose();
