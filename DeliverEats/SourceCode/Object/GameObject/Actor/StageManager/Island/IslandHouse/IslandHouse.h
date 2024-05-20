#pragma once
#include "..\..\..\Actor.h"

class CGarage;
class CCustomer;
class CStageIcon;

/************************************************
*	�����̉ƃN���X.
*		���c�F�P.
**/
class CIslandHouse final
	: public CActor
{
public:
	CIslandHouse();
	virtual ~CIslandHouse();

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

	// �K���[�W�̍��W�̐ݒ�.
	void SetGaragePosition( const D3DXPOSITION3& Pos );

	// �����蔻���ǉ�����.
	void PushCollisionManager();

protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision() override;
	// �����蔻��̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;

private:
	// �J�����ɂ��A���t�@�l�̍X�V.
	void CameraAlphaUpdate();

	// �A�C�R���̍X�V.
	void IconUpdate();
	// ���C�g�̍X�V.
	void LightUpdate();
	// ���C�g������.
	void LightOn( const std::pair<int, int>& Time );
	// ���C�g������.
	void LightOff( const std::pair<int, int>& Time );

	// ���q�����\�����邽�߂̈ړ�.
	void CustomerDispMove();
	// ���q���񂪃v���C���[�̕�������悤�ɉ�].
	void CustomerPlayerLook();
	// ���q������\���ɂ��邽�߂̈ړ�.
	void CustomerHideMove();
	// ���q����̉�].
	void CustomerRotation();

	// �����J����.
	void DoorOpen();
	// ���������.
	void DoorClose();

private:
	CSkinMesh*					m_pHouseMesh;			// �������b�V��.
	CSkinMesh*					m_pDoorMesh;			// �����b�V��.
	CCollisionMesh*				m_pCollisionMesh;		// �����蔻��p���b�V��.
	std::unique_ptr<CStageIcon>	m_pIcon;				// �A�C�R��.
	std::unique_ptr<CGarage>	m_pGarage;				// �K���[�W.
	std::unique_ptr<CCustomer>	m_pCustomer;			// ���q����.
	SMeshPointLight				m_PointLight;			// �|�C���g���C�g.
	SMeshPointLight				m_CustomerLight;		// ���q����̃|�C���g���C�g������������p.
	STransform					m_CustomerTransform;	// ���q����̃g�����X�t�H�[��.
	STransform					m_DoorTransform;		// ���̃g�����X�t�H�[��.
	D3DXROTATION3				m_CustomerAddRotation;	// ���q����̒ǉ��̊p�x.
	D3DXPOSITION3				m_StartPos;				// �J�n���W.
	D3DXPOSITION3				m_EndPos;				// �I�����W.
	D3DXVECTOR3					m_MoveVector;			// �ړ��x�N�g��.
	float						m_MoveCnt;				// �ړ��p�̃J�E���g.
	float						m_DoorMoveCnt;			// ���p�̃J�E���g.
	float						m_Alpha;				// �A���t�@�l.
	int							m_LightOnHour;			// ���C�g�����鎞��.
	int							m_LightOnMinute;		// ���C�g�����镪.
	int							m_LightOffHour;			// ���C�g����������.
	int							m_LightOffMinute;		// ���C�g��������.
	bool						m_IsRotEnd;				// ��]�I��������.
	bool						m_IsOpenEnd;			// �����J������.
	bool						m_IsCloseEnd;			// ���������.
	bool						m_IsLightOn;			// ���C�g��������.
	bool						m_IsLightOff;			// ���C�g����������.
	bool						m_IsOpenSE;				// SE���Đ�������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};
