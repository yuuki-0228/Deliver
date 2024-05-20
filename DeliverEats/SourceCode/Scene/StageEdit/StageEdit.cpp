#include "StageEdit.h"
#include "..\..\Editor\StageEditor\StageEditor.h"
#include "..\..\Common\DirectX\DirectX11.h"
#include "..\..\Object\GameObject\Actor\Wind\Wind.h"

CStageEdit::CStageEdit()
	: m_pStageEditor	( nullptr )
{
	// �J�����̕ύX���֎~����.
	CCameraManager::SetIsCameraChangeLock( true );
}

CStageEdit::~CStageEdit()
{
	// �J�����̕ύX�̋֎~������.
	CCameraManager::SetIsCameraChangeLock( false );
}

//---------------------------.
// ������.
//---------------------------.
bool CStageEdit::Init()
{
	// UI�G�f�B�^�̏�����.
	m_pStageEditor = std::make_unique<CStageEditor>();
	m_pStageEditor->Init();

	// �J�����̐ݒ�.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::FreeCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CFreeCamera>() );

	// ������v���C���[���폜���Ă���.
	CWind::SetPlayer( nullptr );

	// BGM�̐ݒ�.
	m_BGMName = "12-13";
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CStageEdit::Update( const float& DeltaTime )
{
	// UI�G�f�B�^�̍X�V.
	m_pStageEditor->Update( DeltaTime );

	// BGM�̍Đ�.
	CSoundManager::PlayBGM( m_BGMName );

	// �V�[���ڍs.
	if ( CInput::IsKeyDown( "Back" ) ) {
		CSoundManager::PlaySE( "Decision" );
		CSceneManager::SceneChange( ESceneList::Title, EFadeType::MaskFade );
	}
}

//---------------------------.
// ���f���̕`��.
//---------------------------.
void CStageEdit::ModelRender()
{
	m_pStageEditor->ModelRender();
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CStageEdit::SpriteUIRender()
{
	m_pStageEditor->ImGuiRender();
	m_pStageEditor->UIRender();
}
