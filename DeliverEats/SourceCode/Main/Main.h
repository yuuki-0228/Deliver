#pragma once
#include "..\Global.h"
#include "..\Common\Sprite\Sprite.h"

class CFrameRate;
class CLoadManager;
class CLoadRender;
class CMaskFade;

/************************************************
*	���C���N���X.
**/
class CMain final
{
public:
	CMain();
	~CMain();

	void	Update(		const float& DeltaTime );	// �X�V����.
	void	LoadUpdate( const float& DeltaTime );	// ���[�h���̍X�V����.
	HRESULT Create();								// �\�z����.
	void	Loop();									// ���C�����[�v.

	// �E�B���h�E�������֐�.
	HRESULT InitWindow( HINSTANCE hInstance );

private:
	// �E�B���h�E�֐�(���b�Z�[�W���̏���).
	static LRESULT CALLBACK MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam );
	
	// FPS�̕`��.
	void FPSRender();

	// �t�F�[�h�C��.
	bool FadeIn();
	// �t�F�[�h�A�E�g.
	bool FadeOut();

private:
	HWND							m_hWnd;			// �E�B���h�E�n���h��.
	std::unique_ptr<CFrameRate>		m_pFrameRate;	// �t���[�����[�g.
	std::unique_ptr<CLoadManager>	m_pLoadManager;	// ���[�h�}�l�[�W���[.
	std::unique_ptr<CLoadRender>	m_pLoadRender;	// ���[�h���̕`��.
	std::unique_ptr<CSprite>		m_pFade;		// �t�F�[�h.
	SSpriteRenderState				m_FadeState;	// �t�F�[�h�̏��.
	int								m_FadeNo;		// �t�F�[�hNo.
	bool							m_IsGameLoad;	// �ǂݍ��݂��I��������.
	bool							m_IsFPSRender;	// FPS��\�������邩.
	bool							m_IsEndFadeIn;	// �t�F�[�h�C�����I�������.
	bool							m_IsEndFadeOut;	// �t�F�[�h�A�E�g���I�������.
};