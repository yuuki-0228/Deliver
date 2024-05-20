#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	��N���X.
*		���c�F�P.
**/
class CNightUI final
	: public CUIWidget
{
public:
	CNightUI();
	virtual ~CNightUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

private:
	CSprite*			m_pSprite;		// �X�v���C�g.
	SSpriteRenderState	m_SpriteState;	// �X�v���C�g�̏��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};