#include "NowTimeUI.h"
#include "..\..\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\..\Time\Time.h"

CNowTimeUI::CNowTimeUI()
	: m_pFont		( nullptr )
	, m_FontState	()
{
}

CNowTimeUI::~CNowTimeUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CNowTimeUI::Init()
{
	// �t�H���g�̎擾.
	m_pFont		= CFontResource::GetFont( "�R�[�|���[�g�E���S ver2 Medium", &m_FontState );

	// �X�}�z�̉�ʂ̈ʒu���擾.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize = CMiniMapUI::GetScreenSize();
	
	// �\���ʒu�̐ݒ�.
	m_FontState.Transform.Position.x	= ScreenPos.x - ScreenSize.w / 2.0f + m_pFont->GetFontState().Disp.w / 2.0f + 10.0f;
	m_FontState.Transform.Position.y	= ScreenPos.y - ScreenSize.h / 2.0f - m_pFont->GetFontState().Disp.h / 2.0f - 10.0f;
	m_FontState.Transform.Scale			= { 0.5f, 0.5f, 0.5f };

	// �����̏�����.
	m_FontState.Text = "12:00";
	
	// �F�̐ݒ�.
	m_FontState.Color = Color4::Black;

	// �����̕\�����@�𒆉������ɕύX.
	m_FontState.TextAlign = ETextAlign::Left;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CNowTimeUI::Update( const float& DeltaTime )
{
	// ���Ԃ̕�����̎擾.
	const CTime::Time_String NowTime = CTime::GetTimeString();

	// �t�H���g�ɕ������ݒ�.
	m_FontState.Text = NowTime.first + ":" + NowTime.second;
}

//---------------------------.
// �`��.
//---------------------------.
void CNowTimeUI::Render()
{
	m_pFont->RenderUI( &m_FontState );
}
