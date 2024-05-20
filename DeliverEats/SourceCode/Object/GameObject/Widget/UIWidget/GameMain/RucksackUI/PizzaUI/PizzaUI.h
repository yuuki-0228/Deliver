#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	�h��Q�[�WUI�N���X.
*		���c�F�P.
**/
class CPizzaUI final
	: public CUIWidget
{
public:
	using MaskList = std::vector<std::vector<ID3D11ShaderResourceView*>>;

public:
	CPizzaUI();
	virtual ~CPizzaUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	// �s�U�̐ݒ�.
	void SettingPizza();
	// �s�U�̍폜.
	void DeletePizza();

	// �s�U�̈ړ��̍X�V.
	void PizzaMoveUpdate();
	// �s�U�������Ⴎ����ɂ��Ă����X�V.
	void PizzaCrashUpdate();

private:
	CPlayer*				m_pPlayer;					// �v���C���[.
	CSprite*				m_pPizzaBase;				// �s�U���n.
	CSprite*				m_pSauce;					// �\�[�X.
	CSprite*				m_pIngredients;				// ���.
	CSprite*				m_pIngredientSauce;			// ��ނ̃\�[�X.
	SSpriteRenderState		m_PizzaBaseState;			// �s�U���n�̏��.
	SSpriteRenderState		m_IngredientsStateBase;		// ��ނ̏��x�[�X.
	SSpriteRenderState		m_SauceStateBase;			// �\�[�X�̏��x�[�X.
	SSpriteRenderState		m_IngredientSauceState;		// ��ނ̃\�[�X�̏��x�[�X.
	SSpriteRenderStateList	m_SauceStateList;			// �\�[�X�̏�񃊃X�g.
	SSpriteRenderStateList	m_CheeseStateList;			// �`�[�Y�̏�񃊃X�g.
	SSpriteRenderStateList	m_IngredientsStateList;		// ��ނ̏�񃊃X�g.
	MaskList				m_pSauceMaskList;			// �\�[�X�̃}�X�N�摜���X�g.	
	D3DXPOSITION3			m_PizzaBasePos;				// �s�U���n�̍��W.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};