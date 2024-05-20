#pragma once
#include "..\..\UIWidget.h"

class CPlayer;

/************************************************
*	�x��UI.
**/
class CWarningUI final
	: public CUIWidget
{
public:
	CWarningUI();
	virtual ~CWarningUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*	m_pPlayer;				// �v���C���[.
	bool		m_IsUpPushMessage;		// �������ɍs�������ɒʒm�𑗂�����.
	bool		m_IsDownPushMessage;	// �����艺�ɍs�������ɒʒm�𑗂�����.
	bool		m_IsRadiusPushMessage;	// ����̔��a��艜�ɍs�����Ƃ��ɒʒm�𑗂�����.
	float		m_SEVolume;				// SE�̉���.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};