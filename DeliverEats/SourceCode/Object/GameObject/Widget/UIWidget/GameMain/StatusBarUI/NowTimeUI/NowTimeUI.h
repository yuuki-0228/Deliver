#pragma once
#include "..\..\..\UIWidget.h"

/************************************************
*	現在の時間UIクラス.
*		﨑田友輝.
**/
class CNowTimeUI final
	: public CUIWidget
{
public:
	CNowTimeUI();
	virtual ~CNowTimeUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

private:
	CFont*				m_pFont;		// フォント.
	SFontRenderState	m_FontState;	// フォントの情報.
};