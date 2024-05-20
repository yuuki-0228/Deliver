#pragma once
#include "..\SceneWidget.h"

class CConfilmationWindowUI;
class CStatusBarUI;

/************************************************
*	�^�C�g��UI�N���X.
*		��J��.
**/
class CTitleWidget final
	: public CSceneWidget
{
public:
	CTitleWidget();
	virtual ~CTitleWidget();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	void SetConfilmationWindowUI( CConfilmationWindowUI& CnfWndUI ) { m_pConfilmationWindowUI = &CnfWndUI; }

protected:
	// �ǉ��ōs���`��.
	virtual void LateAddRender() override;

private:
	float	m_DeltaTime;

	int		m_Choice;	// �I����.

	CSprite*						m_pText;					// �e�L�X�g.
	CConfilmationWindowUI*			m_pConfilmationWindowUI;	// �m�F���.
	std::unique_ptr<CStatusBarUI>	m_pStatusBarUI;				// �X�e�[�^�X�o�[UI.
	SSpriteRenderState				m_TextState;				// �e�L�X�g�̏��.
};
