#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	���x�Q�[�WUI�N���X.
*		���c�F�P.
**/
class CTemperatureGaugeUI final
	: public CUIWidget
{
public:
	CTemperatureGaugeUI();
	virtual ~CTemperatureGaugeUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[��ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

	// �����b�N�̈ʒu���擾.
	D3DXPOSITION3 GetRucksackPos() const { return m_GaugeState.Transform.Position; }

private:
	CPlayer*			m_pPlayer;		// �v���C���[.
	CSprite*			m_pGauge;		// ���x�Q�[�W.
	CSprite*			m_pFrame;		// �t���[��.
	SSpriteRenderState	m_GaugeState;	// ���x�Q�[�W�̏��.
	SSpriteRenderState	m_FrameState;	// �t���[���̏��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};