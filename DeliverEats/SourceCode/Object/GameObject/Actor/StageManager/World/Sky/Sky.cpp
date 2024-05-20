#include "Sky.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Common\Mesh\SkyBoxMesh\SkyBoxMesh.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr float		END_TIME		= 10800.0f;	// ゲーム終了時間.
	constexpr float		CLOUD_SPPED		= 0.03f;	// 雲の移動速度.
	const D3DXVECTOR3	START_COLOR		= { 1.00f, 0.98f, 0.80f };
	const D3DXVECTOR3	CENTER_COLOR	= Color::Orange;
	const D3DXVECTOR3	END_COLOR		= Color::Gray;
}

CSky::CSky()
	: m_pColorMesh			( nullptr )
	, m_pSunAndMonn			( nullptr )
	, m_pCloudMesh			( nullptr )
	, m_CloudTransform		()
	, m_SunAndMonnTransform	()
	, m_SkyColor			( Color::White )
	, m_SkyColorVector		()
{
}

CSky::~CSky()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CSky::Init()
{
	// メッシュの取得.
	m_pColorMesh	= CMeshResource::GetSkyBox( "sky_b" );
	m_pSunAndMonn	= CMeshResource::GetSkyBox( "sun_moon_b" );
	m_pCloudMesh	= CMeshResource::GetSkyBox( "cloud_b" );

	// 空の色の変化用ベクトルの作成.
	m_SkyColorVector.resize( 3 );
	m_SkyColorVector[0] = START_COLOR	- Color::White;
	m_SkyColorVector[1] = CENTER_COLOR	- START_COLOR;
	m_SkyColorVector[2] = END_COLOR		- CENTER_COLOR;

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CSky::Update( const float& DeltaTime )
{
	// 雲をゆっくり回転させる.
	m_CloudTransform.Rotation.y += Math::ToRadian( CLOUD_SPPED );
	m_CloudTransform.RotationClamp();

	// 太陽と月をゆっくり回転させる.
	const float Speed = Math::DEGREE_MID * ( CTime::GetElapsedTime() / CONSTANT.CLEAR_SCENE_MOVE_TIME_MAX );
	m_SunAndMonnTransform.Rotation.z = Math::ToRadian( Speed );
	m_SunAndMonnTransform.RotationClamp();

	// 空の色の変更.
	const int No = static_cast<int>( CTime::GetElapsedTime() ) / static_cast<int>( END_TIME );
	switch ( No ) {
	case 0:
		m_SkyColor = Color::White + m_SkyColorVector[0] * ( CTime::GetElapsedTime() / END_TIME );
		break;
	case 1:
		m_SkyColor = START_COLOR  + m_SkyColorVector[1] * ( ( CTime::GetElapsedTime() - END_TIME ) / END_TIME );
		break;
	case 2:
		m_SkyColor = CENTER_COLOR + m_SkyColorVector[2] * ( ( CTime::GetElapsedTime() - END_TIME * 2.0f ) / END_TIME );
		break;
	default:
		break;
	}

	// 雲の色の変更.
	const float Color = Color::COLOR_MAX - Color::COLOR_MID * ( CTime::GetElapsedTime() / CConstant::GetTime().CLEAR_SCENE_MOVE_TIME_MAX );
	m_CloudColor = { Color, Color, Color };
}

//---------------------------.
// 描画.
//---------------------------.
void CSky::Render()
{
	CDirectX11::SetAlphaBlend( true );

	// 空の色の描画.
	m_pColorMesh->SetColor( m_SkyColor );
	m_pColorMesh->Render();

	// 雲の描画.
	m_pCloudMesh->SetColor( m_CloudColor );
	m_pCloudMesh->Render( &m_CloudTransform );

	CDirectX11::SetAlphaBlend( false );

	// 太陽と月の描画.
	m_pSunAndMonn->SetDither( true );
	m_pSunAndMonn->Render( &m_SunAndMonnTransform );
	m_pSunAndMonn->SetDither( false );
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CSky::Sprite3DRender_A()
{
}
