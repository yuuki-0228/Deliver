#pragma once
#include "..\..\..\UIWidget.h"

/************************************************
*	�o�b�e���[UI
**/
class CBatteryUI final
	: public CUIWidget
{
public:
	CBatteryUI();
	virtual ~CBatteryUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �[�d����.
	void SetIsCharging( const bool Flag ) { m_IsCharging = Flag; }

private:
	CSprite*			m_pChargingBack;		// �[�d�̔w�i.
	CSprite*			m_pChargingGage;		// �[�d�̃Q�[�W.
	CSprite*			m_pChargingFrame;		// �[�d�̘g.
	CSprite*			m_pChargingModeSprite;	// �[�d�̘g.
	SSpriteRenderState	m_ChargingBackState;	// �[�d�̔w�i�̏��.
	SSpriteRenderState	m_ChargingGageState;	// �[�d�̃Q�[�W�̏��.
	SSpriteRenderState	m_ChargingFrameState;	// �[�d�̘g�̏��.
	int					m_Charging;				// �c��d�r.
	bool				m_IsCharging;			// �[�d����.
};