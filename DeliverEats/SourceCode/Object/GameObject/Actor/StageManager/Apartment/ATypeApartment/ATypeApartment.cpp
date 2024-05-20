#include "ATypeApartment.h"
#include "..\ApartmentFloor\ATypeApartmentFloor\ATypeApartmentFloor.h"
#include "..\..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"

#include "..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr char ROOFTOP_UP_BONE_NAME[] = "rooftop_up";	// 屋上の一番上のボーン名.
}

CATypeApartment::CATypeApartment()
	: m_pEmptyFloor			()
	, m_pFloor				()
	, m_pRooftop			( nullptr )
	, m_pCollisionMesh		( nullptr )
	, m_CollisionTransform	()
	, m_CreatePosition		( INIT_FLOAT3 )
	, m_Alpha				( Color::ALPHA_MAX )
{
}

CATypeApartment::~CATypeApartment()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CATypeApartment::Init()
{
	// 屋上の初期化.
	m_pRooftop = CMeshResource::GetSkin( "rooftop_3x3_s" );
	m_pRooftop->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_Transform.Position = m_pFloor.back()->GetSettingPosition();
	m_pRooftop->SetPosition( m_Transform.Position );
	m_CollisionTransform			= m_Transform;
	m_CollisionTransform.Position	= m_pRooftop->GetPosFromBone( ROOFTOP_UP_BONE_NAME );

	// タグの設定.
	m_ObjectTag = EObjectTag::ApartmentA;

	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CATypeApartment::Update( const float& DeltaTime )
{
	for ( auto& f : m_pEmptyFloor	) f->Update( DeltaTime );
	for ( auto& f : m_pFloor		) f->Update( DeltaTime );

	// 距離によるアルファフェードの更新.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE + CONSTANT.FLOOR_SIZE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 3.0f;

	// カメラによるアルファ値の更新.
	CameraAlphaUpdate();

	// ミニマップにアイコンを追加する.
	CMiniMapUI::PushApartmentAIcon( this );

	UpdateCollision();
	CActorCollisionManager::PushObject( this );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CATypeApartment::LateUpdate( const float& DeltaTime )
{
	for ( auto& f : m_pFloor ) f->LateUpdate( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CATypeApartment::Render()
{
	for ( auto& f : m_pEmptyFloor ) {
		f->SetAlpha( m_Alpha );
		f->Render();
	}
	for ( auto& f : m_pFloor ) {
		f->SetAlpha( m_Alpha );
		f->Render();
	}

	// 屋上の描画.
	if ( m_IsDisp == false ) return;
	m_pRooftop->SetDither( true );
	m_pRooftop->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pRooftop->Render( &m_Transform );
	m_pRooftop->SetDither( false );

	CCollisionRender::PushMesh( m_pCollisionMesh, &m_CollisionTransform );
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CATypeApartment::Sprite3DRender_A()
{
	for ( auto& f : m_pFloor ) f->Sprite3DRender_A();
}

//---------------------------.
// ステージエディタのImGuiの描画.
//---------------------------.
void CATypeApartment::StageEditorImGuiRender()
{
	// マンションの位置の設定.
	const D3DXVECTOR3&	OldApartmentPos = m_pEmptyFloor[0]->GetPosition();
	D3DXVECTOR3			ApartmentPos	= OldApartmentPos;
	CImGuiManager::InputVec3( u8"マンションの配置座標", &ApartmentPos );
	if ( ApartmentPos.y != OldApartmentPos.y ) ApartmentPos.y = OldApartmentPos.y;
	if ( ApartmentPos	!= OldApartmentPos ) {
		m_CreatePosition = ApartmentPos;

		// 部屋の無いフロアの移動.
		for ( auto& f : m_pEmptyFloor ) {
			f->FloorMove( m_CreatePosition );
			m_CreatePosition = f->GetSettingPosition();
		}

		// フロアの移動.
		for ( auto& f : m_pFloor ) {
			f->FloorMove( m_CreatePosition );
			m_CreatePosition = f->GetSettingPosition();
		}
		m_Transform.Position = m_CreatePosition;
	}

	// 階層の設定.
	const int	OldFloarSize	= static_cast<int>( m_pFloor.size() );
	int			FloarSize		= OldFloarSize;
	ImGui::SetNextTreeNodeOpen( true, ImGuiCond_Once );
	if ( ImGui::TreeNode( u8"マンションの階層" ) ) {
		ImGui::InputInt( "##FloorSize", &FloarSize );
		ImGui::TreePop();
	}
	if ( FloarSize <= 0 ) FloarSize = 1;
	ImGui::Separator();

	// 階層が増えた.
	if ( FloarSize > OldFloarSize ) {
		// フロアの追加.
		const int Num = FloarSize - OldFloarSize;
		for ( int i = 0; i < Num; ++i ) {
			m_pFloor.emplace_back( std::make_unique<CATypeApartmentFloor>() );
			m_pFloor.back()->SetPosition( m_CreatePosition );

			// フロアの作成.
			m_pFloor.back()->SetEmptyFloor();
			m_pFloor.back()->Create( nullptr );
			m_pFloor.back()->Init();
			m_CreatePosition = m_pFloor.back()->GetSettingPosition();
		}
		m_Transform.Position = m_CreatePosition;
	}

	// 階層が減った.
	if ( FloarSize < OldFloarSize ) {
		// フロアの削除.
		const int Num = OldFloarSize - FloarSize;
		for ( int i = 0; i < Num; ++i ) m_pFloor.pop_back();
		m_CreatePosition	 = m_pFloor.back()->GetSettingPosition();
		m_Transform.Position = m_CreatePosition;
	}

	// フロアの設定.
	const int FloorSize = static_cast<int>( m_pFloor.size() );
	for ( int i = 0; i < FloorSize; ++i ) {
		if ( CImGuiManager::Button( std::string( "x##" + std::to_string( i ) ).c_str(), FloorSize > 1, Color4::Red ) ) {
			m_pFloor.erase( m_pFloor.begin() + i );
			
			// フロアの位置の更新.
			m_CreatePosition = m_pEmptyFloor.back()->GetSettingPosition();
			for ( auto& f : m_pFloor ) {
				f->FloorMove( m_CreatePosition );
				m_CreatePosition = f->GetSettingPosition();
			}
			m_Transform.Position = m_CreatePosition;
			break;
		}
		ImGui::SameLine();
		if ( ImGui::TreeNode( std::string( "Floor_" + std::to_string( i ) ).c_str() ) ) {
			ImGui::Separator();
			m_pFloor[i]->StageEditorImGuiRender();
			ImGui::TreePop();
		}
		ImGui::Separator();
	}
}

//---------------------------.
// フロアの情報をImGuiで描画.
//---------------------------.
void CATypeApartment::FloorStateImGuiRender()
{
	ImGui::Text( u8"階層            : %dF", m_pFloor.size() );
	ImGui::Separator();
	int ShopNum = 0, RoomNum = 0, EmptyRoomNum = 0;
	for ( auto& f : m_pFloor ) {
		std::pair<int, std::pair<int, int>> Num = f->GetRoomNum();
		ShopNum			+= Num.first;
		RoomNum			+= Num.second.first;
		EmptyRoomNum	+= Num.second.second;
	}
	ImGui::Text( u8"Shop            : %d個", ShopNum );
	ImGui::Text( u8"Room            : %d個", RoomNum );
	ImGui::Text( u8"EmptyRoom       : %d個", EmptyRoomNum );
	ImGui::Separator();
}

//---------------------------.
// マンションのデータの取得.
//---------------------------.
json CATypeApartment::GetApartmentData()
{
	json Out;
	const D3DXPOSITION3& Pos = m_pEmptyFloor[0]->GetPosition();
	Out["Position"]["x"] = Pos.x;
	Out["Position"]["z"] = Pos.z;
	for ( auto& f : m_pFloor ) Out["Floar"].emplace_back( f->GetFloorData() );
	return Out;
}

//---------------------------.
// マンションの作成.
//---------------------------.
void CATypeApartment::Create( json j )
{
	// マンションの作成する場所の取得.
	m_CreatePosition.x = j["Position"]["x"];
	m_CreatePosition.y = CConstant::GetStage().CLOUD_DOWN_POS_Y;
	m_CreatePosition.z = j["Position"]["z"];

	// 部屋の無いフロアを積み上げる.
	m_pEmptyFloor.resize( CONSTANT.START_NOROOM_FLOOR_NUM );
	for ( int i = 0; i < CONSTANT.START_NOROOM_FLOOR_NUM; ++i ) {
		m_pEmptyFloor[i] = std::make_unique<CATypeApartmentFloor>();
		m_pEmptyFloor[i]->SetPosition( m_CreatePosition );

		// フロアの作成.
		m_pEmptyFloor[i]->SetEmptyFloor();
		m_pEmptyFloor[i]->Create( nullptr );
		m_pEmptyFloor[i]->Init();
		m_CreatePosition = m_pEmptyFloor[i]->GetSettingPosition();
	}

	// フロアを積み上げていく.
	const int FloarSize = static_cast<int>( j["Floar"].size() );
	m_pFloor.resize( FloarSize );
	for ( int i = 0; i < FloarSize; ++i ) {
		m_pFloor[i] = std::make_unique<CATypeApartmentFloor>();
		m_pFloor[i]->SetPosition( m_CreatePosition );

		// フロアの作成.
		if ( j["Floar"][i] == nullptr ) m_pFloor[i]->SetEmptyFloor();
		m_pFloor[i]->Create( j["Floar"][i] );
		m_pFloor[i]->Init();
		m_CreatePosition = m_pFloor[i]->GetSettingPosition();
	}
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CATypeApartment::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "apart_3x3_c" );

	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CATypeApartment::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_CollisionTransform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_CollisionTransform );
}

//---------------------------.
// カメラによるアルファ値の更新.
//---------------------------.
void CATypeApartment::CameraAlphaUpdate()
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