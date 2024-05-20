#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	���ԃQ�[�WUI.
**/
class CTimeGaugeUI final
	: public CUIWidget
{
public:
	CTimeGaugeUI();
	virtual ~CTimeGaugeUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

private:
	CSprite*			m_pGauge;		// �Q�[�W.
	CSprite*			m_pFrame;		// �t���[��.
	CFont*				m_pFont;		// �t�H���g.
	SSpriteRenderState	m_GaugeState;	// �Q�[�W�̏��.
	SSpriteRenderState	m_FrameState;	// �t���[���̏��.
	SFontRenderState	m_FontState;	// �t�H���g�̏��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};