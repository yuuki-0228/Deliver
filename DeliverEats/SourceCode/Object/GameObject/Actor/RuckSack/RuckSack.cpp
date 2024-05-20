#include "RuckSack.h"
#include "..\..\..\Time\Time.h"
#include "..\..\..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\..\..\Utility\Input\Input.h"

namespace {
	// ����SE.
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
// ������.
//---------------------------.
bool CRuckSack::Init()
{
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CRuckSack::Update( const float& DeltaTime )
{
	m_DeltaTime		= DeltaTime;
	m_JetPackPower	= CXInput::GetLTrigger() + CXInput::GetRTrigger();

	LRGaugeResetUpdate();	// ���E�̃Q�[�W��^�񒆂ɖ߂��X�V.
	UDGaugeResetUpdate();	// �㉺�̃Q�[�W��^�񒆂ɖ߂��X�V.

	// ���i�������Ă��Ȃ��ꍇ������.
	if ( m_IsFoodHave == false ) return;

	LRGaugeUpdate();		// ���E�̃Q�[�W�̍X�V.
	UDGaugeUpdate();		// �㉺�̃Q�[�W�̍X�V.

	TemperatureUpdate();	// ���x�̍X�V.
	DiscountUpdate();		// �����z�̍X�V.

	HotBonusUpdate();		// ���x�{�[�i�X�̍X�V.
	GoldenBonusUpdate();	// �S�[���f���^�C���{�[�i�X�̍X�V.
	DiscountMoneyUpdate();	// �X�������̍X�V.
	EndMoneyUpdate();		// �ŏI���炦�邨���̍X�V.

	UDGaugeSpeedUpdate();	// �㉺�̃Q�[�W�̑��x�̍X�V.
}

//-----------------------.
// �f�o�b�N�̍X�V.
//-----------------------.
void CRuckSack::DebugUpdate( const float& DeltaTime )
{
	CDebugWindow::PushProc( "RuckSack",
		[&] () {
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"���E�Q�[�W               : %5f", m_LRGauge );
		ImGui::Text( u8"�㉺�Q�[�W               : %5f", m_UDGauge );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"�ŏI���炦�邨��         : %5f", m_FoodState.EndMoney );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"�Œ�����炦�邨��       : %5f", m_FoodState.Money );
		ImGui::Text( u8"�X������                 : %5f", m_FoodState.DiscountMoney );
		ImGui::Text( u8"���x�{�[�i�X             : %5f", m_FoodState.HotBonus );
		ImGui::Text( u8"�S�[���f���^�C���{�[�i�X : %5f", m_FoodState.GoldenBonus );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"���x                     : %5f", m_FoodState.Temperature );
		ImGui::Text( u8"������                   : %5f", m_FoodState.Discount );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"���XNo                   : %d", m_FoodState.ShopNo );
		ImGui::Text( u8"------------------------------" );
	} );
}

//-----------------------.
// �V�������i�̐ݒ�.
//-----------------------.
void CRuckSack::SetNewFood( const int Money, int ShopNo )
{
	if ( m_IsFoodHave ) return;

	// ���i�������Ă���.
	m_IsFoodHave				= true;

	// ����������XNo��ۑ�.
	m_FoodState.ShopNo			= ShopNo;

	// ���i�̏�����.
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
// ���E�̃Q�[�W��^�񒆂ɖ߂��X�V.
//---------------------------.
void CRuckSack::LRGaugeResetUpdate()
{
	// �����ɖ߂��Ă���.
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
// �㉺�̃Q�[�W��^�񒆂ɖ߂��X�V.
//---------------------------.
void CRuckSack::UDGaugeResetUpdate()
{
	// �����ɖ߂��Ă���.
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
// ���E�̃Q�[�W�̍X�V.
//---------------------------.
void CRuckSack::LRGaugeUpdate()
{
	// �Q�[�W�𓮂���.
	m_LRGauge += ( *m_pTilt / Math::ToRadian( CConstant::GetPlayer().END_TILE ) ) * ( m_DeltaTime * CONSTANT.GAUGE_LR_SPPED );
	
	// �Q�[�W��͈͓��Ɏ��߂�.
	m_LRGauge = std::clamp( m_LRGauge, CONSTANT.MIN_GAUGE, CONSTANT.MAX_GAUGE );
}

//---------------------------.
// �㉺�̃Q�[�W�̍X�V.
//---------------------------.
void CRuckSack::UDGaugeUpdate()
{
	// �Q�[�W�𓮂���.
	m_UDGauge += m_UDGaugeSpeed * ( m_DeltaTime * CONSTANT.GAUGE_UD_SPPED );

	// �Q�[�W��͈͓��Ɏ��߂�.
	m_UDGauge = std::clamp( m_UDGauge, CONSTANT.MIN_GAUGE, CONSTANT.MAX_GAUGE );
}

//-----------------------.
// �㉺�Q�[�W�̑��x�̍X�V.
//-----------------------.
void CRuckSack::UDGaugeSpeedUpdate()
{
	// ���x�̍X�V.
	const float& PowerDiff	= m_OldJetPackPower - m_JetPackPower;
	m_UDGaugeSpeed -= PowerDiff;

	// �㉺�Q�[�W�̑��x�����炵�Ă���.
	if ( m_UDGaugeSpeed > 0.0f ) {
		m_UDGaugeSpeed -= CONSTANT.UD_GAUGE_DOWN_SPEED;
		if ( m_UDGaugeSpeed <= 0.0f ) m_UDGaugeSpeed = 0.0f;
	}
	if ( m_UDGaugeSpeed < 0.0f ) {
		m_UDGaugeSpeed += CONSTANT.UD_GAUGE_DOWN_SPEED;
		if ( m_UDGaugeSpeed >= 0.0f ) m_UDGaugeSpeed = 0.0f;
	}

	// ����̃W�F�b�g�p�b�N�̋�����ۑ�.
	m_OldJetPackPower = m_JetPackPower;
}

//-----------------------.
// ���x�̍X�V.
//-----------------------.
void CRuckSack::TemperatureUpdate()
{
	if ( m_FoodState.Temperature <= CONSTANT.MIN_TEMPERATURE ) return;

	// ���x���܂��Ă���.
	m_FoodState.Temperature -= CONSTANT.COOLDOWN_SPEED * m_DeltaTime;
	if ( m_FoodState.Temperature <= CONSTANT.MIN_TEMPERATURE )
		m_FoodState.Temperature = CONSTANT.MIN_TEMPERATURE;
}

//-----------------------.
// �����̍X�V.
//-----------------------.
void CRuckSack::DiscountUpdate()
{
	if ( ( CONSTANT.MIN_GAUGE < m_LRGauge && m_LRGauge < CONSTANT.MAX_GAUGE ) &&
		 ( CONSTANT.MIN_GAUGE < m_UDGauge && m_UDGauge < CONSTANT.MAX_GAUGE )	) return;
	if ( m_FoodState.Discount	>= CONSTANT.MAX_DISCOUNT						) return;

	// �����z�𑝂₵�Ă���.
	m_FoodState.Discount		+= CONSTANT.DISCOUNT_SPPED * m_DeltaTime;
	if ( m_FoodState.Discount	>= CONSTANT.MAX_DISCOUNT ) m_FoodState.Discount = CONSTANT.MAX_DISCOUNT;

	// SE�̍Đ�.
	CSoundManager::PlaySE( DISCOUNT_SE, 0.5f );
}

//-----------------------.
// ���x�{�[�i�X�z�̍X�V.
//-----------------------.
void CRuckSack::HotBonusUpdate() {
	const float Bonus		= ( m_FoodState.Money - m_FoodState.DiscountMoney ) * ( 1.0f + m_FoodState.Discount ) * ( m_FoodState.Temperature / CONSTANT.MAX_TEMPERATURE );
	m_FoodState.HotBonus	= static_cast<int>( Bonus );
}

//-----------------------.
// �S�[���f���^�C���{�[�i�X�z�̍X�V.
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
// �X�������z�̍X�V.
//-----------------------.
void CRuckSack::DiscountMoneyUpdate() {
	const float Money			= m_FoodState.Money * m_FoodState.Discount;
	m_FoodState.DiscountMoney	= static_cast<int>( Money );
}

//-----------------------.
// �ŏI���炦�������̍X�V.
//-----------------------.
void CRuckSack::EndMoneyUpdate() {
	m_FoodState.EndMoney = m_FoodState.Money - m_FoodState.DiscountMoney + m_FoodState.HotBonus + m_FoodState.GoldenBonus;
}