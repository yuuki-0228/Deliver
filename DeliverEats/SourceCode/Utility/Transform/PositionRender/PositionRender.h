#pragma once
#include "..\..\..\Utility\Transform\Transform.h"

class CStaticMesh;

/************************************************
*	���W�̕\���N���X.
**/
class CPositionRender final
{
public:
	CPositionRender();
	~CPositionRender();

	// �C���X�^���X�̎擾.
	static CPositionRender* GetInstance();

	// �X�V.
	static void Update( const float& DeltaTime );

	// �`��.
	static void Render( const STransform& Transform );

private:
	CStaticMesh*	m_pMesh;		// ���f��.
	STransform		m_Transform;	// �g�����X�t�H�[��.
	CBool			m_IsRender;		// �\�����邩.
};