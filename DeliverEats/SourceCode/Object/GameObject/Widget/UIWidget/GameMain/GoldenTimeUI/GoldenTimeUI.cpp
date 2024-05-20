#include "GoldenTimeUI.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Utility\Easing\Easing.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr float EASING_MAX			= 2.0f;		// �C�[�W���O�̍ő�l.
	constexpr float EFFECT_CNT_MAX		= 180.0f;	// �G�t�F�N�g�̃A�j���[�V�����p�J�E���g�̍ő�l.
	constexpr float EFFECT_ADD_SCALE	= 0.5f;		// �G�t�F�N�g�̒ǉ�����g�k.
}

CGoldenTimeUI::CGoldenTimeUI()
	: m_pNotice				( nullptr )
	, m_pEffect				( nullptr )
	, m_NoticeState			()
	, m_EffectStateBase		()
	, m_EffectStateList		()
	, m_AnimTimeCnt			( INIT_FLOAT )
	, m_EffectPlayTimeCnt	( INIT_FLOAT )
	, m_IsOpen				( false )
	, m_IsClose				( false )
{
}

CGoldenTimeUI::~CGoldenTimeUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CGoldenTimeUI::Init()
{
	m_pNotice	= CSpriteResource::GetSprite( "GoldenTimeNotice", &m_NoticeState );
	m_pEffect	= CSpriteResource::GetSprite( "GoldenTimeEffect", &m_EffectStateBase );
	m_NoticeState.IsDisp						= false;
	m_EffectStateBase.AnimState.IsSetAnimNumber = true;

	// �ʒu�̐ݒ�.
	m_NoticeState.Transform.Position.x = FWND_W / 2.0f;
	m_NoticeState.Transform.Position.y = 0.0f;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGoldenTimeUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	NoticeOpen();			// �Ŕ̕\���̐ݒ�.
	NoticeClose();			// �Ŕ̔�\���̐ݒ�.
	NoticeOpenUpdate();		// �Ŕ̕\���̍X�V.
	NoticeCloseUpdate();	// �Ŕ̔�\���̍X�V.
	EffectPlay();			// �G�t�F�N�g�̍Đ�.
	EffectUpdate();			// �G�t�F�N�g�̍X�V.
}

//---------------------------.
// �`��.
//---------------------------.
void CGoldenTimeUI::Render()
{
	for ( auto& e : m_EffectStateList ) m_pEffect->RenderUI( &e );
	m_pNotice->RenderUI( &m_NoticeState );
}

//---------------------------.
// �Ŕ̕\���̐ݒ�.
//---------------------------.
void CGoldenTimeUI::NoticeOpen()
{
	if ( m_NoticeState.IsDisp				) return;
	if ( CTime::GetIsGoldenTime() == false	) return;

	// �Ŕ�\��������.
	m_NoticeState.IsDisp	= true;
	m_IsOpen				= true;
	m_IsClose				= false;
}

//---------------------------.
// �Ŕ��\���̐ݒ�.
//---------------------------.
void CGoldenTimeUI::NoticeClose()
{
	if ( m_NoticeState.IsDisp == false	) return;
	if ( CTime::GetIsGoldenTime()		) return;

	// �Ŕ��\���ɂ�����.
	m_IsOpen	= false;
	m_IsClose	= true;
}

//---------------------------.
// �Ŕ̕\���̍X�V.
//---------------------------.
void CGoldenTimeUI::NoticeOpenUpdate()
{
	if ( m_NoticeState.IsDisp	== false ) return;
	if ( m_IsOpen				== false ) return;

	// �T�C�Y�̎擾.
	const SSize Size = m_pNotice->GetSpriteState().Disp;

	// �ʒu���X�V.
	m_AnimTimeCnt += m_DeltaTime;
	const float NowPoint = CEasing::OutBounce( m_AnimTimeCnt, EASING_MAX );
	m_NoticeState.Transform.Position.y = Size.h * NowPoint;

	if ( m_AnimTimeCnt > EASING_MAX ) {
		m_AnimTimeCnt	= 0.0f;
		m_IsOpen		= false;
	}
}

//---------------------------.
// �Ŕ̔�\���̍X�V.
//---------------------------.
void CGoldenTimeUI::NoticeCloseUpdate()
{
	if ( m_NoticeState.IsDisp	== false ) return;
	if ( m_IsClose				== false ) return;

	// �T�C�Y�̎擾.
	const SSize Size = m_pNotice->GetSpriteState().Disp;

	// �ʒu���X�V.
	m_AnimTimeCnt += m_DeltaTime;
	const float NowPoint = 1.0f - CEasing::InQuart( m_AnimTimeCnt, EASING_MAX );
	m_NoticeState.Transform.Position.y = Size.h * NowPoint;

	if ( m_AnimTimeCnt > EASING_MAX ) {
		m_AnimTimeCnt			= 0.0f;
		m_IsOpen				= false;
		m_NoticeState.IsDisp	= false;
	}
}

//---------------------------.
// �G�t�F�N�g�̍Đ�.
//---------------------------.
void CGoldenTimeUI::EffectPlay()
{
	if ( m_NoticeState.IsDisp == false ) return;

	m_EffectPlayTimeCnt += m_DeltaTime;
	if ( m_EffectPlayTimeCnt < 0.0f ) return;
	m_EffectPlayTimeCnt = INIT_FLOAT;

	// �\���ʒu�̐ݒ�.
	D3DXPOSITION3 DispPos = INIT_FLOAT3;
	DispPos.x = CRandom::GetRand( 0.0f, FWND_W );
	DispPos.y = CRandom::GetRand( 0.0f, 128.0f );
	if ( CRandom::Probability( 1, 2 ) ) DispPos.y = FWND_H - DispPos.y;

	// �A�j���[�V����No�̐ݒ�.
	const int No = CRandom::GetRand( 0, m_EffectStateBase.AnimState.AnimNumber );

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& s : m_EffectStateList ) {
		if ( s.IsDisp ) continue;

		// �A�j���[�V�������J�n������.
		s = m_EffectStateBase;
		s.Transform.Position	= DispPos;
		s.AnimState.AnimNumber	= No;
		return;
	}

	// �V�����쐬.
	m_EffectStateList.emplace_back( m_EffectStateBase );
	m_EffectStateList.back().Transform.Position		= DispPos;
	m_EffectStateList.back().AnimState.AnimNumber	= No;
}

//---------------------------.
// �G�t�F�N�g�̍X�V.
//---------------------------.
void CGoldenTimeUI::EffectUpdate()
{
	// �A�j���[�V�����̍X�V.
	for ( auto& e : m_EffectStateList ) {
		if ( e.IsDisp == false ) continue;

		// �J�E���g�̒ǉ�.
		e.UIAnimState.AnimCnt += EFFECT_CNT_MAX * m_DeltaTime;
		if ( e.UIAnimState.AnimCnt >= EFFECT_CNT_MAX ) {
			e.UIAnimState.AnimCnt	= EFFECT_CNT_MAX;
			e.IsDisp				= false;
			continue;
		}

		// �A���t�@�l�̐ݒ�.
		e.Color.w = sinf( Math::ToRadian( e.UIAnimState.AnimCnt ) );

		// �g�k�̐ݒ�.
		const float Scale = STransform::NORMAL_SCALE + EFFECT_ADD_SCALE * sinf( Math::ToRadian( e.UIAnimState.AnimCnt ) );
		e.Transform.Scale = { Scale, Scale, Scale };
	}

	// �g�p���Ă��Ȃ��G�t�F�N�g�̍폜.
	const int Size = static_cast<int>( m_EffectStateList.size() );
	for ( int i = Size - 1; i >= 0; --i ) {
		if ( m_EffectStateList[i].IsDisp ) return;
		m_EffectStateList.pop_back();
	}
}
