#pragma once
#include "..\..\Global.h"

class CInputSetting;
class CWindowSetting;
class CBugReport;
class CSprite;

/************************************************
*	システム設定をするウィンドウマネージャー.
*		﨑田友輝.
**/
class CSystemWindowManager final
{
public:
	CSystemWindowManager();
	~CSystemWindowManager();

	// インスタンスの取得.
	static CSystemWindowManager* GetInstance();

	// 更新.
	static void Update( const float& DeltaTime );
	// 描画.
	static void Render();

private:
	std::unique_ptr<CInputSetting>	m_pInputSetting;	// 入力設定を行うウィンドウ.
	std::unique_ptr<CWindowSetting> m_pWindowSetting;	// ウィンドウ設定を行うウィンドウ.
	std::unique_ptr<CBugReport>		m_pBugReport;		// バグ報告を行うウィンドウ.
	CSprite*						m_pButton;			// ボタン.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CSystemWindowManager( const CSystemWindowManager & )				= delete;
	CSystemWindowManager& operator = ( const CSystemWindowManager & )	= delete;
	CSystemWindowManager( CSystemWindowManager && )						= delete;
	CSystemWindowManager& operator = ( CSystemWindowManager && )		= delete;
};
