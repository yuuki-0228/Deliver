#pragma once
#include "..\..\..\..\Actor.h"

/************************************************
*	ガレージクラス.
**/
class CGarage final
	: public CActor
{
public:
	CGarage();
	virtual ~CGarage();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// 当たり判定を追加する.
	void PushCollisionManager();

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
	CSkinMesh*		m_pGarageMesh;		// ガレージメッシュ.
	CCollisionMesh*	m_pCollisionMesh;	// 当たり判定用メッシュ.
	float			m_Alpha;			// アルファ値.

private:
	// 外部データの定数.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};