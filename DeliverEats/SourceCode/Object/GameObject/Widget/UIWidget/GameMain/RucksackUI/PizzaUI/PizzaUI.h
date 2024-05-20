#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	揺れゲージUIクラス.
*		﨑田友輝.
**/
class CPizzaUI final
	: public CUIWidget
{
public:
	using MaskList = std::vector<std::vector<ID3D11ShaderResourceView*>>;

public:
	CPizzaUI();
	virtual ~CPizzaUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	// ピザの設定.
	void SettingPizza();
	// ピザの削除.
	void DeletePizza();

	// ピザの移動の更新.
	void PizzaMoveUpdate();
	// ピザをぐちゃぐちゃにしていく更新.
	void PizzaCrashUpdate();

private:
	CPlayer*				m_pPlayer;					// プレイヤー.
	CSprite*				m_pPizzaBase;				// ピザ生地.
	CSprite*				m_pSauce;					// ソース.
	CSprite*				m_pIngredients;				// 具材.
	CSprite*				m_pIngredientSauce;			// 具材のソース.
	SSpriteRenderState		m_PizzaBaseState;			// ピザ生地の情報.
	SSpriteRenderState		m_IngredientsStateBase;		// 具材の情報ベース.
	SSpriteRenderState		m_SauceStateBase;			// ソースの情報ベース.
	SSpriteRenderState		m_IngredientSauceState;		// 具材のソースの情報ベース.
	SSpriteRenderStateList	m_SauceStateList;			// ソースの情報リスト.
	SSpriteRenderStateList	m_CheeseStateList;			// チーズの情報リスト.
	SSpriteRenderStateList	m_IngredientsStateList;		// 具材の情報リスト.
	MaskList				m_pSauceMaskList;			// ソースのマスク画像リスト.	
	D3DXPOSITION3			m_PizzaBasePos;				// ピザ生地の座標.

private:
	// 外部データの定数.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};