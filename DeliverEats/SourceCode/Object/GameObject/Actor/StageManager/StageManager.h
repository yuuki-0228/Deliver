#pragma once
#include "..\Actor.h"

class CPlayer;
class CWorld;
class CATypeApartment;
class CBTypeApartment;
class CIsland;

/************************************************
*	�X�e�[�W�}�l�[�W���[.
*		���c�F�P.
**/
class CStageManager
	: public CActor
{
public:
	CStageManager();
	virtual ~CStageManager();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �����蔻��I����Ăяo�����X�V.
	virtual void LateUpdate( const float& DeltaTime ) override;
	// �f�o�b�N�̍X�V.
	virtual void DebugUpdate( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;
	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

	// �X�e�[�W�G�f�B�^��ImGui�̕`��.
	void StageEditorImGuiRender();
	// �X�e�[�W�̏���ImGui�ŕ`��.
	void StageStateImGuiRender();

	// �I�u�W�F�N�g�̒ǉ�.
	//	ATypeApartment / BTypeApartment / Island.
	void AddObject( const std::string& ObjectName );

	// �����̃f�[�^�̎擾.
	json GetStageData();

	// �v���C���[�̊J�n�ʒu�̎擾.
	void SetPlayerStartPos( CPlayer* pPlayer );

private:
	std::unique_ptr<CWorld>							m_pWorld;				// ���[���h.
	std::vector<std::unique_ptr<CATypeApartment>>	m_pATypeApartmentList;	// A�^�C�v�̃}���V�������X�g.
	std::vector<std::unique_ptr<CBTypeApartment>>	m_pBTypeApartmentList;	// B�^�C�v�̃}���V�������X�g.
	std::vector<std::unique_ptr<CIsland>>			m_pIslandList;			// �������X�g.
	D3DXPOSITION3									m_PlayerStartPos;		// �v���C���[�̊J�n�ʒu.
};