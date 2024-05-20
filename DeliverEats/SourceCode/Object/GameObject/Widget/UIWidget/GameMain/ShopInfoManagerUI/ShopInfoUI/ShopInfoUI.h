#pragma once
#include "..\..\..\UIWidget.h"
#include "..\..\..\..\..\Actor\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\..\..\..\..\..\Common\Font\Font.h"

class CPlayer;
class CEmployee;

/************************************************
*	���X�̏ڍ׏��N���X.
**/
class CShopInfoUI final
	: public CUIWidget
{
public:
	CShopInfoUI();
	~CShopInfoUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �\��.
	void Open( int No, CLinking::Handle Handle );
	// ��\��.
	void Close();

	// �A�j���[�V���������擾.
	inline bool GetIsAnimation() const { return m_IsOpen || m_IsClose; }
	// �\�����Ă��邩�擾.
	inline bool GetIsDisp() const { return m_IsDisp; }

	// �v���C���[�̐ݒ�.
	inline void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	// �ڍ׏��̍X�V.
	void InfoUpdate();
	// �J������̍X�V.
	void OpenUpdate();
	// ���铮��̍X�V.
	void CloseUpdate();

private:
	CPlayer*				m_pPlayer;			// �v���C���[.
	CEmployee*				m_pEmployee;		// �X��.
	CSprite*				m_pGauge;			// �w�i.
	CSprite*				m_pIcon;			// �A�C�R��.
	CSprite*				m_pFrame;			// �t���[��.
	CFont*					m_pFont;			// �t�H���g.
	SSpriteRenderState		m_GaugeState;		// �w�i�̏��.
	SSpriteRenderState		m_IconState;		// �A�C�R���̏��.
	SSpriteRenderState		m_FrameState;		// �t���[���̏��.
	SFontRenderState		m_FontStateBase;	// �t�H���g�̏��x�[�X.
	SFontRenderStateList	m_FontState;		// �t�H���g�̏��.
	CLinking::Handle		m_Handle;			// �n���h��.
	D3DXCOLOR4				m_Color;			// �F.
	float					m_AnimTimeCnt;		// �A�j���[�V�����p�̎��ԃJ�E���g.
	bool					m_IsDisp;			// �\�����Ă��邩.
	bool					m_IsOpen;			// �\�����邩.
	bool					m_IsClose;			// ���邩.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SShopInfo& CONSTANT = CConstant::GetShopInfo();
};