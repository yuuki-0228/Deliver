#include "ApartmentRoom.h"
#include "..\..\..\StageIcon\StageIcon.h"
#include "..\..\..\..\Character\DeliveryManager\Customer\Customer.h"
#include "..\..\..\..\..\ActorCollisionManager\ActorCollisionManager.h"
#include "..\..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr float DROT_MAX		= 180.0f;	// 回転の最大値.
	constexpr float MOVE_MIN		= 0.0f;		// 移動の最小値.
	constexpr float MOVE_MAX		= 1.0f;		// 移動の最大値.
	constexpr float WINDOW_MOVE_MIN	= 0.0f;		// 窓の移動の最小値.
	constexpr float WINDOW_MOVE_MAX = 1.0f;		// 窓の移動の最大値.
	constexpr float INVERSION_DROT	= 180.0f;	// 角度を反転させる角度(度).

	constexpr char CUSTOMER_START_POS_BONE_NAME[]	= "costomer_position1";		// お客さんの開始の位置のボーン名.
	constexpr char CUSTOMER_END_POS_BONE_NAME[]		= "costomer_position2";		// お客さんの終了の位置のボーン名.
	constexpr char ICON_POS_BONE_NAME[]				= "speech_bubble_position";	// アイコンの位置のボーン名.
	constexpr char WINDOW_START_POS_BONE_NAME[]		= "window_position1";		// 窓の開始時のボーン名.
	constexpr char WINDOW_END_POS_BONE_NAME[]		= "window_position2";		// 窓の終了時のボーン名.
	constexpr char ROOM_OPEN_SE[]					= "RoomOpen";				// カーテンを開けた時のSE.
}

CApartmentRoom::CApartmentRoom()	
	: m_pRoomMesh			( nullptr )
	, m_pWindowMesh			( nullptr )
	, m_pIcon				( nullptr )
	, m_pCustomer			( nullptr )
	, m_PointLight			()
	, m_CustomerTransform	()
	, m_WindowTransform		()
	, m_CustomerAddRotation	( INIT_FLOAT3 )
	, m_StartPos			( INIT_FLOAT3 )
	, m_EndPos				( INIT_FLOAT3 )
	, m_MoveVector			( INIT_FLOAT3 )
	, m_WindowStartPos		( INIT_FLOAT3 )
	, m_WindowEndPos		( INIT_FLOAT3 )
	, m_WindowMoveVector	( INIT_FLOAT3 )
	, m_MoveCnt				( INIT_FLOAT )
	, m_WindowMoveCnt		( INIT_FLOAT )
	, m_Alpha				( Color::ALPHA_MAX )
	, m_LightOnHour			( INIT_INT )
	, m_LightOnMinute		( INIT_INT )
	, m_LightOffHour		( INIT_INT )
	, m_LightOffMinute		( INIT_INT )
	, m_FloorIsDisp			( true )
	, m_IsRotEnd			( false )
	, m_IsOpenEnd			( false )
	, m_IsCloseEnd			( true )
	, m_IsLightOn			( false )
	, m_IsLightOff			( false )
	, m_IsOpenSE			( false )
{
}

CApartmentRoom::~CApartmentRoom()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CApartmentRoom::Init()
{
	// メッシュの取得.
	m_pRoomMesh = CMeshResource::GetSkin( "room_s" );
	m_pRoomMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pRoomMesh->SetTransform( m_Transform );

	// アイコンの取得.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pRoomMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::Customer );

	// 座標の取得.
	m_StartPos						= m_pRoomMesh->GetPosFromBone( CUSTOMER_START_POS_BONE_NAME );
	m_EndPos						= m_pRoomMesh->GetPosFromBone( CUSTOMER_END_POS_BONE_NAME );
	m_MoveVector					= m_EndPos - m_StartPos;
	m_CustomerTransform				= m_Transform;
	m_CustomerTransform.Position	= m_StartPos;
	m_WindowStartPos				= m_pRoomMesh->GetPosFromBone( WINDOW_START_POS_BONE_NAME );
	m_WindowEndPos					= m_pRoomMesh->GetPosFromBone( WINDOW_END_POS_BONE_NAME );
	m_WindowMoveVector				= m_WindowEndPos - m_WindowStartPos;
	m_WindowTransform				= m_Transform;
	m_WindowTransform.Position		= m_WindowStartPos;

	// 窓のメッシュの取得.
	m_pWindowMesh = CMeshResource::GetSkin( "room_window_s" );
	m_pWindowMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pWindowMesh->SetTransform( m_WindowTransform );

	// お客さんの初期化.
	m_pCustomer = std::make_unique<CCustomer>( EObjectTag::Apartment );
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetAddMoney( CONSTANT.APARTMENT_ADD_MONEY );
	m_pCustomer->SetDirection( GetRoomDirection() );
	m_pCustomer->Init();

	// ライトの設定.
	m_LightOnHour		= CRandom::GetRand( 16, 17 );
	m_LightOnMinute		= CRandom::GetRand( 0,  50 );
	m_LightOffHour		= CRandom::GetRand( 22, 24 );
	m_LightOffMinute	= CRandom::GetRand( 0,  50 );
	m_PointLight.Brightness = INIT_FLOAT;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CApartmentRoom::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// 距離によるアルファフェードの更新.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	
	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.6f;

	CustomerDispMove();		// お客さんを表示するための移動.
	CustomerHideMove();		// お客さんを非表示にするための移動.
	CustomerUpdate();		// お客さんの更新.
	IconUpdate();			// アイコンの更新.
	LightUpdate();			// ライトの更新.
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CApartmentRoom::LateUpdate( const float& DeltaTime )
{
	m_pCustomer->LateUpdate( DeltaTime );

	// お客さんをプレイヤーの方を見るように回転させる.
	CustomerPlayerLook();
}

//---------------------------.
// 描画.
//---------------------------.
void CApartmentRoom::Render()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	// 家の描画.
	m_pRoomMesh->SetDither( true );
	m_pRoomMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pRoomMesh->SetMeshPointLight( m_PointLight );
	m_pRoomMesh->Render( &m_Transform );
	m_pRoomMesh->SetDither( false );

	// 窓の描画.
	m_pWindowMesh->SetDither( true );
	m_pWindowMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pWindowMesh->SetMeshPointLight( m_PointLight );
	m_pWindowMesh->Render( &m_WindowTransform );
	m_pWindowMesh->SetDither( false );

	// お客さんの描画.
	if ( m_CustomerTransform.Position == m_StartPos ) return;
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetRotation( m_CustomerTransform.Rotation + m_CustomerAddRotation );
	m_pCustomer->SetMeshPointLight( m_PointLight );
	m_pCustomer->Render();
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CApartmentRoom::Sprite3DRender_A()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	m_pCustomer->Sprite3DRender_A();

	if ( m_pCustomer->GetIsLink() == false ) return;
	m_pIcon->Sprite3DRender_A();
}

//---------------------------.
// アルファの設定.
//---------------------------.
void CApartmentRoom::SetAlpha( const float Alpha )
{
	m_Alpha = Alpha;
	m_pCustomer->SetAlpha( m_Alpha );
}

//---------------------------.
// 部屋の向きを取得.
//---------------------------.
EDirection CApartmentRoom::GetRoomDirection()
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
void CApartmentRoom::IconUpdate()
{
	// アイコンの更新.
	if ( m_pCustomer->GetIsLink() == false ) return;
	m_pIcon->IconUpdate( m_pCustomer->GetHandle() );

	// アイコンのタグの更新.
	m_pIcon->SetTag( EObjectTag::Customer );
	if ( m_pCustomer->GetIsGoHandingCustomer() == false ) return;
	m_pIcon->SetTag(  EObjectTag::GoHandingCustomer );
}

//---------------------------.
// ライトの更新.
//---------------------------.
void CApartmentRoom::LightUpdate()
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
void CApartmentRoom::LightOn( const std::pair<int, int>& Time )
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
void CApartmentRoom::LightOff( const std::pair<int, int>& Time )
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
// お客さんの更新.
//---------------------------.
void CApartmentRoom::CustomerUpdate()
{
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetRotation( m_CustomerTransform.Rotation + m_CustomerAddRotation );
	m_pCustomer->Update( m_DeltaTime );
}

//---------------------------.
// お客さんの表示するための移動.
//---------------------------.
void CApartmentRoom::CustomerDispMove()
{
	if ( m_CustomerTransform.Position	== m_EndPos	) return;
	if ( m_pCustomer->GetIsLink()		== false	) return;

	// 窓を開ける.
	WindowOpen();

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
void CApartmentRoom::CustomerPlayerLook()
{
	if ( m_CustomerTransform.Position			!= m_EndPos ) return;
	if ( m_pCustomer->GetIsLink()				== false	) return;
	if ( m_pCustomer->GetIsGoHandingCustomer()	== false	) return;

	const float InitRot			= m_CustomerTransform.Rotation.y;	// 初期角度.
	const float InversionRot	= Math::ToRadian( INVERSION_DROT );	// 反転させる角度.

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
void CApartmentRoom::CustomerHideMove()
{
	if ( m_pCustomer->GetIsLink()							) return;
	if ( m_pCustomer->GetIsGetPizzaAnimEnd() == false		) return;
	if ( m_CustomerTransform.Position		 == m_StartPos	) {
		if ( m_IsCloseEnd ) return;
		// 窓を閉じる.
		WindowClose();
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
void CApartmentRoom::CustomerRotation()
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
// 窓を開ける.
//---------------------------.
void CApartmentRoom::WindowOpen()
{
	if ( m_IsOpenEnd ) return;

	// SEの再生.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( ROOM_OPEN_SE, m_WindowTransform.Position );
	}

	m_WindowMoveCnt += m_DeltaTime * CONSTANT.WINDOW_MOVE_SPEED;
	m_WindowTransform.Position = m_WindowStartPos + ( m_WindowMoveVector * m_WindowMoveCnt );

	// 移動終了.
	if ( m_WindowMoveCnt >= WINDOW_MOVE_MAX ) {
		m_WindowMoveCnt				= WINDOW_MOVE_MIN;
		m_WindowTransform.Position	= m_WindowEndPos;
		m_IsOpenEnd					= true;
		m_IsOpenSE					= false;
	}
}

//---------------------------.
// 窓を閉じたか.
//---------------------------.
void CApartmentRoom::WindowClose()
{
	if ( m_IsCloseEnd ) return;

	// SEの再生.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( ROOM_OPEN_SE, m_WindowTransform.Position );
	}

	m_WindowMoveCnt += m_DeltaTime * CONSTANT.WINDOW_MOVE_SPEED;
	m_WindowTransform.Position = m_WindowEndPos - ( m_WindowMoveVector * m_WindowMoveCnt );

	// 移動終了.
	if ( m_WindowMoveCnt >= WINDOW_MOVE_MAX ) {
		m_WindowMoveCnt				= WINDOW_MOVE_MIN;
		m_WindowTransform.Position	= m_WindowStartPos;
		m_IsCloseEnd				= true;
		m_IsOpenSE					= false;
	}
}
