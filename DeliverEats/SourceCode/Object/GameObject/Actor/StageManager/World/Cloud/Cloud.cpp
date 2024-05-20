#include "Cloud.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	// 雲のNo.
	enum enCloudNo : unsigned char {
		Cloud_End,
		Cloud_4,
		Cloud_3,
		CloudGroup_2,
		CloudGroup_1,

		CloudNo_Max
	} typedef ECloudNo;

	// 雲のファイル名のリスト.
	const std::vector<std::string> CLOUD_NAME_LIST = {
		"under_cloud_5",
		"under_cloud_4",
		"under_cloud_3",
		"under_cloud_2_3", "under_cloud_2_2", "under_cloud_2_1",
		"under_cloud_1_3", "under_cloud_1_2", "under_cloud_1_1",
		"under_cloud_2_3", "under_cloud_2_2", "under_cloud_2_1",
		"under_cloud_1_3", "under_cloud_1_2", "under_cloud_1_1"
	};

	// 雲の数.
	const int CLOUD_MAX = static_cast<int>( CLOUD_NAME_LIST.size() );

	// 雲の番号のリスト.
	constexpr int CLOUD_NO_LIST[] = {
		Cloud_End,
		Cloud_4,
		Cloud_3,
		CloudGroup_2, CloudGroup_2, CloudGroup_2,
		CloudGroup_2, CloudGroup_2, CloudGroup_2,
		CloudGroup_2, CloudGroup_2, CloudGroup_2,
		CloudGroup_1, CloudGroup_1, CloudGroup_1,
		CloudGroup_1, CloudGroup_1, CloudGroup_1,
		CloudGroup_1, CloudGroup_1, CloudGroup_1,
	};
}

CCloud::CCloud()
	: m_pCloud		( CLOUD_MAX )
	, m_CloudState	( CLOUD_MAX )
{
}

CCloud::~CCloud()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CCloud::Init()
{
	// 作成する雲のトランスフォームの作成.
	D3DXPOSITION3			CreatePos		= { 0.0f, CONSTANT.CLOUD_DOWN_POS_Y, 0.0f };
	const D3DXROTATION3&	CreareRot		= { Math::ToRadian( 90.0f ), 0.0f, 0.0f };
	const D3DXSCALE3&		CreareScale		= { CONSTANT.CLOUD_SCALE, CONSTANT.CLOUD_SCALE, CONSTANT.CLOUD_SCALE };
	const float				UVScrollSpeed	= CONSTANT.CLOUD_MAX_SPEED / CloudNo_Max;

	// 雲の初期化.
	for ( int i = 0; i < CLOUD_MAX; ++i ) {
		m_pCloud[i] = CSpriteResource::GetSprite( CLOUD_NAME_LIST[i], &m_CloudState[i] );
		m_CloudState[i].Transform.Position		= CreatePos;
		m_CloudState[i].Transform.Rotation		= CreareRot;
		m_CloudState[i].Transform.Scale			= CreareScale;
		m_CloudState[i].AnimState.IsUVScrollX	= true;
		m_CloudState[i].AnimState.ScrollSpeed.x = UVScrollSpeed * CLOUD_NO_LIST[i];

		// 作成する場所を少し上に上げる.
		CreatePos.y += CONSTANT.CLOUD_INTERVAL;
	}
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CCloud::Update( const float& DeltaTime )
{
	const float Color = Color::COLOR_MAX - Color::COLOR_MID * ( CTime::GetElapsedTime() / CConstant::GetTime().CLEAR_SCENE_MOVE_TIME_MAX );
	for ( auto& c : m_CloudState ) {
		c.Color = Color4::RGBA( Color, Color, Color );
	}
}

//---------------------------.
// 描画.
//---------------------------.
void CCloud::Render()
{
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CCloud::Sprite3DRender_A()
{
	for ( int i = 0; i < CLOUD_MAX; ++i ) {
		m_pCloud[i]->Render3D( &m_CloudState[i] );
	}
}