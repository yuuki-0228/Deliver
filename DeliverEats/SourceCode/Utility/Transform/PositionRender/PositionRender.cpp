#include "PositionRender.h"
#include "..\..\..\Common\DirectX\DirectX11.h"
#include "..\..\..\Common\Mesh\StaticMesh\StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\Input\Input.h"

CPositionRender::CPositionRender()
	: m_pMesh		( nullptr )
	, m_Transform	()
	, m_IsRender	( false, u8"座標の表示/非表示", u8"System" )
{
}

CPositionRender::~CPositionRender()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CPositionRender* CPositionRender::GetInstance()
{
	static std::unique_ptr<CPositionRender> pInstance = std::make_unique<CPositionRender>();
	return pInstance.get();
}

//---------------------------.
// 更新.
//---------------------------.
void CPositionRender::Update( const float& DeltaTime )
{
	CPositionRender* pI = GetInstance();

	// 表示の切り替え.
	if ( CDebugKeyInput::IsANDKeyDown( VK_CONTROL, VK_F11 ) ) {
		pI->m_IsRender = !pI->m_IsRender;
	}
}

//---------------------------.
// 描画.
//---------------------------.
void CPositionRender::Render( const STransform& Transform )
{
#ifdef _DEBUG
	if ( Transform.IsPostionRender == false ) return;

	CPositionRender* pI = GetInstance();

	// 表示するか.
	if ( pI->m_IsRender == false ) return;

	// モデルの取得.
	if ( pI->m_pMesh == nullptr ) pI->m_pMesh = CMeshResource::GetStatic( "XYZModel" );

	// 一番大きいサイズを取得.
	float Scale = Transform.Scale.x;
	if ( Scale < Transform.Scale.y ) Scale = Transform.Scale.y;
	if ( Scale < Transform.Scale.z ) Scale = Transform.Scale.z;

	// トランスフォームの更新.
	pI->m_Transform.Position		= Transform.Position;
	pI->m_Transform.Scale			= { Scale, Scale, Scale };
	pI->m_Transform.IsPostionRender	= false;

	// トランスフォームの描画.
	pI->m_pMesh->Render( &pI->m_Transform );
#endif // #ifdef _DEBUG.
}
