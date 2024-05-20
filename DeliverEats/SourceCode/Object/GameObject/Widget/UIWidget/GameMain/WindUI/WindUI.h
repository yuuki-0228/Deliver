#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	風UIクラス.
*		﨑田友輝.
**/
class CWindUI final
	: public CUIWidget
{
public:
	CWindUI();
	virtual ~CWindUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

private:
	// 風の線の移動.
	void WindLineMove();

	// 風の線を召喚.
	void SpownWindLine();

	// リストの削除.
	void ListDelete();

private:
	CSprite*				m_pWindLine;			// 風の線.
	SSpriteRenderState		m_WindLineStateBase;	// 風の線の情報ベース.
	SSpriteRenderStateList	m_WindLineStateList;	// 風の線の情報リスト.
	float					m_DispWindLineTimeCnt;	// 風の線を表示していく時間用カウント.

private:
	// 外部データの定数.
	const ConstantStruct::SWindUI& CONSTANT = CConstant::GetWindUI();

};