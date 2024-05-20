#include "RuckSack.h"
#include "..\..\..\Time\Time.h"
#include "..\..\..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\..\..\Utility\Input\Input.h"

namespace {
	// 割引SE.
	constexpr char DISCOUNT_SE[] = "Discount";
}

CRuckSack::CRuckSack()
	: m_FoodState		()
	, m_IsFoodHave		( false )
	, m_JetPackPower	( INIT_FLOAT )
	, m_OldJetPackPower	( INIT_FLOAT )
	, m_UDGaugeSpeed	( INIT_FLOAT )
	, m_pTilt			( nullptr )
	, m_LRGauge			( INIT_FLOAT )
	, m_UDGauge			( INIT_FLOAT )
{
}

CRuckSack::~CRuckSack()
{
	CSoundManager::StopSE( DISCOUNT_SE, true );
}

//---------------------------.
// 初期化.
//---------------------------.
bool CRuckSack::Init()
{
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CRuckSack::Update( const float& DeltaTime )
{
	m_DeltaTime		= DeltaTime;
	m_JetPackPower	= CXInput::GetLTrigger() + CXInput::GetRTrigger();

	LRGaugeResetUpdate();	// 左右のゲージを真ん中に戻す更新.
	UDGaugeResetUpdate();	// 上下のゲージを真ん中に戻す更新.

	// 商品を持っていない場合抜ける.
	if ( m_IsFoodHave == false ) return;

	LRGaugeUpdate();		// 左右のゲージの更新.
	UDGaugeUpdate();		// 上下のゲージの更新.

	TemperatureUpdate();	// 温度の更新.
	DiscountUpdate();		// 割引額の更新.

	HotBonusUpdate();		// 温度ボーナスの更新.
	GoldenBonusUpdate();	// ゴールデンタイムボーナスの更新.
	DiscountMoneyUpdate();	// 傾き割引の更新.
	EndMoneyUpdate();		// 最終もらえるお金の更新.

	UDGaugeSpeedUpdate();	// 上下のゲージの速度の更新.
}

//-----------------------.
// デバックの更新.
//-----------------------.
void CRuckSack::DebugUpdate( const float& DeltaTime )
{
	CDebugWindow::PushProc( "RuckSack",
		[&] () {
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"左右ゲージ               : %5f", m_LRGauge );
		ImGui::Text( u8"上下ゲージ               : %5f", m_UDGauge );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"最終もらえるお金         : %5f", m_FoodState.EndMoney );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"最低限もらえるお金       : %5f", m_FoodState.Money );
		ImGui::Text( u8"傾き割引                 : %5f", m_FoodState.DiscountMoney );
		ImGui::Text( u8"温度ボーナス             : %5f", m_FoodState.HotBonus );
		ImGui::Text( u8"ゴールデンタイムボーナス : %5f", m_FoodState.GoldenBonus );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"温度                     : %5f", m_FoodState.Temperature );
		ImGui::Text( u8"割引率                   : %5f", m_FoodState.Discount );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"お店No                   : %d", m_FoodState.ShopNo );
		ImGui::Text( u8"------------------------------" );
	} );
}

//-----------------------.
// 新しい商品の設定.
//-----------------------.
void CRuckSack::SetNewFood( const int Money, int ShopNo )
{
	if ( m_IsFoodHave ) return;

	// 商品を持っている.
	m_IsFoodHave				= true;

	// もらったお店Noを保存.
	m_FoodState.ShopNo			= ShopNo;

	// 商品の初期化.
	m_FoodState.EndMoney		= Money;
	m_FoodState.Money			= Money;
	m_FoodState.DiscountMoney	= INIT_INT;
	m_FoodState.HotBonus		= Money;
	m_FoodState.GoldenBonus		= INIT_INT;
	m_FoodState.Temperature		= CONSTANT.MAX_TEMPERATURE;
	m_FoodState.Discount		= CONSTANT.MIN_DISCOUNT;
	m_FoodState.Discount		= CONSTANT.MIN_DISCOUNT;
}

//---------------------------.
// 左右のゲージを真ん中に戻す更新.
//---------------------------.
void CRuckSack::LRGaugeResetUpdate()
{
	// 中央に戻していく.
	if ( m_LRGauge >= CONSTANT.INIT_GAUGE ) {
		m_LRGauge -= CONSTANT.GAUGE_CENTER_SPPED;
		if ( m_LRGauge <= CONSTANT.INIT_GAUGE ) m_LRGauge = CONSTANT.INIT_GAUGE;
	}
	if ( m_LRGauge <= CONSTANT.INIT_GAUGE ) {
		m_LRGauge += CONSTANT.GAUGE_CENTER_SPPED;
		if ( m_LRGauge >= CONSTANT.INIT_GAUGE ) m_LRGauge = CONSTANT.INIT_GAUGE;
	}
}

//---------------------------.
// 上下のゲージを真ん中に戻す更新.
//---------------------------.
void CRuckSack::UDGaugeResetUpdate()
{
	// 中央に戻していく.
	if ( m_UDGauge >= CONSTANT.INIT_GAUGE ) {
		m_UDGauge -= CONSTANT.GAUGE_CENTER_SPPED;
		if ( m_UDGauge <= CONSTANT.INIT_GAUGE ) m_UDGauge = CONSTANT.INIT_GAUGE;
	}
	if ( m_UDGauge <= CONSTANT.INIT_GAUGE ) {
		m_UDGauge += CONSTANT.GAUGE_CENTER_SPPED;
		if ( m_UDGauge >= CONSTANT.INIT_GAUGE ) m_UDGauge = CONSTANT.INIT_GAUGE;
	}
}

//---------------------------.
// 左右のゲージの更新.
//---------------------------.
void CRuckSack::LRGaugeUpdate()
{
	// ゲージを動かす.
	m_LRGauge += ( *m_pTilt / Math::ToRadian( CConstant::GetPlayer().END_TILE ) ) * ( m_DeltaTime * CONSTANT.GAUGE_LR_SPPED );
	
	// ゲージを範囲内に収める.
	m_LRGauge = std::clamp( m_LRGauge, CONSTANT.MIN_GAUGE, CONSTANT.MAX_GAUGE );
}

//---------------------------.
// 上下のゲージの更新.
//---------------------------.
void CRuckSack::UDGaugeUpdate()
{
	// ゲージを動かす.
	m_UDGauge += m_UDGaugeSpeed * ( m_DeltaTime * CONSTANT.GAUGE_UD_SPPED );

	// ゲージを範囲内に収める.
	m_UDGauge = std::clamp( m_UDGauge, CONSTANT.MIN_GAUGE, CONSTANT.MAX_GAUGE );
}

//-----------------------.
// 上下ゲージの速度の更新.
//-----------------------.
void CRuckSack::UDGaugeSpeedUpdate()
{
	// 速度の更新.
	const float& PowerDiff	= m_OldJetPackPower - m_JetPackPower;
	m_UDGaugeSpeed -= PowerDiff;

	// 上下ゲージの速度を減らしていく.
	if ( m_UDGaugeSpeed > 0.0f ) {
		m_UDGaugeSpeed -= CONSTANT.UD_GAUGE_DOWN_SPEED;
		if ( m_UDGaugeSpeed <= 0.0f ) m_UDGaugeSpeed = 0.0f;
	}
	if ( m_UDGaugeSpeed < 0.0f ) {
		m_UDGaugeSpeed += CONSTANT.UD_GAUGE_DOWN_SPEED;
		if ( m_UDGaugeSpeed >= 0.0f ) m_UDGaugeSpeed = 0.0f;
	}

	// 今回のジェットパックの強さを保存.
	m_OldJetPackPower = m_JetPackPower;
}

//-----------------------.
// 温度の更新.
//-----------------------.
void CRuckSack::TemperatureUpdate()
{
	if ( m_FoodState.Temperature <= CONSTANT.MIN_TEMPERATURE ) return;

	// 温度を冷ましていく.
	m_FoodState.Temperature -= CONSTANT.COOLDOWN_SPEED * m_DeltaTime;
	if ( m_FoodState.Temperature <= CONSTANT.MIN_TEMPERATURE )
		m_FoodState.Temperature = CONSTANT.MIN_TEMPERATURE;
}

//-----------------------.
// 割引の更新.
//-----------------------.
void CRuckSack::DiscountUpdate()
{
	if ( ( CONSTANT.MIN_GAUGE < m_LRGauge && m_LRGauge < CONSTANT.MAX_GAUGE ) &&
		 ( CONSTANT.MIN_GAUGE < m_UDGauge && m_UDGauge < CONSTANT.MAX_GAUGE )	) return;
	if ( m_FoodState.Discount	>= CONSTANT.MAX_DISCOUNT						) return;

	// 割引額を増やしていく.
	m_FoodState.Discount		+= CONSTANT.DISCOUNT_SPPED * m_DeltaTime;
	if ( m_FoodState.Discount	>= CONSTANT.MAX_DISCOUNT ) m_FoodState.Discount = CONSTANT.MAX_DISCOUNT;

	// SEの再生.
	CSoundManager::PlaySE( DISCOUNT_SE, 0.5f );
}

//-----------------------.
// 温度ボーナス額の更新.
//-----------------------.
void CRuckSack::HotBonusUpdate() {
	const float Bonus		= ( m_FoodState.Money - m_FoodState.DiscountMoney ) * ( 1.0f + m_FoodState.Discount ) * ( m_FoodState.Temperature / CONSTANT.MAX_TEMPERATURE );
	m_FoodState.HotBonus	= static_cast<int>( Bonus );
}

//-----------------------.
// ゴールデンタイムボーナス額の更新.
//-----------------------.
void CRuckSack::GoldenBonusUpdate() {
	if ( CTime::GetIsGoldenTime() == false ) {
		m_FoodState.GoldenBonus = 0;
		return;
	}

	m_FoodState.GoldenBonus = static_cast<int>( ( m_FoodState.Money - m_FoodState.DiscountMoney * 2.0f + m_FoodState.HotBonus ) / 2.0f );
	if ( m_FoodState.GoldenBonus < 0 ) m_FoodState.GoldenBonus = 0;
}

//-----------------------.
// 傾き割引額の更新.
//-----------------------.
void CRuckSack::DiscountMoneyUpdate() {
	const float Money			= m_FoodState.Money * m_FoodState.Discount;
	m_FoodState.DiscountMoney	= static_cast<int>( Money );
}

//-----------------------.
// 最終もらえたお金の更新.
//-----------------------.
void CRuckSack::EndMoneyUpdate() {
	m_FoodState.EndMoney = m_FoodState.Money - m_FoodState.DiscountMoney + m_FoodState.HotBonus + m_FoodState.GoldenBonus;
}