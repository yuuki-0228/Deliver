#include "TimeGaugeUI.h"
#include "..\..\..\..\..\Time\Time.h"

namespace {
	// �Q�[�W�̊J�n����y���W��UV�̈ʒu.
	constexpr float GAUGE_END_UV_X = 0.76f;
}

CTimeGaugeUI::CTimeGaugeUI()
	: m_pGauge		( nullptr )	
	, m_pFrame		( nullptr )	
	, m_pFont		( nullptr )	
	, m_GaugeState	()
	, m_FrameState	()
	, m_FontState	()
{
}

CTimeGaugeUI::~CTimeGaugeUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CTimeGaugeUI::Init()
{
	m_pGauge	= CSpriteResource::GetSprite( "TimeGauge",		&m_GaugeState );
	m_pFrame	= CSpriteResource::GetSprite( "TimeGaugeFlame",	&m_FrameState );
	m_pFont		= CFontResource::GetFont( "NasuM",				&m_FontState );
	m_GaugeState.AnimState.UV.x = 0.0f;

	// �t�H���g�̐ݒ�.
	m_FontState.TextAlign		= ETextAlign::Center;
	m_FontState.IsOutLine		= true;
	m_FontState.OutLineSize		= 2.0f;
	m_FontState.Color			= Color4::Black;
	m_FontState.OutLineColor	= Color4::White;
	m_FontState.Text			= "12:00";

	// �}�X�N�̐ݒ�.
	m_GaugeState.pMaskTexture = CSpriteResource::GetSprite( "TimeGaugeMask" )->GetTexture();

	// ���W�̐ݒ�.
	const SSize& FrameSize	= m_pFrame->GetSpriteState().Disp;
	const SSize& FontSize	= m_pFont->GetFontState().Disp;
	m_GaugeState.Transform.Position.x	= FWND_W / 2.0f;
	m_GaugeState.Transform.Position.y	= FWND_H - FrameSize.h / 2.0f - 10.0f;
	m_FrameState.Transform.Position		= m_GaugeState.Transform.Position;
	m_FontState.Transform.Position		= m_GaugeState.Transform.Position;
	m_FontState.Transform.Position.y	-= FrameSize.h / 2.0f + FontSize.h + 2.0f;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CTimeGaugeUI::Update( const float& DeltaTime )
{
	// �Q�[�W�̍X�V.
	const float NewTime	= CTime::GetElapsedTime();
	const float Rata	= NewTime / CONSTANT.CLEAR_SCENE_MOVE_TIME_MAX;
	m_GaugeState.AnimState.UV.x = GAUGE_END_UV_X * Rata;

	// ���Ԃ̕�����̎擾.
	const CTime::Time_String NowTime = CTime::GetTimeString();

	// �t�H���g�ɕ������ݒ�.
	m_FontState.Text	= NowTime.first + ":" + NowTime.second;

}

//---------------------------.
// �`��.
//---------------------------.
void CTimeGaugeUI::Render()
{
	m_pGauge->RenderUI( &m_GaugeState );
	m_pFrame->RenderUI( &m_FrameState );
	m_pFont->RenderUI( &m_FontState );
}
