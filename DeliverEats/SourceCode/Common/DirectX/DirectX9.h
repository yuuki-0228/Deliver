#pragma once
#include "..\..\Global.h"

/************************************************
*	DirectX9 セットアップ.
**/
class CDirectX9 final
{
public:
	CDirectX9();
	~CDirectX9();

	// インスタンスの取得.
	static CDirectX9* GetInstance();

	// DirectX9構築.
	static HRESULT Create( HWND hWnd );
	// DirectX9解放.
	static void Release();

	// デバイスオブジェクトを取得.
	static LPDIRECT3DDEVICE9 GetDevice() { return GetInstance()->m_pDevice9; }

private:
	LPDIRECT3DDEVICE9	m_pDevice9;	// デバイスオブジェクト.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDirectX9( const CDirectX9 & ) = delete;
	CDirectX9& operator = ( const CDirectX9 & ) = delete;
	CDirectX9( CDirectX9 && ) = delete;
	CDirectX9& operator = ( CDirectX9 && ) = delete;
};