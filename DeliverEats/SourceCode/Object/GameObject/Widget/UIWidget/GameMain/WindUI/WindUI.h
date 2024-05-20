#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	��UI�N���X.
*		���c�F�P.
**/
class CWindUI final
	: public CUIWidget
{
public:
	CWindUI();
	virtual ~CWindUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

private:
	// ���̐��̈ړ�.
	void WindLineMove();

	// ���̐�������.
	void SpownWindLine();

	// ���X�g�̍폜.
	void ListDelete();

private:
	CSprite*				m_pWindLine;			// ���̐�.
	SSpriteRenderState		m_WindLineStateBase;	// ���̐��̏��x�[�X.
	SSpriteRenderStateList	m_WindLineStateList;	// ���̐��̏�񃊃X�g.
	float					m_DispWindLineTimeCnt;	// ���̐���\�����Ă������ԗp�J�E���g.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SWindUI& CONSTANT = CConstant::GetWindUI();

};