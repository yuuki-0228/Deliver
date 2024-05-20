#pragma once
#include "..\..\..\..\..\Actor.h"

/************************************************
*	�s�U�q�̉��N���X.
*		���c�F�P.
**/
class CPizzaKilnFire final
	: public CActor
{
public:
	CPizzaKilnFire();
	virtual ~CPizzaKilnFire();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �\�����邩.
	void SetIsDisp( const bool Flag ) { m_FloorIsDisp = Flag; }
	// �A���t�@�̐ݒ�.
	void SetAlpha( const float Alpha ) { m_Alpha = Alpha; }

private:
	CStaticMesh*	m_pFire;		// �s�U�q�̉�.
	D3DXCOLOR3		m_Color;		// �F.
	D3DXCOLOR3		m_AddColor;		// ���Z����F.
	float			m_Alpha;		// �A���t�@�l.
	float			m_AddColorAng;	// ���Z���Ă����F�p�̊p�x.
	bool			m_FloorIsDisp;	// 1�t���A�P�ʂ̕\�����邩�ǂ���.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPizzaKilnFire& CONSTANT = CConstant::GetPizzaKilnFire();
};