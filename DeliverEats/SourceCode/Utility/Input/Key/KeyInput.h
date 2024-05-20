#pragma once
#include "..\..\..\Global.h"
#include "..\..\..\Common\DirectX\DirectX11.h"

/************************************************
*	キーボードのキー入力クラス.
*		﨑田友輝.
**/
class CKeyInput final
{
private:
	static constexpr int KEY_MAX = 256;	// キーの最大値.

public:
	CKeyInput();
	~CKeyInput();

	// インスタンスの取得.
	static CKeyInput* GetInstance();

	// 状態を更新.
	static void Update();

	// 全てのキーを停止するか.
	inline static void IsAllKeyStop( const bool Flag ) { GetInstance()->m_KeyStop = Flag; };

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
	template<typename... T>
	// 複数個のキーのどれかを離した瞬間.
	static bool IsORKeyUp( const int Key, T... t );
	template<typename... T>
	// 複数個のキーのどれかを押し続けている.
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
	BYTE	m_NowState[KEY_MAX];	// 現在のキーの状態.
	BYTE	m_OldState[KEY_MAX];	// 前回のキーの状態.
	CBool	m_KeyStop;				// 全てのキーを停止する.
	bool	m_IsNotActiveStop;		// アクティブウィンドウではない時に停止させるか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CKeyInput( const CKeyInput& )				= delete;
	CKeyInput( CKeyInput&& )					= delete;
	CKeyInput& operator = ( const CKeyInput& )	= delete;
	CKeyInput& operator = ( CKeyInput&& )		= delete;
};


//----------------------------.
// 複数個のキーのどれかが押されたことを取得する.
//----------------------------.
template<typename... T>
bool CKeyInput::IsORPress( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) ) return true;
	}
	return false;
}

//----------------------------.
// 複数個のキーのどれかを押した瞬間.
//----------------------------.
template<typename... T>
bool CKeyInput::IsORKeyDown( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyDown( k ) ) return true;
	}
	return false;
}

//----------------------------.
// 複数個のキーのどれかを離した瞬間.
//----------------------------.
template<typename... T>
bool CKeyInput::IsORKeyUp( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyUp( k ) ) return true;
	}
	return false;
}

//----------------------------.
// 複数個のキーのどれかを押し続けている.
//----------------------------.
template<typename... T>
bool CKeyInput::IsORKeyRepeat( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyRepeat( k ) ) return true;
	}
	return false;
}

//----------------------------.
// 複数個のキーが全て押されたことを取得する.
//----------------------------.
template<typename... T>
bool CKeyInput::IsANDPress( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) == false ) return false;
	}
	return true;
}

//----------------------------.
// 複数個のキーが押した瞬間.
//----------------------------.
template<typename... T>
bool CKeyInput::IsANDKeyDown( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	if ( IsKeyDown( KeyList.back() ) == false ) return false;
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) == false ) return false;
	}
	return true;
}

//----------------------------.
// 複数個のキーが離した瞬間.
//----------------------------.
template<typename... T>
bool CKeyInput::IsANDKeyUp( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	if ( IsKeyUp( KeyList.back() ) == false ) return false;
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) ) return false;
	}
	return true;
}

//----------------------------.
// 複数個のキーが全て押し続けている.
//----------------------------.
template<typename... T>
bool CKeyInput::IsANDKeyRepeat( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyRepeat( k ) == false ) return false;
	}
	return true;
}
