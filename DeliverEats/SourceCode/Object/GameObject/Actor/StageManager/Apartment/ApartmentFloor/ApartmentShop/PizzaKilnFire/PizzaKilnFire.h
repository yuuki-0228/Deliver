#pragma once
#include "..\..\..\..\..\Actor.h"

/************************************************
*	ピザ窯の炎クラス.
*		﨑田友輝.
**/
class CPizzaKilnFire final
	: public CActor
{
public:
	CPizzaKilnFire();
	virtual ~CPizzaKilnFire();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// 表示するか.
	void SetIsDisp( const bool Flag ) { m_FloorIsDisp = Flag; }
	// アルファの設定.
	void SetAlpha( const float Alpha ) { m_Alpha = Alpha; }

private:
	CStaticMesh*	m_pFire;		// ピザ窯の炎.
	D3DXCOLOR3		m_Color;		// 色.
	D3DXCOLOR3		m_AddColor;		// 加算する色.
	float			m_Alpha;		// アルファ値.
	float			m_AddColorAng;	// 加算していく色用の角度.
	bool			m_FloorIsDisp;	// 1フロア単位の表示するかどうか.

private:
	// 外部データの定数.
	const ConstantStruct::SPizzaKilnFire& CONSTANT = CConstant::GetPizzaKilnFire();
};