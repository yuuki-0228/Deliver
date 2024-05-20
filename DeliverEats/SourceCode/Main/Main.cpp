#include "Main.h"
#include "..\Common\DirectX\DirectX9.h"
#include "..\Common\DirectX\DirectX11.h"
#include "..\Common\WindowText\WindowText.h"
#include "..\Common\SoundManeger\SoundManeger.h"
#include "..\Object\Camera\CameraManager\CameraManager.h"
#include "..\Object\Light\Light.h"
#include "..\Object\Collision\CollisionRender\CollisionRender.h"
#include "..\Resource\LoadManager\LoadManager.h"
#include "..\Resource\LoadManager\LoadRender\LoadRender.h"
#include "..\Scene\SceneManager\SceneManager.h"
#include "..\Utility\FrameRate\FrameRate.h"
#include "..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\Utility\Random\Random.h"
#include "..\Utility\Input\Input.h"
#include "..\Utility\FileManager\FileManager.h"
#include "..\Utility\FileManager\DragAndDrop\DragAndDrop.h"
#include "..\Utility\FileManager\DragAndDrop\DragAndDrop.h"
#include "..\Utility\Message\Message.h"
#include "..\System\SystemWindowManager\SystemWindowManager.h"

// ImGUiで使用.
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND, UINT, WPARAM, LPARAM );

namespace{
	constexpr char		WINDOW_SETTING_FILE_PATH[]	= "Data\\Parameter\\Config\\WindowSetting.json";	// ウィンドウの設定のファイルパス.
	constexpr float		FPS_RENDER_SIZE				= 0.5f;												// FPS描画サイズ.
	constexpr float		FPS_RENDER_POS[2]			= { 0.0f, 1.0f };									// FPS描画位置.
	constexpr float		FPS_RENDER_COLOR[4]			= { 0.9f, 0.2f, 0.2f, 1.0f };						// FPS描画色.

	// フェードの状態.
	enum enFadeState : unsigned char {
		None,
		FadeIn,		// フェードイン中.
		FadeOut		// フェードアウト中.
	} typedef EFadeState;
}

CMain::CMain()
	: m_hWnd			( nullptr )
	, m_pFrameRate		( nullptr )
	, m_pLoadManager	( nullptr )
	, m_pLoadRender		( nullptr )
	, m_pFade			( nullptr )
	, m_FadeState		()
	, m_IsGameLoad		( false )
	, m_IsFPSRender		( false )
	, m_IsEndFadeIn		( false )
	, m_IsEndFadeOut	( false )
{
	m_pFrameRate	= std::make_unique<CFrameRate>( FPS );
	m_pLoadManager	= std::make_unique<CLoadManager>();
	m_pLoadRender	= std::make_unique<CLoadRender>();
	m_pFade			= std::make_unique<CSprite>();
}

CMain::~CMain()
{
	DeleteObject( m_hWnd );
}

//---------------------------.
// 更新処理.
//---------------------------.
void CMain::Update( const float& DeltaTime )
{
	// 更新処理.
	CDirectX11::CheckActiveWindow();
	CImGuiManager::SetingNewFrame();
	CInput::Update();
	CSceneManager::Update( DeltaTime );
	CCameraManager::Update( DeltaTime );
	CLight::Update();
	CMessage::Update( DeltaTime );
	CSystemWindowManager::Update( DeltaTime );

	// バックバッファをクリアにする.
	CDirectX11::ClearBackBuffer();

	// 描画処理.
	CSceneManager::Render();

	// フェードアウト.
	if ( m_IsEndFadeOut == false ) {
		m_IsEndFadeOut = FadeOut();
		m_pFade->RenderUI( &m_FadeState );
	}

	FPSRender();
	CCollisionRender::Render();
	CImGuiManager::Render();

	// 画面に表示.
	CDirectX11::Present();

	// 操作のログを出力.
	CInput::KeyLogOutput();

	// ホイール値を初期化.
	CInput::SetMouseWheelDelta( 0 );
}

//---------------------------.
// ロード中の更新処理.
//---------------------------.
void CMain::LoadUpdate( const float& DeltaTime )
{
	// ロード中の背景の更新.
	m_pLoadRender->Update( DeltaTime );
	CInput::Update();

	//バックバッファをクリアにする.
	CDirectX11::ClearBackBuffer();

	// ロード中の描画.
	m_pLoadRender->Render();

	// フェードイン.
	if ( m_IsGameLoad ) {
		m_IsEndFadeIn = FadeIn();
		m_pFade->RenderUI( &m_FadeState );
		if ( m_IsEndFadeIn ) CDirectX11::InitBackColor();
	}

	FPSRender();

	// 画面に表示.
	CDirectX11::Present();
}

//---------------------------.
// 構築処理.
//---------------------------.
HRESULT CMain::Create()
{
	// DirectX9の構築.
	if ( FAILED( CDirectX9::Create( m_hWnd	)	) ) return E_FAIL;
	// DirectX11の構築.
	if ( FAILED( CDirectX11::Create( m_hWnd )	) ) return E_FAIL;
	// 乱数の初期化.
	if ( FAILED( CRandom::Init()				) ) return E_FAIL;
	// 入力クラスの初期化.
	if ( FAILED( CInput::Init()					) ) return E_FAIL;
	// デバックテキストの初期化.
	if ( FAILED( CWindowText::Init()			) ) return E_FAIL;
	// ImGuiの初期化.
	if ( FAILED( CImGuiManager::Init( m_hWnd )	) ) return E_FAIL;

	// フルスクリーンで起動するか.
	json WndSetting = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	if ( WndSetting["IsStartFullScreen"] ) {
		// フルスクリーンに設定.
		CDirectX11::SetFullScreen( true );
	}

	// ロード中に使うデータの読み込み.
	m_pLoadRender->Load();

	// リソースの読み込み.
	m_pLoadManager->LoadResource( m_hWnd );

	// ウィンドウハンドルの設定.
	CInput::SethWnd(		m_hWnd );
	CDragAndDrop::SethWnd(	m_hWnd );

	// フェードの初期化.
	if ( m_pFade->Init( "Data\\Sprite\\Fade\\Fade.png" ) ) return E_FAIL;
	m_FadeState			= m_pFade->GetRenderState();
	m_FadeState.Color.w = Color::ALPHA_MIN;
	return S_OK;
}

//---------------------------.
// メッセージループ.
//---------------------------.
void CMain::Loop()
{
	// メッセージループ.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof( msg ) );

	while ( msg.message != WM_QUIT ) {
		if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else {
			// フレームレートの待機処理.
			if ( m_pFrameRate->Wait() ) continue;

			// ロード中の更新処理.
			m_IsGameLoad = m_pLoadManager->ThreadRelease();
			if ( m_IsGameLoad == false || m_IsEndFadeIn == false ) {
				LoadUpdate( static_cast<float>( m_pFrameRate->GetDeltaTime() ) );
			}
			//更新処理.
			else {
				Update( static_cast<float>( m_pFrameRate->GetDeltaTime() ) );
			}
		}
	}
	CLog::PushLog( "------ メインループ終了 ------" );
}

//---------------------------.
// ウィンドウ初期化関数.
//---------------------------.
HRESULT CMain::InitWindow( HINSTANCE hInstance )
{
	// ウィンドウの設定の取得.
	json WndSetting = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	
	// アプリ名/ウィンドウ名を取得.
	const std::wstring wAppName = StringConversion::to_wString( WndSetting["Name"]["App"], ECodePage::UTF8 );
	const std::wstring wWndName = StringConversion::to_wString( WndSetting["Name"]["Wnd"], ECodePage::UTF8 );

	// FPSを描画するか.
	m_IsFPSRender = WndSetting["IsFPSRender"];

	// ウィンドウの定義.
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );// 初期化(0を設定).

	wc.cbSize			= sizeof( wc );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MsgProc;// WndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( hInstance, _T( "ICON" ) );
	wc.hCursor			= LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	wc.lpszClassName	= wAppName.c_str();
	wc.hIconSm			= LoadIcon( hInstance, _T( "ICON" ) );
	
	// ウィンドウクラスをWindowsに登録.
	if ( !RegisterClassEx( &wc ) ) {
		ErrorMessage( "ウィンドウクラスの登録に失敗" );
		return E_FAIL;
	}

	// ウィンドウ領域の調整.
	RECT	rect;			// 矩形構造体.
	DWORD	dwStyle;		// ウィンドウスタイル.

	rect.top	= 0;						// 上.
	rect.left	= 0;						// 左.
	rect.right	= WndSetting["Size"]["w"];	// 右.
	rect.bottom = WndSetting["Size"]["h"];	// 下.
	dwStyle = WS_OVERLAPPEDWINDOW;	// ウィンドウ種別.
	if ( WndSetting["IsSizeLock"]	) dwStyle ^= WS_THICKFRAME;		// サイズの変更を禁止するか.
	if ( WndSetting["IsMaxLock"]	) dwStyle ^= WS_MAXIMIZEBOX;	// 拡大化を禁止するか.
	if ( WndSetting["IsMinLock"]	) dwStyle ^= WS_MINIMIZEBOX;	// 拡大化を禁止するか.
	if ( WndSetting["IsPopUpWnd"]	) dwStyle  = WS_POPUP;			// 枠無しウィンドウ.

	if ( AdjustWindowRect(
		&rect,	// (in)画面サイズが入った矩形構造体.(out)計算結果.
		dwStyle,// ウィンドウ種別.
		FALSE ) == 0 )// メニューを持つかどうか指定.
	{
		ErrorMessage( "ウィンドウ領域の調整に失敗" );
		return E_FAIL;
	}

	// 画面サイズの取得.
	RECT deskRect = {};
	SystemParametersInfo( SPI_GETWORKAREA, 0, &deskRect, 0 );

	// 画面中央に配置できるように計算.
	int pos_x = WndSetting["Pos"]["x"], pos_y = WndSetting["Pos"]["y"];
	if ( WndSetting["IsDispCenter"] ) {
		pos_x = ( ( ( deskRect.right - deskRect.left ) - ( rect.right - rect.left ) ) / 2 + deskRect.left );
		pos_y = ( ( ( deskRect.bottom - deskRect.top ) - ( rect.bottom - rect.top ) ) / 2 + deskRect.top );
	}

	// ウィンドウの作成.
	m_hWnd = CreateWindowEx(
		NULL,					// 拡張機能( 透過ウィンドウ:WS_EX_LAYERED ).
		wAppName.c_str(),		// アプリ名.
		wWndName.c_str(),		// ウィンドウタイトル.
		dwStyle,				// ウィンドウ種別(普通).
		pos_x, pos_y,			// 表示位置x,y座標.
		rect.right - rect.left,	// ウィンドウ幅.
		rect.bottom - rect.top,	// ウィンドウ高さ.
		nullptr,				// 親ウィンドウハンドル.
		nullptr,				// メニュー設定.
		hInstance,				// インスタンス番号.
		nullptr );				// ウィンドウ作成時に発生するイベントに渡すデータ.
	if ( !m_hWnd ) {
		ErrorMessage( "ウィンドウ作成失敗" );
		return E_FAIL;
	}

	// ウィンドウの表示.
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	// 最前面で固定.
	if ( WndSetting["IsShowOnTop"] ) 
		SetWindowPos( m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, ( SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW ) );

	// マウスカーソルを表示するか.
	if ( WndSetting["IsDispMouseCursor"] == false ) {
		CDirectX11::SetIsDispMouseCursor( false );
		ShowCursor( FALSE );
	}
	return S_OK;
}

//---------------------------.
// ウィンドウ関数(メッセージ毎の処理).
//---------------------------.
LRESULT CALLBACK CMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam )
{
	// ImGuiのウィンドウを動かしたりサイズ変更させたりできるようにする.
	if ( ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam ) ) return true;
	IMGUI_API

	switch ( uMsg ) {
	case WM_CREATE:
		// 背景を透明にする.
//		SetLayeredWindowAttributes( hWnd, RGB( 0, 0, 0 ), 0, LWA_COLORKEY );
		break;
	case WM_KEYDOWN:// キーボードが押されたとき.
	{
		// ウィンドウの設定の取得.
		json WndSetting = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
		if ( WndSetting["IsDispCloseMessage"] == false ) break;

		// キー別の処理.
		switch ( static_cast<char>( wParam ) ) {
		case VK_ESCAPE:	// ESCキー.
			if ( MessageBox( nullptr, _T( "ゲームを終了しますか？" ), _T( "警告" ), MB_YESNO ) == IDYES ) {
				PostQuitMessage( 0 );
			}
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// マウスホイールの入力.
		const int Delta = GET_WHEEL_DELTA_WPARAM(wParam);
		CInput::SetMouseWheelDelta( Delta );
		break;
	}
	case WM_DROPFILES:
	{
		// ドロップしたファイルパスを追加していく.
		HDROP	hDrop = (HDROP) wParam;
		int		Num = DragQueryFile( hDrop, -1, NULL, 0 );
		for ( int i = 0; i < Num; ++i ) {
			// 移動後のファイルパスを作成.
			TCHAR DropPath[100];
			DragQueryFile( hDrop, i, DropPath, sizeof( DropPath ) / sizeof( TCHAR ) );
			CDragAndDrop::AddFilePath( StringConversion::to_String( DropPath ) );
		}
		DragFinish( hDrop );
		break;
	}
	case WM_CLOSE:
		DestroyWindow( hWnd );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	case WM_SIZE:
		CDirectX11::Resize();
		break;
	}

	// メインに返す情報.
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//---------------------------.
// FPSの描画.
//---------------------------.
void CMain::FPSRender()
{
#ifdef _DEBUG
	if ( CDebugKeyInput::IsANDKeyDown( VK_CONTROL, VK_F12 )	) m_IsFPSRender = !m_IsFPSRender;
	if ( m_IsFPSRender == false								) return;

	CDirectX11::SetDepth( false );

	// 表示する文字の作成.
	std::string FPSText =
		"    FPS    : " + std::to_string( static_cast<int>( m_pFrameRate->GetFPS() ) ) + '\n' +
		" DeltaTime : " + std::to_string( m_pFrameRate->GetDeltaTime() );

	// FPSの描画.
	CWindowText::DebugRender( FPSText, D3DXPOSITION3( FPS_RENDER_POS ), FPS_RENDER_SIZE, D3DXCOLOR4( FPS_RENDER_COLOR ) );

	CDirectX11::SetDepth( true );
#endif	// #ifdef _DEBUG.
}

//---------------------------.
// フェードイン.
//---------------------------.
bool CMain::FadeIn()
{
	if ( m_FadeNo == EFadeState::FadeOut ) return false;
	float* Alpha = &m_FadeState.Color.w;

	// フェードイン中ではない場合.
	if ( m_FadeNo == EFadeState::None ){
		// フェードインの準備.
		m_FadeNo	= EFadeState::FadeIn;
		*Alpha		= Color::ALPHA_MIN;
	}

	// 不透明にしていく.
	*Alpha += 0.05f;
	if ( *Alpha < Color::ALPHA_MAX ) return false;

	// フェード終了.
	*Alpha		= Color::ALPHA_MAX;
	m_FadeNo	= EFadeState::None;
	return true;
}

//---------------------------.
// フェードアウト.
//---------------------------.
bool CMain::FadeOut()
{
	if ( m_FadeNo == EFadeState::FadeIn ) return false;
	float* Alpha = &m_FadeState.Color.w;

	// フェードアウト中ではない場合.
	if ( m_FadeNo == EFadeState::None ){
		// フェードアウトの準備.
		m_FadeNo	= EFadeState::FadeOut;
		*Alpha		= Color::ALPHA_MAX;
	}

	// 透明にしていく.
	*Alpha -= 0.05f;
	if ( *Alpha > Color::ALPHA_MIN ) return false;

	// フェード終了.
	*Alpha		= Color::ALPHA_MIN;
	m_FadeNo	= EFadeState::None;
	return true;
}
