#pragma once
#include "..\..\UIWidget.h"

class CRadioWavesUI;
class CNowTimeUI;
class CBatteryUI;

/************************************************
*	�X�e�[�^�X�o�[UI�N���X.
*		���c�F�P.
**/
class CStatusBarUI final
	: public CUIWidget
{
public:
	CStatusBarUI();
	virtual ~CStatusBarUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �[�d����.
	void SetIsCharging( const bool Flag ) { m_IsCharging = Flag; }

private:
	std::unique_ptr<CRadioWavesUI>	m_pRadioWaves;	// �d�g.
	std::unique_ptr<CNowTimeUI>		m_pNowTime;		// ���݂̎���.
	std::unique_ptr<CBatteryUI>		m_pBattery;		// �o�b�e���[.
	bool							m_IsCharging;	// �[�d����.
};