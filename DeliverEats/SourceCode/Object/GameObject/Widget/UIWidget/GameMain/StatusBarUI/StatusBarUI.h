#pragma once
#include "..\..\UIWidget.h"

class CRadioWavesUI;
class CNowTimeUI;
class CBatteryUI;

/************************************************
*	ステータスバーUIクラス.
*		﨑田友輝.
**/
class CStatusBarUI final
	: public CUIWidget
{
public:
	CStatusBarUI();
	virtual ~CStatusBarUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// 充電中か.
	void SetIsCharging( const bool Flag ) { m_IsCharging = Flag; }

private:
	std::unique_ptr<CRadioWavesUI>	m_pRadioWaves;	// 電波.
	std::unique_ptr<CNowTimeUI>		m_pNowTime;		// 現在の時間.
	std::unique_ptr<CBatteryUI>		m_pBattery;		// バッテリー.
	bool							m_IsCharging;	// 充電中か.
};