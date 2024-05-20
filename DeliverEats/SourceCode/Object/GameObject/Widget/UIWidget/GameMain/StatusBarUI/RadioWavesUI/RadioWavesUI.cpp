#include "RadioWavesUI.h"
#include "..\..\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr int RADIOWAVES_MAX = 3;	// �d�g�̍ő�.
	constexpr int RADIOWAVES_MIN = 0;	// �d�g�̍ŏ�.
}

CRadioWavesUI::CRadioWavesUI()
	: m_pRadioWaves			( nullptr )
	, m_RadioWavesState		()
{
}

CRadioWavesUI::~CRadioWavesUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CRadioWavesUI::Init()
{
	// �d�g�̉摜�̎擾.
	m_pRadioWaves = CSpriteResource::GetSprite( "RadioWaves", &m_RadioWavesState );

	// �X�}�z�̉�ʂ̈ʒu���擾.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize	= CMiniMapUI::GetScreenSize();

	// �\���ʒu�̐ݒ�.
	m_RadioWavesState.Transform.Position.x = ScreenPos.x + ScreenSize.w / 2.0f - 34.0f;
	m_RadioWavesState.Transform.Position.y = ScreenPos.y - ScreenSize.h / 2.0f - 12.0f;

	// �A�j���[�V�����ԍ���ݒ�ł���悤�ɕύX.
	m_RadioWavesState.AnimState.IsSetAnimNumber = true;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CRadioWavesUI::Update( const float& DeltaTime )
{
	if ( CRandom::Probability( 1, 100 ) == false ) return;

	// �d�g�̋������グ��.
	if ( m_RadioWavesState.AnimState.AnimNumber == RADIOWAVES_MIN ) {
		m_RadioWavesState.AnimState.AnimNumber++;
		return;
	}

	// �d�g�̋�����������.
	if ( m_RadioWavesState.AnimState.AnimNumber == RADIOWAVES_MAX ) {
		m_RadioWavesState.AnimState.AnimNumber--;
		return;
	}

	// �d�g�̋����������_���ŕύX����.
	if ( CRandom::Probability( 1, 3 ) ) {
		m_RadioWavesState.AnimState.AnimNumber--;
	}
	else {
		m_RadioWavesState.AnimState.AnimNumber++;
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CRadioWavesUI::Render()
{
	m_pRadioWaves->RenderUI( &m_RadioWavesState );
}
