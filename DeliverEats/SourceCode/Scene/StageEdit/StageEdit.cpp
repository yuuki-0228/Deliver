#include "StageEdit.h"
#include "..\..\Editor\StageEditor\StageEditor.h"
#include "..\..\Common\DirectX\DirectX11.h"
#include "..\..\Object\GameObject\Actor\Wind\Wind.h"

CStageEdit::CStageEdit()
	: m_pStageEditor	( nullptr )
{
	// カメラの変更を禁止する.
	CCameraManager::SetIsCameraChangeLock( true );
}

CStageEdit::~CStageEdit()
{
	// カメラの変更の禁止を解除.
	CCameraManager::SetIsCameraChangeLock( false );
}

//---------------------------.
// 初期化.
//---------------------------.
bool CStageEdit::Init()
{
	// UIエディタの初期化.
	m_pStageEditor = std::make_unique<CStageEditor>();
	m_pStageEditor->Init();

	// カメラの設定.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::FreeCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CFreeCamera>() );

	// 風からプレイヤーを削除しておく.
	CWind::SetPlayer( nullptr );

	// BGMの設定.
	m_BGMName = "12-13";
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CStageEdit::Update( const float& DeltaTime )
{
	// UIエディタの更新.
	m_pStageEditor->Update( DeltaTime );

	// BGMの再生.
	CSoundManager::PlayBGM( m_BGMName );

	// シーン移行.
	if ( CInput::IsKeyDown( "Back" ) ) {
		CSoundManager::PlaySE( "Decision" );
		CSceneManager::SceneChange( ESceneList::Title, EFadeType::MaskFade );
	}
}

//---------------------------.
// モデルの描画.
//---------------------------.
void CStageEdit::ModelRender()
{
	m_pStageEditor->ModelRender();
}

//---------------------------.
// スプライト(UI)の描画.
//---------------------------.
void CStageEdit::SpriteUIRender()
{
	m_pStageEditor->ImGuiRender();
	m_pStageEditor->UIRender();
}
