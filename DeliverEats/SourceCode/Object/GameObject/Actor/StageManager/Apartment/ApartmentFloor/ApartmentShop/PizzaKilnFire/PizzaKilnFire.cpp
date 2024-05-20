#include "PizzaKilnFire.h"
#include "..\..\..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\..\..\Common\Mesh\StaticMesh\StaticMesh.h"
#include "..\..\..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr float ADD_COLOR_ANG_END = 180.0f;	// 加算する色用の最終角度(度).
}

CPizzaKilnFire::CPizzaKilnFire()
	: m_pFire		( nullptr )
	, m_Color		( Color::Red )
	, m_AddColor	( Color::INIT )
	, m_Alpha		( Color::ALPHA_MAX )
	, m_AddColorAng	( INIT_FLOAT )
	, m_FloorIsDisp	( true )
{
}

CPizzaKilnFire::~CPizzaKilnFire()
{

}

//---------------------------.
// 初期化.
//---------------------------.
bool CPizzaKilnFire::Init()
{
	// メッシュの取得.
	m_pFire = CMeshResource::GetStatic( "pizza_fire" );
	m_pFire->SetTransform( m_Transform );

	// 色の設定.
	m_Color		= CONSTANT.START_COLOR;
	m_AddColor	= CONSTANT.END_COLOR - CONSTANT.START_COLOR;

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CPizzaKilnFire::Update( const float& DeltaTime )
{
	// 距離によるアルファフェードの更新.
	m_IsDisp = true;
	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	// 色を少しずつ変化させる.
	m_AddColorAng += CONSTANT.ADD_COLOR_SPEED * DeltaTime;
	if ( m_AddColorAng >= ADD_COLOR_ANG_END ) m_AddColorAng -= ADD_COLOR_ANG_END;
	m_Color = CONSTANT.START_COLOR + m_AddColor * sinf( Math::ToRadian( m_AddColorAng ) );
}

//---------------------------.
// 描画.
//---------------------------.
void CPizzaKilnFire::Render()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	CDirectX11::SetAlphaBlend( true );

	m_pFire->SetDither( true );
	m_pFire->SetColor( Color4::RGBA( m_Color ) );
	m_pFire->SetAlpha( ( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) ) * CONSTANT.ALPHA );
	m_pFire->Render( &m_Transform );
	m_pFire->SetDither( false );

	CDirectX11::SetAlphaBlend( false );
}
