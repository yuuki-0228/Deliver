#pragma once
#include "..\CameraBase.h"
#include "..\..\..\Utility\Constant\Constant.h"

/************************************************
*	���C���̃J�����N���X.
*		��J��.
**/
class CMainCamera final
	: public CCameraBase
{
public:
	CMainCamera();
	~CMainCamera();

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �ŏI�I�Ȋp�x��Ԃ�.
	float GetRotX() { return m_RotX; }

	// �n���ɍs���ׂ��q��ݒ肷��.
	void SetCustomer(CObject* pCustomer ) { m_pCustomer = pCustomer; }

	// �����J��������؂�ւ�����ۂɌĂяo��.
	void StartMove();

	// �؂�ւ������擾����.
	bool GetIsSwitching() { return m_IsSwitching; }
private:
	// �J���������̊p�x�ɖ߂�����.
	void UpdateReturnRotation( float& RotTheta, const float Rot = 0.0f );
	// �J���������̊p�x�ɖ߂�����s������.
	void InitRotation( const float Rot = 0.0f );

	// R�X�e�B�b�N�ł̑��쏈��.
	void Rotate();

	// �����J��������؂�ւ�������̏���.
	void MoveFromGazingCamera( const D3DXVECTOR3 GoalPos, const float& DeltaTime );

private:
	float			m_Theta;					// R�X�e�B�b�N�̓��͂ŉ����l.
	float			m_RotX;						// �J�����̍ŏI�I�Ȋp�x.

	// �����J��������؂�ւ�����ۂɎg�p.
	CObject*		m_pCustomer;				// �n���ɍs���ׂ��q.(��������؂�ւ�����ۂɎg�p).
	bool			m_IsSwitching;				// �����J��������؂�ւ������H
	float			m_Count;					// �ړ����Ɏg�p����J�E���g.
	D3DXPOSITION3	m_StartPosition;			// �ړ��J�n�ʒu.
	D3DXPOSITION3	m_EndPosition;				// �ړ��I���ʒu.

	bool			m_IsReturnDefaultPos;		// ���̊p�x�ɖ߂낤�Ƃ��Ă��邩?
	float			m_ReturnValue;				// ���̊p�x�ɖ߂����߂ɉ����Z����l.
	float			m_AddValue;					// m_ReturnValue�ɉ��Z����l.

	float			m_OldTarPosY;				// 1F�O�̃^�[�Q�b�g��Y���W.

	bool			m_IsCompleteReturnRStick;	// R�X�e�B�b�N�����̈ʒu�ɂ��邩?
	float			m_OldRTrigger;				// 1F�O��R�X�e�B�b�N�̓��͒l.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SCamera& CONSTANT = CConstant::GetCamera();
};
