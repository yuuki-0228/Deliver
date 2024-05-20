#pragma once
#include "..\..\..\Global.h"
#include "..\ImGuiManager.h"

/************************************************
*	デバッグウィンドウクラス.
*		﨑田友輝.
**/
class CDebugWindow final
{
public:
	CDebugWindow();
	~CDebugWindow();

	// インスタンスの取得.
	static CDebugWindow* GetInstance();

	// 描画.
	static void Render();

	// ウィンドウに出力する.
	static void PushProc( const char* Name, const std::function<void()>& Proc );

	// 描画するかの設定.
	static void SetIsRender( const bool Flag ) { GetInstance()->m_IsRender = Flag; }

private:
	std::queue<std::function<void()>>	m_WatchQueue;		// ウォッチ階層用の関数のキュー.
	std::queue<std::function<void()>>	m_MenuQueue;		// デバッグメニュー階層用の関数のキュー.
	bool								m_IsRender;			// 描画するか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDebugWindow( const CDebugWindow & )				= delete;
	CDebugWindow& operator = ( const CDebugWindow & )	= delete;
	CDebugWindow( CDebugWindow && )						= delete;
	CDebugWindow& operator = ( CDebugWindow && )		= delete;
};
