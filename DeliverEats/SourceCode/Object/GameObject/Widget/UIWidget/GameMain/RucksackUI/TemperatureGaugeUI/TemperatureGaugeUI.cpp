#include "TemperatureGaugeUI.h"
#include "..\..\..\..\..\Actor\Character\Player\Player.h"

namespace {
	// アイコンの背景の開始時のy座標のUVの位置.
	constexpr float BACK_START_UV_Y = 0.5f;
}

CTemperatureGaugeUI::CTemperatureGaugeUI()
	: m_pPlayer		( nullptr )
	, m_pGauge		( nullptr )
	, m_pFrame		( nullptr )
	, m_GaugeState	()
	, m_FrameState	()
{
}

CTemperatureGaugeUI::~CTemperatureGaugeUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CTemperatureGaugeUI::Init()
{
	// 画像の取得.
	m_pGauge	= CSpriteResource::GetSprite( "RucksackGauge",	&m_GaugeState );
	m_pFrame	= CSpriteResource::GetSprite( "RucksackFrame",	&m_FrameState );

	// マスク画像の設定.
	m_GaugeState.pMaskTexture = CSpriteResource::GetSprite( "RucksackGaugeMask" )->GetTexture();

	// UVの設定.
	m_GaugeState.AnimState.UV.y = BACK_START_UV_Y;

	// 座標の設定.
	const SSize& Size = m_pGauge->GetSpriteState().Disp;
	m_FrameState.Transform.Position.x	= Size.w / 2.0f + 5.0f;
	m_FrameState.Transform.Position.y	= Size.h / 2.0f + 15.0f;
	m_GaugeState.Transform.Position		= m_FrameState.Transform.Position;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CTemperatureGaugeUI::Update( const float& DeltaTime )
{
	if ( m_pPlayer->GetIsFoodHave() == false ) return;

	// 温度の更新.
	const SFoodState&	FoodState	= m_pPlayer->GetFoodState();
	const float			Ratio		= ( FoodState.Temperature - CONSTANT.MIN_TEMPERATURE ) / ( CONSTANT.MAX_TEMPERATURE - CONSTANT.MIN_TEMPERATURE );
	m_GaugeState.AnimState.UV.y = BACK_START_UV_Y * Ratio;
}

//---------------------------.
// 描画.
//---------------------------.
void CTemperatureGaugeUI::Render()
{
	m_pGauge->RenderUI( &m_GaugeState );
	m_pFrame->RenderUI( &m_FrameState );
}
