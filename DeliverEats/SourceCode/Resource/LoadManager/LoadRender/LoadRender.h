#pragma once
#include "..\..\..\Global.h"
#include "..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	���[�h���̕`��N���X.
*		���c�F�P.
**/
class CLoadRender final
{
public:
	CLoadRender();
	~CLoadRender();

	// �ǂݍ���.
	HRESULT Load();

	// �X�V.
	void Update( const float& DeltaTime );
	// �`��.
	void Render();

private:
	// �_�̏���.
	void CloudSpown();
	// �_�̈ړ�.
	void CloudMove();
	// �g�p���Ă��Ȃ��_�̍폜.
	void CloudDelete();

	// �e�L�X�g�̃A�j���[�V����.
	void TextAnimation();

	// �v���C���[�̃W�����v.
	void PlayerJump();
	// �v���C���[�̗���.
	void PlayerFall();
	// �v���C���[�̉_�ɓ����������̃A�j���[�V����.
	void PlayerCloudHitAnimation();
	// �v���C���[�̍��W�̍X�V.
	void PlayerPositionUpdate();
	// �v���C���[�̉��̍X�V.
	void PlayerFireUpdate();

	// �n�ʂ̓����蔻��.
	void GroundCollision();
	// �V��̓����蔻��.
	void CeilingCollision();
	// �_�̓����蔻��.
	void CloudCollision();

	// �N�[���^�C���̍X�V.
	void CoolTimeUpdate();

private:
	std::unique_ptr<CSprite>	m_pText;			// �e�L�X�g�摜.
	std::unique_ptr<CSprite>	m_pPlayer;			// �v���C���[�摜.
	std::unique_ptr<CSprite>	m_pPlayerFire;		// �v���C���[�̉��摜.
	std::unique_ptr<CSprite>	m_pPlayerIcon;		// �v���C���[�A�C�R���摜.
	std::unique_ptr<CSprite>	m_pCloud;			// �_�摜.
	std::unique_ptr<CSprite>	m_pLRTrigger;		// LR�g���K�[�̃e�L�X�g�摜.
	SSpriteRenderStateList		m_TextStateList;	// �e�L�X�g�̏�񃊃X�g.
	SSpriteRenderStateList		m_CloudStateList;	// �_�̏�񃊃X�g.
	SSpriteRenderState			m_CloudStateBase;	// �_�̏��x�[�X.
	SSpriteRenderState			m_PlayerState;		// �v���C���[�̏��.
	SSpriteRenderState			m_PlayerFireState;	// �v���C���[�̉����.
	SSpriteRenderState			m_PlayerIconState;	// �v���C���[�A�C�R���̏��.
	SSpriteRenderState			m_LRTriggerState;	// LR�g���K�[�̃e�L�X�g�̏��.
	float						m_AnimTimeCnt;		// �A�j���[�V�����̎��ԗp�J�E���g.
	float						m_Power;			// �v���C���[�̃W�����v�◎���̗�.
	float						m_CloudMoveSpeed;	// �_�̈ړ����x.
	float						m_PlayerFireCnt;	// �v���C���[�̉��̃A�j���[�V�����p�J�E���g.
	int							m_CloudTimeCnt;		// �_�̏o�����ԗp�̃J�E���g.
	int							m_TextAnimNo;		// �A�j���[�V��������e�L�X�g��No.
	int							m_CoolTime;			// �����蔻��̃N�[���^�C��.
	bool						m_IsCloudHit;		// �_�ɓ���������.
	bool						m_LeftRotation;		// ����]�����邩.
};
