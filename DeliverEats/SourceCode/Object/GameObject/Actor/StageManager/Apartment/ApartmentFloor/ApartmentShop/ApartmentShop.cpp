#include "ApartmentShop.h"
#include "PizzaKilnFire\PizzaKilnFire.h"
#include "..\..\..\StageIcon\StageIcon.h"
#include "..\..\..\..\Character\DeliveryManager\Employee\Employee.h"
#include "..\..\..\..\Wind\Wind.h"
#include "..\..\..\..\SmokeManager\SmokeManager.h"
#include "..\..\..\..\..\ActorCollisionManager\ActorCollisionManager.h"
#include "..\..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\..\Common\Sprite\Sprite.h"

namespace {
	constexpr float MOVE_MIN			= 0.0f;		// 移動の最小値.
	constexpr float MOVE_MAX			= 1.0f;		// 移動の最大値.
	constexpr float GETGAUGE_MAX		= 1.0f;		// 受け取りゲージの最大値.
	constexpr float GETGAUGE_MIN		= 0.0f;		// 受け取りゲージの最小値.
	constexpr float INVERSION_DROT		= 180.0f;	// 角度を反転させる角度(度).

	constexpr char EMPLOYEE_RESISTOR_POS_BONE_NAME[]		= "employee_position1";		// 店員のレジの位置のボーン名.
	constexpr char EMPLOYEE_KILN_POS_BONE_NAME[]			= "employee_position2";		// 店員のピザ窯の位置のボーン名.
	constexpr char EMPLOYEE_REFRIGERATOR_POS_BONE_NAME[]	= "employee_position3";		// 店員の冷蔵庫の位置のボーン名.
	constexpr char SMOKE_POS_BONE_NAME[]					= "kemuri_position";		// 煙の位置のボーン名.
	constexpr char FIRE_POS_BONE_NAME[]						= "fire1";					// ピザ窯の炎の位置のボーン名.
	constexpr char ICON_POS_BONE_NAME[]						= "hukidasi_bone";			// アイコンの位置のボーン名.

	// アニメーションNo.
	enum enAnimationNo : unsigned char {
		MoveResistorAnim,		// レジに移動.
		ResistorAnim,			// レジ.
		MoveKilnAnim,			// 窯に移動.
		KilnAnim,				// 窯.
		MoveRefrigeratorAnim,	// 冷蔵庫に移動.
		RefrigeratorAnim,		// 冷蔵庫.

		AnimationNo_Max
	} typedef EAnimationNo;

	// 店員の移動場所No.
	enum enEmployeePositionNo : unsigned char {
		Resistor,		// レジ.
		Kiln,			// 窯.
		Refrigerator,	// 冷蔵庫.

		EmployeePosition_Max
	} typedef EEmployeePositionNo;
}

CApartmentShop::CApartmentShop()
	: m_pShopMesh			( nullptr )
	, m_pIcon				( nullptr )
	, m_pFire				( nullptr )
	, m_pEmployee			( nullptr )
	, m_pSmokeManager		( nullptr )
	, m_PointLight			()
	, m_EmployeeTransform	()
	, m_EmployeeAddRotation	( INIT_FLOAT3 )
	, m_SmokePlayPosition	( INIT_FLOAT3 )
	, m_EmployeeSetPosition	( EmployeePosition_Max )
	, m_EmployeeMoveVectir	( EmployeePosition_Max )
	, m_Alpha				( Color::ALPHA_MAX )
	, m_MoveCnt				( INIT_FLOAT )
	, m_ShopNo				( 1 )
	, m_AnimationNo			( KilnAnim )
	, m_FloorIsDisp			( true )
	, m_RotationEnd			( false )
	, m_IsSmoke				( true )
	, m_OldSEBGMPlay		( false )
{
}

CApartmentShop::~CApartmentShop()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CApartmentShop::Init()
{
	// メッシュの取得.
	m_pShopMesh = CMeshResource::GetSkin( "shop_apart_s" );
	m_pShopMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pShopMesh->SetTransform( m_Transform );

	// ピザ窯の炎の初期化.
	m_pFire = std::make_unique<CPizzaKilnFire>();
	m_pFire->SetTransform( m_Transform );
	m_pFire->SetPosition( m_pShopMesh->GetPosFromBone( FIRE_POS_BONE_NAME ) );
	m_pFire->Init();

	// アイコンの取得.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pShopMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::Employee );
	 
	// 座標の保存.
	m_EmployeeSetPosition[Resistor		] = m_pShopMesh->GetPosFromBone( EMPLOYEE_RESISTOR_POS_BONE_NAME );
	m_EmployeeSetPosition[Kiln			] = m_pShopMesh->GetPosFromBone( EMPLOYEE_KILN_POS_BONE_NAME );
	m_EmployeeSetPosition[Refrigerator	] = m_pShopMesh->GetPosFromBone( EMPLOYEE_REFRIGERATOR_POS_BONE_NAME );
	m_EmployeeMoveVectir[Resistor		] = m_EmployeeSetPosition[Resistor]		- m_EmployeeSetPosition[Kiln];
	m_EmployeeMoveVectir[Kiln			] = m_EmployeeSetPosition[Kiln]			- m_EmployeeSetPosition[Refrigerator];
	m_EmployeeMoveVectir[Refrigerator	] = m_EmployeeSetPosition[Refrigerator]	- m_EmployeeSetPosition[Resistor];

	// 店員の初期化.
	m_pEmployee						= std::make_unique<CEmployee>( EObjectTag::Apartment, m_ShopNo );
	m_EmployeeTransform				= m_Transform;
	m_EmployeeTransform.Position	= m_EmployeeSetPosition[Kiln];
	m_EmployeeTransform.Rotation.y	+= Math::ToRadian( 90.0f );
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetAddMoney( CONSTANT.APARTMENT_ADD_MONEY );
	m_pEmployee->SetDirection( GetRoomDirection() );
	m_pEmployee->Init();

	// 煙の初期化.
	m_pSmokeManager		= std::make_unique<CSmokeManager>();
	m_pSmokeManager->Init();
	m_SmokePlayPosition = m_pShopMesh->GetPosFromBone( SMOKE_POS_BONE_NAME );
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CApartmentShop::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// 距離によるアルファフェードの更新.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.6f;

	GoResistor();							// レジに移動.
	GoRefrigerator();						// 冷蔵庫に移動.
	GoKiln();								// 窯に移動.
	PlaySmoke();							// 煙の再生.
	EmployeeUpdate();						// 店員の更新.
	m_pSmokeManager->Update( DeltaTime );	// 煙の更新.
	m_pFire->Update( DeltaTime );			// ピザ窯の炎の更新.

	// ピザ窯SEを再生するか.
	if ( m_AnimationNo == KilnAnim && m_pEmployee->GetIsShortly() ) {
		CDeliveryManager::AddKilnEmployee( m_pEmployee.get() );
	}

	IconUpdate();							// アイコンの表示の更新.
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CApartmentShop::LateUpdate( const float& DeltaTime )
{
	m_pEmployee->LateUpdate( DeltaTime );

	// 店員をプレイヤーの方を見るように回転させる.
	EmployeePlayerLook();
}

//---------------------------.
// 描画.
//---------------------------.
void CApartmentShop::Render()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	// お店の描画.
	m_pShopMesh->SetDither( true );
	m_pShopMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pShopMesh->ChangeTexture( 0, m_ShopNo - 1 );
	m_pShopMesh->SetMeshPointLight( m_PointLight );
	m_pShopMesh->Render( &m_Transform );
	m_pShopMesh->SetDither( false );

	// ピザ窯の描画.
	m_pFire->SetPosition( m_pShopMesh->GetPosFromBone( FIRE_POS_BONE_NAME ) );
	m_pFire->Render();

	// 店員の描画.
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetRotation( m_EmployeeTransform.Rotation + m_EmployeeAddRotation );
	m_pEmployee->SetMeshPointLight( m_PointLight );
	m_pEmployee->Render();

	// 煙の描画.
	m_pSmokeManager->Render();
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CApartmentShop::Sprite3DRender_A()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	m_pEmployee->Sprite3DRender_A();

	if ( m_pEmployee->GetIsLink() == false ) return;
	m_pIcon->Sprite3DRender_A();
}

//---------------------------.
// アルファの設定.
//---------------------------.
void CApartmentShop::SetAlpha( const float Alpha )
{
	m_Alpha = Alpha;
	m_pEmployee->SetAlpha( m_Alpha );
}

//---------------------------.
// 部屋の向きを取得.
//---------------------------.
EDirection CApartmentShop::GetRoomDirection()
{
	const float RotY = m_Transform.Rotation.y;
	if (		RotY == 0.0f					 ) return EDirection::Down;
	else if (	RotY == Math::ToRadian( 90.0f  ) ) return EDirection::Left;
	else if (	RotY == Math::ToRadian( 180.0f ) ) return EDirection::Up;
	else if (	RotY == Math::ToRadian( 270.0f ) ) return EDirection::Right;
	return EDirection::None;
}

//---------------------------.
// アイコンの更新.
//---------------------------.
void CApartmentShop::IconUpdate()
{
	// アイコンの更新.
	if ( m_pEmployee->GetIsLink() == false ) return;
	m_pIcon->IconUpdate( m_pEmployee->GetHandle() );
}

//---------------------------.
// 店員の更新.
//---------------------------.
void CApartmentShop::EmployeeUpdate()
{
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetRotation( m_EmployeeTransform.Rotation + m_EmployeeAddRotation );
	m_pEmployee->Update( m_DeltaTime );
}

//---------------------------.
// 店員の回転.
//---------------------------.
void CApartmentShop::EmployeeRotation( const float EndRot )
{
	m_EmployeeAddRotation.y -= Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;

	// 回転が終了したか.
	if ( m_EmployeeAddRotation.y <= Math::ToRadian( EndRot ) ) {
		m_EmployeeAddRotation.y = Math::ToRadian( EndRot );
		if ( m_EmployeeAddRotation.y < Math::RADIAN_MIN ) m_EmployeeAddRotation.y += Math::RADIAN_MAX;
		m_RotationEnd = true;
	}
}

//---------------------------.
// レジに移動.
//---------------------------.
void CApartmentShop::GoResistor()
{
	if ( m_EmployeeTransform.Position	== m_EmployeeSetPosition[Resistor]	) return;
	if ( m_pEmployee->GetIsLink()		== false							) return;
	m_pEmployee->SetIsLookPlayer( false );
	m_IsSmoke = false;

	// 店員の回転.
	if ( m_RotationEnd == false ) {
		EmployeeRotation( -180.0f );
		return;
	}
	if ( m_RotationEnd == false ) return;

	// レジの前まで移動させる.
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_EmployeeSetPosition[Kiln] + ( m_EmployeeMoveVectir[Resistor] * m_MoveCnt );

	// 移動終了.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeTransform.Position	= m_EmployeeSetPosition[Resistor];
		m_RotationEnd					= false;
		m_AnimationNo					= ResistorAnim;
		m_pEmployee->SetIsLookPlayer( true );
	}
}

//---------------------------.
// 冷蔵庫に移動.
//---------------------------.
void CApartmentShop::GoRefrigerator()
{
	if ( m_AnimationNo == MoveKilnAnim || m_AnimationNo == KilnAnim ) return;
	if ( m_pEmployee->GetIsLink()									) return;
	m_pEmployee->SetIsLookPlayer( false );

	// 店員の回転.
	if ( m_RotationEnd == false ) {
		EmployeeRotation( -180.0f );
		return;
	}
	if ( m_RotationEnd == false ) return;

	// 冷蔵庫の前まで移動させる.
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_EmployeeSetPosition[Resistor] + ( m_EmployeeMoveVectir[Refrigerator] * m_MoveCnt );

	// 移動終了.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeTransform.Position	= m_EmployeeSetPosition[Refrigerator];
		m_RotationEnd					= false;
		m_AnimationNo					= MoveKilnAnim;
	}
}

//---------------------------.
// 窯に移動.
//---------------------------.
void CApartmentShop::GoKiln()
{
	if ( m_AnimationNo != MoveKilnAnim	) return;
	if ( m_pEmployee->GetIsLink()		) return;

	// 店員の回転.
	if ( m_RotationEnd == false ) {
		EmployeeRotation( 0.0f );
		return;
	}
	if ( m_RotationEnd == false ) return;

	// 窯の前まで移動させる.
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_EmployeeSetPosition[Refrigerator] + ( m_EmployeeMoveVectir[Kiln] * m_MoveCnt );

	// 移動終了.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeTransform.Position	= m_EmployeeSetPosition[Kiln];
		m_RotationEnd					= false;
		m_AnimationNo					= KilnAnim;
		m_IsSmoke						= true;
	}
}

//---------------------------.
// 煙の再生.
//---------------------------.
void CApartmentShop::PlaySmoke()
{
	if ( m_pEmployee->GetIsLink()	) return;
	if ( m_IsSmoke == false			) return;

	// 煙の再生.
	m_pSmokeManager->PlaySmoke( m_SmokePlayPosition, -m_Transform.GetRotationVector(), CWind::GetWindVector( m_SmokePlayPosition.y ) * 1.5f );
}

//---------------------------.
// 店員がプレイヤーの方を見るように回転.
//---------------------------.
void CApartmentShop::EmployeePlayerLook()
{
	if ( m_AnimationNo				!= ResistorAnim ) return;
	if ( m_pEmployee->GetIsLink()	== false		) return;

	const float InitRot			= m_EmployeeTransform.Rotation.y;	// 初期角度.
	const float InversionRot	= Math::ToRadian( INVERSION_DROT );	// 反転させる角度.

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
