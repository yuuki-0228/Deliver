#include "KeyInput.h"
#include "..\..\FileManager\FileManager.h"

namespace {
	constexpr char WINDOW_SETTING_FILE_PATH[] = "Data\\Parameter\\Config\\WindowSetting.json";	// ウィンドウの設定のファイルパス.
}

CKeyInput::CKeyInput()
	: m_NowState		()
	, m_OldState		()
	, m_KeyStop			( false, u8"キーボードのキーを停止するか", "Input" )
	, m_IsNotActiveStop	( false )
{
	// ウィンドウの設定の取得.
	json WndSetting		= FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	m_IsNotActiveStop	= WndSetting["IsInputNotActiveStop"];
}

CKeyInput::~CKeyInput()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CKeyInput* CKeyInput::GetInstance()
{
	static std::unique_ptr<CKeyInput> pInstance = std::make_unique<CKeyInput>();
	return pInstance.get();
}

//----------------------------.
// 状態を更新.
//----------------------------.
void CKeyInput::Update()
{
	CKeyInput* pI = GetInstance();

	// 状態を更新する前に現在の状態をコピー.
	memcpy_s( pI->m_OldState, sizeof( m_OldState ), pI->m_NowState, sizeof( m_NowState ) );

	// 押しているキーを調べる.
	if( !GetKeyboardState( pI->m_NowState ) ) return;
}

//----------------------------.
// キーが押されたことを取得する.
//	キーが停止中なら処理は行わない.
//----------------------------.
bool CKeyInput::IsKeyPress( const int Key )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	if (		pI->m_KeyStop.get() ) return false;
	return (	pI->m_NowState[Key] & 0x80 ) != 0;
}

//----------------------------.
// 押した瞬間.
//	キーが停止中なら処理は行わない.
//----------------------------.
bool CKeyInput::IsKeyDown( const int Key )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	// 現在入力で前回入力してなければ.
	if (		pI->m_KeyStop.get() ) return false;
	return	(	pI->m_OldState[Key] & 0x80 ) == 0 &&
			(	pI->m_NowState[Key] & 0x80 ) != 0;
}

//----------------------------.
// 離した瞬間.
//	キーが停止中なら処理は行わない.
//----------------------------.
bool CKeyInput::IsKeyUp( const int Key )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	// 現在も入力で前回も入力なら.
	if (		pI->m_KeyStop.get() ) return false;
	return	(	pI->m_OldState[Key] & 0x80 ) != 0 &&
			(	pI->m_NowState[Key] & 0x80 ) == 0;
}

//----------------------------.
// 押し続けている.
//	キーが停止中なら処理は行わない.
//----------------------------.
bool CKeyInput::IsKeyRepeat( const int Key )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	// 現在も入力で前回も入力なら.
	if (		pI->m_KeyStop.get() ) return false;
	return	(	pI->m_OldState[Key] & 0x80 ) != 0 &&
			(	pI->m_NowState[Key] & 0x80 ) != 0;
}
