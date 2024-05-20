#pragma once
#include "..\..\..\Actor.h"

class CPlayer;
class CCustomer;

/************************************************
*	�n���ɍs�����q����̕����N���X.
**/
class CCustomerDire final
	: public CActor
{
public:
	CCustomerDire();
	virtual ~CCustomerDire();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[�̐ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CCollisionMesh*				m_pCustomerDire;		// ���q����̕������b�V��.
	CPlayer*					m_pPlayer;				// �v���C���[.
	CCustomer*					m_pGoHandingCustomer;	// �n���ɍs�����q����.
	D3DXMATRIX					m_mCustomerDireRot;		// ���q����̌����s��.
	std::vector<STransform>		m_TransformList;		// �g�����X�t�H�[�����X�g.
	float						m_AnimCnt;				// �A�j���[�V�����p�J�E���g.
	float						m_DecAlpha;				// ���炷�A���t�@�l.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SCustomerDire& CONSTANT = CConstant::GetCustomerDire();
};
