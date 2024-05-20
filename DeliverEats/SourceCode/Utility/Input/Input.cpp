#include "Input.h"
#include "InputList.h"
#include "Key\KeyInput.h"
#include "Controller\XInput\XInput.h"
#include "KeyLog\KeyLog.h"
#include "..\Random\Random.h"
#include "..\FileManager\FileManager.h"
#include "..\..\Scene\FadeManager\FadeManager.h"

namespace {
	constexpr char KEY_BIND_FILE_PATH[]			= "Data\\Parameter\\Config\\KeyBind.json";	// キーバインドの保存場所.
	constexpr char WINDOW_SETTING_FILE_PATH[]	= "Data\\Parameter\\Config\\WindowSetting.json";	// ウィンドウの設定のファイルパス.
}

CInput::CInput()
	: m_hWnd			()
	, m_KeyBindList		()
	, m_pKeyLog			( nullptr )
	, m_MouseNowPoint	( { 0, 0 } )
	, m_MouseOldPoint	( { 0, 0 } )
	, m_MouseWheel		( 0 )
	, m_IsGrab			( false )
	, m_IsNotActiveStop	( false )
	, m_IsUpdateStop	( false )
	, m_IsSaveKeyLog	( false )

	// キーログを再生したい場合は( true )にする.
	, m_IsKeyLogPlay	( false )
{
	m_pKeyLog = std::make_unique<CKeyLog>( m_IsKeyLogPlay );

	// ウィンドウの設定の取得.
	json WndSetting		= FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	m_IsNotActiveStop	= WndSetting["IsInputNotActiveStop"];
}

CInput::~CInput()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CInput* CInput::GetInstance()
{
	static std::unique_ptr<CInput> pInstance = std::make_unique<CInput>();
	return pInstance.get();
}

//----------------------------.
// 状態を更新.
//----------------------------.
void CInput::Update()
{
	if ( GetInstance()->m_IsUpdateStop || CFadeManager::GetIsFade() ) return;

	CKeyInput	::Update();
	CXInput		::Update();

	// マウスの更新.
	UpdateMouse();
}

//----------------------------.
// ログを出力.
//----------------------------.
void CInput::KeyLogOutput()
{
	CInput* pI = GetInstance();

	if ( pI->m_IsSaveKeyLog == false ) return;
	if ( pI->m_IsKeyLogPlay == false )	pI->m_pKeyLog->Output();
	else								pI->m_pKeyLog->AddFrameCnt();
}

//----------------------------.
// 初期設定.
//----------------------------.
HRESULT CInput::Init()
{
	CInput* pI = GetInstance();

	// 現在の設定を取得する.
	json KeyBindData = FileManager::JsonLoad( KEY_BIND_FILE_PATH );
	for ( auto& [ActionName, Obj] : KeyBindData.items() ) {
		for ( auto& [KeyType, Value] : Obj.items() ) {
			// キー割り当ての取得.
			if ( KeyType == "Key" ) {
				for ( auto& v : Value ) pI->m_KeyBindList[ActionName].Key.emplace_back( ToKeyData( v ) );
			}
			// ボタン割り当ての取得.
			else if ( KeyType == "But" ) {
				for ( auto& v : Value ) pI->m_KeyBindList[ActionName].But.emplace_back( ToButData( v ) );
			}
		}
	}

	// ログを再生中なら.
	if ( pI->m_IsKeyLogPlay == true ){
		// キーログを読み込む.
		pI->m_pKeyLog->ReadKeyLog();

		// キーを止める.
		CKeyInput	::IsAllKeyStop( true );
		CXInput		::IsAllKeyStop( true );

		// 乱数を固定にする.
		CRandom::IsRandLock( true );
	}

	CLog::PushLog( "入力クラスの初期化 : 成功" );
	return S_OK;
}

//----------------------------.
// 押されたことを取得する.
//----------------------------.
bool CInput::IsKeyPress( const std::string& Key )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	bool IsKeyPress = false;	// キーが押されているか.
	bool IsButPress = false;	// ボタンが押されたか.

	// キーボードが押されているか調べる.
	for ( auto& k : pI->m_KeyBindList[Key].Key ) {
		IsKeyPress = CKeyInput::IsKeyPress( k );
		if ( IsKeyPress ) break;
	}
	// コントローラが押されているか調べる.
	//	キーボードが押されていた場合調べる必要がないため処理は行わない.
	if ( IsKeyPress == false ) {
		for ( auto& b : pI->m_KeyBindList[Key].But ) {
			IsButPress = CXInput::IsKeyPress( b );
			if ( IsButPress ) break;
		}
	}

	// 配列番号を取得.
	int No = 0;
	for ( auto& [k, v] : pI->m_KeyBindList ) {
		if ( k == Key ) break;
		No++;
	}

	// 押したかどうか.
	if ( IsKeyPress || IsButPress ||
		 pI->m_pKeyLog->ReadFrameKey() & 1 << No )
	{
		pI->m_pKeyLog->AddKeyState( No );
		return true;
	}
	return false;
}

//----------------------------.
// 押した瞬間.
//----------------------------.
bool CInput::IsKeyDown( const std::string& Key )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	bool IsKeyPress = false;	// キーが押されているか.
	bool IsButPress = false;	// ボタンが押されたか.

	// キーボードが押されているか調べる.
	for ( auto& k : pI->m_KeyBindList[Key].Key ) {
		IsKeyPress = CKeyInput::IsKeyDown( k );
		if ( IsKeyPress ) break;
	}
	// コントローラが押されているか調べる.
	//	キーボードが押されていた場合調べる必要がないため処理は行わない.
	if ( IsKeyPress == false ) {
		for ( auto& b : pI->m_KeyBindList[Key].But ) {
			IsButPress = CXInput::IsKeyDown( b );
			if ( IsButPress ) break;
		}
	}

	// 配列番号を取得.
	int No = 0;
	for ( auto& [k, v] : pI->m_KeyBindList ) {
		if ( k == Key ) break;
		No++;
	}

	// 押したかどうか.
	if ( IsKeyPress || IsButPress ||
		 pI->m_pKeyLog->ReadFrameKey() & 1 << No )
	{
		pI->m_pKeyLog->AddKeyState( No );
		return true;
	}
	return false;
}

//----------------------------.
// 離した瞬間.
//----------------------------.
bool CInput::IsKeyUp( const std::string& Key )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	bool IsKeyPress = false;	// キーが押されているか.
	bool IsButPress = false;	// ボタンが押されたか.

	// キーボードが押されているか調べる.
	for ( auto& k : pI->m_KeyBindList[Key].Key ) {
		IsKeyPress = CKeyInput::IsKeyUp( k );
		if ( IsKeyPress ) break;
	}
	// コントローラが押されているか調べる.
	//	キーボードが押されていた場合調べる必要がないため処理は行わない.
	if ( IsKeyPress == false ) {
		for ( auto& b : pI->m_KeyBindList[Key].But ) {
			IsButPress = CXInput::IsKeyUp( b );
			if ( IsButPress ) break;
		}
	}

	// 配列番号を取得.
	int No = 0;
	for ( auto& [k, v] : pI->m_KeyBindList ) {
		if ( k == Key ) break;
		No++;
	}

	// 押したかどうか.
	if ( IsKeyPress || IsButPress ||
		 pI->m_pKeyLog->ReadFrameKey() & 1 << No )
	{
		pI->m_pKeyLog->AddKeyState( No );
		return true;
	}
	return false;
}

//----------------------------.
// 押し続けている.
//----------------------------.
bool CInput::IsKeyRepeat( const std::string& Key )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	bool IsKeyPress = false;	// キーが押されているか.
	bool IsButPress = false;	// ボタンが押されたか.

	// キーボードが押されているか調べる.
	for ( auto& k : pI->m_KeyBindList[Key].Key ) {
		IsKeyPress = CKeyInput::IsKeyRepeat( k );
		if ( IsKeyPress ) break;
	}
	// コントローラが押されているか調べる.
	//	キーボードが押されていた場合調べる必要がないため処理は行わない.
	if ( IsKeyPress == false ) {
		for ( auto& b : pI->m_KeyBindList[Key].But ) {
			IsButPress = CXInput::IsKeyRepeat( b );
			if ( IsButPress ) break;
		}
	}

	// 配列番号を取得.
	int No = 0;
	for ( auto& [k, v] : pI->m_KeyBindList ) {
		if ( k == Key ) break;
		No++;
	}

	// 押したかどうか.
	if ( IsKeyPress || IsButPress ||
		 pI->m_pKeyLog->ReadFrameKey() & 1 << No )
	{
		pI->m_pKeyLog->AddKeyState( No );
		return true;
	}
	return false;
}

//---------------------------.
// マウスがスクリーンの中か.
//---------------------------.
bool CInput::IsScreenMiddleMouse()
{
	return false;
}

//---------------------------.
// キーバインドのデータ変換.
//---------------------------.
int CInput::ToKeyData( std::string Key )
{
	for ( auto& l : InputList::KeyList ) {
		if ( l.first == Key ) return l.second;
	}
	return 0;
}
WORD CInput::ToButData( std::string But )
{
	for ( auto& l : InputList::ButList ) {
		if ( l.first == But ) return l.second;
	}
	return 0;
}

//---------------------------.
// マウスの更新.
//---------------------------.
void CInput::UpdateMouse()
{
	CInput* pI = GetInstance();

	// 前回を設定.
	pI->m_MouseOldPoint = pI->m_MouseNowPoint;
	// マウスカーソル位置取得.
	GetCursorPos( &pI->m_MouseNowPoint );
	// スクリーン座標→クライアント座標に変換.
	ScreenToClient( pI->m_hWnd, &pI->m_MouseNowPoint );
}