#pragma once
#include "..\SceneWidget.h"

class CPlayer;
class CHeightGaugeUI;
class CRucksackUI;
class CFoodGaugeUI;
class CWarningUI;
class CStatusBarUI;
class CWindUI;
class CNightUI;
class CShopInfoManagerUI;
class CTimeGaugeUI;
class CGoldenTimeUI;

/************************************************
*	ゲームメインUIクラス.
*		井谷凌.
**/
class CGameMainWidget final
	: public CSceneWidget
{
public:
	CGameMainWidget();
	virtual ~CGameMainWidget();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 表示/非表示の一括切替.
	void SwitchDisp( bool flag );

	// ポーズ画面の拡縮,透過を一括管理.
	void SetPauseScaleAndAlpha( float value );

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer );

protected:
	// UIエディタの前に追加で行う描画.
	virtual void FastAddRender() override;
	// UIエディタの後に追加で行う描画.
	virtual void LateAddRender() override;

private:
	// ポーズUIの更新.
	void PauseWindowUpdate();

	// フェードインの処理.
	void UpdateFadeIn();

	// フェードアウトの処理.
	void UpdateFadeOut();

	// ポーズ画面の処理.
	void UpdatePauseWindow();

	// ポーズが押されたときの処理.
	void UpdatePushPause();

private:
	float								m_DeltaTime;

	std::unique_ptr<CHeightGaugeUI>		m_pHeightGaugeUI;		// 高度計UI.
	std::unique_ptr<CRucksackUI>		m_pRucksackUI;			// リュックUI.
	std::unique_ptr<CFoodGaugeUI>		m_pFoodGaugeUI;			// 受け渡しゲージUI.
	std::unique_ptr<CWarningUI>			m_pWarningUI;			// 警告UI.
	std::unique_ptr<CStatusBarUI>		m_pStatusBarUI;			// ステータスバーUI.
	std::unique_ptr<CWindUI>			m_pWindUI;				// 風UI.
	std::unique_ptr<CNightUI>			m_pNightUI;				// 夜UI.
	std::unique_ptr<CShopInfoManagerUI>	m_pShopInfoUI;			// お店の詳細情報UI.
	std::unique_ptr<CTimeGaugeUI>		m_pTimeGaugeUI;			// 時間ゲージUI.
	std::unique_ptr<CGoldenTimeUI>		m_pGoldenTimeUI;		// ゴールデンタイムUI.

	int									m_Choice;				// 選択.

	std::vector<CSprite*>				m_pSpriteList;
	std::vector<SSpriteRenderState>		m_SpriteStateList;
	int									m_ScalingSwitch;		// 拡縮の管理.
	float								m_PauseWindowScale;		// ポーズ画面の拡縮に使用.
	float								m_AnimCnt;				// アニメーション用のカウント.
};
