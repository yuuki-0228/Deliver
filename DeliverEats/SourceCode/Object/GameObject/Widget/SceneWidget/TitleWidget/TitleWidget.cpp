#include "TitleWidget.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Utility\Math\Math.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\UIWidget\Title\CofilmationWindowUI\ConfilmationWindowUI.h"
#include "..\..\UIWidget\GameMain\StatusBarUI\StatusBarUI.h"

namespace {
	constexpr int	CHOICE_GAMESTART 		= static_cast<int>( ETitleChoice::GameStart		);	// �Q�[���J�n.
	constexpr int	CHOICE_MINIGAME			= static_cast<int>( ETitleChoice::MiniGame		);	// �~�j�Q�[��.
	constexpr int	CHOICE_PIZZAEDIT		= static_cast<int>( ETitleChoice::PizzaEdit		);	// �s�U�G�f�B�^.
	constexpr int	CHOICE_RANKINGDELETE	= static_cast<int>( ETitleChoice::RankingDelete );	// �����L���O�̍폜.
	constexpr int	CHOICE_GAMEEND			= static_cast<int>( ETitleChoice::GameEnd		);	// �Q�[���I��.

	constexpr int	CHOICE_MAX				= static_cast<int>( ETitleChoice::Max			);	// �ő�v�f��.
	constexpr int	CHOICE_START			= static_cast<int>( ETitleChoice::Start			);	// �ő�v�f��.
	constexpr int	CHOICE_END				= static_cast<int>( ETitleChoice::End			);	// �ő�v�f��.

	constexpr char	CURSOR_MOVE_SE[]		= "CursorMove";										// �J�[�\���ړ�����SE.
}

CTitleWidget::CTitleWidget()
	: m_DeltaTime				( INIT_FLOAT )
	, m_Choice					( INIT_INT )
	, m_pText					( nullptr )
	, m_pConfilmationWindowUI	( nullptr )
	, m_pStatusBarUI			( nullptr )
	, m_TextState				()
{
}

CTitleWidget::~CTitleWidget()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CTitleWidget::Init()
{
	SetSceneName( ESceneList::Title );
	GetSpriteList();

	// �~�j�}�b�v�̏�����.
	CMiniMapUI::Init();

	m_pStatusBarUI = std::make_unique<CStatusBarUI>();
	m_pStatusBarUI->SetSpriteState( m_SpriteState );
	m_pStatusBarUI->SetIsCharging( true );
	if ( m_pStatusBarUI->Init() == false ) return false;

	m_pText		= CSpriteResource::GetSprite( "Text2",	&m_TextState	);
	m_TextState.AnimState.IsSetAnimNumber = true;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CTitleWidget::Update( const float & DeltaTime )
{
	if ( m_pConfilmationWindowUI->GetIsShowWindow() ) return;

	m_DeltaTime = DeltaTime;

	// �X�e�[�^�X�o�[�̍X�V.
	m_pStatusBarUI->Update( DeltaTime );

	// �I��.
	if ( CInput::IsKeyDown( "Right" ) ) {
		m_Choice--;
		if ( m_Choice <= -1 ) m_Choice = CHOICE_END;

		// SE�̍Đ�.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Left" ) ) {
		m_Choice++;
		if ( m_Choice >= CHOICE_MAX ) m_Choice = CHOICE_START;

		// SE�̍Đ�.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}

	// �e�L�X�g�̕ύX.
	m_TextState.AnimState.AnimNumber = m_Choice;

	// �I�������擾����.
	switch ( m_Choice ) {
	// �u�o�΂���v���I�����ꂽ��.
	case CHOICE_GAMESTART:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 923.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 292.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 105.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 105.0f;
		break;
	// �u�W�F�b�g���x����V�ԁv���I�����ꂽ��.
	case CHOICE_MINIGAME:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 587.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 361.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 190.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 190.0f;
		break;
	// �u�s�U�����v���I�����ꂽ��.
	case CHOICE_PIZZAEDIT:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 514.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 456.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 155.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 155.0f;
		break;
	//�u�����L���O�̍폜�v���I�����ꂽ��.
	case CHOICE_RANKINGDELETE:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 408.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 450.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 190.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 190.0f;
		break;
	//�u�Q��v���I�����ꂽ��.
	case CHOICE_GAMEEND:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 210.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 440.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 65.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 65.0f;
		break;
	default:
		break;
	}
	const SSize Size = m_SpriteList["Yubi"][0]->GetSpriteState().Disp;
	m_TextState.Transform.Position								= m_SpriteState["Yubi"][0].Transform.Position;
	m_TextState.Transform.Position.y							-= Size.h;
	m_SpriteState["RightCursor"][0].Transform.Position.y		= m_TextState.Transform.Position.y;
	m_SpriteState["LeftCursor" ][0].Transform.Position.y		= m_TextState.Transform.Position.y;
	m_SpriteState["RightCursor"][0].UIAnimState.StartTransform	= m_SpriteState["RightCursor"][0].Transform;
	m_SpriteState["LeftCursor" ][0].UIAnimState.StartTransform	= m_SpriteState["LeftCursor" ][0].Transform;
	m_SpriteState["Yubi"][0].UIAnimState.StartTransform			= m_SpriteState["Yubi"][0].Transform;

	// �V�[���J��.
	if ( CInput::IsKeyDown( "Decision" ) ) {
		CSoundManager::PlaySE( "Decision" );

		// �I�������擾����.
		switch ( m_Choice ) {
		// �u�o�΂���v���I�����ꂽ��.
		case CHOICE_GAMESTART:
			// ���C���V�[���ɑJ��.
			CSceneManager::SceneChange( ESceneList::GameMain, EFadeType::MaskFade );
			break;
		// �u�W�F�b�g���x����V�ԁv���I�����ꂽ��.
		case CHOICE_MINIGAME:
			// �~�j�Q�[���ɑJ��.
			CSceneManager::SceneChange( ESceneList::MiniGame, EFadeType::MaskFade );
			break;
		// �u�s�U�����v���I�����ꂽ��.
		case CHOICE_PIZZAEDIT:
			// �s�U�G�f�B�^�ɑJ��.
			CSceneManager::SceneChange( ESceneList::PizzaEdit, EFadeType::MaskFade );
			break;
		//�u�����L���O�̍폜�v���I�����ꂽ��.
		case CHOICE_RANKINGDELETE:
			// �m�F��ʂ̊g����n�߂�.
			if ( m_pConfilmationWindowUI == nullptr )return;
			m_pConfilmationWindowUI->StartExpand( true );
			break;
		//�u�Q��v���I�����ꂽ��.
		case CHOICE_GAMEEND:
			// �m�F��ʂ̊g����n�߂�.
			if ( m_pConfilmationWindowUI == nullptr )return;
			m_pConfilmationWindowUI->StartExpand();
			break;
		default:
			break;
		}
	}
}

//---------------------------.
// �ǉ��ōs���`��.
//---------------------------.
void CTitleWidget::LateAddRender()
{
	// �e�L�X�g�̕`��.
	m_pText->RenderUI( &m_TextState );

	// �X�e�[�^�X�o�[�̕`��.
	m_pStatusBarUI->Render();
}
