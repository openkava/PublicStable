



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

runTests( 'Euler', [['RotationOrder','ro1'], ['Euler','e1'], ['Euler','e2'], ['Euler','res'], ['Mat33','m1'], ['Vec3','v1']] );

F.flush();
FC.dispose();
