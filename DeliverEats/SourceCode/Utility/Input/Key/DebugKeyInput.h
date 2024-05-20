#pragma once
#include "KeyInput.h"

/************************************************
*	キーボードのキー入力クラス.
*		﨑田友輝.
**/
class CDebugKeyInput final
{
public:
	CDebugKeyInput();
	~CDebugKeyInput();

	// キーが押されたことを取得する.
	static bool IsKeyPress( const int Key );
	// 押した瞬間.
	static bool IsKeyDown( const int Key );
	// 離した瞬間.
	static bool IsKeyUp( const int Key );
	// 押し続けている.
	static bool IsKeyRepeat( const int Key );

	// 複数個のキーのどれかが押されたことを取得する.
	template<typename... T>
	static bool IsORPress( const int Key, T... t );
	// 複数個のキーのどれかを押した瞬間.
	template<typename... T>
	static bool IsORKeyDown( const int Key, T... t );
	// 複数個のキーのどれかを離した瞬間.
	template<typename... T>
	static bool IsORKeyUp( const int Key, T... t );
	// 複数個のキーのどれかを押し続けている.
	template<typename... T>
	static bool IsORKeyRepeat( const int Key, T... t );

	// 複数個のキーが全て押されたことを取得する.
	template<typename... T>
	static bool IsANDPress( const int Key, T... t );
	// 複数個のキーが押した瞬間.
	//	※最後に指定したキーが押された瞬間に全て押しておく必要がある.
	template<typename... T>
	static bool IsANDKeyDown( const int Key, T... t );
	// 複数個のキーが離した瞬間.
	//	※最後に指定したキーが離された瞬間に全て離しておく必要がある.
	template<typename... T>
	static bool IsANDKeyUp( const int Key, T... t );
	// 複数個のキーが全て押し続けている.
	template<typename... T>
	static bool IsANDKeyRepeat( const int Key, T... t );

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDebugKeyInput( const CDebugKeyInput& )					= delete;
	CDebugKeyInput( CDebugKeyInput&& )						= delete;
	CDebugKeyInput& operator = ( const CDebugKeyInput& )	= delete;
	CDebugKeyInput& operator = ( CDebugKeyInput&& )			= delete;
};


//----------------------------.
// 複数個のキーのどれかが押されたことを取得する.
//	Releaseの場合は処理は行わない.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsORPress( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsORPress( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 複数個のキーのどれかを押した瞬間.
//	Releaseの場合は処理は行わない.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsORKeyDown( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsORKeyDown( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 複数個のキーのどれかを離した瞬間.
//	Releaseの場合は処理は行わない.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsORKeyUp( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsORKeyUp( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 複数個のキーのどれかを押し続けている.
//	Releaseの場合は処理は行わない.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsORKeyRepeat( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsORKeyRepeat( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 複数個のキーが全て押されたことを取得する.
//	Releaseの場合は処理は行わない.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsANDPress( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsANDPress( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 複数個のキーが押した瞬間.
//	Releaseの場合は処理は行わない.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsANDKeyDown( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsANDKeyDown( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 複数個のキーが離した瞬間.
//	Releaseの場合は処理は行わない.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsANDKeyUp( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsANDKeyUp( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// 複数個のキーが全て押し続けている.
//	Releaseの場合は処理は行わない.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsANDKeyRepeat( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsANDKeyRepeat( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}