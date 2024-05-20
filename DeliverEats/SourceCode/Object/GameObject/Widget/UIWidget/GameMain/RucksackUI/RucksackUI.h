#pragma once
#include "..\..\UIWidget.h"

class CPlayer;
class CTemperatureGaugeUI;
class CShakingBarUI;
class CPizzaUI;
class CDeliverCustomerIconUI;
class CGetMoneyUI;

/************************************************
*	リュッククラス.
*		井谷凌.
**/
class CRucksackUI final
	: public CUIWidget
{
public:
	CRucksackUI();
	virtual ~CRucksackUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer );

	// 注視のアニメーションを行うか.
	void SetIsAnimPlay( bool Flag );

private:
	// リュックの蓋の更新.
	void RucksackLidUpdate();

private:
	CPlayer*								m_pPlayer;				// プレイヤー.
	CSprite*								m_pRucksackLid;			// リュックの蓋.
	SSpriteRenderState						m_RucksackLidState;		// リュックの蓋の情報.
	std::unique_ptr<CTemperatureGaugeUI>	m_pTemperatureGaugeUI;	// 温度ゲージUI.
	std::unique_ptr<CShakingBarUI>			m_pShakingBarUI;		// 傾きゲージUI.
	std::unique_ptr<CPizzaUI>				m_pPizzaUI;				// ピザUI.
	std::unique_ptr<CDeliverCustomerIconUI>	m_pIconUI;				// 渡しに行くお客さんのアイコンUI.
	std::unique_ptr<CGetMoneyUI>			m_pGetMoneyUI;			// 最低限もらえるお金の表示UI.
	D3DXPOSITION3							m_RucksackPos;			// リュックの座標.

private:
	// 外部データの定数.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};