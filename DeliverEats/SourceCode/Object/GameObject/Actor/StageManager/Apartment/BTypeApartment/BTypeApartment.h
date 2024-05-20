#pragma once
#include "..\..\..\Actor.h"

class CBTypeApartmentFloor;

/************************************************
*	マンションBパターン(8x2)クラス.
*		﨑田友輝.
**/
class CBTypeApartment final
	: public CActor
{
public:
	CBTypeApartment();
	virtual ~CBTypeApartment();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 当たり判定終了後呼び出される更新.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;
	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;

	// ステージエディタのImGuiの描画.
	void StageEditorImGuiRender();
	// フロアの情報をImGuiで描画.
	void FloorStateImGuiRender();

	// マンションのデータの取得.
	json GetApartmentData();

	// マンションの作成.
	void Create( json j );

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
	std::vector<std::unique_ptr<CBTypeApartmentFloor>>	m_pEmptyFloor;			// 空の1フロア.
	std::vector<std::unique_ptr<CBTypeApartmentFloor>>	m_pFloor;				// 1フロア.
	CSkinMesh*											m_pRooftop;				// 屋上.
	CCollisionMesh*										m_pCollisionMesh;		// 当たり判定用メッシュ.
	STransform											m_CollisionTransform;	// 当たり判定の配置する座標.
	D3DXPOSITION3										m_CreatePosition;		// フロアを作っていく座標.
	float												m_Alpha;				// アルファ値.

private:
	// 外部データの定数.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
