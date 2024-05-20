#pragma once
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Common\Font\Font.h"

/************************************************
*	確認画面UIクラス.
*		井谷凌.
**/
class CConfilmationWindowUI
{
public:
	CConfilmationWindowUI();
	~CConfilmationWindowUI();

	// 初期化.
	bool Init();
	// 更新.
	void Update(const float& DeltaTime = 0);
	// 描画.
	void Render();

	// 確認画面が表示されているかを返す.
	bool GetIsShowWindow() { return m_IsShowWindow; }

	// 拡大開始.
	void StartExpand( const bool IsRankingDelete = false );

	// 表示/非表示の一括切替.
	void SwitchDisp( bool flag );

	// 確認画面の拡縮を一括管理.
	void SetConfilmationScaleAndAlpha( float value );

	// 確認画面表示中の処理.
	void UpdateConfilmationWindow();

private:
	// 拡大処理.
	void Expand();

	// 縮小処理.
	void Shrink();

private:
	float	m_DeltaTime;

	bool	m_IsRankingDelete;			// ランキングの削除の確認か?
	bool	m_IsShowWindow;				// 確認画面が表示されているか?

	int		m_ScalingSwitch;			// 拡縮の管理.
	float	m_ConfilmationWindowScale;	// 確認画面の拡縮に使用.

	int		m_Choice;					// 選択肢.

	float	m_AnimCnt;					// アニメーション用のカウント.

	// UI.
	std::vector<CSprite*>			m_pSpriteList;
	std::vector<SSpriteRenderState> m_SpriteStateList;
};