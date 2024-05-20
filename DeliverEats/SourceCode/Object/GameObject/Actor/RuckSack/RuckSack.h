#pragma once
#include "..\Actor.h"

// ���i�\����.
struct stFoodState {
	int		EndMoney;			// �ŏI���炦�邨��.
	int		Money;				// �Œ�����炦�邨��.
	int		DiscountMoney;		// �X������.
	int		HotBonus;			// ���x�{�[�i�X.
	int		GoldenBonus;		// �S�[���f���^�C���{�[�i�X.
	int		ShopNo;				// ���X�̔ԍ�.

	float	Temperature;		// ���x( MIN_TEMPERATURE ~ MAX_TEMPERATURE ).
	float	Discount;			// ������( MIN_TILT ~ MAX_TILT ).
} typedef SFoodState;

/************************************************
*	�����b�N�N���X.
*		���c�F�P.
**/
class CRuckSack final
	: public CActor
{
public:
	CRuckSack();
	virtual ~CRuckSack();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �f�o�b�N�̍X�V.
	virtual void DebugUpdate( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override {};

	// ���i��n��.
	inline void FoodPassed() { m_IsFoodHave = false; }

	// ���i�������Ă��邩�擾.
	inline bool GetIsFoodHave() const { return m_IsFoodHave; }
	// ���i�̏�Ԃ̎擾.
	inline SFoodState GetFoodState() const { return m_FoodState; }

	// ���E�̃Q�[�W�̈ʒu���擾( -1.0f ~ 1.0f ).
	inline float GetLRGauge() { return m_LRGauge; }
	// �㉺�̃Q�[�W�̈ʒu���擾( -1.0f ~ 1.0f ).
	inline float GetUDGauge() { return m_UDGauge; }

	// �V�������i�̐ݒ�.
	void SetNewFood( const int Money, int ShopNo );

	// �X���̐ݒ�.
	inline void SetTilt( float* Tilt ) { m_pTilt = Tilt; }

private:
	// ���E�̃Q�[�W��^�񒆂ɖ߂��X�V.
	void LRGaugeResetUpdate();
	// �㉺�̃Q�[�W��^�񒆂ɖ߂��X�V.
	void UDGaugeResetUpdate();
	// ���E�̃Q�[�W�̍X�V.
	void LRGaugeUpdate();
	// �㉺�̃Q�[�W�̍X�V.
	void UDGaugeUpdate();
	// �㉺�Q�[�W�̑��x�̍X�V.
	void UDGaugeSpeedUpdate();

	// ���x�̍X�V.
	void TemperatureUpdate();
	// �������̍X�V.
	void DiscountUpdate();

	// ���x�{�[�i�X�̍X�V.
	void HotBonusUpdate();
	// �S�[���f���^�C���{�[�i�X�̍X�V.
	void GoldenBonusUpdate();
	// �X�������̍X�V.
	void DiscountMoneyUpdate();
	// �ŏI���炦�������̍X�V.
	void EndMoneyUpdate();

private:
	SFoodState		m_FoodState;		// ���i�̏��.
	bool			m_IsFoodHave;		// ���i�������Ă��邩.
	float			m_JetPackPower;		// �W�F�b�g�p�b�N�̋���.
	float			m_OldJetPackPower;	// �O�̃W�F�b�g�p�b�N�̋���.
	float			m_UDGaugeSpeed;		// �㉺�̃Q�[�W�̐i�ޑ��x.
	float*			m_pTilt;			// �X��.
	float			m_LRGauge;			// ���E�̃Q�[�W( -1.0f ~ 1.0f ).
	float			m_UDGauge;			// �㉺�̃Q�[�W( -1.0f ~ 1.0f ).

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};
