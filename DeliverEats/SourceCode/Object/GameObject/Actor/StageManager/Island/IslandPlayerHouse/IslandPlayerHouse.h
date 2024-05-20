#pragma once
#include "..\..\..\Actor.h"

class CGarage;
class CCustomer;
class CStageIcon;

/************************************************
*	�����̃v���C���[�̉ƃN���X.
*		���c�F�P.
**/
class CIslandPlayerHouse final
	: public CActor
{
public:
	CIslandPlayerHouse();
	virtual ~CIslandPlayerHouse();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;
	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;

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

private:
	CSkinMesh*					m_pHouseMesh;			// �������b�V��.
	CCollisionMesh*				m_pCollisionMesh;		// �����蔻��p���b�V��.
	std::unique_ptr<CStageIcon>	m_pIcon;				// �A�C�R��.
	float						m_Alpha;				// �A���t�@�l.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};
