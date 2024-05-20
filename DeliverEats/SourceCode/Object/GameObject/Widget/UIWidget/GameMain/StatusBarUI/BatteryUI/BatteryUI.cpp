#include "BatteryUI.h"
#include "..\..\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\..\Time\Time.h"

namespace {
	constexpr int CHARGING_MAX = 100;	// 充電の最大値.
	constexpr int CHARGING_MIN = 60;	// 充電の最小値.
}

CBatteryUI::CBatteryUI()
	: m_pChargingBack		( nullptr )
	, m_pChargingGage		( nullptr )
	, m_pChargingFrame		( nullptr )
	, m_pChargingModeSprite	( nullptr )
	, m_ChargingBackState	()
	, m_ChargingGageState	()
	, m_ChargingFrameState	()
	, m_Charging			( CHARGING_MAX )
	, m_IsCharging			( false )
{
}

CBatteryUI::~CBatteryUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CBatteryUI::Init()
{
	// 充電の画像の取得.
	m_pChargingBack			= CSpriteResource::GetSprite( "BatteryBack", &m_ChargingBackState );
	m_pChargingGage			= CSpriteResource::GetSprite( "BatteryBar", &m_ChargingGageState );
	m_pChargingFrame		= CSpriteResource::GetSprite( "BatteryFrame", &m_ChargingFrameState );
	m_pChargingModeSprite	= CSpriteResource::GetSprite( "BatteryCharging" );

	// スマホの画面の位置を取得.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize	= CMiniMapUI::GetScreenSize();

	// 表示位置の設定.
	const SSize&		 IconSize	= m_pChargingGage->GetSpriteState().Disp;
	m_ChargingBackState.Transform.Position.x	= ScreenPos.x + ScreenSize.w / 2.0f - IconSize.w - 5.0f;
	m_ChargingBackState.Transform.Position.y	= ScreenPos.y - ScreenSize.h / 2.0f - 12.0f;
	m_ChargingGageState.Transform.Position.x	= m_ChargingBackState.Transform.Position.x;
	m_ChargingGageState.Transform.Position.y	= m_ChargingBackState.Transform.Position.y;
	m_ChargingFrameState.Transform.Position.x	= m_ChargingBackState.Transform.Position.x;
	m_ChargingFrameState.Transform.Position.y	= m_ChargingBackState.Transform.Position.y;

	// 描画エリアの設定.
	m_ChargingGageState.RenderArea = { 1224.0f, 272.0f, 17.0f, 7.0f };
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CBatteryUI::Update( const float& DeltaTime )
{
	if ( m_Charging <= CHARGING_MIN ) return;

	// 充電を減らす.
	m_Charging = static_cast<int>( std::ceil( CHARGING_MAX * ( 1.0f - CTime::GetTimeRatio() ) ) );
	if ( m_Charging <= CHARGING_MIN ) {
		m_Charging = CHARGING_MIN;
	}

	// 充電のゲージの更新.
	m_ChargingGageState.Transform.Scale.x = static_cast<float>( m_Charging ) / CHARGING_MAX;
}

//---------------------------.
// 描画.
//---------------------------.
void CBatteryUI::Render()
{
	if ( m_IsCharging ) {
		m_pChargingModeSprite->RenderUI( &m_ChargingFrameState );
		return;
	}

	m_pChargingGage->RenderUI( &m_ChargingGageState );
	m_pChargingFrame->RenderUI( &m_ChargingFrameState );
}
