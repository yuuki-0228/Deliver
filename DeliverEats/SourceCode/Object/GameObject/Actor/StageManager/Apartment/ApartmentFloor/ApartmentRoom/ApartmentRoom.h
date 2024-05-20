#pragma once
#include "..\..\..\..\Actor.h"

class CCustomer;
class CStageIcon;

/************************************************
*	�}���V�����^�C�v�̕����N���X.
*		���c�F�P.
**/
class CApartmentRoom final
	: public CActor
{
public:
	CApartmentRoom();
	virtual ~CApartmentRoom();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �����蔻��I����Ăяo�����X�V.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;
	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;

	// �\�����邩.
	void SetIsDisp( const bool Flag ) { m_FloorIsDisp = Flag; }
	// �A���t�@�̐ݒ�.
	void SetAlpha( const float Alpha );

private:
	// �����̌������擾.
	EDirection GetRoomDirection();

	// �A�C�R���̍X�V.
	void IconUpdate();
	// ���C�g�̍X�V.
	void LightUpdate();
	// ���C�g������.
	void LightOn( const std::pair<int, int>& Time );
	// ���C�g������.
	void LightOff( const std::pair<int, int>& Time );

	// ���q����̍X�V.
	void CustomerUpdate();
	// ���q����̕\�����邽�߂̈ړ�.
	void CustomerDispMove();
	// ���q���񂪃v���C���[�̕�������悤�ɉ�].
	void CustomerPlayerLook();
	// ���q������\���ɂ��邽�߂̈ړ�.
	void CustomerHideMove();
	// ���q����̉�].
	void CustomerRotation();

	// �����J����.
	void WindowOpen();
	// ���������.
	void WindowClose();

private:
	CSkinMesh*					m_pRoomMesh;			// �������b�V��.
	CSkinMesh*					m_pWindowMesh;			// �����b�V��.
	std::unique_ptr<CStageIcon>	m_pIcon;				// �A�C�R��.
	std::unique_ptr<CCustomer>	m_pCustomer;			// ���q����.
	SMeshPointLight				m_PointLight;			// �|�C���g���C�g.
	STransform					m_CustomerTransform;	// ���q����̃g�����X�t�H�[��.
	STransform					m_WindowTransform;		// ���̃g�����X�t�H�[��.
	D3DXROTATION3				m_CustomerAddRotation;	// ���q����̒ǉ��̊p�x.
	D3DXPOSITION3				m_StartPos;				// �J�n���W.
	D3DXPOSITION3				m_EndPos;				// �I�����W.
	D3DXVECTOR3					m_MoveVector;			// �ړ��x�N�g��.
	D3DXPOSITION3				m_WindowStartPos;		// ���̊J�n���W.
	D3DXPOSITION3				m_WindowEndPos;			// ���̏I�����W.
	D3DXVECTOR3					m_WindowMoveVector;		// ���̈ړ��x�N�g��.
	float						m_MoveCnt;				// �ړ��p�̃J�E���g.
	float						m_WindowMoveCnt;		// ���ړ��p�̃J�E���g.
	float						m_Alpha;				// �A���t�@�l.
	int							m_LightOnHour;			// ���C�g�����鎞��.
	int							m_LightOnMinute;		// ���C�g�����镪.
	int							m_LightOffHour;			// ���C�g����������.
	int							m_LightOffMinute;		// ���C�g��������.
	bool						m_FloorIsDisp;			// 1�t���A�P�ʂ̕\�����邩�ǂ���.
	bool						m_IsRotEnd;				// ��]�I��������.
	bool						m_IsOpenEnd;			// �����J������.
	bool						m_IsCloseEnd;			// ���������.
	bool						m_IsLightOn;			// ���C�g��������.
	bool						m_IsLightOff;			// ���C�g����������.
	bool						m_IsOpenSE;				// SE���Đ�������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
