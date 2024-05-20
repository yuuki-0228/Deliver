#pragma once
#include "..\..\..\..\..\..\Global.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\..\Widget.h"

class CActor;
class CPlayer;

/************************************************
*	�~�j�}�b�vUI�N���X.
*		���c�F�P.
**/
class CMiniMapUI final
{
public:
	using TagIconNo_map = std::unordered_map<EObjectTag, SHORT>;
	using DireIcon_map	= std::unordered_map<EDirection, SHORT>;
	using DireMask_map	= std::unordered_map<EDirection, ID3D11ShaderResourceView*>;

private:
	// �A�C�R�����.
	struct stIconState {
		D3DXVECTOR3			Pos;		// ���W.
		D3DXCOLOR3			Color;		// �J���[.
		SHORT				Pattern;	// �A�C�R���p�^�[��.
		EDirection			Dire;		// ����.
		CLinking::Handle	Handle;		// �R�Â��n���h��.
		bool				IsBack;		// �w�i��\�����邩.

		stIconState()
			: Pos		( INIT_FLOAT3 )
			, Color		( Color::White )
			, Pattern	( 0 )
			, Dire		( EDirection::None )
			, Handle	( 0 )
			, IsBack	( true )
		{}

	} typedef SIconState;

public:
	CMiniMapUI();
	~CMiniMapUI();

	// �C���X�^���X�̎擾.
	static CMiniMapUI* GetInstance();

	// ������.
	static void Init();

	// �`��.
	static void Render();

	// �L�����N�^�[�̃A�C�R���̒ǉ�.
	static void PushCharacterIcon( CActor* pActor, const EDirection Dire, const CLinking::Handle Handle, const EObjectTag Tag = EObjectTag::None );
	// A�^�C�v�}���V�����̃A�C�R���̒ǉ�.
	static void PushApartmentAIcon( CActor* pActor );
	// B�^�C�v�}���V�����̃A�C�R���̒ǉ�.
	static void PushApartmentBIcon( CActor* pActor );
	// �����̃A�C�R���̒ǉ�.
	static void PushIslandIcon( CActor* pActor );

	// �X�}�z�̉�ʂ̒����ʒu�̎擾.
	static D3DXPOSITION3 GetScreenPos() { return GetInstance()->m_pBack->GetRenderPos(); }
	// �X�}�z�̉�ʂ̑傫���̎擾.
	static SSize GetScreenSize() { return GetInstance()->m_pBack->GetSpriteState().Disp; }

	// �v���C���[�̐ݒ�.
	static void SetPlayer( CPlayer* pPlayer ) { GetInstance()->m_pPlayer = pPlayer; }

private:
	// ��ʂ��~���Ă��鎞�̕`��.
	static void StopRender();

	// �v���C���[�̉Ƃ̒ǉ�.
	static void PushPlayerHouse( const D3DXPOSITION3& DispPos, const D3DXVECTOR3& MiniMapPos, const float MiniMapSizeW, const float MiniMapSizeH );

	// �O���b�h���̕`��.
	static void GridRender();
	// �A�C�R���̕`��.
	static void IconRender();
	// �O�̃t���[���̃A�C�R���̕`��.
	static void OldIconRender();
	// �v���C���[�̃A�C�R���̕`��.
	static void PlayerIconRender();
	// �n���ɍs�����q����̕����̃A�C�R���̕`��.
	static void DireRender();
	// ���X�̕����̃A�C�R���̕`��.
	static void ShopDireRender();
	// �X�e�[�W�͈̔͂̕`��.
	static void StageOutRender();

private:
	CPlayer*					m_pPlayer;							// �v���C���[.
	CSprite*					m_pBack;							// �w�i.
	CSprite*					m_pIconBack;						// �A�C�R���̔w�i.
	CSprite*					m_pIcon;							// �A�C�R��.
	CSprite*					m_pIslandIcon;						// �����̃A�C�R��.
	CSprite*					m_pATypeApartmentIcon;				// A�^�C�v�̃}���V�����̃A�C�R��.
	CSprite*					m_pBTypeApartmentIcon;				// B�^�C�v�̃}���V�����̃A�C�R��.
	CSprite*					m_pDireIcon;						// �n���ɍs�����q����̌����A�C�R��.
	CSprite*					m_pGrid;							// �O���b�h��.
	CSprite*					m_pStageOut;						// �X�e�[�W�͈̔�.
	SSpriteRenderState			m_IconBackState;					// �A�C�R���̔w�i�̏��.
	SSpriteRenderState			m_DireIconState;					// �n���ɍs�����q����̌����A�C�R���̏��.
	SSpriteRenderState			m_GridState;						// �O���b�h���̏��.
	SSpriteRenderState			m_StageOutState;					// �X�e�[�W�͈̔͂̏��.
	SSpriteRenderState			m_IconState;						// �A�C�R���̏��.
	SSpriteRenderState			m_IslandIconStateBase;				// �����̃A�C�R���̏��x�[�X.
	SSpriteRenderState			m_ATypeApartmentIconStateBase;		// A�^�C�v�̃}���V�����̃A�C�R���̏��x�[�X.
	SSpriteRenderState			m_BTypeApartmentIconStateBase;		// B�^�C�v�̃}���V�����̃A�C�R���̏��x�[�X.
	SSpriteRenderStateList		m_IslandIconStateList;				// �����̃A�C�R���̏�񃊃X�g.
	SSpriteRenderStateList		m_OldIslandIconStateList;			// �O�̕����̃A�C�R���̏�񃊃X�g.
	SSpriteRenderStateList		m_ATypeApartmentIconStateList;		// A�^�C�v�̃}���V�����̃A�C�R���̏�񃊃X�g.
	SSpriteRenderStateList		m_OldATypeApartmentIconStateList;	// �O�̃t���[����A�^�C�v�̃}���V�����̃A�C�R���̏�񃊃X�g.
	SSpriteRenderStateList		m_BTypeApartmentIconStateList;		// B�^�C�v�̃}���V�����̃A�C�R���̏�񃊃X�g.
	SSpriteRenderStateList		m_OldBTypeApartmentIconStateList;	// �O�̃t���[����B�^�C�v�̃}���V�����̃A�C�R���̏�񃊃X�g.
	std::vector<SIconState>		m_IconStateList;					// �A�C�R���̏�񃊃X�g.
	std::vector<SIconState>		m_OldIconStateList;					// �O�̃t���[���̃A�C�R���̏�񃊃X�g.
	SIconState					m_PlayerHouseIconState;				// �v���C���[�̉Ƃ̃A�C�R���̏��.
	TagIconNo_map				m_TagImageNo;						// �^�O���Ƃ̉摜No.
	DireIcon_map				m_DireImageNo;						// �������Ƃ̉摜No.
	DireMask_map				m_DireMask;							// �������Ƃ̃}�X�N�e�N�X�`��.
	CActor*						m_pGoHandingCustomer;				// �n���ɍs�����q����.
	bool						m_IsDisp;							// �\�����邩.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SMiniMap& CONSTANT = CConstant::GetMiniMap();
};
