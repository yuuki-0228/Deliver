#pragma once
#include "..\..\..\..\Actor.h"

/************************************************
*	�K���[�W�N���X.
**/
class CGarage final
	: public CActor
{
public:
	CGarage();
	virtual ~CGarage();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

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
	CSkinMesh*		m_pGarageMesh;		// �K���[�W���b�V��.
	CCollisionMesh*	m_pCollisionMesh;	// �����蔻��p���b�V��.
	float			m_Alpha;			// �A���t�@�l.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};