#include "DebugKeyInput.h"

CDebugKeyInput::CDebugKeyInput()
{
}

CDebugKeyInput::~CDebugKeyInput()
{
}

//----------------------------.
// キーが押されたことを取得する.
//	Releaseの場合は処理は行わない.
//----------------------------.
bool CDebugKeyInput::IsKeyPress( const int Key )
{
#ifdef _DEBUG
	return CKeyInput::IsKeyPress( Key );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 押した瞬間.
//	Releaseの場合は処理は行わない.
//----------------------------.
bool CDebugKeyInput::IsKeyDown( const int Key )
{
#ifdef _DEBUG
	return CKeyInput::IsKeyDown( Key );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 離した瞬間.
//	Releaseの場合は処理は行わない.
//----------------------------.
bool CDebugKeyInput::IsKeyUp( const int Key )
{
#ifdef _DEBUG
	return CKeyInput::IsKeyUp( Key );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 押し続けている.
//	Releaseの場合は処理は行わない.
//----------------------------.
bool CDebugKeyInput::IsKeyRepeat( const int Key )
{
#ifdef _DEBUG
	return CKeyInput::IsKeyRepeat( Key );
#endif // #ifdef _DEBUG.
	return false;
}