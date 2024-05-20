#pragma once
#include "..\..\..\UIWidget.h"

/************************************************
*	���݂̎���UI�N���X.
*		���c�F�P.
**/
class CNowTimeUI final
	: public CUIWidget
{
public:
	CNowTimeUI();
	virtual ~CNowTimeUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

private:
	CFont*				m_pFont;		// �t�H���g.
	SFontRenderState	m_FontState;	// �t�H���g�̏��.
};