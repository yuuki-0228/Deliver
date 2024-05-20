#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	揺れゲージUIクラス.
*		井谷凌.
**/
class CShakingBarUI final
	: public CUIWidget
{
public:
	CShakingBarUI();
	virtual ~CShakingBarUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*			m_pPlayer;						// プレイヤー.
	CSprite*			m_pHorizontalShakingGauge;		// 横の揺れゲージ.
	CSprite*			m_pVerticalShakingGauge;		// 縦の揺れゲージ.
	CSprite*			m_pHorizontalShakingBar;		// 横の揺れゲージのバー.
	CSprite*			m_pVerticalShakingBar;			// 縦の揺れゲージのバー.
	SSpriteRenderState	m_HorizontalShakingGaugeState;	// 横の揺れゲージの情報.
	SSpriteRenderState	m_VerticalShakingGaugeState;	// 縦の揺れゲージの情報.
	SSpriteRenderState	m_VerticalShakingBarState;		// 横の揺れゲージのバーの情報.
	SSpriteRenderState	m_HorizontalShakingBarState;	// 縦の揺れゲージのバーの情報.

private:
	// 外部データの定数.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};