#pragma once
#include "..\Actor.h"

class CSmoke;

/************************************************
*	���G�t�F�N�g�}�l�[�W���[�N���X.
**/
class CSmokeManager final
	: public CActor
{
public:
	CSmokeManager();
	virtual ~CSmokeManager();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& deltaTime ) override;
	// �`��.
	virtual void Render() override;

	// ���̔���.
	void PlaySmoke( const D3DXPOSITION3& Pos, const D3DXVECTOR3& MoveVec, const D3DXVECTOR3& pAddMoveVec = { 0.0f, 0.0f, 0.0f } );
	// ���G�t�F�N�g���I�����Ă��邩.
	bool IsEffectEnd() { return m_pSmokeList.size() == 0; }

private:
	// �`�揇�Ԃ���ѕς���.
	void RenderOrder();
	// �����X�g�̍폜.
	void SmokeListDelete();
	// �N�[���^�C���̍X�V.
	void CoolTimeUpdate();

private:
	std::vector<std::unique_ptr<CSmoke>>	m_pSmokeList;	// �����X�g.
	float									m_TimeCnt;		// ���̉����o��܂ł̃N�[���^�C��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SSmoke& CONSTANT = CConstant::GetSmoke();
};