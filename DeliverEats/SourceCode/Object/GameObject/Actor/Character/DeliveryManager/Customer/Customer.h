#pragma once
#include "..\Linking\Linking.h"
#include "..\..\Character.h"
#include "..\..\..\RuckSack\RuckSack.h"

class CEmployee;
class CPlayer;
class CEffectManager;

/************************************************
*	���q����N���X.
*		���c�F�P.
**/
class CCustomer final
	: public CCharacter
{
public:
	CCustomer( const EObjectTag TypeTag );
	~CCustomer();

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

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// �R�Â��n���h���̎擾.
	inline CLinking::Handle GetHandle() const { return m_Handle; }
	// �F�̎擾.
	inline D3DXCOLOR3 GetLinkColor() const { return CLinking::GetLinkColor( m_Handle ); }

	// �v���C���[���߂��ɂ��邩�擾.
	inline bool GetIsShortly() const { return m_IsShortly; }
	// �R�Â�����Ă��邩�擾.
	inline bool GetIsLink() const { return m_Handle != NO_LINK_HANDLE; }
	// �҂����Ԃ��擾.
	inline bool GetIsWait() const { return m_DetachCoolTime > 0.0f; }
	// �n���ɍs�����q���񂩎擾.
	inline bool GetIsGoHandingCustomer() const { return m_IsGoHandingCustomer; }
	// �s�U�̎擾�A�j���[�V�������I���������擾.
	inline bool GetIsGetPizzaAnimEnd() const { return m_IsGetPizzaAnimEnd; }

	// �󂯎��Q�[�W�̎擾.
	inline float GetGauge() const { return m_Gauge; }
	// �Q�[�W�폜�J�E���g�̎擾.
	inline float GetDeleteTimeCnt() const { return m_GaugeDeleteTimeCnt; }
	// �ǉ����邨���̎擾.
	inline float GetAddMoney() const { return m_AddMoney; }

	// �����̐ݒ�.
	inline void SetDirection( const EDirection Dire ) { m_Direction = Dire; }
	// �ǉ��̂����̐ݒ�.
	inline void SetAddMoney( const float AddMoney ) { m_AddMoney = AddMoney; }
	// �A���t�@�̐ݒ�.
	inline void SetAlpha( const float Alpha ) { m_Alpha = Alpha; }
	// ���b�V���|�C���g���C�g�̐ݒ�.
	inline void SetMeshPointLight( const SMeshPointLight& Light ) { m_PointLight = Light; }
	// �n���ɍs�����q����ɐݒ�.
	void SetGoHandingCustomer();

	// ���X�̐ݒ�.
	void AttachLink( const CLinking::Handle& handle );
	// �R�Â��̔j��.
	void DetachLink();

	// �ʏ펞�ɕύX.
	void ChangeNormal();
	// ���U��A�j���[�V�����ɕύX.
	void ChangeWaveHands();

protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision() override;
	// �����蔻��̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;

private:
	// �v���C���[���߂��ɂ��邩�̓����蔻��.
	void PlayerShortlyCollision( CActor* pActor );
	// ���i�̎󂯎��p�̓����蔻��.
	void GetFoodCollision( CActor* pActor );

	// �g�����X�t�H�[���̍X�V.
	void TransformUpdate();
	// �A�j���[�V�����̍X�V.
	void AnimationUpdate();

	// �ʒm�𑗂�.
	void PushNotification( const SFoodState& FoodState );

	// �A���ŕR�Â�����Ȃ��悤�ɂ���N�[���^�C���̍X�V.
	void DetachCoolTimeUpdate();

	// �s�U�̎擾�A�j���[�V�����ɕύX.
	void ChangeGetPizza();

	// �ʏ펞�ɕύX�̍X�V.
	void ChangeNormalAnimUpdate();
	// ���U��A�j���[�V�����ɕύX�̍X�V.
	void ChangeWaveHandsAnimUpdate();
	// ���U��A�j���[�V�����̍X�V.
	void WaveHandsAnimUpdate();
	// �s�U�̎擾�A�j���[�V�����ɕύX�̍X�V.
	void ChangeGetPizzaAnimUpdate();
	// �s�U�̎擾�A�j���[�V�����̍X�V.
	void GetPizzaAnimUpdate();
	// �ʏ�ɖ߂��A�j���[�V�����̍X�V.
	void GetPizzaAnimEndUpdate();

private:
	std::vector<CSkinMesh*>			m_pSkinMesh;			// �X�L�����b�V��.
	std::vector<STransform>			m_TransformList;		// �g�����X�t�H�[�����X�g.
	std::unique_ptr<CEffectManager>	m_pGetEffect;			// ���i������������̃G�t�F�N�g.
	CSkinMesh*						m_pPizzaBox;			// �s�U��.
	CPlayer*						m_pPlayer;				// �v���C���[.
	SMeshPointLight					m_PointLight;			// �|�C���g���C�g.
	STransform						m_PizzaBoxTransform;	// �s�U���̃g�����X�t�H�[��.
	CLinking::Handle				m_Handle;				// �R�Â��n���h��.
	D3DXVECTOR3						m_RightHandMoveVector;	// �E��̈ړ��x�N�g��.
	D3DXVECTOR3						m_LeftHandMoveVector;	// ����̈ړ��x�N�g��.
	D3DXPOSITION3					m_RightHandsStartPos;	// �E��̃A�j���[�V�����̊J�n�ʒu.
	D3DXPOSITION3					m_LeftHandsStartPos;	// ����̃A�j���[�V�����̊J�n�ʒu.
	D3DXMATRIX						m_HeadRotMatrix;		// ���̉�]�s��.
	EObjectTag						m_Type;					// �^�C�v�̃^�O( Apartment, Island�̂ݎg�p ).
	EDirection						m_Direction;			// ����.
	float							m_AddMoney;				// �ǉ����邨��.
	float							m_Gauge;				// �󂯎��Q�[�W( 0.0f ~ 1.0f ).
	float							m_GaugeDeleteTimeCnt;	// �Q�[�W�̍폜�J�E���g.
	float							m_Alpha;				// �A���t�@�l.
	float							m_DetachCoolTime;		// �A���ŕR�Â�����Ȃ��悤�ɂ���N�[���^�C��.
	float							m_PizzaBoxAlpha;		// �s�U���̃A���t�@�l.
	float							m_AnimCnt;				// �A�j���[�V�����p�̃J�E���g.
	int								m_AnimationNo;			// �A�j���[�V����No.
	int								m_BodyNo;				// �̂�No.
	int								m_HeadNo;				// ����No.
	int								m_PizzaBoxNo;			// �s�U����No.
	bool							m_IsHitPlayer;			// �v���C���[�ɓ���������.
	bool							m_IsShortly;			// �v���C���[���߂��ɂ��邩.
	bool							m_IsGoHandingCustomer;	// �n���ɍs�����q����.
	bool							m_IsWaveHandsMoveUp;	// ���U��Ƃ��ɏ�ɂ����邩.
	bool							m_IsGetPizzaAnimEnd;	// �s�U�̎擾�A�j���[�V�������I��������.

private:
	// �f�t�H���g�R���X�g���N�^���֎~����.
	CCustomer() = delete;
	CCustomer( const CCustomer& ) = delete;
	CCustomer& operator = ( const CCustomer& rhs ) = default;

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SDeliveryCharacter& CONSTANT = CConstant::GetDeliveryCharacter();
};