#include "PizzaEdit.h"
#include "..\..\Editor\PizzaEditor\PizzaEditor.h"
#include "..\..\Common\DirectX\DirectX11.h"
#include "..\..\Object\GameObject\Actor\Wind\Wind.h"
#include "..\..\Utility\Random\Random.h"

CPizzaEdit::CPizzaEdit()
	: m_pPizzaEditor	( nullptr )
{
	// カメラの変更を禁止する.
	CCameraManager::SetIsCameraChangeLock( true );
}

CPizzaEdit::~CPizzaEdit()
{
	// カメラの変更の禁止を解除.
	CCameraManager::SetIsCameraChangeLock( false );

	// BGMの停止.
	CSoundManager::StopSE( m_BGMName );
}

//---------------------------.
// 初期化.
//---------------------------.
bool CPizzaEdit::Init()
{
	// UIエディタの初期化.
	m_pPizzaEditor = std::make_unique<CPizzaEditor>();
	m_pPizzaEditor->Init();

	// カメラの設定.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::FreeCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CFreeCamera>() );

	// 風からプレイヤーを削除しておく.
	CWind::SetPlayer( nullptr );

	// BGMの設定.
	m_BGMName = "PizzaEditor_" + std::to_string( CRandom::GetRand( 1, 20 ) );
	CSoundManager::PlaySE( m_BGMName );
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CPizzaEdit::Update( const float& DeltaTime )
{
	// UIエディタの更新.
	m_pPizzaEditor->Update( DeltaTime );

	// BGMの再生.
	if ( CSoundManager::IsStoppedSE( m_BGMName ) == false ) return;
	m_BGMName = "PizzaEditor_" + std::to_string( CRandom::GetRand( 1, 20 ) );
	CSoundManager::PlaySE( m_BGMName );
}

//---------------------------.
// モデルの描画.
//---------------------------.
void CPizzaEdit::ModelRender()
{
	m_pPizzaEditor->ModelRender();
}

//---------------------------.
// スプライト(UI)の描画.
//---------------------------.
void CPizzaEdit::SpriteUIRender()
{
	m_pPizzaEditor->ImGuiRender();
	m_pPizzaEditor->UIRender();
}
