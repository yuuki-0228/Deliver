#pragma once
#include "..\SceneBase.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Common\Font\Font.h"

/************************************************
*	�Q�[���N���X.
*		���c�F�P.
**/
class CMiniGame final
	: public CSceneBase
{
public:
	using SortCloudList = std::vector<std::pair<float, SSpriteRenderState*>>;

public:
	CMiniGame();
	~CMiniGame();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// ���f���̕`��.
	virtual void ModelRender() override {};
	// �X�v���C�g(UI)�̕`��.
	virtual void SpriteUIRender() override;
	// �X�v���C�g(3D)/Effect�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override {};
	virtual void Sprite3DRender_B() override {};

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

	// �|�[�Y�̃{�^���̃e�L�X�g�̍X�V.
	void PauseButtonUpdate();
	// �T�E���h�̍X�V.
	void SoundUpdate();
	// �X�R�A�̍X�V.
	void ScoreUpdate();
	// �e�L�X�g�̍X�V.
	void TextUpdate();
	// �N�[���^�C���̍X�V.
	void CoolTimeUpdate();

	// ��������̍X�V.
	void AutoPlayUpdate();
	// �_�̃\�[�g���X�g�̍쐬.
	void CreateSortCloudList();
	// ��������̍s���̍X�V.
	void AutoPlayActionUpdate();
	// ��Ɉړ��ł��邩���f.
	bool UpMoveCheck( const int CloudNo );
	// ���Ɉړ��ł��邩���f.
	bool DownMoveCheck( const int CloudNo );
	// ��������̏o�͂̍X�V.
	void AutoPlayOutUpdate();

	// �|�[�Y�Ɉړ�.
	void MovePause();
	// �|�[�Y��ʂ��J��.
	void PauseOpen();
	// �|�[�Y��ʂ����.
	void PauseClose();
	// �|�[�Y���̍X�V.
	void PauseUpdate();

private:
	CSprite*					m_pText;				// �e�L�X�g�摜.
	CSprite*					m_pPlayer;				// �v���C���[�摜.
	CSprite*					m_pPlayerFire;			// �v���C���[�̉��摜.
	CSprite*					m_pPlayerIcon;			// �v���C���[�A�C�R���摜.
	CSprite*					m_pCloud;				// �_�摜.
	CSprite*					m_pLRTrigger;			// LR�g���K�[�̃e�L�X�g�摜.
	CSprite*					m_pPauseButton;			// �|�[�Y�̃{�^���̃e�L�X�g�摜.
	CSprite*					m_pPauseBack;			// �|�[�Y�̔w�i�摜.
	CSprite*					m_pPauseText;			// �|�[�Y���̃e�L�X�g�摜.
	CSprite*					m_pPauseCursor;			// �|�[�Y���̃J�[�\���摜.
	CFont*						m_pFont;				// �t�H���g.
	SFontRenderStateList		m_FontStateList;		// �t�H���g�̏��.
	SSpriteRenderStateList		m_TextStateList;		// �e�L�X�g�̏�񃊃X�g.
	SSpriteRenderStateList		m_CloudStateList;		// �_�̏�񃊃X�g.
	SSpriteRenderStateList		m_PauseTextStateList;	// �|�[�Y���̃e�L�X�g�̏�񃊃X�g.
	SSpriteRenderState			m_PauseTextStateBase;	// �|�[�Y���̃e�L�X�g�̏��x�[�X.
	SSpriteRenderState			m_CloudStateBase;		// �_�̏��x�[�X.
	SSpriteRenderState			m_PlayerState;			// �v���C���[�̏��.
	SSpriteRenderState			m_PlayerFireState;		// �v���C���[�̉����.
	SSpriteRenderState			m_PlayerIconState;		// �v���C���[�A�C�R���̏��.
	SSpriteRenderState			m_LRTriggerState;		// LR�g���K�[�̃e�L�X�g�̏��.
	SSpriteRenderState			m_PauseButtonState;		// �|�[�Y�̃{�^���̃e�L�X�g�̏��.
	SSpriteRenderState			m_PauseBackState;		// �|�[�Y�̔w�i�̏��.
	SSpriteRenderState			m_PauseCursorState;		// �|�[�Y�̃J�[�\���̏��.
	SortCloudList				m_SortCloudList;		// �_�̃\�[�g���X�g.
	std::vector<int>			m_PlayData;				// �v���C�f�[�^���X�g.
	float						m_DeltaTime;			// �f���^�^�C��.
	float						m_AnimTimeCnt;			// �A�j���[�V�����̎��ԗp�J�E���g.
	float						m_Power;				// �v���C���[�̃W�����v�◎���̗�.
	float						m_CloudMoveSpeed;		// �_�̈ړ����x.
	float						m_Score;				// �X�R�A.
	float						m_PlayerFireCnt;		// �v���C���[�̉��̃A�j���[�V�����p�J�E���g.
	float						m_AutoPlay;				// �����v���C.
	int							m_PreviousScore;		// �O��̃X�R�A.
	int							m_BestScore;			// �x�X�g�X�R�A.
	int							m_CloudTimeCnt;			// �_�̏o�����ԗp�̃J�E���g.
	int							m_TextAnimNo;			// �A�j���[�V��������e�L�X�g��No.
	int							m_CoolTime;				// �����蔻��̃N�[���^�C��.
	int							m_AutoPlayAction;		// ��������̍s��.
	int							m_Choice;				// �I���ӏ�.
	bool						m_IsPlay;				// ���쒆��.
	bool						m_IsCloudHit;			// �_�ɓ���������.
	bool						m_LeftRotation;			// ����]�����邩.
	bool						m_IsPause;				// �|�[�Y����.
	bool						m_IsPauseAnim;			// �|�[�Y�̃A�j���[�V��������.
};