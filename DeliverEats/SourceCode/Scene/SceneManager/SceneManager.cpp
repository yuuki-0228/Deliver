#include "SceneManager.h"
#include "..\Title\Title.h"
#include "..\GameMain\GameMain.h"
#include "..\GameClear\GameClear.h"
#include "..\MiniGame\MiniGame.h"
#include "..\UIEdit\UIEdit.h"
#include "..\StageEdit\StageEdit.h"
#include "..\PizzaEdit\PizzaEdit.h"
#include "..\..\Common\DirectX\DirectX11.h"
#include "..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\Object\GameObject\ActorCollisionManager\ActorCollisionManager.h"
#include "..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\Utility\ImGuiManager\MessageWindow\MessageWindow.h"
#include "..\..\Utility\StringConversion\StringConversion.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Utility\Transform\PositionRender\PositionRender.h"

CSceneManager::CSceneManager()
	: m_pScene			( nullptr )
	, m_NowScene		( ESceneList::Title )
	, m_ChangeScene		( ESceneList::None )
	, m_IsSceneChange	( false )
	, m_IsFadeOut		( false )
	, m_UpdateStopFlag	( false )
	, m_IsSceneStop		( false )
	, m_IsPause			( false )
	, m_IsUIRender		( true, u8"UIを表示するか", "System" )
{
	m_pScene	= std::make_unique<CTitle>();

	// 初期化.
	m_pScene->Init();
	CFadeManager::Init();
}

CSceneManager::~CSceneManager()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CSceneManager* CSceneManager::GetInstance()
{
	static std::unique_ptr<CSceneManager> pInstance = std::make_unique<CSceneManager>();
	return pInstance.get();
}

//---------------------------.
// 更新.
//---------------------------.
void CSceneManager::Update( const float& DeltaTime )
{
	CSceneManager* pI = GetInstance();

#ifdef _DEBUG
	CPositionRender::Update( DeltaTime );
#endif

	// シーンの変更の更新.
	SceneChangeUpdate();

	// シーンの更新.
	if ( pI->m_UpdateStopFlag == false && pI->m_IsSceneStop == false ) {
		pI->m_pScene->Update( DeltaTime );
	}

	// UIの表示の切り替え.
	if ( CDebugKeyInput::IsANDKeyDown( VK_CONTROL, VK_F10 ) ) pI->m_IsUIRender.Inversion();

	CDebugWindow::PushProc( u8"SceneManager",
	[&] {
		if ( CImGuiManager::Button( u8"タイトルへ移動"			) ) SceneChange( ESceneList::Title		);
		if ( CImGuiManager::Button( u8"ゲームメインへ移動"		) ) SceneChange( ESceneList::GameMain	);
		if ( CImGuiManager::Button( u8"ゲームクリアへ移動"		) ) SceneChange( ESceneList::GameClear	);
		if ( CImGuiManager::Button( u8"ミニゲームへ移動"			) ) SceneChange( ESceneList::MiniGame	);
		if ( CImGuiManager::Button( u8"UIエディタへ移動"			) ) SceneChange( ESceneList::UIEdit		);
		if ( CImGuiManager::Button( u8"ステージエディタへ移動"	) ) SceneChange( ESceneList::StageEdit	);
		if ( CImGuiManager::Button( u8"ピザエディタへ移動"		) ) SceneChange( ESceneList::PizzaEdit	);
	} );
}

//---------------------------.
// 描画.
//---------------------------.
void CSceneManager::Render()
{
	CSceneManager* pI = GetInstance();

	// シーンの描画.
	pI->m_pScene->Sprite3DRender_B();
	pI->m_pScene->ModelRender();
	pI->m_pScene->Sprite3DRender_A();
	if ( pI->m_IsUIRender == true ) {
		CDirectX11::SetDepth( false );
		pI->m_pScene->SpriteUIRender();
		CDirectX11::SetDepth( true );
	}

	// フェードの描画.
	CFadeManager::Render();
}

//---------------------------.
// シーンの変更.
//---------------------------.
void CSceneManager::SceneChange( const ESceneList NewScene, const EFadeType FadeType, const float FadeSpeed )
{
	CSceneManager* pI = GetInstance();

	if ( pI->m_IsSceneChange ) return;

	// シーン遷移情報を保存.
	pI->m_IsSceneChange = true;
	pI->m_ChangeScene	= NewScene;

	// フェード情報を保存.
	pI->m_FadeType		= FadeType;
	pI->m_FadeSpeed		= FadeSpeed;

	// フェードの設定.
	CFadeManager::FadeSetting( FadeType );

	CMessageWindow::PushMessage( StringConversion::Enum( NewScene ) + u8"に移動しました", Color4::Yellow );
}

//---------------------------.
// シーンの変更の更新.
//	シーンの変更を行っていなく
//	更新も止めていない 場合は処理は行わない.
//---------------------------.
void CSceneManager::SceneChangeUpdate()
{
	CSceneManager* pI = GetInstance();

	if ( pI->m_IsSceneChange == false && pI->m_UpdateStopFlag == false ) return;

	// 更新を停止させる.
	pI->m_UpdateStopFlag = true;

	// フェードインを行う.
	if ( pI->m_IsFadeOut == false && CFadeManager::FadeIn( pI->m_FadeType, pI->m_FadeSpeed ) ) {
		// 当たり判定リストを初期化する.
		CActorCollisionManager::Reset();

		// シーンを変更.
		switch ( pI->m_ChangeScene ) {
		case ESceneList::Title:
			CSoundManager::BGMAllStop();
			pI->m_pScene	= std::make_unique<CTitle>();
			break;
		case ESceneList::GameMain:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CGameMain>();
			break;
		case ESceneList::GameClear:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CGameClear>();
			break;
		case ESceneList::MiniGame:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CMiniGame>();
			break;
		case ESceneList::UIEdit:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CUIEdit>();
			break;
		case ESceneList::StageEdit:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CStageEdit>();
			break;
		case ESceneList::PizzaEdit:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CPizzaEdit>();
			break;
		default:
			break;
		}

		// 新しいシーンの初期化.
		pI->m_pScene->Init();
		pI->m_pScene->Update( 0.0f );
		pI->m_IsFadeOut = true;
		pI->m_NowScene = pI->m_ChangeScene;


		// コントローラの振動の停止.
		CXInput::SetVibration( 0.0f, 0.0f );
	}
	// フェードアウトを行う.
	else if ( pI->m_IsFadeOut && CFadeManager::FadeOut( pI->m_FadeType, pI->m_FadeSpeed ) ) {
		pI->m_IsFadeOut			= false;
		pI->m_UpdateStopFlag	= false;
		pI->m_IsSceneChange		= false;
		pI->m_ChangeScene		= ESceneList::None;
	}
}
