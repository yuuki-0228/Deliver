#include "Signboard.h"
#include "..\..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\..\..\Utility\Random\Random.h"

CSignboard::CSignboard()
	: m_pSignboardMesh	( nullptr )
	, m_pCollisionMesh	( nullptr )
	, m_PointLight		()
	, m_Alpha			( Color::ALPHA_MAX )
	, m_LightAng		( INIT_FLOAT )
	, m_LightFlashing	( INIT_FLOAT )
	, m_ShopNo			( 1 )
{
}

CSignboard::~CSignboard()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CSignboard::Init()
{
	// 看板のメッシュの取得.
	m_pSignboardMesh		= CMeshResource::GetSkin( "island_signboard_s" );
	m_Transform.Rotation	= { 0.0f, Math::ToRadian( -90.0f ), 0.0f };
	m_pSignboardMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pSignboardMesh->SetTransform( m_Transform );

	// タグの設定.
	m_ObjectTag = EObjectTag::IslandObject;

	// ポイントライトの設定.
	m_PointLight.Brightness = 1.0f;
	m_LightFlashing			= CRandom::GetRand( 0.2f, 0.3f );

	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CSignboard::Update( const float& DeltaTime )
{
	// カメラによるアルファ処理.
	CameraAlphaUpdate();

	// アニメーションの角度を加算.
	m_LightAng += 20.0f * DeltaTime;
	if ( m_LightAng >= Math::DEGREE_MID ) m_LightAng -= Math::DEGREE_MID;

	// ライトの明るさを変更する.
	m_PointLight.Brightness = 1.0f - sinf( Math::ToRadian( m_LightAng ) ) * m_LightFlashing;
}

//---------------------------.
// 描画.
//---------------------------.
void CSignboard::Render()
{
	// 看板の描画.
	m_pSignboardMesh->SetDither( true );
	m_pSignboardMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pSignboardMesh->ChangeTexture( 0, m_ShopNo - 3 );
	m_pSignboardMesh->SetMeshPointLight( m_PointLight );
	m_pSignboardMesh->Render( &m_Transform );
	m_pSignboardMesh->SetDither( false );

	CCollisionRender::PushMesh( m_pCollisionMesh, &m_Transform );
}

//---------------------------.
// 当たり判定を追加する.
//---------------------------.
void CSignboard::PushCollisionManager()
{
	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CSignboard::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "signboard_c" );
	m_pCollisions	 = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CSignboard::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// カメラによるアルファ値の更新.
//---------------------------.
void CSignboard::CameraAlphaUpdate()
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