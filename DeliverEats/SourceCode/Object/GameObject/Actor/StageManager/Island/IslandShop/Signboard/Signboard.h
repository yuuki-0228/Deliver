#pragma once
#include "..\..\..\..\Actor.h"

/************************************************
*	�ŔN���X.
**/
class CSignboard final
	: public CActor
{
public:
	CSignboard();
	virtual ~CSignboard();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �����蔻���ǉ�����.
	void PushCollisionManager();

	// ���X�̃^�C�v�̎擾.
	int GetShopNo() const { return m_ShopNo; }
	// ���X�̃^�C�v�̎w��.
	void SetShopNo( const int No ) { m_ShopNo = No; }

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
	CSkinMesh*			m_pSignboardMesh;	// �Ŕ��b�V��.
	CCollisionMesh*		m_pCollisionMesh;	// �����蔻��p���b�V��.
	SMeshPointLight		m_PointLight;		// �|�C���g���C�g.
	float				m_Alpha;			// �A���t�@�l.
	float				m_LightAng;			// ���C�g�̃A�j���[�V�����p�̊p�x.
	float				m_LightFlashing;	// ���C�g�̓_�ŋ.
	int					m_ShopNo;			// ���X�̃^�C�v.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};