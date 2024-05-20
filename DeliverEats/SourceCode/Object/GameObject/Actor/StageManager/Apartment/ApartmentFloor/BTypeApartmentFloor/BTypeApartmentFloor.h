#pragma once
#include "..\..\..\..\Actor.h"

class CApartmentEmptyRoom;
class CApartmentRoom;
class CApartmentShop;

/************************************************
*	�}���V����B�p�^�[��(8x2)��1�t���A�N���X.
*		���c�F�P.
**/
class CBTypeApartmentFloor final
	: public CActor
{
public:
	using EmptyRoomList = std::vector<std::pair<std::unique_ptr<CApartmentEmptyRoom>, int>>;
	using RoomList		= std::vector<std::pair<std::unique_ptr<CApartmentRoom>, int>>;
	using ShopList		= std::vector<std::pair<std::unique_ptr<CApartmentShop>, int>>;

public:
	CBTypeApartmentFloor();
	virtual ~CBTypeApartmentFloor();

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

	// �t���A�̍쐬.
	void Create( json j );
	// �t���A�̈ړ�.
	void FloorMove( const D3DXPOSITION3& NewPos );
	// �����̐����擾<�X�̐�, <�����̐�, �󕔉��̐�>>.
	std::pair<int, std::pair<int, int>> GetRoomNum() const {
		return std::make_pair( static_cast<int>( m_pShopList.size() ), std::make_pair(
			static_cast<int>( m_pRoomList.size() ),
			static_cast<int>( m_pEmptyRoomList.size() ) 
		) );
	}

	// ��ɐݒu������W���擾����.
	D3DXPOSITION3 GetSettingPosition();

	// �t���A�̃f�[�^�̎擾.
	json GetFloorData();

	// �A���t�@�̐ݒ�.
	void SetAlpha( const float& Alpha );
	// ��̃t���A�ɐݒ�.
	void SetEmptyFloor() { m_IsEmptyFloor = true; }

private:
	// ���C�g�̍X�V.
	void LightUpdate();

private:
	CSkinMesh*		m_pApartmentMesh;	// �}���V����(1�K)�̃��b�V��.
	SMeshPointLight	m_PointLight;		// �|�C���g���C�g.
	EmptyRoomList	m_pEmptyRoomList;	// �󕔉����X�g.
	RoomList		m_pRoomList;		// �������X�g.
	ShopList		m_pShopList;		// ���X���X�g.
	float			m_Alpha;			// �A���t�@�l.
	int				m_LightHour;		// ���C�g�����鎞��.
	int				m_LightMinute;		// ���C�g�����镪.
	bool			m_IsEmptyFloor;		// ��̃t���A��.
	bool			m_IsLightOn;		// ���C�g��������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
