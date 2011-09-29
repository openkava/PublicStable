

FC = createFabricClient();
F = FC.wrapFabricClient(FC);

load( "unitTestUtils.js.inc" );
load( "../../../Web/SceneGraph/RT/Math.js" );
Math.verboseLogFunction = print;

loadType( 'Vec3' );
loadType( 'Mat33' );
loadType( 'Vec4' );
appendToKLCode('Mat44', "\nfunction Vec4 Mat44.multiplyVector( in Vec4 other ) {\n  return this * other;\n}\n\n");
loadType( 'Mat44' );
appendKLOpAdaptors('Euler', []);
loadType( 'Euler' );
loadType( 'Quat' );
loadType( 'Xfo' );

runTests( 'Xfo', [['Xfo','xfo1'], ['Xfo','xfo2'], ['Xfo','res'], ['RotationOrder','ro1'], ['Euler','e1'], ['Quat','q1'], ['Mat44','m1'], ['Vec3','v1'], ['Vec3','v2']] );

F.flush();
FC.dispose();
