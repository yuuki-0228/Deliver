#pragma once
#include "..\..\..\UIWidget.h"

class CPlayer;

/************************************************
*	温度ゲージUIクラス.
*		﨑田友輝.
**/
class CTemperatureGaugeUI final
	: public CUIWidget
{
public:
	CTemperatureGaugeUI();
	virtual ~CTemperatureGaugeUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

	// リュックの位置を取得.
	D3DXPOSITION3 GetRucksackPos() const { return m_GaugeState.Transform.Position; }

private:
	CPlayer*			m_pPlayer;		// プレイヤー.
	CSprite*			m_pGauge;		// 温度ゲージ.
	CSprite*			m_pFrame;		// フレーム.
	SSpriteRenderState	m_GaugeState;	// 温度ゲージの情報.
	SSpriteRenderState	m_FrameState;	// フレームの情報.

private:
	// 外部データの定数.
	const ConstantStruct::SRuckSack& CONSTANT = CConstant::GetRuckSack();
};