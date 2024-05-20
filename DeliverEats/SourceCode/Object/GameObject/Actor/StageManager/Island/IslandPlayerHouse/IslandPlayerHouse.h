#pragma once
#include "..\..\..\Actor.h"

class CGarage;
class CCustomer;
class CStageIcon;

/************************************************
*	浮島のプレイヤーの家クラス.
*		﨑田友輝.
**/
class CIslandPlayerHouse final
	: public CActor
{
public:
	CIslandPlayerHouse();
	virtual ~CIslandPlayerHouse();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;
	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;

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
	CSkinMesh*					m_pHouseMesh;			// 部屋メッシュ.
	CCollisionMesh*				m_pCollisionMesh;		// 当たり判定用メッシュ.
	std::unique_ptr<CStageIcon>	m_pIcon;				// アイコン.
	float						m_Alpha;				// アルファ値.

private:
	// 外部データの定数.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};
