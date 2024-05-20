#pragma once
#include "..\Character.h"
#include "..\..\RuckSack\RuckSack.h"

class CMainCamera;
class CGazingCamera;
class CEffectManager;
class CCustomer;
class CEmployee;
class CCustomerDire;

/**************************************************
*	�v���C���[�N���X.
*		���c�F�P.
**/
class CPlayer final
	: public CCharacter
{
public:
	using Effect_List = std::vector<std::unique_ptr<CEffectManager>>;

public:
	CPlayer();
	virtual ~CPlayer();

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
	// �e�̕`��.
	//	�v���C���[���B�ꂽ���ɕ`�悷��.
	void ShadowRender();

	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// ���i��n��.
	void FoodPassed();

	// �����x�N�g���̎擾.
	D3DXVECTOR3 GetDireVector();

	// ���i�������Ă��邩�擾.
	inline bool GetIsFoodHave() const { return m_pRuckSack->GetIsFoodHave(); }
	// ���݂̏��i�����擾.
	inline SFoodState GetFoodState() const { return m_pRuckSack->GetFoodState(); }
	// ���E�̃Q�[�W�̈ʒu���擾( -1.0f ~ 1.0f ).
	inline float GetRuckSackLRGauge() const { return m_pRuckSack->GetLRGauge(); }
	// �㉺�̃Q�[�W�̈ʒu���擾( -1.0f ~ 1.0f ).
	inline float GetRuckSackUDGauge() const { return m_pRuckSack->GetUDGauge(); }

	// �X��/���q���񂪌���ʒu���擾.
	D3DXPOSITION3 GetLookPos();

	// �J�n���̈ʒu���擾.
	D3DXPOSITION3 GetStartPosition() const { return m_StartPosition; }

	// ���n���Ă��邩���擾.
	bool GetIsLanding();
	// �����̋߂��ɂ��邩�擾.
	bool GetIsIsland();
	// �����J�������g�p���Ă��邩���擾.
	bool GetIsActiveGazingCam();

	// �x�N�g���̒ǉ�.
	inline void PushVector( const D3DXVECTOR3& Vec ) { m_MoveVectorList.emplace_back( Vec ); }

	// �J�n�ʒu�ɐݒ�.
	void SetStartPosition( const D3DXPOSITION3& Pos );

	//�@���i���󂯎��.
	void SetNewFood( const int Money, int ShopNo );
	// ���̐ݒ�.
	void SetWind( const D3DXVECTOR3& Vec );

protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision() override;
	// �����蔻��̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;

private:
	// �����̓����蔻��.
	void IslandCollision( CActor* pActor );
	// �v���C���[�̉Ƃ̒��n�̓����蔻��.
	void IsPlayerHouseCollision( CActor* pActor );
	// �W�F�b�g�p�b�N�̃G�t�F�N�g�̓����蔻��.
	void JetPackEffectCollision( CActor* pActor );
	// �n�ʂ̓����蔻��.
	void GroundCollision( CActor* pActor );
	// �n�ʂɋ߂����̓����蔻��.
	void GroundCloseCollision( CActor* pActor );
	// �e�̓����蔻��.
	void ShadowCollision( CActor* pActor );
	// �V��̓����蔻��.
	void CeilingCollision( CActor* pActor );
	// �ǂ̓����蔻��.
	void WallCollision( CActor* pActor );

	// �J�����̍X�V.
	void CameraControl();
	// �����J�������烁�C���J�����ɐ؂�ւ���.
	void SwitchToMain();
	// ���C���J�������璍���J�����ɐ؂�ւ���.
	void SwitchToGazing(CActor& pCustmer);

	// �g�����X�t�H�[���̍X�V.
	void TransformUpdate();
	// �����b�N�̐F�̐ݒ�.
	void SetRuckSackColor();

	// �W�F�b�g�p�b�N�̕���.
	void JetPackJetControl();
	// �W�F�b�g�p�b�N�̒�~.
	void JetPackStop();
	// �W�F�b�g�p�b�N�̒�~�̏C��.
	void JetPackRepair();
	// �W�F�b�g�p�b�N�̉�].
	void JetPackRotationControl();
	// �v���C���[�̈ړ�.
	void PlayerMove();
	// �v���C���[�̉�].
	void PlayerRotation();
	// �v���C���[�̌X���̍X�V.
	void PlayerTiltUpdate();
	
	// �ړ��x�N�g���̍쐬.
	void CreateMoveVec();
	// ���E�̃W�F�b�g�p�b�N�̉�]�x�N�g���̍쐬.
	void CreateRotVec();

	// �d�͂̒ǉ�.
	void AddGravity();
	// �����x�̏��/�����̐ݒ�.
	void AccelerationClamp();
	// ���C
	void Friction( const float Value );

	// ���n�G�t�F�N�g�̍Đ�.
	void LandingEffectPlay( const D3DXPOSITION3& Pos );
	// ���n�G�t�F�N�g�̍X�V.
	void LandingEffectUpdate();
	// ���i�擾�A�j���[�V�����̍X�V.
	void GetFoodAnimUpdate();

	// �X�e�[�W�O���m�F.
	void OffStageCheck();
	// �X�e�[�W�O�Ɉړ��������̍X�V.
	void StageOutUpdate();

	// �ړ���SE�̍Đ�.
	void PlayMoveSE();

protected:
	std::unique_ptr<CMainCamera>				m_pMainCamera;				// ���C���J����.
	std::unique_ptr<CGazingCamera>				m_pGazingCamera;			// �����J����.
	std::vector<std::unique_ptr<CRay>>			m_pRayList;					// ���C���X�g.
	std::unique_ptr<CRuckSack>					m_pRuckSack;				// �����b�N.
	std::unique_ptr<CCustomerDire>				m_pCustomerDire;			// �n���ɍs�����q����̕���.
	std::vector<CSkinMesh*>						m_pSkinMesh;				// �X�L�����b�V��.
	std::vector<STransform>						m_TransformList;			// �g�����X�t�H�[��.
	std::vector<D3DXVECTOR3>					m_MoveVectorList;			// �ړ��x�N�g�����X�g.
	std::vector<D3DXVECTOR3>					m_VectorList;				// �x�N�g�����X�g.
	std::vector<float>							m_LJetPackInputList;		// ���W�F�b�g�p�b�N�̓��̓��X�g.
	std::vector<float>							m_RJetPackInputList;		// �E�W�F�b�g�p�b�N�̓��̓��X�g.
	std::vector<float>							m_CntList;					// �J�E���g���X�g.
	std::vector<float>							m_SEVolumeList;				// SE�̉��ʃ��X�g.
	std::vector<bool>							m_FlagList;					// �t���O���X�g.
	CSprite*									m_pShadow;					// �e.
	SSpriteRenderState							m_ShadowState;				// �e�̏��.
	Effect_List									m_pEffectList;				// �G�t�F�N�g���X�g.
	Effect_List									m_pLandingEffectList;		// ���n�G�t�F�N�g.
	CCustomer*									m_pOldCustomer;				// 1�t���[���O�̂��q����̏��(�n�����u�Ԃ��擾����̂ɕK�v).
	SAnimationController						m_AC;						// �A�j���[�V�����R���g���[���[.
	D3DXPOSITION3								m_LandingEndPos;			// ���n�̍Ō�̍��W.
	D3DXPOSITION3								m_StartPosition;			// �J�n���̈ʒu.
	std::pair<float, float>						m_ShadowPosY;				// �e�̈ʒu<����, Y���W>.
	float										m_Alpha;					// �A���t�@�l.
	float										m_SideMoveSpeed;			// �������ɑ΂��Ă̈ړ����x.
	float										m_VerticalMoveSpeed;		// �c�����ɑ΂��Ă̈ړ����x.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};
