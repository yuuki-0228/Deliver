#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	ゴールデンタイムクラス.
*		井谷凌.
**/
class CGoldenTimeUI
	: public CUIWidget
{
public:
	CGoldenTimeUI();
	virtual ~CGoldenTimeUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

private:
	// 看板の表示の設定.
	void NoticeOpen();
	// 看板を非表示の設定.
	void NoticeClose();

	// 看板の表示の更新.
	void NoticeOpenUpdate();
	// 看板の非表示の更新.
	void NoticeCloseUpdate();

	// エフェクトの再生.
	void EffectPlay();
	// エフェクトの更新.
	void EffectUpdate();

private:
	CSprite*				m_pNotice;				// 看板.
	CSprite*				m_pEffect;				// エフェクト.
	SSpriteRenderState		m_NoticeState;			// 看板の情報.
	SSpriteRenderState		m_EffectStateBase;		// エフェクトの情報ベース.
	SSpriteRenderStateList	m_EffectStateList;		// エフェクトの情報ベース.
	float					m_AnimTimeCnt;			// アニメーション用の時間カウント.
	float					m_EffectPlayTimeCnt;	// エフェクトの再生用の時間カウント.
	bool					m_IsOpen;				// 開いているか.
	bool					m_IsClose;				// 閉じているか.
};