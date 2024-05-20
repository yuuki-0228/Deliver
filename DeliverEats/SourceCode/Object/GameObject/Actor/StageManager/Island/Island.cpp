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
	// ������z�u����ꏊ�̃{�[����.
	constexpr char N_BONE_NAME[] = "island_n";
	constexpr char W_BONE_NAME[] = "island_w";

	// �v���C���[�̊J�n�ʒu�̃{�[����.
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
// ������.
//---------------------------.
bool CIsland::Init()
{
	InitCollision();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CIsland::Update( const float& DeltaTime )
{
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->Update( DeltaTime );
	else if (	m_pHouse		!= nullptr ) m_pHouse->Update( DeltaTime );
	else if (	m_pShop			!= nullptr ) m_pShop->Update( DeltaTime );

	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector = m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance = D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.0f;

	CameraAlphaUpdate(); // �J�����ɂ��A���t�@����.

	// �~�j�}�b�v�ɃA�C�R����ǉ�����.
	CMiniMapUI::PushIslandIcon( this );

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->PushCollisionManager();
	else if (	m_pHouse		!= nullptr ) m_pHouse->PushCollisionManager();
	else if (	m_pShop			!= nullptr ) m_pShop->PushCollisionManager();
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CIsland::LateUpdate( const float& DeltaTime )
{
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->LateUpdate( DeltaTime );
	else if (	m_pHouse		!= nullptr ) m_pHouse->LateUpdate( DeltaTime );
	else if (	m_pShop			!= nullptr ) m_pShop->LateUpdate( DeltaTime );
}

//---------------------------.
// �`��.
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
// �X�v���C�g(3D)�̕`��.
//---------------------------.
void CIsland::Sprite3DRender_A()
{
	if (		m_pPlayerHouse	!= nullptr ) m_pPlayerHouse->Sprite3DRender_A();
	else if (	m_pHouse		!= nullptr ) m_pHouse->Sprite3DRender_A();
	else if (	m_pShop			!= nullptr ) m_pShop->Sprite3DRender_A();
}

//---------------------------.
// �X�e�[�W�G�f�B�^��ImGui�̕`��.
//---------------------------.
void CIsland::StageEditorImGuiRender()
{
	// �����̈ʒu�̐ݒ�.
	const D3DXVECTOR3&	OldIslandPos = m_Transform.Position;
	D3DXVECTOR3			IslandPos	 = OldIslandPos;
	CImGuiManager::InputVec3( u8"�����̔z�u���W", &IslandPos );
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

	// �������Ă邩�̐ݒ�.
	std::string OldWhat = "";
	if (		m_pPlayerHouse	!= nullptr ) OldWhat = "PlayerHouse";
	else if (	m_pHouse		!= nullptr ) OldWhat = "House";
	else									 OldWhat = "Shop_" + std::to_string( m_pShop->GetShopNo() );
	std::string What	= OldWhat;
	CImGuiManager::Combo( "IslandWhat##NoName", &What, { "PlayerHouse", "House", "Shop_3", "Shop_4" });

	// ���Ă镨���ύX���ꂽ.
	if ( OldWhat == What ) return;
	m_pIslandMash->SetTransform( m_Transform );
	if ( What == "PlayerHouse" ) {
		// �v���C���[�̉Ƃ̍쐬.
		m_pShop			= nullptr;
		m_pHouse		= nullptr;
		m_pPlayerHouse	= std::make_unique<CIslandPlayerHouse>();
		m_pPlayerHouse->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pPlayerHouse->Init();
	}
	else if ( What == "House" ) {
		// �Ƃ̍쐬.
		m_pShop			= nullptr;
		m_pPlayerHouse	= nullptr;
		m_pHouse		= std::make_unique<CIslandHouse>();
		m_pHouse->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pHouse->Init();
		m_pHouse->SetGaragePosition( m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
	}
	else {
		const int No = std::stoi( What.substr( What.length() - 1 ) );

		// ���X�̍쐬.
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
// �����̏���ImGui�ŕ`��.
//---------------------------.
void CIsland::IslandStateImGuiRender()
{
	if (		m_pPlayerHouse	!= nullptr ) ImGui::Text( "What            : PlayerHouse" );
	if (		m_pHouse		!= nullptr ) ImGui::Text( "What            : House" );
	else if (	m_pShop			!= nullptr ) ImGui::Text( "What            : Shop" );
	ImGui::Separator();
}

//---------------------------.
// �����̃f�[�^�̎擾
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
// �����̍쐬.
//---------------------------.
void CIsland::Create( json j )
{
	// ���b�V���̎擾.
	m_pIslandMash = CMeshResource::GetSkin( "floating_island_s" );
	m_pIslandMash->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	// ���W�̐ݒ�.
	m_Transform.Position.x = j["Position"]["x"];
	m_Transform.Position.y = j["Position"]["y"];
	m_Transform.Position.z = j["Position"]["z"];
	m_pIslandMash->SetTransform( m_Transform );

	// �^�O�̐ݒ�.
	m_ObjectTag = EObjectTag::Island;

	// �Ƃ����X���쐬.
	const std::string What = j["What"];
	if ( !What.find( "Shop" ) ) {
		const int No = std::stoi( What.substr( What.length() - 1 ) );

		// ���X�̍쐬.
		m_pShop = std::make_unique<CIslandShop>();
		m_pShop->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pShop->SetShopNo( No );
		m_pShop->Init();
		m_pShop->SetSignboardPosition( m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
	}
	else if ( What == "PlayerHouse" ) {
		// �v���C���[�̉Ƃ̍쐬.
		m_ObjectTag		= EObjectTag::PlayerIsland;
		m_pPlayerHouse	= std::make_unique<CIslandPlayerHouse>();
		m_pPlayerHouse->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pPlayerHouse->Init();
	}
	else {
		// �Ƃ̍쐬.
		m_pHouse = std::make_unique<CIslandHouse>();
		m_pHouse->SetPosition( m_pIslandMash->GetPosFromBone( W_BONE_NAME ) );
		m_pHouse->Init();
		m_pHouse->SetGaragePosition( m_pIslandMash->GetPosFromBone( N_BONE_NAME ) );
	}
}

//---------------------------.
// �v���C���[���J�n�ʒu�ɐݒ�.
//---------------------------.
bool CIsland::SetPlayerStartPos( CPlayer* pPlayer )
{
	if ( m_pPlayerHouse == nullptr ) return false;
	m_pIslandMash->SetTransform( m_Transform );
	pPlayer->SetStartPosition( m_pIslandMash->GetPosFromBone( PLAYER_START_POSITION ) );
	return true;
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CIsland::InitCollision()
{
	// �����蔻����s�����̔���p�̋��̂̓����蔻��̏�����.
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( CONSTANT.COLL_STOP_SPHERE_RADIUS );

	// �����̓����蔻��.
	m_pCollisionMesh = CMeshResource::GetCollision( "island_c" );
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CIsland::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetTransform( m_Transform );

	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// �J�����ɂ��A���t�@�l�̍X�V.
//---------------------------.
void CIsland::CameraAlphaUpdate()
{
	if ( m_IsDisp == false ) return;

	// �J�������B���Ă��邩.
	bool IsHit = Coll::IsRayToMesh(
		CCameraManager::GetRay(),
		m_pCollisions->GetCollision<CMesh>() );

	// �����ɂ��Ă���.
	if ( IsHit ) {
		if ( m_Alpha <= CONSTANT.CAMERA_ALPHA_MIN ) return;
		m_Alpha -= CONSTANT.CAMERA_ALPHA_SPEED;
		if ( m_Alpha <= CONSTANT.CAMERA_ALPHA_MIN )
			m_Alpha = CONSTANT.CAMERA_ALPHA_MIN;
	}
	// �s�����ɂ��Ă���.
	else {
		if ( m_Alpha >= CONSTANT.CAMERA_ALPHA_MAX ) return;
		m_Alpha += CONSTANT.CAMERA_ALPHA_SPEED;
		if ( m_Alpha >= CONSTANT.CAMERA_ALPHA_MAX )
			m_Alpha = CONSTANT.CAMERA_ALPHA_MAX;
	}
}
