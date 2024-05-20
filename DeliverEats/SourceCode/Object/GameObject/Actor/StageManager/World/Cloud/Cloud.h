#pragma once
#include "..\..\..\Actor.h"

/************************************************
*	�_�N���X.
*		���c�F�P.
**/
class CCloud final
	: public CActor
{
public:
	CCloud();
	virtual ~CCloud();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;
	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;

private:
	std::vector<CSprite*>	m_pCloud;				// �_.
	SSpriteRenderStateList	m_CloudState;			// �_�̏��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SStage& CONSTANT = CConstant::GetStage();
};