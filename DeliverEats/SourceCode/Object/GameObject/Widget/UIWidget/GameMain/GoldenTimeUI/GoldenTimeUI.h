#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	�S�[���f���^�C���N���X.
*		��J��.
**/
class CGoldenTimeUI
	: public CUIWidget
{
public:
	CGoldenTimeUI();
	virtual ~CGoldenTimeUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

private:
	// �Ŕ̕\���̐ݒ�.
	void NoticeOpen();
	// �Ŕ��\���̐ݒ�.
	void NoticeClose();

	// �Ŕ̕\���̍X�V.
	void NoticeOpenUpdate();
	// �Ŕ̔�\���̍X�V.
	void NoticeCloseUpdate();

	// �G�t�F�N�g�̍Đ�.
	void EffectPlay();
	// �G�t�F�N�g�̍X�V.
	void EffectUpdate();

private:
	CSprite*				m_pNotice;				// �Ŕ�.
	CSprite*				m_pEffect;				// �G�t�F�N�g.
	SSpriteRenderState		m_NoticeState;			// �Ŕ̏��.
	SSpriteRenderState		m_EffectStateBase;		// �G�t�F�N�g�̏��x�[�X.
	SSpriteRenderStateList	m_EffectStateList;		// �G�t�F�N�g�̏��x�[�X.
	float					m_AnimTimeCnt;			// �A�j���[�V�����p�̎��ԃJ�E���g.
	float					m_EffectPlayTimeCnt;	// �G�t�F�N�g�̍Đ��p�̎��ԃJ�E���g.
	bool					m_IsOpen;				// �J���Ă��邩.
	bool					m_IsClose;				// ���Ă��邩.
};