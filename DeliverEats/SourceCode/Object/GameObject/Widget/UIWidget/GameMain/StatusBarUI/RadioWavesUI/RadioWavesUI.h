#pragma once
#include "..\..\..\UIWidget.h"

/************************************************
*	電波UI.
**/
class CRadioWavesUI final
	: public CUIWidget
{
public:
	CRadioWavesUI();
	virtual ~CRadioWavesUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

private:
	CSprite*			m_pRadioWaves;		// 電波.
	SSpriteRenderState	m_RadioWavesState;	// 電波の情報.
};
