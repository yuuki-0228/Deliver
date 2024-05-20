#include "TemperatureGaugeUI.h"
#include "..\..\..\..\..\Actor\Character\Player\Player.h"

namespace {
	// �A�C�R���̔w�i�̊J�n����y���W��UV�̈ʒu.
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
// ������.
//---------------------------.
bool CTemperatureGaugeUI::Init()
{
	// �摜�̎擾.
	m_pGauge	= CSpriteResource::GetSprite( "RucksackGauge",	&m_GaugeState );
	m_pFrame	= CSpriteResource::GetSprite( "RucksackFrame",	&m_FrameState );

	// �}�X�N�摜�̐ݒ�.
	m_GaugeState.pMaskTexture = CSpriteResource::GetSprite( "RucksackGaugeMask" )->GetTexture();

	// UV�̐ݒ�.
	m_GaugeState.AnimState.UV.y = BACK_START_UV_Y;

	// ���W�̐ݒ�.
	const SSize& Size = m_pGauge->GetSpriteState().Disp;
	m_FrameState.Transform.Position.x	= Size.w / 2.0f + 5.0f;
	m_FrameState.Transform.Position.y	= Size.h / 2.0f + 15.0f;
	m_GaugeState.Transform.Position		= m_FrameState.Transform.Position;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CTemperatureGaugeUI::Update( const float& DeltaTime )
{
	if ( m_pPlayer->GetIsFoodHave() == false ) return;

	// ���x�̍X�V.
	const SFoodState&	FoodState	= m_pPlayer->GetFoodState();
	const float			Ratio		= ( FoodState.Temperature - CONSTANT.MIN_TEMPERATURE ) / ( CONSTANT.MAX_TEMPERATURE - CONSTANT.MIN_TEMPERATURE );
	m_GaugeState.AnimState.UV.y = BACK_START_UV_Y * Ratio;
}

//---------------------------.
// �`��.
//---------------------------.
void CTemperatureGaugeUI::Render()
{
	m_pGauge->RenderUI( &m_GaugeState );
	m_pFrame->RenderUI( &m_FrameState );
}
