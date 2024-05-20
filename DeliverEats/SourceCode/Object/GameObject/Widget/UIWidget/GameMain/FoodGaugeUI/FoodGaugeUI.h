#pragma once
#include "..\..\UIWidget.h"

namespace {
	// ゲージ構造体.
	struct stGauge {
		float Value;
		float Alpha;

		stGauge()
			:stGauge( INIT_FLOAT, INIT_FLOAT )
		{}
		stGauge( float value, float alpha )
			: Value( value )
			, Alpha( alpha )
		{}
	}typedef SGauge;
}

class CEmployee;
class CCustomer;

/************************************************
*	商品受け取り/受け渡しゲージUIクラス.
*	井谷凌.
**/
class CFoodGaugeUI final
	: public CUIWidget
{
public:
	CFoodGaugeUI();
	virtual ~CFoodGaugeUI();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

private:
	// 受け取りゲージの処理.
	void ReceiveGauge( const float& DeltaTime, CEmployee* pEmployee );

	// 受け渡しゲージの処理.
	void SendGauge( const float& DeltaTime, CCustomer* pCustomer );

	// 表示/非表示切り替え.
	void SetReceiveDisp( bool Flag );

	// Alphaの一括管理.
	void SetReceiveAlpha( float Alpha );

	// 表示/非表示切り替え.
	void SetSendDisp( bool Flag );

	// Alphaの一括管理.
	void SetSendAlpha( float Alpha );

private:
	SGauge		m_ReceiveGauge;
	SGauge		m_SendGauge;
};