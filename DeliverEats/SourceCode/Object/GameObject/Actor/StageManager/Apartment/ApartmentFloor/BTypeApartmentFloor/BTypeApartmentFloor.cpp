#include "BTypeApartmentFloor.h"
#include "..\ApartmentEmptyRoom\ApartmentEmptyRoom.h"
#include "..\ApartmentRoom\ApartmentRoom.h"
#include "..\ApartmentShop\ApartmentShop.h"
#include "..\..\..\..\..\ActorCollisionManager\ActorCollisionManager.h"
#include "..\..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\..\Utility\Math\Math.h"
#include "..\..\..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr float	SCREENOUT_SIZE		= 50000.0f;						// 画面外の判定のサイズ.
	constexpr int	DIRE_NUM			= 2;							// 部屋を表示する方向の数.
	constexpr int	ONE_DIRE_ROOM_NUM	= 8;							// 1方向の部屋の数.
	constexpr int	ROOM_NUM			= DIRE_NUM * ONE_DIRE_ROOM_NUM;	// 部屋の数.

	constexpr char	ROOM_SET_POS_BONE_NAME[]	= "a_1f_";				// 部屋を配置するボーン名("_"の後ろに1~16の数字を組み合わせる).
	constexpr char	NEXT_FLOOR_POS_BONE_NAME[]	= "apart_8_room_up";	// 次の部屋の乗せるのボーン名.
}

CBTypeApartmentFloor::CBTypeApartmentFloor()
	: m_pApartmentMesh		( nullptr )
	, m_PointLight			()
	, m_pEmptyRoomList		()
	, m_pRoomList			()
	, m_pShopList			()
	, m_Alpha				( Color::ALPHA_MAX )
	, m_LightHour			( INIT_INT )
	, m_LightMinute			( INIT_INT )
	, m_IsEmptyFloor		( false )
	, m_IsLightOn			( false )
{
}

CBTypeApartmentFloor::~CBTypeApartmentFloor()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CBTypeApartmentFloor::Init()
{
	// ライトをつける時間の設定.
	m_LightHour		= 16;
	m_LightMinute	= CRandom::GetRand( 3, 4 );
	m_PointLight.Brightness = INIT_FLOAT;

	// タグの設定.
	m_ObjectTag = EObjectTag::ApartmentB;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CBTypeApartmentFloor::Update( const float& DeltaTime )
{
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE + CONSTANT.FLOOR_SIZE, CONSTANT.ALPHA_DISTANCE );	// 距離によるアルファフェードの更新.

	LightUpdate();	// ライトの更新.

	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 3.0f;

	if ( m_IsEmptyFloor ) return;
	for ( auto& [Room, No] : m_pEmptyRoomList	) Room->Update( DeltaTime );
	for ( auto& [Room, No] : m_pRoomList		) Room->Update( DeltaTime );
	for ( auto& [Shop, No] : m_pShopList		) Shop->Update( DeltaTime );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CBTypeApartmentFloor::LateUpdate( const float& DeltaTime )
{
	if ( m_IsEmptyFloor ) return;
	for ( auto& [Room, No] : m_pRoomList ) Room->LateUpdate( DeltaTime );
	for ( auto& [Shop, No] : m_pShopList ) Shop->LateUpdate( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CBTypeApartmentFloor::Render()
{
	if ( m_IsDisp ) {
		m_pApartmentMesh->SetDither( true );
		m_pApartmentMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
		m_pApartmentMesh->SetMeshPointLight( m_PointLight );
		m_pApartmentMesh->Render( &m_Transform );
		m_pApartmentMesh->SetDither( false );
	}
	if ( m_IsEmptyFloor ) return;
	for ( auto& [Room, No] : m_pEmptyRoomList	) Room->Render();
	for ( auto& [Room, No] : m_pRoomList		) Room->Render();
	for ( auto& [Shop, No] : m_pShopList		) Shop->Render();
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CBTypeApartmentFloor::Sprite3DRender_A()
{
	if ( m_IsEmptyFloor ) return;
	for ( auto& [Room, No] : m_pRoomList ) Room->Sprite3DRender_A();
	for ( auto& [Shop, No] : m_pShopList ) Shop->Sprite3DRender_A();
}

//---------------------------.
// ステージエディタのImGuiの描画.
//---------------------------.
void CBTypeApartmentFloor::StageEditorImGuiRender()
{
	const bool OldIsEmptyFloor = m_IsEmptyFloor;
	CImGuiManager::CheckBox( u8"部屋を作成しないフロアするか", &m_IsEmptyFloor );
	ImGui::Separator();

	// 部屋を作成しないフロアか変更された.
	if ( m_IsEmptyFloor != OldIsEmptyFloor ) {
		// メッシュの読み込み.
		if ( m_IsEmptyFloor )	m_pApartmentMesh = CMeshResource::GetSkin( "apart_8x2_plus_room_s" );
		else					m_pApartmentMesh = CMeshResource::GetSkin( "apart_8x2_1f_s" );
		m_pApartmentMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
		m_pApartmentMesh->SetTransform( m_Transform );

		// 部屋を全て削除.
		if ( m_IsEmptyFloor ) return;
		m_pEmptyRoomList.clear();
		m_pRoomList.clear();
		m_pShopList.clear();

		// 空部屋の作成.
		for ( int i = 0; i < ROOM_NUM; ++i ) {
			const std::string&	 BoneName	= std::string( ROOM_SET_POS_BONE_NAME + std::to_string( i + 1 ) );
			const D3DXPOSITION3& Pos		= m_pApartmentMesh->GetPosFromBone( BoneName.c_str() );
			const D3DXROTATION3& Rot		= { 0.0f, Math::ToRadian( 180.0f * ( i / ONE_DIRE_ROOM_NUM ) ), 0.0f };

			m_pEmptyRoomList.emplace_back( std::make_pair( std::make_unique<CApartmentEmptyRoom>(), i ) );
			m_pEmptyRoomList.back().first->SetPosition( Pos );
			m_pEmptyRoomList.back().first->SetRotation( Rot );
			m_pEmptyRoomList.back().first->Init();
		}
	}
	if ( m_IsEmptyFloor ) return;

	// 現在のフロアの部屋の並びの取得.
	std::vector<std::pair<std::string, int>> OldFloorList;
	OldFloorList.resize( ROOM_NUM );
	const int EmptySize		= static_cast<int>( m_pEmptyRoomList.size() );
	for ( int i = 0; i < EmptySize; ++i ) {
		const int No		= m_pEmptyRoomList[i].second;
		OldFloorList[No]	= std::make_pair( "EmptyRoom", i );
	}
	const int RoomSize		= static_cast<int>( m_pRoomList.size() );
	for ( int i = 0; i < RoomSize; ++i ) {
		const int No		= m_pRoomList[i].second;
		OldFloorList[No]	= std::make_pair( "Room", i );
	}
	const int ShopSize		= static_cast<int>( m_pShopList.size() );
	for ( int i = 0; i < ShopSize; ++i ) {
		const std::string&	Name = "Shop_" + std::to_string( m_pShopList[i].first->GetShopNo() );
		const int			No	 = m_pShopList[i].second;
		OldFloorList[No]		 = std::make_pair( Name, i );
	}

	// 部屋の設定.
	std::vector<std::pair<std::string, int>> FloorList = OldFloorList;
	for ( int i = 0; i < ROOM_NUM; ++i ) {
		CImGuiManager::Combo( std::string( "Room" + std::to_string( i ) + "##NoName" ).c_str(), &FloorList[i].first, { "EmptyRoom", "Room", "Shop_1", "Shop_2", "Shop_3" } );
		ImGui::SameLine();
		ImGui::Text( "%d", i );
	}

	// 部屋が変更された.
	for ( int i = 0; i < ROOM_NUM; ++i ) {
		if ( OldFloorList[i] == FloorList[i] ) continue;

		m_pApartmentMesh->SetTransform( m_Transform );
		const std::string&	 BoneName	= std::string( ROOM_SET_POS_BONE_NAME + std::to_string( i + 1 ) );
		const D3DXPOSITION3& Pos		= m_pApartmentMesh->GetPosFromBone( BoneName.c_str() );
		const D3DXROTATION3& Rot		= { 0.0f, Math::ToRadian( 180.0f * ( i / ONE_DIRE_ROOM_NUM ) ), 0.0f };

		// 前の部屋を削除.
		if (	 !OldFloorList[i].first.find( "Shop" )	) m_pShopList.erase(	  m_pShopList.begin()		+ OldFloorList[i].second );
		else if ( OldFloorList[i].first == "Room"		) m_pRoomList.erase(	  m_pRoomList.begin()		+ OldFloorList[i].second );
		else											  m_pEmptyRoomList.erase( m_pEmptyRoomList.begin()	+ OldFloorList[i].second );

		// 部屋を作成する.
		if ( !FloorList[i].first.find( "Shop" ) ) {
			const int No = std::stoi( FloorList[i].first.substr( FloorList[i].first.length() - 1 ) );

			// お店の作成.
			m_pShopList.emplace_back( std::make_pair( std::make_unique<CApartmentShop>(), i ) );
			m_pShopList.back().first->SetPosition( Pos );
			m_pShopList.back().first->SetRotation( Rot );
			m_pShopList.back().first->SetShopNo( No );
			m_pShopList.back().first->Init();
		}
		else if ( FloorList[i].first == "Room" ) {
			// 部屋の作成.
			m_pRoomList.emplace_back( std::make_pair( std::make_unique<CApartmentRoom>(), i ) );
			m_pRoomList.back().first->SetPosition( Pos );
			m_pRoomList.back().first->SetRotation( Rot );
			m_pRoomList.back().first->Init();
		}
		else {
			// 空部屋の作成.
			m_pEmptyRoomList.emplace_back( std::make_pair( std::make_unique<CApartmentEmptyRoom>(), i ) );
			m_pEmptyRoomList.back().first->SetPosition( Pos );
			m_pEmptyRoomList.back().first->SetRotation( Rot );
			m_pEmptyRoomList.back().first->Init();
		}
	}
}

//---------------------------.
// フロアの作成.
//---------------------------.
void CBTypeApartmentFloor::Create( json j )
{
#ifdef _DEBUG
	// メッシュの読み込み.
	if ( m_IsEmptyFloor )	m_pApartmentMesh = CMeshResource::GetSkin( "apart_8x2_plus_room_s" );
	else					m_pApartmentMesh = CMeshResource::GetSkin( "apart_8x2_1f_s" );
#else 
	// 全てを空部屋に変更する.
	m_pApartmentMesh = CMeshResource::GetSkin( "apart_8x2_1f_s" );
	if ( m_IsEmptyFloor ) {
	for ( int i = 0; i < ROOM_NUM; ++i ) j[std::to_string( i )] = nullptr;
	m_IsEmptyFloor = false;
}
#endif

	// メッシュの設定.
	m_pApartmentMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pApartmentMesh->SetTransform( m_Transform );
	if ( m_IsEmptyFloor ) return;

	// 各部屋の設定.
	for ( int i = 0; i < ROOM_NUM; ++i ) {
		const std::string&	 BoneName	= std::string( ROOM_SET_POS_BONE_NAME + std::to_string( i + 1 ) );
		const D3DXPOSITION3& Pos		= m_pApartmentMesh->GetPosFromBone( BoneName.c_str() );
		const D3DXROTATION3& Rot		= { 0.0f, Math::ToRadian( 180.0f * ( i / ONE_DIRE_ROOM_NUM ) ), 0.0f };

		// お店か部屋を作成する.
		const std::string& What = j[std::to_string( i )] == nullptr ? "EmptyRoom" : j[std::to_string( i )];
		if ( !What.find( "Shop" ) ) {
			const int No = std::stoi( What.substr( What.length() - 1 ) );

			// お店の作成.
			m_pShopList.emplace_back( std::make_pair( std::make_unique<CApartmentShop>(), i ) );
			m_pShopList.back().first->SetPosition( Pos );
			m_pShopList.back().first->SetRotation( Rot );
			m_pShopList.back().first->SetShopNo( No );
			m_pShopList.back().first->Init();
		}
		else if ( What == "Room" ) {
			// 部屋の作成.
			m_pRoomList.emplace_back( std::make_pair( std::make_unique<CApartmentRoom>(), i ) );
			m_pRoomList.back().first->SetPosition( Pos );
			m_pRoomList.back().first->SetRotation( Rot );
			m_pRoomList.back().first->Init();
		}
		else {
			// 空部屋の作成.
			m_pEmptyRoomList.emplace_back( std::make_pair( std::make_unique<CApartmentEmptyRoom>(), i ) );
			m_pEmptyRoomList.back().first->SetPosition( Pos );
			m_pEmptyRoomList.back().first->SetRotation( Rot );
			m_pEmptyRoomList.back().first->Init();
		}
	}
}

//---------------------------.
// フロアの移動.
//---------------------------.
void CBTypeApartmentFloor::FloorMove( const D3DXPOSITION3& NewPos )
{
	// フロアの移動.
	m_Transform.Position.x = NewPos.x;
	m_Transform.Position.y = NewPos.y;
	m_Transform.Position.z = NewPos.z;
	m_pApartmentMesh->SetTransform( m_Transform );

	// 各部屋の移動.
	if ( m_IsEmptyFloor ) return;
	for ( auto& [Room, No] : m_pEmptyRoomList ) {
		const std::string&		BoneName = std::string( ROOM_SET_POS_BONE_NAME + std::to_string( No + 1 ) );
		const D3DXPOSITION3&	Pos		 = m_pApartmentMesh->GetPosFromBone( BoneName.c_str() );
		Room->SetPosition( Pos );
	}
	for ( auto& [Room, No] : m_pRoomList ) {
		const std::string&		BoneName = std::string( ROOM_SET_POS_BONE_NAME + std::to_string( No + 1 ) );
		const D3DXPOSITION3&	Pos		 = m_pApartmentMesh->GetPosFromBone( BoneName.c_str() );
		Room->SetPosition( Pos );
	}
	for ( auto& [Shop, No] : m_pShopList ) {
		const std::string&		BoneName = std::string( ROOM_SET_POS_BONE_NAME + std::to_string( No + 1 ) );
		const D3DXPOSITION3&	Pos		 = m_pApartmentMesh->GetPosFromBone( BoneName.c_str() );
		Shop->SetPosition( Pos );
	}
}

//---------------------------.
// 上に設置する座標を取得する.
//---------------------------.
D3DXVECTOR3 CBTypeApartmentFloor::GetSettingPosition()
{
	m_pApartmentMesh->SetTransform( m_Transform );
	return m_pApartmentMesh->GetPosFromBone( NEXT_FLOOR_POS_BONE_NAME );
}

//---------------------------.
// フロアのデータの取得.
//---------------------------.
json CBTypeApartmentFloor::GetFloorData()
{
	if ( m_IsEmptyFloor ) return nullptr;

	json Out;
	for ( auto& [Room, No] : m_pEmptyRoomList	) Out[std::to_string( No )] = nullptr;
	for ( auto& [Room, No] : m_pRoomList		) Out[std::to_string( No )] = "Room";
	for ( auto& [Shop, No] : m_pShopList		) Out[std::to_string( No )] = "Shop_" + std::to_string( Shop->GetShopNo() );
	return Out;
}

//---------------------------.
// アルファの設定.
//---------------------------.
void CBTypeApartmentFloor::SetAlpha( const float& Alpha )
{
	m_Alpha = Alpha;

	if ( m_IsEmptyFloor ) return;
	for ( auto& [Room, No] : m_pEmptyRoomList	) Room->SetAlpha( m_Alpha );
	for ( auto& [Room, No] : m_pRoomList		) Room->SetAlpha( m_Alpha );
	for ( auto& [Shop, No] : m_pShopList		) Shop->SetAlpha( m_Alpha );
}

//---------------------------.
// ライトの更新.
//---------------------------.
void CBTypeApartmentFloor::LightUpdate()
{
	if ( m_IsLightOn ) return;

	// 時間の取得.
	const CTime::Time& Time = CTime::GetTime();

	// 電気をつける時間かどうか.
	if ( m_LightHour	> Time.first	) return;
	if ( m_LightMinute	> Time.second	) return;

	// 電気をつける.
	m_IsLightOn				= true;
	m_PointLight.Brightness = 1.0f;
}
