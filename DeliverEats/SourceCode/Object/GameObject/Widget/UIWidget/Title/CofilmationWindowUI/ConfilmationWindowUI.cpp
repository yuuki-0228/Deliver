#include "ConfilmationWindowUI.h"
#include "..\..\UIWidget.h"
#include "..\..\..\..\..\Score\Ranking\Ranking.h"
#include "..\..\..\..\..\..\Resource\FontResource\FontResource.h"
#include "..\..\..\..\..\..\Resource\FontResource\FontResource.h"
#include "..\..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"

namespace {
	enum enSpriteList : unsigned char {
		BackGround,	// �w�i.
		Yubi,		// �J�[�\��.
		Text,		// �e�L�X�g.
		Yes,		// �u�͂��v.
		No,			// �u�������v.

		Max
	}typedef ESpriteList;

	enum enTextNo : unsigned char {
		SelectYes,
		NotYes,
		SelectNo,
		NotNo
	} typedef ETextNo;

	constexpr int	CHOICE_STARTCHOICE	= static_cast<int>( EChoice::StartChoice );		// �ŏ��̑I����.
	constexpr int	CHOICE_ENDCHOICE	= static_cast<int>( EChoice::EndChoice );		// �Ō�̑I����.
	constexpr int	CHOICE_MAX			= static_cast<int>( EChoice::Max );				// �ő�v�f��.

	constexpr int	SCALING_NONE		= static_cast<int>( EScaling::None );			// �ҋ@��.
	constexpr int	SCALING_EXPAND		= static_cast<int>( EScaling::Expand );			// �g�咆.
	constexpr int	SCALING_SHRINK		= static_cast<int>( EScaling::Shrink );			// �k����.

	constexpr char	OPEN_SE[]			= "Open";										// �|�[�Y���J������SE.
	constexpr char	CLOSE_SE[]			= "Close";										// �|�[�Y����鎞��SE.
	constexpr char	CURSOR_MOVE_SE[]	= "CursorMove";									// �J�[�\���ړ�����SE.
	constexpr char	TRASH_SE[]			= "Trash";										// �x�X�g�X�R�A�̍폜����SE.
}

CConfilmationWindowUI::CConfilmationWindowUI()
	: m_DeltaTime				( INIT_FLOAT )
	, m_IsRankingDelete			( false )
	, m_IsShowWindow			( false )
	, m_ScalingSwitch			( INIT_INT )
	, m_ConfilmationWindowScale	( INIT_FLOAT )
	, m_Choice					( INIT_INT )
	, m_AnimCnt					( INIT_FLOAT )
	, m_pSpriteList				( Max )
	, m_SpriteStateList			( Max, SSpriteRenderState() )
{
}

CConfilmationWindowUI::~CConfilmationWindowUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CConfilmationWindowUI::Init()
{
	m_IsShowWindow				= false;
	m_ScalingSwitch				= SCALING_NONE;
	m_ConfilmationWindowScale	= INIT_FLOAT;
	m_Choice					= INIT_INT;

	// �摜�̐ݒ�.
	m_pSpriteList[BackGround]	= CSpriteResource::GetSprite( "back",	&m_SpriteStateList[BackGround]	);
	m_pSpriteList[Yubi]			= CSpriteResource::GetSprite( "Yubi"									);
	m_pSpriteList[Text]			= CSpriteResource::GetSprite( "Text1",	&m_SpriteStateList[Text]		);
	m_pSpriteList[Yes]			= CSpriteResource::GetSprite( "YesNo",	&m_SpriteStateList[Yes]			);
	m_pSpriteList[No]			= CSpriteResource::GetSprite( "YesNo",	&m_SpriteStateList[No]			);

	// �e�̐ݒ�.
	m_SpriteStateList[Yubi].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Text].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Yes].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[No].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );

	// UI�̈ʒu�ݒ�.
	m_SpriteStateList[BackGround].Transform.Position.x	= FWND_W / 2.0f;
	m_SpriteStateList[BackGround].Transform.Position.y	= FWND_H / 2.0f;
	m_SpriteStateList[Text].Transform.Position.x		= 0.0f;
	m_SpriteStateList[Text].Transform.Position.y		= -100.0f;
	m_SpriteStateList[Text].AnimState.IsSetAnimNumber	= true;
	m_SpriteStateList[Text].AnimState.AnimNumber		= 0;
	m_SpriteStateList[Yes].Transform.Position.x			= 0.0f;
	m_SpriteStateList[Yes].Transform.Position.y			= 192.0f;
	m_SpriteStateList[Yes].AnimState.IsSetAnimNumber	= true;
	m_SpriteStateList[Yes].AnimState.AnimNumber			= SelectYes;
	m_SpriteStateList[No].Transform.Position.x			= 0.0f;
	m_SpriteStateList[No].Transform.Position.y			= 64.0f;
	m_SpriteStateList[No].AnimState.IsSetAnimNumber		= true;
	m_SpriteStateList[No].AnimState.AnimNumber			= NotNo;

	SwitchDisp( false );
	SetConfilmationScaleAndAlpha( m_ConfilmationWindowScale );

	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CConfilmationWindowUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	if (m_IsShowWindow == false) return;

	// �A�j���[�V�����p�̃J�E���g�̉��Z.
	m_AnimCnt += Math::ToRadian( 80.0f ) * DeltaTime;
	if ( m_AnimCnt >= Math::RADIAN_MID ) m_AnimCnt -= Math::RADIAN_MID;

	// �I�������擾����.
	const SSize Size = m_pSpriteList[Yes]->GetSpriteState().Disp;
	switch ( m_Choice ) {
	// ��������I�������Ƃ�.
	case CHOICE_STARTCHOICE:
		m_SpriteStateList[Yubi].Transform.Position		= m_SpriteStateList[No].Transform.Position;
		m_SpriteStateList[Yubi].Transform.Position.x	-= Size.w / 2.0f - 10.0f * sinf( m_AnimCnt );
		m_SpriteStateList[Yubi].Transform.Position.y	-= Size.h / 2.0f;
		m_SpriteStateList[Yes].AnimState.AnimNumber		= NotYes;
		m_SpriteStateList[No ].AnimState.AnimNumber		= SelectNo;
		break;
	// �͂���I�������Ƃ�.
	case CHOICE_ENDCHOICE:
		m_SpriteStateList[Yubi].Transform.Position		= m_SpriteStateList[Yes].Transform.Position;
		m_SpriteStateList[Yubi].Transform.Position.x	-= Size.w / 2.0f - 10.0f * sinf( m_AnimCnt );
		m_SpriteStateList[Yubi].Transform.Position.y	-= Size.h / 2.0f;
		m_SpriteStateList[Yes].AnimState.AnimNumber		= SelectYes;
		m_SpriteStateList[No].AnimState.AnimNumber		= NotNo;
		break;
	default:
		break;
	}

	switch ( m_ScalingSwitch ) {
	case SCALING_EXPAND:	Expand();					break; // �g�咆�̏���.
	case SCALING_SHRINK:	Shrink();					break; // �k�����̏���.
	case SCALING_NONE:		UpdateConfilmationWindow(); break; // �\�����̏���.
	default:											break;
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CConfilmationWindowUI::Render()
{
	for ( int i = 0; i < Max;++i ) {
		m_pSpriteList[i]->RenderUI( &m_SpriteStateList[i] );
	}
}

//---------------------------.
//�t�F�[�h�C���J�n.
//---------------------------.
void CConfilmationWindowUI::StartExpand( const bool IsRankingDelete )
{
	// �m�F��ʂ�\��.
	SwitchDisp( true );
	// �m�F��ʂ̊g��J�n.
	m_ScalingSwitch = SCALING_EXPAND;
	// �m�F��ʂ�\����Ԃɂ���.
	m_IsShowWindow	= true;
	// �����L���O�̍폜���ۑ�.
	m_IsRankingDelete = IsRankingDelete;
	// �e�L�X�g�̍X�V.
	if ( m_IsRankingDelete ) m_SpriteStateList[Text].AnimState.AnimNumber = 0;
	else					 m_SpriteStateList[Text].AnimState.AnimNumber = 1;

	// SE�̍Đ�.
	CSoundManager::PlaySE( OPEN_SE );
}

//---------------------------.
// �\��/��\���̈ꊇ�؂�ւ�.
//	(true:�\��/false:��\��).
//---------------------------.
void CConfilmationWindowUI::SwitchDisp( bool flag )
{
	for ( auto& ss : m_SpriteStateList ) ss.IsDisp = flag;
}

//---------------------------.
// �m�F��ʂ̊g�k���ꊇ�Ǘ�.
//---------------------------.
void CConfilmationWindowUI::SetConfilmationScaleAndAlpha( float value )
{
	D3DXVECTOR3 Scale = { value, value, 1.0f };
	m_SpriteStateList[BackGround].Transform.Scale = Scale;

	for ( auto& ss : m_SpriteStateList ) ss.Color.w = value;
}

//---------------------------.
// �g�又��.
//---------------------------.
void CConfilmationWindowUI::Expand()
{
	// �m�F��ʂ��g�傷��.
	m_ConfilmationWindowScale += 3.0f * m_DeltaTime;
	if ( m_ConfilmationWindowScale >= 1.0f ) {
		m_ConfilmationWindowScale = 1.0f;

		// �m�F��ʂ̊g�����������.
		m_ScalingSwitch = SCALING_NONE;
	}
	SetConfilmationScaleAndAlpha( m_ConfilmationWindowScale );
}

//---------------------------.
// �k������.
//---------------------------.
void CConfilmationWindowUI::Shrink()
{
	// �m�F��ʂ��k������.
	m_ConfilmationWindowScale -= 3.0f * m_DeltaTime;
	if ( m_ConfilmationWindowScale <= 0.0f ) {
		m_ConfilmationWindowScale = 0.0f;

		// �m�F��ʂ̏k������������.
		m_ScalingSwitch = SCALING_NONE;
		m_Choice = 0;

		// �m�F��ʂ��\����Ԃɂ���.
		m_IsShowWindow = false;
		SwitchDisp( false );
	}
	SetConfilmationScaleAndAlpha( m_ConfilmationWindowScale );
}

//---------------------------.
// �m�F��ʕ\�����̏���.
//---------------------------.
void CConfilmationWindowUI::UpdateConfilmationWindow()
{
	// �I��.
	if ( CInput::IsKeyDown( "Up" ) ) {
		// �J�[�\�����グ��.
		m_Choice--;
		if ( m_Choice <= -1 ) m_Choice = CHOICE_ENDCHOICE;

		// SE�̍Đ�.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Down" ) ) {
		// �J�[�\����������.
		m_Choice++;
		if ( m_Choice >= CHOICE_MAX ) m_Choice = CHOICE_STARTCHOICE;

		// SE�̍Đ�.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}

	if ( CInput::IsKeyDown( "Decision" ) ) {
		CSoundManager::PlaySE( "Decision" );
		CSoundManager::PlaySE( CLOSE_SE );

		// �I�������擾����.
		switch ( m_Choice ) {
		case CHOICE_STARTCHOICE:	// ��������I�������Ƃ�.
			// �m�F��ʂ̌����t�F�[�h�C������.

			// �m�F��ʂ̏k���J�n.
			m_ScalingSwitch = SCALING_SHRINK;
			break;
		case CHOICE_ENDCHOICE:	// �͂���I�������Ƃ�.
			// �����L���O�̍폜.
			if ( m_IsRankingDelete ) {
				CRanking::Reset();

				// �m�F��ʂ̏k���J�n.
				m_ScalingSwitch = SCALING_SHRINK;

				// SE�̍Đ�.
				CSoundManager::PlaySE( TRASH_SE );
				break;
			}
			// �Q�[�����I������.
			PostQuitMessage( 0 );
			break;
		default:
			break;
		}
	}
}