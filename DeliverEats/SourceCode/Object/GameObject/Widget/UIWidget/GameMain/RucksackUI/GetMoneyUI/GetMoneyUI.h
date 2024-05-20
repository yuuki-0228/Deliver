#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	�Œ�����炦�邨���̕\��UI�N���X.
*		���c�F�P.
**/
class CGetMoneyUI final
	: public CUIWidget
{
public:
	CGetMoneyUI();
	virtual ~CGetMoneyUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*				m_pPlayer;		// �v���C���[.
	CSprite*				m_pBack;		// �w�i.
	CFont*					m_pFont;		// �t�H���g.
	SSpriteRenderState		m_BackState;	// �w�i�̏��.
	SFontRenderState		m_FontState;	// �t�H���g�̏��.

};