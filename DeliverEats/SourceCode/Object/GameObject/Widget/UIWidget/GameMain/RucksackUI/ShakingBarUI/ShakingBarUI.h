#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	hêQ[WUINX.
*		äJ½.
**/
class CShakingBarUI final
	: public CUIWidget
{
public:
	CShakingBarUI();
	virtual ~CShakingBarUI();

	// ú».
	virtual bool Init() override;
	// XV.
	virtual void Update( const float& DeltaTime ) override;
	// `æ.
	virtual void Render() override;

	// vC[ðÝè.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*			m_pPlayer;						// vC[.
	CSprite*			m_pHorizontalShakingGauge;		// ¡ÌhêQ[W.
	CSprite*			m_pVerticalShakingGauge;		// cÌhêQ[W.
	CSprite*			m_pHorizontalShakingBar;		// ¡ÌhêQ[WÌo[.
	CSprite*			m_pVerticalShakingBar;			// cÌhêQ[WÌo[.
	SSpriteRenderState	m_HorizontalShakingGaugeState;	// ¡ÌhêQ[WÌîñ.
	SSpriteRenderState	m_VerticalShakingGaugeState;	// cÌhêQ[WÌîñ.
	SSpriteRenderState	m_VerticalShakingBarState;		// ¡ÌhêQ[WÌo[Ìîñ.
	SSpriteRenderState	m_HorizontalShakingBarState;	// cÌhêQ[WÌo[Ìîñ.

private:
	// Of[^Ìè.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};