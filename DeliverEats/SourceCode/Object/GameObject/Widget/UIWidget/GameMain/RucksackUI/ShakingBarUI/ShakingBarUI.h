#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	�h��Q�[�WUI�N���X.
*		��J��.
**/
class CShakingBarUI final
	: public CUIWidget
{
public:
	CShakingBarUI();
	virtual ~CShakingBarUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*			m_pPlayer;						// �v���C���[.
	CSprite*			m_pHorizontalShakingGauge;		// ���̗h��Q�[�W.
	CSprite*			m_pVerticalShakingGauge;		// �c�̗h��Q�[�W.
	CSprite*			m_pHorizontalShakingBar;		// ���̗h��Q�[�W�̃o�[.
	CSprite*			m_pVerticalShakingBar;			// �c�̗h��Q�[�W�̃o�[.
	SSpriteRenderState	m_HorizontalShakingGaugeState;	// ���̗h��Q�[�W�̏��.
	SSpriteRenderState	m_VerticalShakingGaugeState;	// �c�̗h��Q�[�W�̏��.
	SSpriteRenderState	m_VerticalShakingBarState;		// ���̗h��Q�[�W�̃o�[�̏��.
	SSpriteRenderState	m_HorizontalShakingBarState;	// �c�̗h��Q�[�W�̃o�[�̏��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};