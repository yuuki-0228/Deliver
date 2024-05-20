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

// ImGUi�Ŏg�p.
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND, UINT, WPARAM, LPARAM );

namespace{
	constexpr char		WINDOW_SETTING_FILE_PATH[]	= "Data\\Parameter\\Config\\WindowSetting.json";	// �E�B���h�E�̐ݒ�̃t�@�C���p�X.
	constexpr float		FPS_RENDER_SIZE				= 0.5f;												// FPS�`��T�C�Y.
	constexpr float		FPS_RENDER_POS[2]			= { 0.0f, 1.0f };									// FPS�`��ʒu.
	constexpr float		FPS_RENDER_COLOR[4]			= { 0.9f, 0.2f, 0.2f, 1.0f };						// FPS�`��F.

	// �t�F�[�h�̏��.
	enum enFadeState : unsigned char {
		None,
		FadeIn,		// �t�F�[�h�C����.
		FadeOut		// �t�F�[�h�A�E�g��.
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
// �X�V����.
//---------------------------.
void CMain::Update( const float& DeltaTime )
{
	// �X�V����.
	CDirectX11::CheckActiveWindow();
	CImGuiManager::SetingNewFrame();
	CInput::Update();
	CSceneManager::Update( DeltaTime );
	CCameraManager::Update( DeltaTime );
	CLight::Update();
	CMessage::Update( DeltaTime );
	CSystemWindowManager::Update( DeltaTime );

	// �o�b�N�o�b�t�@���N���A�ɂ���.
	CDirectX11::ClearBackBuffer();

	// �`�揈��.
	CSceneManager::Render();

	// �t�F�[�h�A�E�g.
	if ( m_IsEndFadeOut == false ) {
		m_IsEndFadeOut = FadeOut();
		m_pFade->RenderUI( &m_FadeState );
	}

	FPSRender();
	CCollisionRender::Render();
	CImGuiManager::Render();

	// ��ʂɕ\��.
	CDirectX11::Present();

	// ����̃��O���o��.
	CInput::KeyLogOutput();

	// �z�C�[���l��������.
	CInput::SetMouseWheelDelta( 0 );
}

//---------------------------.
// ���[�h���̍X�V����.
//---------------------------.
void CMain::LoadUpdate( const float& DeltaTime )
{
	// ���[�h���̔w�i�̍X�V.
	m_pLoadRender->Update( DeltaTime );
	CInput::Update();

	//�o�b�N�o�b�t�@���N���A�ɂ���.
	CDirectX11::ClearBackBuffer();

	// ���[�h���̕`��.
	m_pLoadRender->Render();

	// �t�F�[�h�C��.
	if ( m_IsGameLoad ) {
		m_IsEndFadeIn = FadeIn();
		m_pFade->RenderUI( &m_FadeState );
		if ( m_IsEndFadeIn ) CDirectX11::InitBackColor();
	}

	FPSRender();

	// ��ʂɕ\��.
	CDirectX11::Present();
}

//---------------------------.
// �\�z����.
//---------------------------.
HRESULT CMain::Create()
{
	// DirectX9�̍\�z.
	if ( FAILED( CDirectX9::Create( m_hWnd	)	) ) return E_FAIL;
	// DirectX11�̍\�z.
	if ( FAILED( CDirectX11::Create( m_hWnd )	) ) return E_FAIL;
	// �����̏�����.
	if ( FAILED( CRandom::Init()				) ) return E_FAIL;
	// ���̓N���X�̏�����.
	if ( FAILED( CInput::Init()					) ) return E_FAIL;
	// �f�o�b�N�e�L�X�g�̏�����.
	if ( FAILED( CWindowText::Init()			) ) return E_FAIL;
	// ImGui�̏�����.
	if ( FAILED( CImGuiManager::Init( m_hWnd )	) ) return E_FAIL;

	// �t���X�N���[���ŋN�����邩.
	json WndSetting = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	if ( WndSetting["IsStartFullScreen"] ) {
		// �t���X�N���[���ɐݒ�.
		CDirectX11::SetFullScreen( true );
	}

	// ���[�h���Ɏg���f�[�^�̓ǂݍ���.
	m_pLoadRender->Load();

	// ���\�[�X�̓ǂݍ���.
	m_pLoadManager->LoadResource( m_hWnd );

	// �E�B���h�E�n���h���̐ݒ�.
	CInput::SethWnd(		m_hWnd );
	CDragAndDrop::SethWnd(	m_hWnd );

	// �t�F�[�h�̏�����.
	if ( m_pFade->Init( "Data\\Sprite\\Fade\\Fade.png" ) ) return E_FAIL;
	m_FadeState			= m_pFade->GetRenderState();
	m_FadeState.Color.w = Color::ALPHA_MIN;
	return S_OK;
}

//---------------------------.
// ���b�Z�[�W���[�v.
//---------------------------.
void CMain::Loop()
{
	// ���b�Z�[�W���[�v.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof( msg ) );

	while ( msg.message != WM_QUIT ) {
		if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else {
			// �t���[�����[�g�̑ҋ@����.
			if ( m_pFrameRate->Wait() ) continue;

			// ���[�h���̍X�V����.
			m_IsGameLoad = m_pLoadManager->ThreadRelease();
			if ( m_IsGameLoad == false || m_IsEndFadeIn == false ) {
				LoadUpdate( static_cast<float>( m_pFrameRate->GetDeltaTime() ) );
			}
			//�X�V����.
			else {
				Update( static_cast<float>( m_pFrameRate->GetDeltaTime() ) );
			}
		}
	}
	CLog::PushLog( "------ ���C�����[�v�I�� ------" );
}

//---------------------------.
// �E�B���h�E�������֐�.
//---------------------------.
HRESULT CMain::InitWindow( HINSTANCE hInstance )
{
	// �E�B���h�E�̐ݒ�̎擾.
	json WndSetting = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	
	// �A�v����/�E�B���h�E�����擾.
	const std::wstring wAppName = StringConversion::to_wString( WndSetting["Name"]["App"], ECodePage::UTF8 );
	const std::wstring wWndName = StringConversion::to_wString( WndSetting["Name"]["Wnd"], ECodePage::UTF8 );

	// FPS��`�悷�邩.
	m_IsFPSRender = WndSetting["IsFPSRender"];

	// �E�B���h�E�̒�`.
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );// ������(0��ݒ�).

	wc.cbSize			= sizeof( wc );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MsgProc;// WndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( hInstance, _T( "ICON" ) );
	wc.hCursor			= LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	wc.lpszClassName	= wAppName.c_str();
	wc.hIconSm			= LoadIcon( hInstance, _T( "ICON" ) );
	
	// �E�B���h�E�N���X��Windows�ɓo�^.
	if ( !RegisterClassEx( &wc ) ) {
		ErrorMessage( "�E�B���h�E�N���X�̓o�^�Ɏ��s" );
		return E_FAIL;
	}

	// �E�B���h�E�̈�̒���.
	RECT	rect;			// ��`�\����.
	DWORD	dwStyle;		// �E�B���h�E�X�^�C��.

	rect.top	= 0;						// ��.
	rect.left	= 0;						// ��.
	rect.right	= WndSetting["Size"]["w"];	// �E.
	rect.bottom = WndSetting["Size"]["h"];	// ��.
	dwStyle = WS_OVERLAPPEDWINDOW;	// �E�B���h�E���.
	if ( WndSetting["IsSizeLock"]	) dwStyle ^= WS_THICKFRAME;		// �T�C�Y�̕ύX���֎~���邩.
	if ( WndSetting["IsMaxLock"]	) dwStyle ^= WS_MAXIMIZEBOX;	// �g�剻���֎~���邩.
	if ( WndSetting["IsMinLock"]	) dwStyle ^= WS_MINIMIZEBOX;	// �g�剻���֎~���邩.
	if ( WndSetting["IsPopUpWnd"]	) dwStyle  = WS_POPUP;			// �g�����E�B���h�E.

	if ( AdjustWindowRect(
		&rect,	// (in)��ʃT�C�Y����������`�\����.(out)�v�Z����.
		dwStyle,// �E�B���h�E���.
		FALSE ) == 0 )// ���j���[�������ǂ����w��.
	{
		ErrorMessage( "�E�B���h�E�̈�̒����Ɏ��s" );
		return E_FAIL;
	}

	// ��ʃT�C�Y�̎擾.
	RECT deskRect = {};
	SystemParametersInfo( SPI_GETWORKAREA, 0, &deskRect, 0 );

	// ��ʒ����ɔz�u�ł���悤�Ɍv�Z.
	int pos_x = WndSetting["Pos"]["x"], pos_y = WndSetting["Pos"]["y"];
	if ( WndSetting["IsDispCenter"] ) {
		pos_x = ( ( ( deskRect.right - deskRect.left ) - ( rect.right - rect.left ) ) / 2 + deskRect.left );
		pos_y = ( ( ( deskRect.bottom - deskRect.top ) - ( rect.bottom - rect.top ) ) / 2 + deskRect.top );
	}

	// �E�B���h�E�̍쐬.
	m_hWnd = CreateWindowEx(
		NULL,					// �g���@�\( ���߃E�B���h�E:WS_EX_LAYERED ).
		wAppName.c_str(),		// �A�v����.
		wWndName.c_str(),		// �E�B���h�E�^�C�g��.
		dwStyle,				// �E�B���h�E���(����).
		pos_x, pos_y,			// �\���ʒux,y���W.
		rect.right - rect.left,	// �E�B���h�E��.
		rect.bottom - rect.top,	// �E�B���h�E����.
		nullptr,				// �e�E�B���h�E�n���h��.
		nullptr,				// ���j���[�ݒ�.
		hInstance,				// �C���X�^���X�ԍ�.
		nullptr );				// �E�B���h�E�쐬���ɔ�������C�x���g�ɓn���f�[�^.
	if ( !m_hWnd ) {
		ErrorMessage( "�E�B���h�E�쐬���s" );
		return E_FAIL;
	}

	// �E�B���h�E�̕\��.
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	// �őO�ʂŌŒ�.
	if ( WndSetting["IsShowOnTop"] ) 
		SetWindowPos( m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, ( SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW ) );

	// �}�E�X�J�[�\����\�����邩.
	if ( WndSetting["IsDispMouseCursor"] == false ) {
		CDirectX11::SetIsDispMouseCursor( false );
		ShowCursor( FALSE );
	}
	return S_OK;
}

//---------------------------.
// �E�B���h�E�֐�(���b�Z�[�W���̏���).
//---------------------------.
LRESULT CALLBACK CMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam )
{
	// ImGui�̃E�B���h�E�𓮂�������T�C�Y�ύX��������ł���悤�ɂ���.
	if ( ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam ) ) return true;
	IMGUI_API

	switch ( uMsg ) {
	case WM_CREATE:
		// �w�i�𓧖��ɂ���.
//		SetLayeredWindowAttributes( hWnd, RGB( 0, 0, 0 ), 0, LWA_COLORKEY );
		break;
	case WM_KEYDOWN:// �L�[�{�[�h�������ꂽ�Ƃ�.
	{
		// �E�B���h�E�̐ݒ�̎擾.
		json WndSetting = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
		if ( WndSetting["IsDispCloseMessage"] == false ) break;

		// �L�[�ʂ̏���.
		switch ( static_cast<char>( wParam ) ) {
		case VK_ESCAPE:	// ESC�L�[.
			if ( MessageBox( nullptr, _T( "�Q�[�����I�����܂����H" ), _T( "�x��" ), MB_YESNO ) == IDYES ) {
				PostQuitMessage( 0 );
			}
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// �}�E�X�z�C�[���̓���.
		const int Delta = GET_WHEEL_DELTA_WPARAM(wParam);
		CInput::SetMouseWheelDelta( Delta );
		break;
	}
	case WM_DROPFILES:
	{
		// �h���b�v�����t�@�C���p�X��ǉ����Ă���.
		HDROP	hDrop = (HDROP) wParam;
		int		Num = DragQueryFile( hDrop, -1, NULL, 0 );
		for ( int i = 0; i < Num; ++i ) {
			// �ړ���̃t�@�C���p�X���쐬.
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

	// ���C���ɕԂ����.
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//---------------------------.
// FPS�̕`��.
//---------------------------.
void CMain::FPSRender()
{
#ifdef _DEBUG
	if ( CDebugKeyInput::IsANDKeyDown( VK_CONTROL, VK_F12 )	) m_IsFPSRender = !m_IsFPSRender;
	if ( m_IsFPSRender == false								) return;

	CDirectX11::SetDepth( false );

	// �\�����镶���̍쐬.
	std::string FPSText =
		"    FPS    : " + std::to_string( static_cast<int>( m_pFrameRate->GetFPS() ) ) + '\n' +
		" DeltaTime : " + std::to_string( m_pFrameRate->GetDeltaTime() );

	// FPS�̕`��.
	CWindowText::DebugRender( FPSText, D3DXPOSITION3( FPS_RENDER_POS ), FPS_RENDER_SIZE, D3DXCOLOR4( FPS_RENDER_COLOR ) );

	CDirectX11::SetDepth( true );
#endif	// #ifdef _DEBUG.
}

//---------------------------.
// �t�F�[�h�C��.
//---------------------------.
bool CMain::FadeIn()
{
	if ( m_FadeNo == EFadeState::FadeOut ) return false;
	float* Alpha = &m_FadeState.Color.w;

	// �t�F�[�h�C�����ł͂Ȃ��ꍇ.
	if ( m_FadeNo == EFadeState::None ){
		// �t�F�[�h�C���̏���.
		m_FadeNo	= EFadeState::FadeIn;
		*Alpha		= Color::ALPHA_MIN;
	}

	// �s�����ɂ��Ă���.
	*Alpha += 0.05f;
	if ( *Alpha < Color::ALPHA_MAX ) return false;

	// �t�F�[�h�I��.
	*Alpha		= Color::ALPHA_MAX;
	m_FadeNo	= EFadeState::None;
	return true;
}

//---------------------------.
// �t�F�[�h�A�E�g.
//---------------------------.
bool CMain::FadeOut()
{
	if ( m_FadeNo == EFadeState::FadeIn ) return false;
	float* Alpha = &m_FadeState.Color.w;

	// �t�F�[�h�A�E�g���ł͂Ȃ��ꍇ.
	if ( m_FadeNo == EFadeState::None ){
		// �t�F�[�h�A�E�g�̏���.
		m_FadeNo	= EFadeState::FadeOut;
		*Alpha		= Color::ALPHA_MAX;
	}

	// �����ɂ��Ă���.
	*Alpha -= 0.05f;
	if ( *Alpha > Color::ALPHA_MIN ) return false;

	// �t�F�[�h�I��.
	*Alpha		= Color::ALPHA_MIN;
	m_FadeNo	= EFadeState::None;
	return true;
}
