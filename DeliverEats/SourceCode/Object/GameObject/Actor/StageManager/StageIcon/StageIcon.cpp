#include "StageIcon.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace {
	// アイコンの背景の開始時のy座標のUVの位置.
	constexpr float ICON_BACK_START_UV_Y = 0.5f;
}

CStageIcon::CStageIcon()
	: m_Transform		()
	, m_pIcon			( nullptr )
	, m_pIconBack		( nullptr )
	, m_IconState		()
	, m_IconFrameState	()
	, m_IconBackState	()
{
}

CStageIcon::~CStageIcon()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CStageIcon::InitIcon( const EObjectTag Tag )
{
	// 画像の設定.
	m_pIcon		= CSpriteResource::GetSprite( "StageIcon",		&m_IconState );
	m_pIconBack	= CSpriteResource::GetSprite( "StageIconGauge",	&m_IconBackState );
	m_IconState.AnimState.IsSetAnimNumber	= true;
	m_IconFrameState						= m_IconState;
	m_IconFrameState.AnimState.AnimNumber	= 4;
	m_IconBackState.AnimState.UV.y			= ICON_BACK_START_UV_Y;

	// マスク画像の設定.
	m_IconBackState.pMaskTexture = CSpriteResource::GetSprite( "MapIconMaskDown" )->GetTexture();

	// タグに応じて見た目を変更.
	SetTag( Tag );

	// ディザ抜きを使用する.
	m_pIcon->SetDither( true );
	m_pIconBack->SetDither( true );

	// 色の設定.
	if ( Tag == EObjectTag::PlayerIsland ) {
		m_IconState.Color		= Color4::Dodgerblue;
		m_IconBackState.Color	= Color4::Dodgerblue;
	}
	return true;
}

//---------------------------.
// アイコンの更新.
//---------------------------.
void CStageIcon::IconUpdate( const CLinking::Handle Handle )
{
	// アイコンの色の更新.
	m_IconState.Color			= Color4::RGBA( CLinking::GetLinkColor( Handle ) );
	m_IconBackState.Color		= Color4::RGBA( CLinking::GetLinkColor( Handle ) );

	// 残りの紐づけの時間のゲージの更新
	const float Ratio = CLinking::GetLinkEndTimeCnt( Handle ) / CLinking::GetLinkEndTime( Handle );
	m_IconBackState.AnimState.UV.y = ICON_BACK_START_UV_Y - ICON_BACK_START_UV_Y * Ratio;
}

//---------------------------.
// スプライト(3D)の描画.
//---------------------------.
void CStageIcon::Sprite3DRender_A()
{
	// トランスフォームの更新.
	m_IconBackState.Transform	= m_Transform;
	m_IconState.Transform		= m_Transform;
	m_IconFrameState.Transform	= m_Transform;

	// アイコンの描画.
	m_pIcon->Render3D( &m_IconFrameState,		true );
	m_pIcon->Render3D( &m_IconState,			true );
	m_pIconBack->Render3D( &m_IconBackState,	true );
	m_pIconBack->Render3D( &m_IconBackState,	true );
}

//---------------------------.
// タグの設定.
//---------------------------.
void CStageIcon::SetTag( const EObjectTag Tag )
{
	switch ( Tag ) {
	case EObjectTag::Customer:			m_IconState.AnimState.AnimNumber = 0;	break;
	case EObjectTag::Employee:			m_IconState.AnimState.AnimNumber = 1;	break;
	case EObjectTag::GoHandingCustomer: m_IconState.AnimState.AnimNumber = 2;	break;
	case EObjectTag::PlayerIsland:		m_IconState.AnimState.AnimNumber = 3;	break;
	default:																	break;
	}
}
