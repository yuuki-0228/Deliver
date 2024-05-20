#pragma once
#include "..\..\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	�X�e�[�W�̃A�C�R���N���X.
**/
class CStageIcon final
{
public:
	CStageIcon();
	~CStageIcon();

	// �A�C�R���̏�����.
	bool InitIcon( const EObjectTag Tag );

	// �A�C�R���̍X�V.
	void IconUpdate( const CLinking::Handle Handle );

	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	void Sprite3DRender_A();

	// �^�O�̐ݒ�.
	void SetTag( const EObjectTag Tag );
	// ���W�̐ݒ�.
	void SetPosition( const D3DXPOSITION3& Pos ) { m_Transform.Position = Pos; }

private:
	STransform			m_Transform;		// �g�����X�t�H�[��.
	CSprite*			m_pIcon;			// �A�C�R��.
	CSprite*			m_pIconBack;		// �A�C�R���̔w�i.
	SSpriteRenderState	m_IconState;		// �A�C�R���̏��.
	SSpriteRenderState	m_IconFrameState;	// �A�C�R���̃t���[�����.
	SSpriteRenderState	m_IconBackState;	// �A�C�R���̔w�i�̏��.
};