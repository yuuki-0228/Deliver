#pragma once
#include "..\..\..\UIWidget.h"

/************************************************
*	“d”gUI.
**/
class CRadioWavesUI final
	: public CUIWidget
{
public:
	CRadioWavesUI();
	virtual ~CRadioWavesUI();

	// ‰Šú‰».
	virtual bool Init() override;
	// XV.
	virtual void Update( const float& DeltaTime ) override;
	// •`‰æ.
	virtual void Render() override;

private:
	CSprite*			m_pRadioWaves;		// “d”g.
	SSpriteRenderState	m_RadioWavesState;	// “d”g‚Ìî•ñ.
};
