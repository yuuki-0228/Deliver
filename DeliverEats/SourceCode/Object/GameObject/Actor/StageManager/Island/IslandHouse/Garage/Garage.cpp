#include "Garage.h"
#include "..\..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"

CGarage::CGarage()
	: m_pGarageMesh		( nullptr )
	, m_pCollisionMesh	( nullptr )
	, m_Alpha			( Color::ALPHA_MAX )
{
}

CGarage::~CGarage()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CGarage::Init()
{
	// ガレージのメッシュの取得.
	m_pGarageMesh			= CMeshResource::GetSkin( "island_garage_s" );
	m_Transform.Rotation	= { 0.0f, Math::ToRadian( -90.0f ), 0.0f };
	m_pGarageMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pGarageMesh->SetTransform( m_Transform );

	// タグの設定.
	m_ObjectTag = EObjectTag::IslandObject;

	InitCollision();
    return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGarage::Update( const float& DeltaTime )
{
	// カメラによるアルファ処理.
	CameraAlphaUpdate();
}

//---------------------------.
// 描画.
//---------------------------.
void CGarage::Render()
{
	// ガレージの描画.
	m_pGarageMesh->SetDither( true );
	m_pGarageMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) ); 
	m_pGarageMesh->Render( &m_Transform );
	m_pGarageMesh->SetDither( false );

	CCollisionRender::PushMesh( m_pCollisionMesh, &m_Transform );
}

//---------------------------.
// 当たり判定を追加する.
//---------------------------.
void CGarage::PushCollisionManager()
{
	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CGarage::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "garage_c" );
	m_pCollisions	 = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CGarage::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// カメラによるアルファ値の更新.
//---------------------------.
void CGarage::CameraAlphaUpdate()
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
