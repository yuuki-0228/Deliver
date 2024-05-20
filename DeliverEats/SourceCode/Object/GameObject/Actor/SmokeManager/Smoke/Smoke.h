#pragma once
#include "..\..\Actor.h"

/************************************************
*	���N���X.
**/
class CSmoke final
	: public CActor
{
public:
	CSmoke();
	virtual ~CSmoke();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& deltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �A�j���[�V�����̊J�n.
	void AnimPlay( const D3DXPOSITION3& Pos, const D3DXVECTOR3& MoveVec, const D3DXVECTOR3& AddMoveVec );
	// �A�j���[�V�������I��������.
	inline bool IsAnimEnd() const { return m_AnimEnd; }

private:
	CCollisionMesh*	m_pSmokeMesh;	// �����f��.
	D3DXVECTOR3		m_MoveVec;		// �ړ��x�N�g��.
	D3DXVECTOR3		m_AddMoveVec;	// �ǉ��̈ړ��x�N�g��.
	float			m_DAng;			// �g�k�A�j���[�V�����p�p�x.
	float			m_ScaleSpeed;	// �g�k�A�j���[�V�������x.
	float			m_Alpha;		// �A���t�@�l.
	bool			m_AnimEnd;		// �A�j���[�V�������I��������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SSmoke& CONSTANT = CConstant::GetSmoke();
};