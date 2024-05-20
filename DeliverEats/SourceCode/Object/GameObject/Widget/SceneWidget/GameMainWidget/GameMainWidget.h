#pragma once
#include "..\SceneWidget.h"

class CPlayer;
class CHeightGaugeUI;
class CRucksackUI;
class CFoodGaugeUI;
class CWarningUI;
class CStatusBarUI;
class CWindUI;
class CNightUI;
class CShopInfoManagerUI;
class CTimeGaugeUI;
class CGoldenTimeUI;

/************************************************
*	�Q�[�����C��UI�N���X.
*		��J��.
**/
class CGameMainWidget final
	: public CSceneWidget
{
public:
	CGameMainWidget();
	virtual ~CGameMainWidget();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �\��/��\���̈ꊇ�ؑ�.
	void SwitchDisp( bool flag );

	// �|�[�Y��ʂ̊g�k,���߂��ꊇ�Ǘ�.
	void SetPauseScaleAndAlpha( float value );

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer );

protected:
	// UI�G�f�B�^�̑O�ɒǉ��ōs���`��.
	virtual void FastAddRender() override;
	// UI�G�f�B�^�̌�ɒǉ��ōs���`��.
	virtual void LateAddRender() override;

private:
	// �|�[�YUI�̍X�V.
	void PauseWindowUpdate();

	// �t�F�[�h�C���̏���.
	void UpdateFadeIn();

	// �t�F�[�h�A�E�g�̏���.
	void UpdateFadeOut();

	// �|�[�Y��ʂ̏���.
	void UpdatePauseWindow();

	// �|�[�Y�������ꂽ�Ƃ��̏���.
	void UpdatePushPause();

private:
	float								m_DeltaTime;

	std::unique_ptr<CHeightGaugeUI>		m_pHeightGaugeUI;		// ���x�vUI.
	std::unique_ptr<CRucksackUI>		m_pRucksackUI;			// �����b�NUI.
	std::unique_ptr<CFoodGaugeUI>		m_pFoodGaugeUI;			// �󂯓n���Q�[�WUI.
	std::unique_ptr<CWarningUI>			m_pWarningUI;			// �x��UI.
	std::unique_ptr<CStatusBarUI>		m_pStatusBarUI;			// �X�e�[�^�X�o�[UI.
	std::unique_ptr<CWindUI>			m_pWindUI;				// ��UI.
	std::unique_ptr<CNightUI>			m_pNightUI;				// ��UI.
	std::unique_ptr<CShopInfoManagerUI>	m_pShopInfoUI;			// ���X�̏ڍ׏��UI.
	std::unique_ptr<CTimeGaugeUI>		m_pTimeGaugeUI;			// ���ԃQ�[�WUI.
	std::unique_ptr<CGoldenTimeUI>		m_pGoldenTimeUI;		// �S�[���f���^�C��UI.

	int									m_Choice;				// �I��.

	std::vector<CSprite*>				m_pSpriteList;
	std::vector<SSpriteRenderState>		m_SpriteStateList;
	int									m_ScalingSwitch;		// �g�k�̊Ǘ�.
	float								m_PauseWindowScale;		// �|�[�Y��ʂ̊g�k�Ɏg�p.
	float								m_AnimCnt;				// �A�j���[�V�����p�̃J�E���g.
};
