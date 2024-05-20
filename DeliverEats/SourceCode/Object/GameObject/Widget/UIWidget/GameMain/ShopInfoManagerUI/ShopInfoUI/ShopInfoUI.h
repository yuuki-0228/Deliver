#pragma once
#include "..\..\..\UIWidget.h"
#include "..\..\..\..\..\Actor\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\..\..\..\..\..\Common\Font\Font.h"

class CPlayer;
class CEmployee;

/************************************************
*	お店の詳細情報クラス.
**/
class CShopInfoUI final
	: public CUIWidget
{
public:
	CShopInfoUI();
	~CShopInfoUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// 表示.
	void Open( int No, CLinking::Handle Handle );
	// 非表示.
	void Close();

	// アニメーション中か取得.
	inline bool GetIsAnimation() const { return m_IsOpen || m_IsClose; }
	// 表示しているか取得.
	inline bool GetIsDisp() const { return m_IsDisp; }

	// プレイヤーの設定.
	inline void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	// 詳細情報の更新.
	void InfoUpdate();
	// 開く動作の更新.
	void OpenUpdate();
	// 閉じる動作の更新.
	void CloseUpdate();

private:
	CPlayer*				m_pPlayer;			// プレイヤー.
	CEmployee*				m_pEmployee;		// 店員.
	CSprite*				m_pGauge;			// 背景.
	CSprite*				m_pIcon;			// アイコン.
	CSprite*				m_pFrame;			// フレーム.
	CFont*					m_pFont;			// フォント.
	SSpriteRenderState		m_GaugeState;		// 背景の情報.
	SSpriteRenderState		m_IconState;		// アイコンの情報.
	SSpriteRenderState		m_FrameState;		// フレームの情報.
	SFontRenderState		m_FontStateBase;	// フォントの情報ベース.
	SFontRenderStateList	m_FontState;		// フォントの情報.
	CLinking::Handle		m_Handle;			// ハンドル.
	D3DXCOLOR4				m_Color;			// 色.
	float					m_AnimTimeCnt;		// アニメーション用の時間カウント.
	bool					m_IsDisp;			// 表示しているか.
	bool					m_IsOpen;			// 表示するか.
	bool					m_IsClose;			// 閉じるか.

private:
	// 外部データの定数.
	const ConstantStruct::SShopInfo& CONSTANT = CConstant::GetShopInfo();
};