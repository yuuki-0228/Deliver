#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	�n���ɍs�����q����̃A�C�R��UI�N���X.
*		��J��.
**/
class CDeliverCustomerIconUI final
	: public CUIWidget
{
public:
	CDeliverCustomerIconUI();
	virtual ~CDeliverCustomerIconUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

	// �����̃A�j���[�V�������s����.
	void SetIsAnimPlay( bool Flag ) { m_pGazingIcon->SetIsAnimPlay( Flag ); };

private:
	CPlayer*			m_pPlayer;			// �v���C���[.
	CSprite*			m_pIcon;			// ���q����̃A�C�R��.
	CSprite*			m_pBack;			// ���q����̃A�C�R���̔w�i.
	CSprite*			m_pGazingIcon;		// �����A�C�R��.
	SSpriteRenderState	m_IconState;		// ���q����̃A�C�R���̏��.
	SSpriteRenderState	m_BackState;		// ���q����̃A�C�R���̔w�i���.
	SSpriteRenderState	m_GazingIconState;	// �����A�C�R���̏��.
	D3DXCOLOR3			m_Color;			// �F.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};