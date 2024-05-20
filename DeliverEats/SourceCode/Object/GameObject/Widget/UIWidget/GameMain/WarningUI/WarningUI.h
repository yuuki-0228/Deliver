#pragma once
#include "..\..\UIWidget.h"

class CPlayer;

/************************************************
*	警告UI.
**/
class CWarningUI final
	: public CUIWidget
{
public:
	CWarningUI();
	virtual ~CWarningUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーを設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*	m_pPlayer;				// プレイヤー.
	bool		m_IsUpPushMessage;		// 特定より上に行った時に通知を送ったか.
	bool		m_IsDownPushMessage;	// 特定より下に行った時に通知を送ったか.
	bool		m_IsRadiusPushMessage;	// 特定の半径より奥に行ったときに通知を送ったか.
	float		m_SEVolume;				// SEの音量.

private:
	// 外部データの定数.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};