#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	最低限もらえるお金の表示UIクラス.
*		﨑田友輝.
**/
class CGetMoneyUI final
	: public CUIWidget
{
public:
	CGetMoneyUI();
	virtual ~CGetMoneyUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*				m_pPlayer;		// プレイヤー.
	CSprite*				m_pBack;		// 背景.
	CFont*					m_pFont;		// フォント.
	SSpriteRenderState		m_BackState;	// 背景の情報.
	SFontRenderState		m_FontState;	// フォントの情報.

};