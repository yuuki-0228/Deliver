#pragma once
#include "..\CameraBase.h"
#include "..\..\..\Utility\Constant\Constant.h"

/************************************************
*	�����J�����N���X.
*		��J��.
**/
class CGazingCamera final
	: public CCameraBase
{
public:
	CGazingCamera();
	virtual ~CGazingCamera();

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �v���C���[��ݒ�.
	void SetPlayer( CObject* obj ) { m_pPlayer = obj; }

	// ���C���J��������؂�ւ�����ۂɌĂяo��.
	void StartMove();

	// ���C���J�����̌X�����擾����.
	void SetRotX( const float RotX ) { m_RotX = RotX; }

	// �؂�ւ������擾����.
	bool GetIsSwitching() { return m_IsSwitching; }
private:
	// ���C���J��������؂�ւ�������̏���.
	void MoveFromMainCamera( const D3DXVECTOR3 GoalPos, const float& DeltaTime );

private:
	CObject*		m_pPlayer;			// �v���C���[.

	// ���C���J��������؂�ւ�����ۂɎg�p.
	bool			m_IsSwitching;		// ���C���J��������؂�ւ������H
	D3DXPOSITION3	m_StartPosition;	// �J�n�ʒu.
	float			m_Count;			// �ړ����Ɏg�p����J�E���g.
	float			m_RotX;				// �^���I�ȃ��C���J�����̍Č��Ɏg�p.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SCamera& CONSTANT = CConstant::GetCamera();
};