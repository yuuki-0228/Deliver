#pragma once
#include "..\..\..\Actor.h"

class CBTypeApartmentFloor;

/************************************************
*	�}���V����B�p�^�[��(8x2)�N���X.
*		���c�F�P.
**/
class CBTypeApartment final
	: public CActor
{
public:
	CBTypeApartment();
	virtual ~CBTypeApartment();

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

	// �X�e�[�W�G�f�B�^��ImGui�̕`��.
	void StageEditorImGuiRender();
	// �t���A�̏���ImGui�ŕ`��.
	void FloorStateImGuiRender();

	// �}���V�����̃f�[�^�̎擾.
	json GetApartmentData();

	// �}���V�����̍쐬.
	void Create( json j );

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
	std::vector<std::unique_ptr<CBTypeApartmentFloor>>	m_pEmptyFloor;			// ���1�t���A.
	std::vector<std::unique_ptr<CBTypeApartmentFloor>>	m_pFloor;				// 1�t���A.
	CSkinMesh*											m_pRooftop;				// ����.
	CCollisionMesh*										m_pCollisionMesh;		// �����蔻��p���b�V��.
	STransform											m_CollisionTransform;	// �����蔻��̔z�u������W.
	D3DXPOSITION3										m_CreatePosition;		// �t���A������Ă������W.
	float												m_Alpha;				// �A���t�@�l.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
