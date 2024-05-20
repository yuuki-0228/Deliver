#include "ShakingBarUI.h"
#include "..\..\..\..\..\Actor\Character\Player\Player.h"

CShakingBarUI::CShakingBarUI()
	: m_pPlayer						( nullptr )
	, m_pHorizontalShakingGauge		( nullptr )
	, m_pVerticalShakingGauge		( nullptr )
	, m_pVerticalShakingBar			( nullptr )
	, m_pHorizontalShakingBar		( nullptr )
	, m_HorizontalShakingGaugeState	()
	, m_VerticalShakingGaugeState	()
	, m_VerticalShakingBarState		()
	, m_HorizontalShakingBarState	()
{
}

CShakingBarUI::~CShakingBarUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CShakingBarUI::Init()
{
	// �摜�̎擾.
	m_pHorizontalShakingGauge	= CSpriteResource::GetSprite( "HorizontalShakingGauge", &m_HorizontalShakingGaugeState );
	m_pVerticalShakingGauge		= CSpriteResource::GetSprite( "VerticalShakingGauge",	&m_VerticalShakingGaugeState );
	m_pHorizontalShakingBar		= CSpriteResource::GetSprite( "HorizontalShakingBar",	&m_HorizontalShakingBarState );
	m_pVerticalShakingBar		= CSpriteResource::GetSprite( "VerticalShakingBar",		&m_VerticalShakingBarState );

	// ���W�̐ݒ�.
	m_HorizontalShakingGaugeState.Transform.Position.x	= m_Transform.Position.x;
	m_HorizontalShakingGaugeState.Transform.Position.y	= m_Transform.Position.x + 152.0f;
	m_HorizontalShakingBarState.Transform.Position		= m_HorizontalShakingGaugeState.Transform.Position;
	m_VerticalShakingGaugeState.Transform.Position.x	= m_Transform.Position.x + 152.0f;
	m_VerticalShakingGaugeState.Transform.Position.y	= m_Transform.Position.y;
	m_VerticalShakingBarState.Transform.Position		= m_VerticalShakingGaugeState.Transform.Position;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CShakingBarUI::Update( const float& DeltaTime )
{
	// �v���C���[�̌X�����擾����.
	m_HorizontalShakingBarState.Transform.Position.x =
		m_HorizontalShakingGaugeState.Transform.Position.x + m_pPlayer->GetRuckSackLRGauge() * CONSTANT.BAR_MOVE_SCALE;
	m_VerticalShakingBarState.Transform.Position.y =
		m_VerticalShakingGaugeState.Transform.Position.y + m_pPlayer->GetRuckSackUDGauge() * CONSTANT.BAR_MOVE_SCALE;
}

//---------------------------.
// �`��.
//---------------------------.
void CShakingBarUI::Render()
{
	m_pHorizontalShakingGauge->RenderUI( &m_HorizontalShakingGaugeState );
	m_pVerticalShakingGauge->RenderUI( &m_VerticalShakingGaugeState );
	m_pHorizontalShakingBar->RenderUI( &m_HorizontalShakingBarState );
	m_pVerticalShakingBar->RenderUI( &m_VerticalShakingBarState );
}
