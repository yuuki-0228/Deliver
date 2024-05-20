#pragma once
#include "..\..\..\UIWidget.h"

/************************************************
*	ƒoƒbƒeƒŠ[UI
**/
class CBatteryUI final
	: public CUIWidget
{
public:
	CBatteryUI();
	virtual ~CBatteryUI();

	// ‰Šú‰».
	virtual bool Init() override;
	// XV.
	virtual void Update( const float& DeltaTime ) override;
	// •`‰æ.
	virtual void Render() override;

	// [“d’†‚©.
	void SetIsCharging( const bool Flag ) { m_IsCharging = Flag; }

private:
	CSprite*			m_pChargingBack;		// [“d‚Ì”wŒi.
	CSprite*			m_pChargingGage;		// [“d‚ÌƒQ[ƒW.
	CSprite*			m_pChargingFrame;		// [“d‚Ì˜g.
	CSprite*			m_pChargingModeSprite;	// [“d‚Ì˜g.
	SSpriteRenderState	m_ChargingBackState;	// [“d‚Ì”wŒi‚Ìî•ñ.
	SSpriteRenderState	m_ChargingGageState;	// [“d‚ÌƒQ[ƒW‚Ìî•ñ.
	SSpriteRenderState	m_ChargingFrameState;	// [“d‚Ì˜g‚Ìî•ñ.
	int					m_Charging;				// c‚è“d’r.
	bool				m_IsCharging;			// [“d’†‚©.
};