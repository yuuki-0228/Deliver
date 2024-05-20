#include "IslandShop.h"
#include "Signboard\Signboard.h"
#include "..\..\StageIcon\StageIcon.h"
#include "..\..\..\Character\DeliveryManager\Employee\Employee.h"
#include "..\..\..\SmokeManager\SmokeManager.h"
#include "..\..\..\Wind\Wind.h"
#include "..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr float DROT_MAX			= 180.0f;	// 回転の最大値.
	constexpr float MOVE_MIN			= 0.0f;		// 移動の最小値.
	constexpr float MOVE_MAX			= 1.0f;		// 移動の最大値.
	constexpr float DOOR_MOVE_MIN		= 0.0f;		// 扉の移動の最小値.
	constexpr float DOOR_MOVE_MAX		= 1.0f;		// 扉の移動の最大値.
	constexpr float INVERSION_DROT		= 180.0f;	// 角度を反転させる角度(度).

	constexpr char SMOKE_POS_BONE_NAME[]			= "i_shop_smoke";			// 煙の位置のボーン名.
	constexpr char ICON_POS_BONE_NAME[]				= "i_s_hukidashi";			// アイコンの位置のボーン名.
	constexpr char EMPLOYEE_START_POS_BONE_NAME[]	= "i_employee_position1";	// 店員の開始の位置のボーン名.
	constexpr char EMPLOYEE_END_POS_BONE_NAME[]		= "i_employee_position2";	// 店員の終了の位置のボーン名.
	constexpr char DOOR_START_POS_BONE_NAME[]		= "p_door_position1";		// 扉の開始時のボーン名.
	constexpr char DOOR_END_POS_BONE_NAME[]			= "p_door_position2";		// 扉の終了時のボーン名.
	constexpr char SHOP_OPEN_SE[]					= "ShopOpen";				// 扉を開けた時のSE.
}

CIslandShop::CIslandShop()
	: m_pShopMesh			( nullptr )
	, m_pDoorMesh			( nullptr )
	, m_pCollisionMesh		( nullptr )
	, m_pIcon				( nullptr )
	, m_pSignboard			( nullptr )
	, m_pEmployee			( nullptr )
	, m_pSmokeManager		( nullptr )
	, m_PointLight			()
	, m_EmployeeLight		()
	, m_EmployeeTransform	()
	, m_DoorTransform		()
	, m_EmployeeAddRotation	( INIT_FLOAT3 )
	, m_SmokePlayPosition	( INIT_FLOAT3 )
	, m_StartPos			( INIT_FLOAT3 )
	, m_EndPos				( INIT_FLOAT3 )
	, m_MoveVector			( INIT_FLOAT3 )
	, m_DoorStartPos		( INIT_FLOAT3 )
	, m_DoorEndPos			( INIT_FLOAT3 )
	, m_DoorMoveVector		( INIT_FLOAT3 )
	, m_MoveCnt				( INIT_FLOAT )
	, m_DoorMoveCnt			( INIT_FLOAT )
	, m_Alpha				( Color::ALPHA_MAX )
	, m_ShopNo				( 1 )
	, m_IsRotEnd			( false )
	, m_IsOpenEnd			( false )
	, m_IsCloseEnd			( true )
	, m_IsSmoke				( true )
	, m_IsOpenSE			( false )
{
}

CIslandShop::~CIslandShop()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CIslandShop::Init()
{
	// お店のメッシュの取得.
	m_pShopMesh = CMeshResource::GetSkin( "island_shop_s" );
	m_pShopMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pShopMesh->SetTransform( m_Transform );

	// アイコンの取得.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pShopMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::Employee );

	// 煙の初期化.
	m_pSmokeManager					= std::make_unique<CSmokeManager>();
	m_SmokePlayPosition				= m_pShopMesh->GetPosFromBone( SMOKE_POS_BONE_NAME );
	m_pSmokeManager->Init();

	// 座標の取得.
	m_StartPos						= m_pShopMesh->GetPosFromBone( EMPLOYEE_START_POS_BONE_NAME );
	m_EndPos						= m_pShopMesh->GetPosFromBone( EMPLOYEE_END_POS_BONE_NAME );
	m_MoveVector					= m_EndPos - m_StartPos;
	m_EmployeeTransform				= m_Transform;
	m_EmployeeTransform.Position	= m_StartPos;
	m_DoorStartPos					= m_pShopMesh->GetPosFromBone( DOOR_START_POS_BONE_NAME );
	m_DoorEndPos					= m_pShopMesh->GetPosFromBone( DOOR_END_POS_BONE_NAME );
	m_DoorMoveVector				= m_DoorEndPos - m_DoorStartPos;
	m_DoorTransform					= m_Transform;
	m_DoorTransform.Position		= m_DoorStartPos;

	// 扉のメッシュの取得.
	m_pDoorMesh = CMeshResource::GetSkin( "island_pizza_door_s" );
	m_pDoorMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pDoorMesh->SetTransform( m_DoorTransform );

	// 看板の初期化.
	m_pSignboard					= std::make_unique<CSignboard>();
	m_pSignboard->SetTransform( m_Transform );
	m_pSignboard->SetShopNo( m_ShopNo );
	m_pSignboard->Init();

	// 店員の初期化.
	m_pEmployee						= std::make_unique<CEmployee>( EObjectTag::Island, m_ShopNo );
	m_EmployeeTransform.Rotation	= { 0.0f, Math::ToRadian( -90.0f ), 0.0f };
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetDirection( EDirection::Down );
	m_pEmployee->SetIsLookPlayer( true );
	m_pEmployee->Init();

	// ポイントライトの設定.
	m_PointLight.Brightness		= 1.0f;
	m_EmployeeLight.Brightness	= INIT_FLOAT;

	// タグの設定.
	m_ObjectTag = EObjectTag::IslandObject;

	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CIslandShop::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// 距離によるアルファフェードの更新.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector = m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance = D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.0f;

	PlaySmoke();							// 煙の再生.
	m_pSmokeManager->Update( DeltaTime );	// 煙の更新.
	
	EmployeeDispMove();						// 店員を表示するための移動.
	EmployeeHideMove();						// 店員を非表示にするための移動.
	m_pSignboard->Update( DeltaTime );		// 看板の更新.
	m_pEmployee->Update( DeltaTime );		// 店員の更新.

	IconUpdate();							// アイコンの更新.
	CameraAlphaUpdate();					// カメラによるアルファ処理.
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CIslandShop::LateUpdate( const float& DeltaTime )
{
	m_pEmployee->LateUpdate( DeltaTime );

	// 店員をプレイヤーの方を見るように回転させる.
	EmployeePlayerLook();
}

//---------------------------.
// 描画.
//---------------------------.
void CIslandShop::Render()
{
	if ( m_IsDisp == false ) return;

	// お店の描画.
	m_pShopMesh->SetDither( true );
	m_pShopMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pShopMesh->ChangeTexture( 0, m_ShopNo - 3 );
	m_pShopMesh->SetMeshPointLight( m_PointLight );
	m_pShopMesh->Render( &m_Transform );
	m_pShopMesh->SetDither( false );
	CCollisionRender::PushMesh( m_pCollisionMesh, &m_Transform );

	// 扉の描画.
	m_pDoorMesh->SetDither( true );
	m_pDoorMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pDoorMesh->SetTransform( m_DoorTransform );
	m_pDoorMesh->SetMeshPointLight( m_PointLight );
	m_pDoorMesh->Render();
	m_pDoorMesh->SetDither( false );

	// 看板の描画.
	m_pSignboard->Render();

	// 煙の描画.
	m_pSmokeManager->Render();

	// 店員の描画.
	if ( m_EmployeeTransform.Position == m_StartPos ) return;
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetRotation( m_EmployeeTransform.Rotation + m_EmployeeAddRotation );
	m_pEmployee->SetMeshPointLight( m_EmployeeLight );
	m_pEmployee->Render();
}

//---------------------------.
// スプライト(3D)の描画.
//---------------------------.
void CIslandShop::Sprite3DRender_A()
{
	if ( m_IsDisp == false ) return;

	m_pEmployee->Sprite3DRender_A();

	if ( m_pEmployee->GetIsLink() == false ) return;
	m_pIcon->Sprite3DRender_A();
}

//---------------------------.
// 看板の座標の設定.
//---------------------------.
void CIslandShop::SetSignboardPosition( const D3DXPOSITION3& Pos )
{
	m_pSignboard->SetPosition( Pos );
}

//---------------------------.
// 当たり判定を追加する.
//---------------------------.
void CIslandShop::PushCollisionManager()
{
	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );

	m_pSignboard->PushCollisionManager();
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CIslandShop::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "shop_c" );
	m_pCollisions	 = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CIslandShop::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// カメラによるアルファ値の更新.
//---------------------------.
void CIslandShop::CameraAlphaUpdate()
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
// アイコンの表示の更新.
//---------------------------.
void CIslandShop::IconUpdate()
{
	// アイコンの更新.
	if ( m_pEmployee->GetIsLink() == false ) return;
	m_pIcon->IconUpdate( m_pEmployee->GetHandle() );
}

//---------------------------.
// 煙の再生.
//---------------------------.
void CIslandShop::PlaySmoke()
{
	if ( m_pEmployee->GetIsLink()	) return;
	if ( m_IsSmoke == false			) return;
 
	// 煙の再生.
	m_pSmokeManager->PlaySmoke( m_SmokePlayPosition, Math::_X_VECTOR, CWind::GetWindVector( m_SmokePlayPosition.y ) * 1.5f );
}

//---------------------------.
// 店員を表示するための移動.
//---------------------------.
void CIslandShop::EmployeeDispMove()
{
	if ( m_EmployeeTransform.Position	== m_EndPos	) return;
	if ( m_pEmployee->GetIsLink()		== false	) return;
	m_IsSmoke = false;

	// 扉を開ける.
	DoorOpen();

	// 店員の移動.
	if ( m_IsOpenEnd == false ) return;
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_StartPos + ( m_MoveVector * m_MoveCnt );

	// 移動終了.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeTransform.Position	= m_EndPos;
		m_IsRotEnd						= false;
		m_IsCloseEnd					= false;

		// 手を振るアニメーションに変更.
		m_pEmployee->ChangeWaveHands();
	}
}

//---------------------------.
// 店員がプレイヤーの方を見るように回転.
//---------------------------.
void CIslandShop::EmployeePlayerLook()
{
	if ( m_pEmployee->GetIsLink()		== false	) return;
	if ( m_EmployeeTransform.Position	!= m_EndPos ) return;

	const float InitRot			= m_EmployeeTransform.Rotation.y;		// 初期角度.
	const float InversionRot	= Math::ToRadian( INVERSION_DROT );		// 反転させる角度.

	// プレイヤーの方向の角度を求める.
	float EndRotY = m_pEmployee->GetRotation().y - InitRot;
	if ( EndRotY >  InversionRot ) EndRotY -= Math::RADIAN_MAX;
	if ( EndRotY < -InversionRot ) EndRotY += Math::RADIAN_MAX;

	// 回転方向を求める.
	const float OldRotY		= m_EmployeeAddRotation.y;
	float		DireRotY	= m_pEmployee->GetRotation().y - InitRot - OldRotY;
	if ( DireRotY >  InversionRot ) DireRotY -= Math::RADIAN_MAX;
	if ( DireRotY < -InversionRot ) DireRotY += Math::RADIAN_MAX;

	// プレイヤーの方向に回転させる.
	float RotSpeed = Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;
	if ( DireRotY < 0.0f ) RotSpeed = -RotSpeed;
	m_EmployeeAddRotation.y += RotSpeed;
	if (		DireRotY >= 0.0f && m_EmployeeAddRotation.y - OldRotY >= DireRotY ) m_EmployeeAddRotation.y = EndRotY;
	else if (	DireRotY <  0.0f && m_EmployeeAddRotation.y - OldRotY <= DireRotY ) m_EmployeeAddRotation.y = EndRotY;
	if ( m_EmployeeAddRotation.y >  InversionRot ) m_EmployeeAddRotation.y -= Math::RADIAN_MAX;
	if ( m_EmployeeAddRotation.y < -InversionRot ) m_EmployeeAddRotation.y += Math::RADIAN_MAX;
}

//---------------------------.
// 店員を非表示にするための移動.
//---------------------------.
void CIslandShop::EmployeeHideMove()
{
	if ( m_pEmployee->GetIsLink()								) return;
	if ( m_pEmployee->GetIsPizzaPassedAnimEnd() == false		) return;
	if ( m_EmployeeTransform.Position			== m_StartPos	) {
		if ( m_IsCloseEnd ) return;
		// 扉を閉じる.
		DoorClose();
		return;
	}

	// 店員を回転させる.
	EmployeeRotation();

	// 店員の移動.
	if ( m_IsRotEnd == false ) return;
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_EndPos - ( m_MoveVector * m_MoveCnt );

	// 移動終了.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeAddRotation.y			= 0.0f;
		m_EmployeeTransform.Position	= m_StartPos;
		m_IsOpenEnd						= false;
		m_IsSmoke						= true;
	}
}

//---------------------------.
// 店員の回転.
//---------------------------.
void CIslandShop::EmployeeRotation()
{
	if ( m_IsRotEnd												) return;
	if ( m_EmployeeAddRotation.y >= Math::ToRadian( DROT_MAX )	) return;

	// 180度回転させる.
	float RotSpeed = Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;
	if ( m_EmployeeAddRotation.y < 0 ) RotSpeed = -RotSpeed;
	m_EmployeeAddRotation.y += RotSpeed;

	// 回転終了.
	const float& RadRotMax = Math::ToRadian( DROT_MAX );
	if ( m_EmployeeAddRotation.y >= RadRotMax || m_EmployeeAddRotation.y <= -RadRotMax ) {
		m_EmployeeAddRotation.y = RadRotMax;
		m_IsRotEnd				= true;
	}
}

//---------------------------.
// 扉を開ける.
//---------------------------.
void CIslandShop::DoorOpen()
{
	if ( m_IsOpenEnd ) return;

	// SEの再生.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( SHOP_OPEN_SE, m_DoorTransform.Position );
	}

	m_DoorMoveCnt += m_DeltaTime * CONSTANT.DOOR_MOVE_SPEED;
	m_DoorTransform.Position = m_DoorStartPos + ( m_DoorMoveVector * m_DoorMoveCnt );

	// 移動終了.
	if ( m_DoorMoveCnt >= DOOR_MOVE_MAX ) {
		m_DoorMoveCnt				= DOOR_MOVE_MIN;
		m_DoorTransform.Position	= m_DoorEndPos;
		m_IsOpenEnd					= true;
		m_IsOpenSE					= false;
	}
}

//---------------------------.
// 扉を閉じたか.
//---------------------------.
void CIslandShop::DoorClose()
{
	if ( m_IsCloseEnd ) return;

	// SEの再生.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( SHOP_OPEN_SE, m_DoorTransform.Position );
	}

	m_DoorMoveCnt += m_DeltaTime * CONSTANT.DOOR_MOVE_SPEED;
	m_DoorTransform.Position = m_DoorEndPos - ( m_DoorMoveVector * m_DoorMoveCnt );

	// 移動終了.
	if ( m_DoorMoveCnt >= DOOR_MOVE_MAX ) {
		m_DoorMoveCnt				= DOOR_MOVE_MIN;
		m_DoorTransform.Position	= m_DoorStartPos;
		m_IsCloseEnd				= true;
		m_IsOpenSE					= false;
	}
}