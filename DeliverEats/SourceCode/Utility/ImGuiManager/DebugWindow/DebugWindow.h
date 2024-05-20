#pragma once
#include "..\..\..\Global.h"
#include "..\ImGuiManager.h"

/************************************************
*	�f�o�b�O�E�B���h�E�N���X.
*		���c�F�P.
**/
class CDebugWindow final
{
public:
	CDebugWindow();
	~CDebugWindow();

	// �C���X�^���X�̎擾.
	static CDebugWindow* GetInstance();

	// �`��.
	static void Render();

	// �E�B���h�E�ɏo�͂���.
	static void PushProc( const char* Name, const std::function<void()>& Proc );

	// �`�悷�邩�̐ݒ�.
	static void SetIsRender( const bool Flag ) { GetInstance()->m_IsRender = Flag; }

private:
	std::queue<std::function<void()>>	m_WatchQueue;		// �E�H�b�`�K�w�p�̊֐��̃L���[.
	std::queue<std::function<void()>>	m_MenuQueue;		// �f�o�b�O���j���[�K�w�p�̊֐��̃L���[.
	bool								m_IsRender;			// �`�悷�邩.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDebugWindow( const CDebugWindow & )				= delete;
	CDebugWindow& operator = ( const CDebugWindow & )	= delete;
	CDebugWindow( CDebugWindow && )						= delete;
	CDebugWindow& operator = ( CDebugWindow && )		= delete;
};
