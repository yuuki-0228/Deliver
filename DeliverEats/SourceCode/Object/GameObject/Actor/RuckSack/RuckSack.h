#pragma once
#include "..\Actor.h"

// 商品構造体.
struct stFoodState {
	int		EndMoney;			// 最終もらえるお金.
	int		Money;				// 最低限もらえるお金.
	int		DiscountMoney;		// 傾き割引.
	int		HotBonus;			// 温度ボーナス.
	int		GoldenBonus;		// ゴールデンタイムボーナス.
	int		ShopNo;				// お店の番号.

	float	Temperature;		// 温度( MIN_TEMPERATURE ~ MAX_TEMPERATURE ).
	float	Discount;			// 割引率( MIN_TILT ~ MAX_TILT ).
} typedef SFoodState;

/************************************************
*	リュッククラス.
*		﨑田友輝.
**/
class CRuckSack final
	: public CActor
{
public:
	CRuckSack();
	virtual ~CRuckSack();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// デバックの更新.
	virtual void DebugUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override {};

	// 商品を渡す.
	inline void FoodPassed() { m_IsFoodHave = false; }

	// 商品を持っているか取得.
	inline bool GetIsFoodHave() const { return m_IsFoodHave; }
	// 商品の状態の取得.
	inline SFoodState GetFoodState() const { return m_FoodState; }

	// 左右のゲージの位置を取得( -1.0f ~ 1.0f ).
	inline float GetLRGauge() { return m_LRGauge; }
	// 上下のゲージの位置を取得( -1.0f ~ 1.0f ).
	inline float GetUDGauge() { return m_UDGauge; }

	// 新しい商品の設定.
	void SetNewFood( const int Money, int ShopNo );

	// 傾きの設定.
	inline void SetTilt( float* Tilt ) { m_pTilt = Tilt; }

private:
	// 左右のゲージを真ん中に戻す更新.
	void LRGaugeResetUpdate();
	// 上下のゲージを真ん中に戻す更新.
	void UDGaugeResetUpdate();
	// 左右のゲージの更新.
	void LRGaugeUpdate();
	// 上下のゲージの更新.
	void UDGaugeUpdate();
	// 上下ゲージの速度の更新.
	void UDGaugeSpeedUpdate();

	// 温度の更新.
	void TemperatureUpdate();
	// 割引率の更新.
	void DiscountUpdate();

	// 温度ボーナスの更新.
	void HotBonusUpdate();
	// ゴールデンタイムボーナスの更新.
	void GoldenBonusUpdate();
	// 傾き割引の更新.
	void DiscountMoneyUpdate();
	// 最終もらえたお金の更新.
	void EndMoneyUpdate();

private:
	SFoodState		m_FoodState;		// 商品の状態.
	bool			m_IsFoodHave;		// 商品を持っているか.
	float			m_JetPackPower;		// ジェットパックの強さ.
	float			m_OldJetPackPower;	// 前のジェットパックの強さ.
	float			m_UDGaugeSpeed;		// 上下のゲージの進む速度.
	float*			m_pTilt;			// 傾き.
	float			m_LRGauge;			// 左右のゲージ( -1.0f ~ 1.0f ).
	float			m_UDGauge;			// 上下のゲージ( -1.0f ~ 1.0f ).

private:
	// 外部データの定数.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};
