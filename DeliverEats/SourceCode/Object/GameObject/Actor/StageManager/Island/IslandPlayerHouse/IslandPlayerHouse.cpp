#include "IslandPlayerHouse.h"
#include "..\..\StageIcon\StageIcon.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr char ICON_POS_BONE_NAME[] = "player_house_bone2";	// アイコンの位置のボーン名.
}

CIslandPlayerHouse::CIslandPlayerHouse()
	: m_pHouseMesh			( nullptr )
	, m_pCollisionMesh		( nullptr )
	, m_pIcon				( nullptr )
	, m_Alpha				( Color::ALPHA_MAX )
{
}

CIslandPlayerHouse::~CIslandPlayerHouse()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CIslandPlayerHouse::Init()
{
	// 家のメッシュの取得.
	m_pHouseMesh = CMeshResource::GetSkin( "island_p_house_s" );
	m_pHouseMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pHouseMesh->SetTransform( m_Transform );

	// アイコンの取得.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pHouseMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::PlayerIsland );

	// タグの設定.
	m_ObjectTag = EObjectTag::IslandObject;

	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CIslandPlayerHouse::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// 距離によるアルファフェードの更新.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector = m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance = D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.0f;

	// カメラによるアルファ処理.
	CameraAlphaUpdate(); 
}

//---------------------------.
// 描画.
//---------------------------.
void CIslandPlayerHouse::Render()
{
	if ( m_IsDisp == false ) return;

	// 家の描画.
	m_pHouseMesh->SetDither( true );
	m_pHouseMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pHouseMesh->Render( &m_Transform );
	m_pHouseMesh->SetDither( false );

	CCollisionRender::PushMesh( m_pCollisionMesh, &m_Transform );
}

//---------------------------.
// スプライト(3D)の描画.
//---------------------------.
void CIslandPlayerHouse::Sprite3DRender_A()
{
	if ( CTime::GetIsGameEnd() == false ) return;
	m_pIcon->Sprite3DRender_A();
}

//---------------------------.
// 当たり判定を追加する.
//---------------------------.
void CIslandPlayerHouse::PushCollisionManager()
{
	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CIslandPlayerHouse::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "playerhouse_c" );
	m_pCollisions	 = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CIslandPlayerHouse::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// カメラによるアルファ値の更新.
//---------------------------.
void CIslandPlayerHouse::CameraAlphaUpdate()
{
	if ( m_IsDisp == false ) return;

	// カメラを隠しているか.
	bool IsHit = Coll::IsRayToMesh(
		CCameraManager::GetRay(),
		m_pCollisions->GetCollision<CMesh>() );

	// 透明にしていく.
	if ( IsHit ) {
		if ( m_Alpha <= CONSTANT.CAMERA_ALPHA_MIN ) return;
		m_Alpha -= CONSTANT.CAMERA_ALPHA_SPEED;
		if ( m_Alpha <= CONSTANT.CAMERA_ALPHA_MIN )
			m_Alpha = CONSTANT.CAMERA_ALPHA_MIN;
	}
	// 不透明にしていく.
	else {
		if ( m_Alpha >= CONSTANT.CAMERA_ALPHA_MAX ) return;
		m_Alpha += CONSTANT.CAMERA_ALPHA_SPEED;
		if ( m_Alpha >= CONSTANT.CAMERA_ALPHA_MAX )
			m_Alpha = CONSTANT.CAMERA_ALPHA_MAX;
	}
}