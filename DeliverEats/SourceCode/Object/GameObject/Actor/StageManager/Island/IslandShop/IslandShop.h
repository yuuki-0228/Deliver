#pragma once
#include "..\..\..\Actor.h"

class CSignboard;
class CEmployee;
class CSmokeManager;
class CStageIcon;

/************************************************
*	�����̓X�N���X.
*		���c�F�P.
**/
class CIslandShop final
	: public CActor
{
public:
	CIslandShop();
	virtual ~CIslandShop();

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

	// �Ŕ̍��W�̐ݒ�.
	void SetSignboardPosition( const D3DXPOSITION3& Pos );

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

	// ���̍Đ�.
	void PlaySmoke();

	// �X����\�����邽�߂̈ړ�.
	void EmployeeDispMove();
	// �X�����v���C���[�̕�������悤�ɉ�].
	void EmployeePlayerLook();
	// �X�����\���ɂ��邽�߂̈ړ�.
	void EmployeeHideMove();
	// �X���̉�].
	void EmployeeRotation();

	// �����J����.
	void DoorOpen();
	// ���������.
	void DoorClose();

private:
	CSkinMesh*						m_pShopMesh;			// �������b�V��.
	CSkinMesh*						m_pDoorMesh;			// �����b�V��.
	CCollisionMesh*					m_pCollisionMesh;		// �����蔻��p���b�V��.
	std::unique_ptr<CStageIcon>		m_pIcon;				// �A�C�R��.
	std::unique_ptr<CSignboard>		m_pSignboard;			// �Ŕ�.
	std::unique_ptr<CEmployee>		m_pEmployee;			// �X��.
	std::unique_ptr<CSmokeManager>	m_pSmokeManager;		// ���}�l�[�W���[.
	SMeshPointLight					m_PointLight;			// �|�C���g���C�g.
	SMeshPointLight					m_EmployeeLight;		// �X���̃|�C���g���C�g������������p.
	STransform						m_EmployeeTransform;	// �X���̃g�����X�t�H�[��.
	STransform						m_DoorTransform;		// ���̃g�����X�t�H�[��.
	D3DXROTATION3					m_EmployeeAddRotation;	// �X���̒ǉ��̊p�x
	D3DXPOSITION3					m_SmokePlayPosition;	// ���̍Đ��ʒu.
	D3DXPOSITION3					m_StartPos;				// �J�n���W.
	D3DXPOSITION3					m_EndPos;				// �I�����W.
	D3DXVECTOR3						m_MoveVector;			// �ړ��x�N�g��.
	D3DXPOSITION3					m_DoorStartPos;			// ���̊J�n���W.
	D3DXPOSITION3					m_DoorEndPos;			// ���̏I�����W.
	D3DXVECTOR3						m_DoorMoveVector;		// ���̈ړ��x�N�g��.
	float							m_MoveCnt;				// �ړ��p�̃J�E���g.
	float							m_DoorMoveCnt;			// ���ړ��p�̃J�E���g.
	float							m_Alpha;				// �A���t�@�l.
	int								m_ShopNo;				// ���X�̃^�C�v.
	bool							m_IsRotEnd;				// ��]�I��������.
	bool							m_IsOpenEnd;			// �����J������.
	bool							m_IsCloseEnd;			// ���������.
	bool							m_IsSmoke;				// �����o����.
	bool							m_IsOpenSE;				// SE���Đ�������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};