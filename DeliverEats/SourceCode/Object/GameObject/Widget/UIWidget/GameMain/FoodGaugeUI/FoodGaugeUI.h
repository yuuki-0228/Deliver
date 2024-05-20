#pragma once
#include "..\..\UIWidget.h"

namespace {
	// �Q�[�W�\����.
	struct stGauge {
		float Value;
		float Alpha;

		stGauge()
			:stGauge( INIT_FLOAT, INIT_FLOAT )
		{}
		stGauge( float value, float alpha )
			: Value( value )
			, Alpha( alpha )
		{}
	}typedef SGauge;
}

class CEmployee;
class CCustomer;

/************************************************
*	���i�󂯎��/�󂯓n���Q�[�WUI�N���X.
*	��J��.
**/
class CFoodGaugeUI final
	: public CUIWidget
{
public:
	CFoodGaugeUI();
	virtual ~CFoodGaugeUI();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

private:
	// �󂯎��Q�[�W�̏���.
	void ReceiveGauge( const float& DeltaTime, CEmployee* pEmployee );

	// �󂯓n���Q�[�W�̏���.
	void SendGauge( const float& DeltaTime, CCustomer* pCustomer );

	// �\��/��\���؂�ւ�.
	void SetReceiveDisp( bool Flag );

	// Alpha�̈ꊇ�Ǘ�.
	void SetReceiveAlpha( float Alpha );

	// �\��/��\���؂�ւ�.
	void SetSendDisp( bool Flag );

	// Alpha�̈ꊇ�Ǘ�.
	void SetSendAlpha( float Alpha );

private:
	SGauge		m_ReceiveGauge;
	SGauge		m_SendGauge;
};