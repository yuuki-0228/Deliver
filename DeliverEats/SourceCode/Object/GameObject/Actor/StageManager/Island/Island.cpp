#include "Island.h"
#include "IslandPlayerHouse\IslandPlayerHouse.h"
#include "IslandHouse\IslandHouse.h"
#include "IslandShop\IslandShop.h"
#include "..\StageManager.h"
#include "..\..\Character\Player\Player.h"
#include "..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"

namespace {
	// 建物を配置する場所のボーン名.
	constexpr char N_BONE_NAME[] = "island_n";
	constexpr char W_BONE_NAME[] = "island_w";

	// プレイヤーの開始位置のボーン名.
	constexpr char PLAYER_START_POSITION[] = "island_center";
}

CIsland::CIsland()
	: m_pPlayerHouse	( nullptr )
	, m_pHouse			( nullptr )
	, m_pShop			( nullptr )
	, m_pIslandMash		( nullptr )
	, m_pCollisionMesh	( nullptr )
	, m_Alpha			( Color::ALPHA_MAX )
{
}

CIsland::~CIsland()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CIsland::Init()
{
	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CIsland::Update( const float& DeltaTime )
{
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->Update( DeltaTime );
	else if (	m_pHouse		!= nullptr ) m_pHouse->Update( DeltaTime );
	else if (	m_pShop			!= nullptr ) m_pShop->Update( DeltaTime );

	// 距離によるアルファフェードの更新.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector = m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance = D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.0f;

	CameraAlphaUpdate(); // カメラによるアルファ処理.

	// ミニマップにアイコンを追加する.
	CMiniMapUI::PushIslandIcon( this );

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->PushCollisionManager();
	else if (	m_pHouse		!= nullptr ) m_pHouse->PushCollisionManager();
	else if (	m_pShop			!= nullptr ) m_pShop->PushCollisionManager();
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CIsland::LateUpdate( const float& DeltaTime )
{
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->LateUpdate( DeltaTime );
	else if (	m_pHouse		!= nullptr ) m_pHouse->LateUpdate( DeltaTime );
	else if (	m_pShop			!= nullptr ) m_pShop->LateUpdate( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CIsland::Render()
{
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->Render();
	else if (	m_pHouse		!= nullptr ) m_pHouse->Render();
	else if (	m_pShop			!= nullptr ) m_pShop->Render();

	if ( m_IsDisp == false ) return;
	m_pIslandMash->SetDither( true );
	m_pIslandMash->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pIslandMash->Render( &m_Transform );
	m_pIslandMash->SetDither( false );

	CCollisionRender::PushSphere( m_pCollisions->GetCollision<CSphere>() );
	CCollisionRender::PushMesh( m_pCollisionMesh, &m_Transform );
	if ( m_pPlayerHouse == nullptr ) return;
	CCollisionRender::PushPoint( m_pIslandMash->GetPosFromBone( PLAYER_START_POSITION ) );
}

//---------------------------.
// スプライト(3D)の描画.
//---------------------------.
void CIsland::Sprite3DRender_A()
{
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->Sprite3DRender_A();
	else if (	m_pHouse		!= nullptr ) m_pHouse->Sprite3DRender_A();
	else if (	m_pShop			!= nullptr ) m_pShop->Sprite3DRender_A();
}

//---------------------------.
// ステージエディタのImGuiの描画.
//---------------------------.
void CIsland::StageEditorImGuiRender()
{
	// 浮島の位置の設定.
	const D3DXVECTOR3&	OldIslandPos = m_Transform.Position;
	D3DXVECTOR3			IslandPos	 = OldIslandPos;
	CImGuiManager::InputVec3( u8"浮島の配置座標", &IslandPos );
	if ( IslandPos != OldIslandPos ) {
		m_Transform.Position = IslandPos;
		m_pIslandMash->SetTransform( m_Transform );
		if ( m_pPlayerHouse != nullptr ) {
			m_pPlayerHouse->SetPosition(	m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		}
		else if ( m_pHouse != nullptr ) {
			m_pHouse->SetPosition(			m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
			m_pHouse->SetGaragePosition(	m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
		}
		else if ( m_pShop != nullptr ) {
			m_pShop->SetPosition(			m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
			m_pShop->SetSignboardPosition(	m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
		}
	}

	// 何を建てるかの設定.
	std::string OldWhat = "";
	if (		m_pPlayerHouse	!= nullptr ) OldWhat = "PlayerHouse";
	else if (	m_pHouse		!= nullptr ) OldWhat = "House";
	else									 OldWhat = "Shop_" + std::to_string( m_pShop->GetShopNo() );
	std::string What	= OldWhat;
	CImGuiManager::Combo( "IslandWhat##NoName", &What, { "PlayerHouse", "House", "Shop_3", "Shop_4" });

	// 建てる物が変更された.
	if ( OldWhat == What ) return;
	m_pIslandMash->SetTransform( m_Transform );
	if ( What == "PlayerHouse" ) {
		// プレイヤーの家の作成.
		m_pShop			= nullptr;
		m_pHouse		= nullptr;
		m_pPlayerHouse	= std::make_unique<CIslandPlayerHouse>();
		m_pPlayerHouse->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pPlayerHouse->Init();
	}
	else if ( What == "House" ) {
		// 家の作成.
		m_pShop			= nullptr;
		m_pPlayerHouse	= nullptr;
		m_pHouse		= std::make_unique<CIslandHouse>();
		m_pHouse->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pHouse->Init();
		m_pHouse->SetGaragePosition( m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
	}
	else {
		const int No = std::stoi( What.substr( What.length() - 1 ) );

		// お店の作成.
		m_pHouse		= nullptr;
		m_pPlayerHouse	= nullptr;
		m_pShop			= std::make_unique<CIslandShop>();
		m_pShop->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pShop->SetShopNo( No );
		m_pShop->Init();
		m_pShop->SetSignboardPosition( m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
	}
}

//---------------------------.
// 浮島の情報をImGuiで描画.
//---------------------------.
void CIsland::IslandStateImGuiRender()
{
	if (		m_pPlayerHouse	!= nullptr ) ImGui::Text( "What            : PlayerHouse" );
	if (		m_pHouse		!= nullptr ) ImGui::Text( "What            : House" );
	else if (	m_pShop			!= nullptr ) ImGui::Text( "What            : Shop" );
	ImGui::Separator();
}

//---------------------------.
// 浮島のデータの取得
//---------------------------.
json CIsland::GetIslandData()
{
	json Out;
	Out["Position"]["x"] = m_Transform.Position.x;
	Out["Position"]["y"] = m_Transform.Position.y;
	Out["Position"]["z"] = m_Transform.Position.z;
	if (		m_pPlayerHouse	!= nullptr ) Out["What"] = "PlayerHouse";
	if (		m_pHouse		!= nullptr ) Out["What"] = "House";
	else if (	m_pShop			!= nullptr ) Out["What"] = "Shop_" + std::to_string( m_pShop->GetShopNo() );
	return Out;
}

//---------------------------.
// 浮島の作成.
//---------------------------.
void CIsland::Create( json j )
{
	// メッシュの取得.
	m_pIslandMash = CMeshResource::GetSkin( "floating_island_s" );
	m_pIslandMash->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	// 座標の設定.
	m_Transform.Position.x = j["Position"]["x"];
	m_Transform.Position.y = j["Position"]["y"];
	m_Transform.Position.z = j["Position"]["z"];
	m_pIslandMash->SetTransform( m_Transform );

	// タグの設定.
	m_ObjectTag = EObjectTag::Island;

	// 家かお店を作成.
	const std::string What = j["What"];
	if ( !What.find( "Shop" ) ) {
		const int No = std::stoi( What.substr( What.length() - 1 ) );

		// お店の作成.
		m_pShop = std::make_unique<CIslandShop>();
		m_pShop->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pShop->SetShopNo( No );
		m_pShop->Init();
		m_pShop->SetSignboardPosition( m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
	}
	else if ( What == "PlayerHouse" ) {
		// プレイヤーの家の作成.
		m_ObjectTag		= EObjectTag::PlayerIsland;
		m_pPlayerHouse	= std::make_unique<CIslandPlayerHouse>();
		m_pPlayerHouse->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pPlayerHouse->Init();
	}
	else {
		// 家の作成.
		m_pHouse = std::make_unique<CIslandHouse>();
		m_pHouse->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pHouse->Init();
		m_pHouse->SetGaragePosition( m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
	}
}

//---------------------------.
// プレイヤーを開始位置に設定.
//---------------------------.
bool CIsland::SetPlayerStartPos( CPlayer* pPlayer )
{
	if ( m_pPlayerHouse == nullptr ) return false;
	m_pIslandMash->SetTransform( m_Transform );
	pPlayer->SetStartPosition( m_pIslandMash->GetPosFromBone( PLAYER_START_POSITION ) );
	return true;
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CIsland::InitCollision()
{
	// 当たり判定を行うかの判定用の球体の当たり判定の初期化.
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( CONSTANT.COLL_STOP_SPHERE_RADIUS );

	// 浮島の当たり判定.
	m_pCollisionMesh = CMeshResource::GetCollision( "island_c" );
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CIsland::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetTransform( m_Transform );

	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// カメラによるアルファ値の更新.
//---------------------------.
void CIsland::CameraAlphaUpdate()
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
