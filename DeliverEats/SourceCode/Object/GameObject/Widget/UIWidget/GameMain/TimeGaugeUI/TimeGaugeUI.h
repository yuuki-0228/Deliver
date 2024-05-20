#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	時間ゲージUI.
**/
class CTimeGaugeUI final
	: public CUIWidget
{
public:
	CTimeGaugeUI();
	virtual ~CTimeGaugeUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

private:
	CSprite*			m_pGauge;		// ゲージ.
	CSprite*			m_pFrame;		// フレーム.
	CFont*				m_pFont;		// フォント.
	SSpriteRenderState	m_GaugeState;	// ゲージの情報.
	SSpriteRenderState	m_FrameState;	// フレームの情報.
	SFontRenderState	m_FontState;	// フォントの情報.

private:
	// 外部データの定数.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};