#pragma once
#include "..\..\UIWidget.h"

class CPlayer;
class CCustomer;

/************************************************
*	���x�vUI�N���X.
*		��J��.
**/
class CHeightGaugeUI
	: public CUIWidget
{
public:
	CHeightGaugeUI();
	virtual ~CHeightGaugeUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*						m_pPlayer;
	CCustomer*						m_pCustomer;
	std::vector<CSprite*>			m_pSpriteList;
	std::vector<SSpriteRenderState>	m_SpriteStateList;
	SSpriteRenderStateList			m_EmployeeIconState;
	int								m_EmployeeNum;

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};