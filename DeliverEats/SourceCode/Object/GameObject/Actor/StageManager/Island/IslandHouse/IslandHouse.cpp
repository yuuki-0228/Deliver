#include "IslandHouse.h"
#include "Garage\Garage.h"
#include "..\..\StageIcon\StageIcon.h"
#include "..\..\..\Character\DeliveryManager\Customer\Customer.h"
#include "..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr float DROT_MAX			= 180.0f;	// 回転の最大値(度).
	constexpr float MOVE_MIN			= 0.0f;		// 移動の最小値.
	constexpr float MOVE_MAX			= 1.0f;		// 移動の最大値.
	constexpr float DOOR_MIN			= 0.0f;		// 扉の回転の最小値.
	constexpr float DOOR_MAX			= 1.0f;		// 扉の回転の最大値.
	constexpr float DOOR_DROT_MIN		= 0.0f;		// 扉の回転の最小値(度).
	constexpr float DOOR_DROT_MAX		= -90.0f;	// 扉の回転の最大値(度).
	constexpr float INVERSION_DROT		= 180.0f;	// 角度を反転させる角度(度).

	constexpr char ICON_POS_BONE_NAME[]				= "i_h_hukidasi";			// アイコンの位置のボーン名.
	constexpr char CUSTOMER_START_POS_BONE_NAME[]	= "i_customer_position1";	// お客さんの開始の位置のボーン名.
	constexpr char CUSTOMER_END_POS_BONE_NAME[]		= "i_customer_position2";	// お客さんの終了の位置のボーン名.
	constexpr char DOOR_POS_BONE_NAME[]				= "door_position";			// 扉の位置のボーン名.
	constexpr char HOUSE_OPEN_SE[]					= "HouseOpen";				// 扉を開けた時のSE.
}

CIslandHouse::CIslandHouse()
	: m_pHouseMesh			( nullptr )
	, m_pDoorMesh			( nullptr )
	, m_pCollisionMesh		( nullptr )
	, m_pIcon				( nullptr )
	, m_pGarage				( nullptr )
	, m_pCustomer			( nullptr )
	, m_PointLight			()
	, m_CustomerLight		()
	, m_CustomerTransform	()
	, m_DoorTransform		()
	, m_CustomerAddRotation	( INIT_FLOAT3 )
	, m_StartPos			( INIT_FLOAT3 )
	, m_EndPos				( INIT_FLOAT3 )
	, m_MoveVector			( INIT_FLOAT3 )
	, m_MoveCnt				( INIT_FLOAT )
	, m_DoorMoveCnt			( INIT_FLOAT )
	, m_Alpha				( Color::ALPHA_MAX )
	, m_LightOnHour			( INIT_INT )
	, m_LightOnMinute		( INIT_INT )
	, m_LightOffHour		( INIT_INT )
	, m_LightOffMinute		( INIT_INT )
	, m_IsRotEnd			( false )
	, m_IsOpenEnd			( false )
	, m_IsCloseEnd			( true )
	, m_IsLightOn			( false )
	, m_IsLightOff			( false )
	, m_IsOpenSE			( false )
{
}

CIslandHouse::~CIslandHouse()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CIslandHouse::Init()
{
	// 家のメッシュの取得.
	m_pHouseMesh = CMeshResource::GetSkin( "island_house_s" );
	m_pHouseMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pHouseMesh->SetTransform( m_Transform );

	// アイコンの取得.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pHouseMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::Customer );

	// 座標の取得.
	m_StartPos						= m_pHouseMesh->GetPosFromBone( CUSTOMER_START_POS_BONE_NAME );
	m_EndPos						= m_pHouseMesh->GetPosFromBone( CUSTOMER_END_POS_BONE_NAME );
	m_MoveVector					= m_EndPos - m_StartPos;
	m_CustomerTransform				= m_Transform;
	m_CustomerTransform.Position	= m_StartPos;
	m_DoorTransform					= m_Transform;
	m_DoorTransform.Position		= m_pHouseMesh->GetPosFromBone( DOOR_POS_BONE_NAME );

	// 扉のメッシュの取得.
	m_pDoorMesh = CMeshResource::GetSkin( "house_door_s" );
	m_pDoorMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pDoorMesh->SetTransform( m_DoorTransform );

	// ガレージの初期化.
	m_pGarage						= std::make_unique<CGarage>();
	m_pGarage->SetTransform( m_Transform );
	m_pGarage->Init();

	// お客さんの初期化.
	m_pCustomer						= std::make_unique<CCustomer>( EObjectTag::Island );
	m_CustomerTransform.Rotation	= { 0.0f, Math::ToRadian( -90.0f ), 0.0f };
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetDirection( EDirection::Down );
	m_pCustomer->Init();

	// ポイントライトの設定.
	m_LightOnHour		= CRandom::GetRand( 16, 17 );
	m_LightOnMinute		= CRandom::GetRand( 0,  50 );
	m_LightOffHour		= CRandom::GetRand( 22, 24 );
	m_LightOffMinute	= CRandom::GetRand( 0,  50 );
	m_PointLight.Brightness		= INIT_FLOAT;
	m_CustomerLight.Brightness	= INIT_FLOAT;

	// タグの設定.
	m_ObjectTag = EObjectTag::IslandObject;

	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CIslandHouse::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// 距離によるアルファフェードの更新.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector = m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance = D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.0f;

	CustomerDispMove();					// お客さんを表示するための移動.
	CustomerHideMove();					// お客さんを非表示にするための移動.
	m_pGarage->Update( DeltaTime );		// ガレージの更新.
	m_pCustomer->Update( DeltaTime );	// お客さんの更新.

	IconUpdate();						// アイコンの更新.
	LightUpdate();						// ライトの更新.
	CameraAlphaUpdate();				// カメラによるアルファ処理.
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CIslandHouse::LateUpdate( const float& DeltaTime )
{
	m_pCustomer->LateUpdate( DeltaTime );

	// お客さんをプレイヤーの方を見るように回転させる.
	CustomerPlayerLook();
}

//---------------------------.
// 描画.
//---------------------------.
void CIslandHouse::Render()
{
	if ( m_IsDisp == false ) return;

	// 家の描画.
	m_pHouseMesh->SetDither( true );
	m_pHouseMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pHouseMesh->SetMeshPointLight( m_PointLight );
	m_pHouseMesh->Render( &m_Transform );
	m_pHouseMesh->SetDither( false );
	CCollisionRender::PushMesh( m_pCollisionMesh, &m_Transform );

	// 扉の描画.
	CDirectX11::SetRasterizerState( ERS_STATE::None );
	m_pDoorMesh->SetDither( true );
	m_pDoorMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pDoorMesh->SetTransform( m_DoorTransform );
	m_pDoorMesh->SetMeshPointLight( m_PointLight );
	m_pDoorMesh->Render();
	m_pDoorMesh->SetDither( false );
	CDirectX11::SetRasterizerState( ERS_STATE::Back );

	// ガレージの描画.
	m_pGarage->Render();

	// お客さんの描画.
	if ( m_CustomerTransform.Position == m_StartPos ) return;
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetRotation( m_CustomerTransform.Rotation + m_CustomerAddRotation );
	m_pCustomer->SetMeshPointLight( m_CustomerLight );
	m_pCustomer->Render();
}

//---------------------------.
// スプライト(3D)の描画.
//---------------------------.
void CIslandHouse::Sprite3DRender_A()
{
	if ( m_IsDisp == false ) return;

	m_pCustomer->Sprite3DRender_A();

	if ( m_pCustomer->GetIsLink() == false ) return;
	m_pIcon->Sprite3DRender_A();
}

//---------------------------.
// ガレージの座標の設定.
//---------------------------.
void CIslandHouse::SetGaragePosition( const D3DXPOSITION3& Pos )
{
	m_pGarage->SetPosition( Pos );
}

//---------------------------.
// 当たり判定を追加する.
//---------------------------.
void CIslandHouse::PushCollisionManager()
{
	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );

	m_pGarage->PushCollisionManager();
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CIslandHouse::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "house_c" );
	m_pCollisions	 = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CIslandHouse::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// カメラによるアルファ値の更新.
//---------------------------.
void CIslandHouse::CameraAlphaUpdate()
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

//---------------------------.
// アイコンの更新.
//---------------------------.
void CIslandHouse::IconUpdate()
{
	// アイコンの更新.
	if ( m_pCustomer->GetIsLink() == false ) return;
	m_pIcon->IconUpdate( m_pCustomer->GetHandle() );

	// アイコンのタグの更新.
	m_pIcon->SetTag( EObjectTag::Customer );
	if ( m_pCustomer->GetIsGoHandingCustomer() == false ) return;
	m_pIcon->SetTag( EObjectTag::GoHandingCustomer );
}

//---------------------------.
// ライトの更新.
//---------------------------.
void CIslandHouse::LightUpdate()
{
	if ( m_IsLightOff ) return;

	// 時間の取得.
	const std::pair<int, int>& Time = CTime::GetTime();

	// ライトの更新.
	if ( m_IsLightOn )	LightOff( Time );
	else				LightOn(  Time );
}

//---------------------------.
// ライトをつける.
//---------------------------.
void CIslandHouse::LightOn( const std::pair<int, int>& Time )
{
	// 電気をつける時間かどうか.
	if ( m_LightOnHour		> Time.first	) return;
	if ( m_LightOnMinute	> Time.second	) return;

	// 電気をつける.
	m_IsLightOn				= true;
	m_PointLight.Brightness = 1.0f;
}

//---------------------------.
// ライトの消す.
//---------------------------.
void CIslandHouse::LightOff( const std::pair<int, int>& Time )
{
	// 現在の時間を24時以降も2〇時の表示に変更する.
	std::pair<int, int> cTime = Time;
	if ( cTime.first < 12 ) cTime.first += 24;

	// 電気をつける時間かどうか.
	if ( m_LightOffHour		> cTime.first	) return;
	if ( m_LightOffMinute	> cTime.second	) return;

	// 電気を消す.
	m_IsLightOff			= true;
	m_PointLight.Brightness = 0.0f;
}

//---------------------------.
// お客さんを表示するための移動.
//---------------------------.
void CIslandHouse::CustomerDispMove()
{
	if ( m_CustomerTransform.Position	== m_EndPos	) return;
	if ( m_pCustomer->GetIsLink()		== false	) return;

	// 扉を開ける.
	DoorOpen();

	// お客さんの移動.
	if ( m_IsOpenEnd == false ) return;
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_CustomerTransform.Position = m_StartPos + ( m_MoveVector * m_MoveCnt );

	// 移動終了.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_CustomerTransform.Position	= m_EndPos;
		m_IsRotEnd						= false;
		m_IsCloseEnd					= false;

		// 手を振るアニメーションに変更.
		m_pCustomer->ChangeWaveHands();
	}
}

//---------------------------.
// お客さんがプレイヤーの方を見るように回転.
//---------------------------.
void CIslandHouse::CustomerPlayerLook()
{
	if ( m_pCustomer->GetIsLink()		== false	) return;
	if ( m_CustomerTransform.Position	!= m_EndPos ) return;

	const float InitRot			= m_CustomerTransform.Rotation.y;		// 初期角度.
	const float InversionRot	= Math::ToRadian( INVERSION_DROT );		// 反転させる角度.

	// プレイヤーの方向の角度を求める.
	float EndRotY = m_pCustomer->GetRotation().y - InitRot;
	if ( EndRotY >  InversionRot ) EndRotY -= Math::RADIAN_MAX;
	if ( EndRotY < -InversionRot ) EndRotY += Math::RADIAN_MAX;

	// 回転方向を求める.
	const float OldRotY		= m_CustomerAddRotation.y;
	float		DireRotY	= m_pCustomer->GetRotation().y - InitRot - OldRotY;
	if ( DireRotY >  InversionRot ) DireRotY -= Math::RADIAN_MAX;
	if ( DireRotY < -InversionRot ) DireRotY += Math::RADIAN_MAX;


	// プレイヤーの方向に回転させる.
	float RotSpeed = Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;
	if ( DireRotY < 0.0f ) RotSpeed = -RotSpeed;
	m_CustomerAddRotation.y += RotSpeed;
	if (		DireRotY >= 0.0f && m_CustomerAddRotation.y - OldRotY >= DireRotY ) m_CustomerAddRotation.y = EndRotY;
	else if (	DireRotY <  0.0f && m_CustomerAddRotation.y - OldRotY <= DireRotY ) m_CustomerAddRotation.y = EndRotY;
	if ( m_CustomerAddRotation.y >  InversionRot ) m_CustomerAddRotation.y -= Math::RADIAN_MAX;
	if ( m_CustomerAddRotation.y < -InversionRot ) m_CustomerAddRotation.y += Math::RADIAN_MAX;
}

//---------------------------.
// お客さんを非表示にするための移動.
//---------------------------.
void CIslandHouse::CustomerHideMove()
{
	if ( m_pCustomer->GetIsLink()							) return;
	if ( m_pCustomer->GetIsGetPizzaAnimEnd() == false		) return;
	if ( m_CustomerTransform.Position		 == m_StartPos	) {
		if ( m_IsCloseEnd ) return;
		// 扉を閉じる.
		DoorClose();
		return;
	}

	// お客さんを回転させる.
	CustomerRotation();

	// お客さんの移動.
	if ( m_IsRotEnd == false ) return;
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_CustomerTransform.Position = m_EndPos - ( m_MoveVector * m_MoveCnt );

	// 移動終了.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_CustomerAddRotation.y			= 0.0f;
		m_CustomerTransform.Position	= m_StartPos;
		m_IsOpenEnd						= false;
	}
}

//---------------------------.
// お客さんの回転.
//---------------------------.
void CIslandHouse::CustomerRotation()
{
	if ( m_IsRotEnd												) return;
	if ( m_CustomerAddRotation.y >= Math::ToRadian( DROT_MAX )	) return;

	// 180度回転させる.
	float RotSpeed = Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;
	if ( m_CustomerAddRotation.y < 0 ) RotSpeed = -RotSpeed;
	m_CustomerAddRotation.y += RotSpeed;

	// 回転終了.
	const float& RadRotMax = Math::ToRadian( DROT_MAX );
	if ( m_CustomerAddRotation.y >= RadRotMax || m_CustomerAddRotation.y <= -RadRotMax ) {
		m_CustomerAddRotation.y = RadRotMax;
		m_IsRotEnd				= true;
	}
}

//---------------------------.
// 扉を開ける.
//---------------------------.
void CIslandHouse::DoorOpen()
{
	if ( m_IsOpenEnd ) return;

	// SEの再生.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( HOUSE_OPEN_SE, m_DoorTransform.Position );
	}

	m_DoorMoveCnt				+= m_DeltaTime * CONSTANT.DOOR_MOVE_SPEED;
	m_DoorTransform.Rotation.y = Math::ToRadian( DOOR_DROT_MAX * m_DoorMoveCnt );

	// 移動終了.
	if ( m_DoorMoveCnt >= DOOR_MAX ) {
		m_DoorMoveCnt				= DOOR_MIN;
		m_DoorTransform.Rotation.y	= Math::ToRadian( DOOR_DROT_MAX );
		m_IsOpenEnd					= true;
		m_IsOpenSE					= false;
	}
}

//---------------------------.
// 扉を閉じたか.
//---------------------------.
void CIslandHouse::DoorClose()
{
	if ( m_IsCloseEnd ) return;

	// SEの再生.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( HOUSE_OPEN_SE, m_DoorTransform.Position );
	}

	m_DoorMoveCnt				+= m_DeltaTime * CONSTANT.DOOR_MOVE_SPEED;
	m_DoorTransform.Rotation.y	 = Math::ToRadian( DOOR_DROT_MAX - ( DOOR_DROT_MAX * m_DoorMoveCnt ) );

	// 移動終了.
	if ( m_DoorMoveCnt >= DOOR_MAX ) {
		m_DoorMoveCnt				= DOOR_MIN;
		m_DoorTransform.Rotation.y	= Math::ToRadian( DOOR_DROT_MIN );
		m_IsCloseEnd				= true;
		m_IsOpenSE					= false;
	}
}