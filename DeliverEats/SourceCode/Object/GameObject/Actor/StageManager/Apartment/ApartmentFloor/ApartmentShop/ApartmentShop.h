#pragma once
#include "..\..\..\..\Actor.h"

class CEmployee;
class CSmokeManager;
class CPizzaKilnFire;
class CStageIcon;

/************************************************
*	�}���V�����^�C�v�̂��X�N���X.
*		���c�F�P.
**/
class CApartmentShop final
	: public CActor
{
public:
	CApartmentShop();
	virtual ~CApartmentShop();

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

	// ���X�̃^�C�v�̎擾.
	int GetShopNo() const { return m_ShopNo; }
	// ���X�̃^�C�v�̎w��.
	void SetShopNo( const int No ) { m_ShopNo = No; }

	// �\�����邩.
	void SetIsDisp( const bool Flag ) { m_FloorIsDisp = Flag; }
	// �A���t�@�̐ݒ�.
	void SetAlpha( const float Alpha );

private:
	// �����̌������擾.
	EDirection GetRoomDirection();

	// �A�C�R���̍X�V.
	void IconUpdate();

	// �X���̍X�V.
	void EmployeeUpdate();
	// �X���̉�].
	void EmployeeRotation( const float EndRot );

	// ���W�Ɉړ�.
	void GoResistor();
	// �①�ɂɈړ�.
	void GoRefrigerator();
	// �q�Ɉړ�.
	void GoKiln();

	// ���̍Đ�.
	void PlaySmoke();

	// �X�����v���C���[�̕�������悤�ɉ�].
	void EmployeePlayerLook();

private:
	CSkinMesh*						m_pShopMesh;			// �������b�V��.
	std::unique_ptr<CStageIcon>		m_pIcon;				// �A�C�R��.
	std::unique_ptr<CPizzaKilnFire>	m_pFire;				// �s�U�q�̉�.
	std::unique_ptr<CEmployee>		m_pEmployee;			// �X��.
	std::unique_ptr<CSmokeManager>	m_pSmokeManager;		// ���}�l�[�W���[.
	SMeshPointLight					m_PointLight;			// �|�C���g���C�g.
	STransform						m_EmployeeTransform;	// �X���̃g�����X�t�H�[��.
	D3DXROTATION3					m_EmployeeAddRotation;	// �X���̒ǉ��̊p�x.
	D3DXPOSITION3					m_SmokePlayPosition;	// ���̍Đ��ʒu.
	std::vector<D3DXPOSITION3>		m_EmployeeSetPosition;	// �X���̐ݒ肷����W.
	std::vector<D3DXVECTOR3>		m_EmployeeMoveVectir;	// �X���̈ړ��x�N�g��.
	float							m_Alpha;				// �A���t�@�l.
	float							m_MoveCnt;				// �ړ��p�J�E���g.
	int								m_ShopNo;				// ���X�̃^�C�v.
	int								m_AnimationNo;			// �A�j���[�V����No.
	bool							m_FloorIsDisp;			// 1�t���A�P�ʂ̕\�����邩�ǂ���.
	bool							m_RotationEnd;			// ��]���I��������.
	bool							m_IsSmoke;				// �����o����.
	bool							m_OldSEBGMPlay;			// �O��BGM���Đ�������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
