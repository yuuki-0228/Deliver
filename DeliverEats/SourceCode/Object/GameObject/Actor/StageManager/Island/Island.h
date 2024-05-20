#pragma once
#include "..\..\Actor.h"

class CPlayer;
class CIslandPlayerHouse;
class CIslandHouse;
class CIslandShop;

/************************************************
*	�����N���X.
*		���c�F�P.
**/
class CIsland final
	: public CActor
{
public:
	CIsland();
	virtual ~CIsland();

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
	// �����̏���ImGui�ŕ`��.
	void IslandStateImGuiRender();

	// �����̃f�[�^�̎擾.
	json GetIslandData();

	// �����̍쐬.
	void Create( json j );

	// �v���C���[���J�n�ʒu�ɐݒ�.
	bool SetPlayerStartPos( CPlayer* pPlayer );

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
	std::unique_ptr<CIslandPlayerHouse>	m_pPlayerHouse;		// �v���C���[�̉�.
	std::unique_ptr<CIslandHouse>		m_pHouse;			// ��.
	std::unique_ptr<CIslandShop>		m_pShop;			// ���X.
	CSkinMesh*							m_pIslandMash;		// �������f��.
	CCollisionMesh*						m_pCollisionMesh;	// �����蔻��p���b�V��.
	float								m_Alpha;			// �A���t�@�l.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};
