#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	渡しに行くお客さんのアイコンUIクラス.
*		井谷凌.
**/
class CDeliverCustomerIconUI final
	: public CUIWidget
{
public:
	CDeliverCustomerIconUI();
	virtual ~CDeliverCustomerIconUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

	// 注視のアニメーションを行うか.
	void SetIsAnimPlay( bool Flag ) { m_pGazingIcon->SetIsAnimPlay( Flag ); };

private:
	CPlayer*			m_pPlayer;			// プレイヤー.
	CSprite*			m_pIcon;			// お客さんのアイコン.
	CSprite*			m_pBack;			// お客さんのアイコンの背景.
	CSprite*			m_pGazingIcon;		// 注視アイコン.
	SSpriteRenderState	m_IconState;		// お客さんのアイコンの情報.
	SSpriteRenderState	m_BackState;		// お客さんのアイコンの背景情報.
	SSpriteRenderState	m_GazingIconState;	// 注視アイコンの情報.
	D3DXCOLOR3			m_Color;			// 色.

private:
	// 外部データの定数.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};