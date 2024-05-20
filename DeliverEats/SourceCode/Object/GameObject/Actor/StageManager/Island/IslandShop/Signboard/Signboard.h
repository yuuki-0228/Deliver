#pragma once
#include "..\..\..\..\Actor.h"

/************************************************
*	看板クラス.
**/
class CSignboard final
	: public CActor
{
public:
	CSignboard();
	virtual ~CSignboard();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// 当たり判定を追加する.
	void PushCollisionManager();

	// お店のタイプの取得.
	int GetShopNo() const { return m_ShopNo; }
	// お店のタイプの指定.
	void SetShopNo( const int No ) { m_ShopNo = No; }

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

private:
	// カメラによるアルファ値の更新.
	void CameraAlphaUpdate();

private:
	CSkinMesh*			m_pSignboardMesh;	// 看板メッシュ.
	CCollisionMesh*		m_pCollisionMesh;	// 当たり判定用メッシュ.
	SMeshPointLight		m_PointLight;		// ポイントライト.
	float				m_Alpha;			// アルファ値.
	float				m_LightAng;			// ライトのアニメーション用の角度.
	float				m_LightFlashing;	// ライトの点滅具合.
	int					m_ShopNo;			// お店のタイプ.

private:
	// 外部データの定数.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};