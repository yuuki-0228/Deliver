#pragma once
#include "..\..\UIWidget.h"

/************************************************
*	UINX.
*		ú±cFP.
**/
class CWindUI final
	: public CUIWidget
{
public:
	CWindUI();
	virtual ~CWindUI();

	// ú».
	virtual bool Init() override;
	// XV.
	virtual void Update( const float& DeltaTime ) override;
	// `æ.
	virtual void Render() override;

private:
	// ÌüÌÚ®.
	void WindLineMove();

	// Ìüð¢«.
	void SpownWindLine();

	// XgÌí.
	void ListDelete();

private:
	CSprite*				m_pWindLine;			// Ìü.
	SSpriteRenderState		m_WindLineStateBase;	// ÌüÌîñx[X.
	SSpriteRenderStateList	m_WindLineStateList;	// ÌüÌîñXg.
	float					m_DispWindLineTimeCnt;	// Ìüð\¦µÄ¢­ÔpJEg.

private:
	// Of[^Ìè.
	const ConstantStruct::SWindUI& CONSTANT = CConstant::GetWindUI();

};