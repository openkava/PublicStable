



FC = createFabricClient();
F = FC.wrapFabricClient(FC);

load( "unitTestUtils.js.inc" );
load( "../../../Web/SceneGraph/RT/Math.js" );
Math.verboseLogFunction = print;

loadType( 'Vec3' );
appendToKLCode('Mat33', "\nfunction Vec3 Mat33.multiplyVector( in Vec3 other ) {\n  return this * other;\n}\n\n");
loadType( 'Mat33' );
appendKLOpAdaptors('Euler', []);
loadType( 'Euler' );

appendKLOpAdaptors('Quat', [ '+', '+=', '-', '-=', '*', '*=', ['*','Scalar'], ['*=','Scalar'], '/', '/=', ['/','Scalar'], ['/=','Scalar'] ] );
loadType( 'Quat' );
defineInPlaceOpAdaptors('Quat', [ '+=', '-=', '*=', [ '*=', 'Scalar' ], '/=', [ '/=', 'Scalar' ] ] );

runTests( 'Quat', [['Quat','q1'], ['Quat','q2'], ['Quat','res'], ['RotationOrder','ro1'], ['Euler','e1'], ['Mat33','m1'], ['Vec3','v1'], ['Vec3','v2']] );

F.flush();
FC.dispose();
