#pragma once
#include "..\GameObject.h"
#include "..\ActorCollisionManager\ActorCollisionManager.h"
#include "..\..\Collision\CollisionManager\CollisionManager.h"
#include "..\..\Collision\CollisionRender\CollisionRender.h"
#include "..\..\Collision\Collisions\CCollisions.h"
#include "..\..\Light\MeshPointLight\MeshPointLight.h"
#include "..\..\..\Common\Sprite\SpriteStruct.h"

class CSkinMesh;
class CStaticMesh;
class CCollisionMesh;
class CSprite;

namespace {
	// ウィンドウから出たことにするサイズ.
	constexpr float WND_OUT_SIZE = 1000.0f;
}

/************************************************
*	ゲーム内のオブジェクトのベースクラス.
*		﨑田友輝.
**/
class CActor
	: public CGameObject
{
public:
	CActor();
	virtual ~CActor();

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) {};

	// 当たり判定の取得.
	template<class T> T*	GetCollision() { return nullptr; }
	// 当たり判定の取得(球体).
	template<> CSphere*		GetCollision() { return m_pCollisions->GetCollision<CSphere>(); }
	// 当たり判定の取得(円柱).
	template<> CCylinder*	GetCollision() { return m_pCollisions->GetCollision<CCylinder>(); }
	// 当たり判定の取得(レイ).
	template<> CRay*		GetCollision() { return m_pCollisions->GetCollision<CRay>(); }
	// 当たり判定の取得(十字レイ).
	template<> CCrossRay*	GetCollision() { return m_pCollisions->GetCollision<CCrossRay>(); }
	// 当たり判定の取得(メッシュ).
	template<> CMesh*		GetCollision() { return m_pCollisions->GetCollision<CMesh>(); }

	// スクリーン座標を取得.
	D3DXPOSITION3 GetScreenPos();

	// 画面外にいるか.
	bool IsScreenOut( const float& Size = WND_OUT_SIZE );

	// 距離によるフェードの更新.
	void DistanceFadeUpdate( const float& StartDistance, const float& AlphaDistance );

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() {};
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() {};

protected:
	std::unique_ptr<CCollisions>	m_pCollisions;		// 複数の当たり判定を所持しているクラス.
	bool							m_IsDisp;			// 表示するか.
	float							m_Distance;			// 距離(1.0f ~ 0.0f).
};
