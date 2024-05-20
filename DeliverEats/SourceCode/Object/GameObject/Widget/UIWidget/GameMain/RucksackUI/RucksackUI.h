#pragma once
#include "..\..\UIWidget.h"

class CPlayer;
class CTemperatureGaugeUI;
class CShakingBarUI;
class CPizzaUI;
class CDeliverCustomerIconUI;
class CGetMoneyUI;

/************************************************
*	�����b�N�N���X.
*		��J��.
**/
class CRucksackUI final
	: public CUIWidget
{
public:
	CRucksackUI();
	virtual ~CRucksackUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer );

	// �����̃A�j���[�V�������s����.
	void SetIsAnimPlay( bool Flag );

private:
	// �����b�N�̊W�̍X�V.
	void RucksackLidUpdate();

private:
	CPlayer*								m_pPlayer;				// �v���C���[.
	CSprite*								m_pRucksackLid;			// �����b�N�̊W.
	SSpriteRenderState						m_RucksackLidState;		// �����b�N�̊W�̏��.
	std::unique_ptr<CTemperatureGaugeUI>	m_pTemperatureGaugeUI;	// ���x�Q�[�WUI.
	std::unique_ptr<CShakingBarUI>			m_pShakingBarUI;		// �X���Q�[�WUI.
	std::unique_ptr<CPizzaUI>				m_pPizzaUI;				// �s�UUI.
	std::unique_ptr<CDeliverCustomerIconUI>	m_pIconUI;				// �n���ɍs�����q����̃A�C�R��UI.
	std::unique_ptr<CGetMoneyUI>			m_pGetMoneyUI;			// �Œ�����炦�邨���̕\��UI.
	D3DXPOSITION3							m_RucksackPos;			// �����b�N�̍��W.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};