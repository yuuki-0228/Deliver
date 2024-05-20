#pragma once
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Common\Font\Font.h"

/************************************************
*	�m�F���UI�N���X.
*		��J��.
**/
class CConfilmationWindowUI
{
public:
	CConfilmationWindowUI();
	~CConfilmationWindowUI();

	// ������.
	bool Init();
	// �X�V.
	void Update(const float& DeltaTime = 0);
	// �`��.
	void Render();

	// �m�F��ʂ��\������Ă��邩��Ԃ�.
	bool GetIsShowWindow() { return m_IsShowWindow; }

	// �g��J�n.
	void StartExpand( const bool IsRankingDelete = false );

	// �\��/��\���̈ꊇ�ؑ�.
	void SwitchDisp( bool flag );

	// �m�F��ʂ̊g�k���ꊇ�Ǘ�.
	void SetConfilmationScaleAndAlpha( float value );

	// �m�F��ʕ\�����̏���.
	void UpdateConfilmationWindow();

private:
	// �g�又��.
	void Expand();

	// �k������.
	void Shrink();

private:
	float	m_DeltaTime;

	bool	m_IsRankingDelete;			// �����L���O�̍폜�̊m�F��?
	bool	m_IsShowWindow;				// �m�F��ʂ��\������Ă��邩?

	int		m_ScalingSwitch;			// �g�k�̊Ǘ�.
	float	m_ConfilmationWindowScale;	// �m�F��ʂ̊g�k�Ɏg�p.

	int		m_Choice;					// �I����.

	float	m_AnimCnt;					// �A�j���[�V�����p�̃J�E���g.

	// UI.
	std::vector<CSprite*>			m_pSpriteList;
	std::vector<SSpriteRenderState> m_SpriteStateList;
};