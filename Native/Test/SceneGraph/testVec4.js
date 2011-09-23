



FC = createFabricClient();
F = FC.wrapFabricClient(FC);

load( "unitTestUtils.js" );
appendKLOpAdaptors('Vec4', [ '+', '+=', '-', '-=', '*', '*=', ['*','Scalar'], ['*=','Scalar'], '/', '/=', ['/','Scalar'], ['/=','Scalar'] ] );
load( "../../../Web/SceneGraph/RT/Math.js" );
loadType( "Vec4" );
defineInPlaceOpAdaptors('Vec4', [ '+=', '-=', '*=', [ '*=', 'Scalar' ], '/=', [ '/=', 'Scalar' ] ] );

runTests('Vec4', [['Vec4','v1'], ['Vec4','v2'], ['Vec4','v3'], ['Scalar','s1']]);

F.flush();
FC.dispose();
