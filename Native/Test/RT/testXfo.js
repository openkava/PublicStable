

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

runTests( 'Xfo', [['Xfo','xfo1'], ['Xfo','xfo2'], ['RotationOrder','ro1'], ['Euler','e1'], ['Euler','e2'], ['Euler','res'], ['Mat44','m1'], ['Vec4','v1']] );

F.flush();
FC.dispose();
