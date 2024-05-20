#pragma once
#include "..\..\..\Global.h"

/************************************************
*	メッセージウィンドウクラス.
*		﨑田友輝.
**/
class CDebugFlagWindow final
{
public:
	CDebugFlagWindow();
	~CDebugFlagWindow();

	// インスタンスの取得.
	static CDebugFlagWindow* GetInstance();

	// 描画.
	static void Render();

private:
	static void FlagFind();			// フラグ検索処理.

private:
	std::string	m_FindString;			// フラグウィンドウの検索する文字.
	CBool		m_IsDispNameNoneFlag;	// 無名のフラグを表示させるか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDebugFlagWindow( const CDebugFlagWindow & )				= delete;
	CDebugFlagWindow& operator = ( const CDebugFlagWindow & )	= delete;
	CDebugFlagWindow( CDebugFlagWindow && )						= delete;
	CDebugFlagWindow& operator = ( CDebugFlagWindow && )		= delete;
};
