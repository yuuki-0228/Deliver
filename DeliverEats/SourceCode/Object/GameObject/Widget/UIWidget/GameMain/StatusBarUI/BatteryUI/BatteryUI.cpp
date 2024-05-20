#include "BatteryUI.h"
#include "..\..\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\..\Time\Time.h"

namespace {
	constexpr int CHARGING_MAX = 100;	// �[�d�̍ő�l.
	constexpr int CHARGING_MIN = 60;	// �[�d�̍ŏ��l.
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
// ������.
//---------------------------.
bool CBatteryUI::Init()
{
	// �[�d�̉摜�̎擾.
	m_pChargingBack			= CSpriteResource::GetSprite( "BatteryBack", &m_ChargingBackState );
	m_pChargingGage			= CSpriteResource::GetSprite( "BatteryBar", &m_ChargingGageState );
	m_pChargingFrame		= CSpriteResource::GetSprite( "BatteryFrame", &m_ChargingFrameState );
	m_pChargingModeSprite	= CSpriteResource::GetSprite( "BatteryCharging" );

	// �X�}�z�̉�ʂ̈ʒu���擾.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize	= CMiniMapUI::GetScreenSize();

	// �\���ʒu�̐ݒ�.
	const SSize&		 IconSize	= m_pChargingGage->GetSpriteState().Disp;
	m_ChargingBackState.Transform.Position.x	= ScreenPos.x + ScreenSize.w / 2.0f - IconSize.w - 5.0f;
	m_ChargingBackState.Transform.Position.y	= ScreenPos.y - ScreenSize.h / 2.0f - 12.0f;
	m_ChargingGageState.Transform.Position.x	= m_ChargingBackState.Transform.Position.x;
	m_ChargingGageState.Transform.Position.y	= m_ChargingBackState.Transform.Position.y;
	m_ChargingFrameState.Transform.Position.x	= m_ChargingBackState.Transform.Position.x;
	m_ChargingFrameState.Transform.Position.y	= m_ChargingBackState.Transform.Position.y;

	// �`��G���A�̐ݒ�.
	m_ChargingGageState.RenderArea = { 1224.0f, 272.0f, 17.0f, 7.0f };
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CBatteryUI::Update( const float& DeltaTime )
{
	if ( m_Charging <= CHARGING_MIN ) return;

	// �[�d�����炷.
	m_Charging = static_cast<int>( std::ceil( CHARGING_MAX * ( 1.0f - CTime::GetTimeRatio() ) ) );
	if ( m_Charging <= CHARGING_MIN ) {
		m_Charging = CHARGING_MIN;
	}

	// �[�d�̃Q�[�W�̍X�V.
	m_ChargingGageState.Transform.Scale.x = static_cast<float>( m_Charging ) / CHARGING_MAX;
}

//---------------------------.
// �`��.
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
