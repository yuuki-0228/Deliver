#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	夜クラス.
*		﨑田友輝.
**/
class CNightUI final
	: public CUIWidget
{
public:
	CNightUI();
	virtual ~CNightUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

private:
	CSprite*			m_pSprite;		// スプライト.
	SSpriteRenderState	m_SpriteState;	// スプライトの情報.

private:
	// 外部データの定数.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};