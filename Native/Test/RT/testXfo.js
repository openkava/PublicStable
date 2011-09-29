

FC = createFabricClient();
F = FC.wrapFabricClient(FC);

load( "unitTestUtils.js.inc" );
load( "../../../Web/SceneGraph/RT/Math.js" );
Math.verboseLogFunction = print;

loadType( 'Vec3' );
loadType( 'Mat33' );
loadType( 'Vec4' );
appendToKLCode('Mat44', "\nfunction Vec4 Mat44.multiplyVector( in Vec4 other ) {\n  return this * other;\n}\n\n");
appendToKLCode('Mat44', "\nfunction Vec3 Mat44.multiplyVector( in Vec3 other ) {\n  return this * other;\n}\n\n");
appendKLOpAdaptors('Mat44', [ '*', '*=' ] );
loadType( 'Mat44' );
defineInPlaceOpAdaptors('Mat44', [ '*=' ] );
appendKLOpAdaptors('Euler', []);
loadType( 'Euler' );
loadType( 'Quat' );

appendKLOpAdaptors('Xfo', [ '*', '*=' ] );
loadType( 'Xfo' );
defineInPlaceOpAdaptors('Xfo', [ '*=' ] );

runTests( 'Xfo', [['Xfo','xfo1'], ['Xfo','xfo2'], ['Xfo','res'], ['RotationOrder','ro1'], ['Euler','e1'], ['Quat','q1'], ['Mat44','m1'], ['Mat44','m2'], ['Vec3','v1'], ['Vec3','v2']] );

F.flush();
FC.dispose();
