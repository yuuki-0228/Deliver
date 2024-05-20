#pragma once
#include "..\..\UIWidget.h"

class CPlayer;
class CCustomer;

/************************************************
*	高度計UIクラス.
*		井谷凌.
**/
class CHeightGaugeUI
	: public CUIWidget
{
public:
	CHeightGaugeUI();
	virtual ~CHeightGaugeUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*						m_pPlayer;
	CCustomer*						m_pCustomer;
	std::vector<CSprite*>			m_pSpriteList;
	std::vector<SSpriteRenderState>	m_SpriteStateList;
	SSpriteRenderStateList			m_EmployeeIconState;
	int								m_EmployeeNum;

private:
	// 外部データの定数.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};