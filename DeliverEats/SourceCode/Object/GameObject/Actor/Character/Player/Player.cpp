#include "Player.h"
#include "CustomerDire\CustomerDire.h"
#include "..\DeliveryManager\DeliveryManager.h"
#include "..\DeliveryManager\Customer\Customer.h"
#include "..\DeliveryManager\Employee\Employee.h"
#include "..\..\Wind\Wind.h"
#include "..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\Time\Time.h"
#include "..\..\..\..\Camera\MainCamera\MainCamera.h"
#include "..\..\..\..\Camera\GazingCamera\GazingCamera.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\Common\EffectManager\EffectManager.h"
#include "..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\..\..\..\Utility\Math\Math.h"
#include "..\..\..\..\..\Object\Score\Score.h"
#include "..\..\..\..\..\Scene\SceneManager\SceneManager.h"

namespace{
	const D3DXVECTOR3	GRAVITY_VECTOR				= { 0.0f, -1.0f, 0.0f };	// 重力ベクトル.
	constexpr float		GET_FOOD_ANIM_CNT_MAX		= 180.0f;					// 商品取得アニメーションのカウントの上限.

	constexpr char		JETPACK_CENTER_BONE[]		= "p_jet_center";			// ジェットパックの中心の位置のボーン名[Body].
	constexpr char		L_JETPACK_JETCENTER_BONE[]	= "jetpack_l_center";		// 左ジェットパックの噴射口のボーン名[LJetPack].
	constexpr char		L_JETPACK_JETPOS_BONE[]		= "jetpack_l_jet_place";	// 左ジェットパックの噴射口のボーン名[LJetPack].
	constexpr char		L_JETPACK_SETPOS_BONE[]		= "p_jet_l";				// 左ジェットパックの設置場所のボーン名[Body].
	constexpr char		R_JETPACK_JETCENTER_BONE[]	= "jetpack_r_center";		// 右ジェットパックの噴射口のボーン名[RJetPack].
	constexpr char		R_JETPACK_JETPOS_BONE[]		= "jetpack_r_jet_place";	// 右ジェットパックの噴射口のボーン名[RJetPack].
	constexpr char		R_JETPACK_SETPOS_BONE[]		= "p_jet_r";				// 左ジェットパックの設置場所のボーン名[Body].
	constexpr char		BAG_SETPOS_BONE[]			= "p_jet_center";			// リュックの設置場所のボーン名[Bag].
	constexpr char		LOOK_POS_BONE[]				= "neck";					// 店員/お客さんが見る位置.

	constexpr char		JETPACK_START_SE[]			= "JetPackStart";			// ジェットパックの開始音.
	constexpr char		JETPACK_SE[]				= "JetPack";				// ジェットパックの噴射音.
	constexpr char		MOVE_SE[]					= "Move";					// 移動.
	constexpr char		BUILDING_LANDING_SE[]		= "BuildingLanding";		// 建物に着地.
	constexpr char		GRASS_LANDING_SE[]			= "GrassLanding";			// 草に着地.
	constexpr char		EXPLOSION_SE[]				= "Explosion";				// 爆発.
	constexpr char		FOODPASSED_SE[]				= "FoodPassed";				// 商品を渡した.
	constexpr char		GETFOOD_SE[]				= "GetFood";				// 商品をもらった.
	constexpr char		PIZZA_KILN_SE[]				= "PizzaKiln";				// ピザ窯SE.

	// スキンメッシュのNo.
	enum enMeshNo : unsigned char {
		Mesh_Body,					// 表示するプレイヤー本体.
		Mesh_LJetPack,				// 表示する左ジェットパック.
		Mesh_RJetPack,				// 表示する右ジェットパック.
		Mesh_Bag,					// 表示するリュック.

		Mesh_Max,					// 表示するメッシュの数.

		Bone_Body = Mesh_Max,		// ボーン座標の取得で使用するプレイヤー本体.
		Bone_LJetPack,				// ボーン座標の取得で使用する左ジェットパック.
		Bone_RJetPack,				// ボーン座標の取得で使用する右ジェットパック.

		MeshList_Max,				// メッシュの個数.
	} typedef EMeshNo;

	// エフェクトNo.
	enum enEffectNo : unsigned char {
		Left_Smoke,					// 左の煙エフェクト.
		Right_Smoke,				// 右の煙エフェクト.
		Left_Flame,					// 左の炎エフェクト.
		Right_Flame,				// 右の炎エフェクト.
		Left_Explosion,				// 左の爆発エフェクト.
		Right_Explosion,			// 右の爆発エフェクト.

		JetPack_Effect_Max,			// ジェットパック関連のエフェクトの個数.

		Hit = JetPack_Effect_Max,	// 衝突エフェクト.

		Effect_Max					// エフェクトの個数.
	} typedef EEffectNo;

	// ベクトルNo.
	enum enVectorNo : unsigned char {
		Wind,						// 風ベクトル.
		Dire,						// 向きベクトル.
		Tilt,						// 傾きベクトル.
		OldMove,					// 前回の移動ベクトル.
		NowMove,					// 今回の移動ベクトル.
		LJetPackRot,				// 左ジェットパックの回転ベクトル.
		RJetPackRot,				// 右ジェットパックの回転ベクトル.
		LJetPackJet,				// 左ジェットパックの噴射ベクトル
		RJetPackJet,				// 右ジェットパックの噴射ベクトル
		
		Vector_Max					// エフェクトの個数.
	} typedef EVectorNo;

	// カウントNo.
	enum enCntNo : unsigned char {
		LandingEffect,				// 着地エフェクトを再生していく用のカウント.
		LandingAng,					// 着地アニメーション用の角度カウント.
		GetFoodAng,					// 商品取得アニメーション用の角度カウント.
		LJetPackStopTime,			// 左ジェットパックを停止させる時間用のカウント.
		RJetPackStopTime,			// 右ジェットパックを停止させる時間用のカウント.
		StageOutTime,				// ステージ外に出た時にシーンを切り替えるまでの時間用のカウント.

		Cnt_Max						// カウントの個数.
	} typedef ECntNo;

	// フラグNo.
	enum enFlagNo : unsigned char
	{
		IsHitGround,				// 地面に当たったか.
		IsHitPlayerIslandGround,	// プレイヤーの浮島の地面に当たったか.
		IsGroundClose,				// 地面が近くにあるか.
		IsHitCeiling,				// 天井に当たったか.
		IsHitWall,					// 壁に当たったか.
		IsOldHitGround,				// 前のフレームで地面に当たったか.
		IsOldHitCeiling,			// 前のフレームで天井に当たったか.
		IsOldHitWall,				// 前のフレームで壁に当たったか.
		IsGetFood,					// 商品を取得したか.
		IsIslandCollisionStop,		// 浮島の当たり判定を停止させるか.
		IsIsland,					// 浮島の周辺にいるか.
		IsLJetPackStop,				// 左ジェットパックを停止させるか.
		IsRJetPackStop,				// 右ジェットパックを停止させるか.
		IsActiveGazingCamera,		// 注視カメラが使用されているか.
		IsControlStop,				// 操作を停止させるか.
		IsPushGoHomeNotification,	// 帰宅通知を送ったか.
		IsDispShadow,				// 影を表示させるか.
		IsStageOut,					// ステージ外に移動したか.

		Flag_Max					// フラグの個数.
	} typedef EFlagNo;

	// レイNo.
	enum enRayNo : unsigned char {
		Ceiling,					// 天井用.
		GroundClose,				// 地面に近いか調べる用.
		LJetPackEffect,				// 左のジェットパックのエフェクト用.
		RJetPackEffect,				// 右のジェットパックのエフェクト用.

		Ray_Max						// レイの個数.
	} typedef ERayNo;

	// プレイヤーのアニメーションNo.
	enum enAnimationNo : unsigned char {
		StandBy,					// 待機.
		Fly,						// 飛ぶアニメーション.
		Landing,					// 着地アニメーション.
		WallCrash,					// 壁に衝突アニメーション.
		GroundCrash,				// 地面に衝突アニメーション.
		StageOut,					// ステージの外に移動したときのアニメーション.
		PizzGet,					// ピザを受け取る時のアニメーション.
		PizzPassed,					// ピザを渡す時のアニメーション.
	} typedef EAnimationNo;

	// SEリスト.
	enum enSENo : unsigned char {
		Move,						// 移動SE.
		MoveDown,					// 下に移動SE.

		SE_Max
	};
}

CPlayer::CPlayer()
	: m_pMainCamera				( nullptr )
	, m_pGazingCamera			( nullptr )
	, m_pRayList				( Ray_Max )
	, m_pRuckSack				( nullptr )
	, m_pCustomerDire			( nullptr )
	, m_pSkinMesh				( MeshList_Max )
	, m_MoveVectorList			()
	, m_VectorList				( Vector_Max,	INIT_FLOAT3 )
	, m_TransformList			( MeshList_Max )
	, m_LJetPackInputList		()
	, m_RJetPackInputList		()
	, m_CntList					( Cnt_Max,		INIT_FLOAT )
	, m_SEVolumeList			( SE_Max,		INIT_FLOAT )
	, m_FlagList				( Flag_Max,		false )
	, m_pShadow					( nullptr )
	, m_ShadowState				()
	, m_pEffectList				( Effect_Max )
	, m_pLandingEffectList		()
	, m_pOldCustomer			( nullptr )
	, m_AC						()
	, m_LandingEndPos			( INIT_FLOAT3 )
	, m_StartPosition			( INIT_FLOAT3 )
	, m_ShadowPosY				( INIT_FLOAT, INIT_FLOAT )
	, m_Alpha					( Color::ALPHA_MAX )
	, m_SideMoveSpeed			( INIT_FLOAT )
	, m_VerticalMoveSpeed		( INIT_FLOAT )
{
}

CPlayer::~CPlayer()
{
	CSoundManager::StopSE( JETPACK_SE,			true );
	CSoundManager::StopSE( MOVE_SE,				true );
	CSoundManager::StopSE( BUILDING_LANDING_SE,	true );
	CSoundManager::StopSE( GRASS_LANDING_SE,	true );
	CSoundManager::StopSE( EXPLOSION_SE,		true );
	CSoundManager::StopSE( FOODPASSED_SE,		true );
	CSoundManager::StopSE( GETFOOD_SE,			true );
	CSoundManager::StopSE( PIZZA_KILN_SE,		true );
}

//---------------------------.
// 初期化.
//---------------------------.
bool CPlayer::Init()
{
	// カメラの設定.
	m_pMainCamera = std::make_unique<CMainCamera>();
	m_pMainCamera->SetTarget( this );
	CCameraManager::SetCamera( m_pMainCamera.get() );
	
	// 注視カメラの設定.
	m_pGazingCamera = std::make_unique<CGazingCamera>();
	m_pGazingCamera->SetPlayer( this );

	// 商品の設定.
	m_pRuckSack = std::make_unique<CRuckSack>();
	m_pRuckSack->SetTilt( &m_Transform.Rotation.z );
	m_pRuckSack->Init();

	// 渡しに行くお客さんの方向の設定.
	m_pCustomerDire = std::make_unique<CCustomerDire>();
	m_pCustomerDire->SetPlayer( this );
	m_pCustomerDire->Init();

	// メッシュの取得.
	m_pSkinMesh[Mesh_Body]		= CMeshResource::GetSkin( "player_s", &m_AC );
	m_pSkinMesh[Mesh_LJetPack]	= CMeshResource::GetSkin( "l_jet_s" );
	m_pSkinMesh[Mesh_RJetPack]	= CMeshResource::GetSkin( "r_jet_s" );
	m_pSkinMesh[Mesh_Bag]		= CMeshResource::GetSkin( "p_bag_s" );
	m_pSkinMesh[Bone_Body]		= m_pSkinMesh[Mesh_Body];
	m_pSkinMesh[Bone_LJetPack]	= m_pSkinMesh[Mesh_LJetPack];
	m_pSkinMesh[Bone_RJetPack]	= m_pSkinMesh[Mesh_RJetPack];
	m_pSkinMesh[Mesh_Body]->SetAnimSpeed( 0.001f );

	// エフェクトの設定.
	for( auto& e : m_pEffectList ) e = std::make_unique<CEffectManager>();
	m_pEffectList[Left_Flame]->SetEffect( "Flame" );
	m_pEffectList[Left_Smoke]->SetEffect( "Smoke" );
	m_pEffectList[Left_Explosion]->SetEffect( "Explosion" );
	m_pEffectList[Right_Flame]->SetEffect( "Flame" );
	m_pEffectList[Right_Smoke]->SetEffect( "Smoke" );
	m_pEffectList[Right_Explosion]->SetEffect( "Explosion" );
	m_pEffectList[Hit]->SetEffect( "Hit" );

	// 影の画像の取得.
	m_pShadow = CSpriteResource::GetSprite( "RoundShadow", &m_ShadowState );
	m_ShadowState.Transform.Rotation.x	= Math::ToRadian( 90.0f );
	m_ShadowState.Transform.Scale		= { 0.35f, 0.35f, 0.35f };

	// 浮島の当たり判定を停止させる.
	m_FlagList[IsHitGround] = true;

	// ジェットパックの入力リストの設定.
	m_LJetPackInputList.resize( CONSTANT.JETPACK_INPUT_LIST_MAX );
	m_RJetPackInputList.resize( CONSTANT.JETPACK_INPUT_LIST_MAX );

	// 向きの設定.
	for ( auto& t : m_TransformList ) {
		t.Rotation.y = Math::ToRadian( -90.0f );
	}

	// プレイヤーの設定.
	CDeliveryManager::SetPlayer( this );

	// タグの設定.
	m_ObjectTag = EObjectTag::Player;

#ifndef _DEBUG
	// コントローラの振動を使用する.
	CXInput::SetIsVibration( true );
#endif	// #ifndef _DEBUG.

	// ミニマップにプレイヤーのトランスフォームを設定.
	CMiniMapUI::SetPlayer( this );

	// オーディオの設定.
	CSoundManager::SetListener( this );
	CSoundManager::SetSelectNo( 0 );
	CSoundManager::SetIsSERightON(	JETPACK_SE,			false );
	CSoundManager::SetIsSERightON(	JETPACK_START_SE,	false );
	CSoundManager::SetIsSERightON(	EXPLOSION_SE,		false );
	CSoundManager::SetSelectNo( 1 );
	CSoundManager::SetIsSELeftON(	JETPACK_SE,			false );
	CSoundManager::SetIsSELeftON(	JETPACK_START_SE,	false );
	CSoundManager::SetIsSELeftON(	EXPLOSION_SE,		false );
	CSoundManager::ResetSelectNo();

	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CPlayer::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	JetPackRotationControl();				// ジェットパックの回転.
	JetPackJetControl();					// ジェットパックの噴射.
	JetPackStop();							// ジェットパックの停止.
	JetPackRepair();						// ジェットパックの停止の修復.

	AddGravity();							// 重力を追加する.

	CreateMoveVec();						// 移動ベクトルの作成.
	CreateRotVec();							// 左右のジェットパックの回転ベクトルの作成.

	AccelerationClamp();					// 加速度の上限/下限の設定.
	Friction( CONSTANT.AIR_FRICTION );		// 空気摩擦.

	PlayerMove();							// プレイヤーの移動.
	PlayerRotation();						// プレイヤーの回転.

	PlayerTiltUpdate();						// プレイヤーの傾きの更新.

	LandingEffectUpdate();					// 着地エフェクトの更新.
	GetFoodAnimUpdate();					// 商品取得アニメーションの更新.

	StageOutUpdate();						// ステージ外に移動した時の更新.
	PlayMoveSE();							// 移動のSEの再生.

	m_pRuckSack->Update( DeltaTime );		// リュックの更新.
	m_pCustomerDire->Update( DeltaTime );	// 渡しに行くお客さんの方向の更新.

	m_FlagList[IsIsland] = false;

	UpdateCollision();
	CActorCollisionManager::PushObject( this );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CPlayer::LateUpdate( const float& DeltaTime )
{
	// 注視カメラの注視点を変更.
	CCustomer* pGoHandingCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( pGoHandingCustomer != nullptr ) {
		m_pGazingCamera->SetTarget( pGoHandingCustomer );
	}

	// カメラの更新.
	CameraControl();
	// 1F前の渡しに行くべき客を保存.
	m_pOldCustomer = pGoHandingCustomer;

	// アニメーションの変更.
	if ( m_FlagList[IsControlStop] == false ) {
		// 地面が近くの時のアニメーションに変更.
		if ( m_FlagList[IsGroundClose] ) {
			m_pSkinMesh[Mesh_Body]->ChangeAnimBlend( Landing, &m_AC );
		}
		// 空を飛んでいる時のアニメーションに変更.
		else if ( m_FlagList[IsHitGround] == false ) {
			m_pSkinMesh[Mesh_Body]->ChangeAnimBlend( Fly, &m_AC );
		}
	}

	// 影の更新.
	if ( m_FlagList[IsDispShadow] ) {
		m_ShadowState.IsDisp = true;

		// 位置の更新.
		m_ShadowState.Transform.Position	= m_TransformList[Mesh_Body].Position;
		m_ShadowState.Transform.Position.y	= m_ShadowPosY.second + 0.1f;

		// アルファ値の更新.
		const float DistanceRate	= m_ShadowPosY.first / CONSTANT.SHADOW_DISP_DISTANCE;
		m_ShadowState.Color.w		= 1.0f - DistanceRate;

		// 拡縮.
		m_ShadowState.Transform.Scale = CONSTANT.SHADOW_MIN_SCALE + CONSTANT.SHADOW_ADD_ACALE * DistanceRate;
	}
	else m_ShadowState.IsDisp = false;

	// フラグの初期化.
	m_FlagList[IsOldHitGround]			= m_FlagList[IsHitGround];
	m_FlagList[IsOldHitCeiling]			= m_FlagList[IsHitCeiling];
	m_FlagList[IsOldHitWall]			= m_FlagList[IsHitWall];
	m_FlagList[IsHitGround]				= false;
	m_FlagList[IsHitPlayerIslandGround]	= false;
	m_FlagList[IsGroundClose]			= false;
	m_FlagList[IsHitCeiling]			= false;
	m_FlagList[IsHitWall]				= false;
	m_FlagList[IsDispShadow]			= false;
}

//---------------------------.
// デバックの更新.
//---------------------------.
void CPlayer::DebugUpdate( const float& DeltaTime )
{
	CDebugWindow::PushProc( "Player",
	[&](){
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"座標        : %5f, %5f, %5f",	m_Transform.Position.x, m_Transform.Position.y, m_Transform.Position.z );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"向きベクトル : %5f, %5f, %5f",	m_VectorList[Dire].x,		m_VectorList[Dire].y,		m_VectorList[Dire].z );
		ImGui::Text( u8"傾きベクトル : %5f, %5f, %5f",	m_VectorList[Tilt].x,		m_VectorList[Tilt].y,		m_VectorList[Tilt].z );
		ImGui::Text( u8"移動ベクトル : %5f, %5f, %5f",	m_VectorList[NowMove].x,	m_VectorList[NowMove].y,	m_VectorList[NowMove].z );
		ImGui::Text( u8" 風 ベクトル : %5f, %5f, %5f",	m_VectorList[Wind].x,		m_VectorList[Wind].y,		m_VectorList[Wind].z );
		ImGui::Text( u8"移動速度(左右): %5f",				m_SideMoveSpeed );
		ImGui::Text( u8"移動速度(上下): %5f",				m_VerticalMoveSpeed );
		ImGui::Text( u8"------------------------------" );
	} );
	m_pRuckSack->DebugUpdate( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CPlayer::Render()
{
	TransformUpdate();

	// プレイヤーの影の描画.
	ShadowRender();

	// プレイヤーの描画.
	SetRuckSackColor();
	for ( int i = 0; i < Mesh_Max; ++i ) {
		m_pSkinMesh[i]->Render( &m_TransformList[i], &m_AC );
	}

	// 渡しに行くお客さんの方向の描画.
	m_pCustomerDire->Render();

	CCollisionRender::PushRay( m_pCollisions->GetCollision<CRay>() );
	CCollisionRender::PushRay( m_pCollisions->GetCollision<CCrossRay>() );
	for( auto& r : m_pRayList ) CCollisionRender::PushRay( r.get() );
	CCollisionRender::PushPoint( m_Transform.Position );
}

//---------------------------.
// 影の描画.
//---------------------------.
void CPlayer::ShadowRender()
{
	// プレイヤーを黒色の半透明で描画.
	CDirectX11::SetDepth( false );
	for ( int i = 0; i < Mesh_Max; ++i ) {
		m_pSkinMesh[i]->SetColor( Color::Black );
		m_pSkinMesh[i]->SetAlpha( CONSTANT.SHADOW_ALPHA );
		m_pSkinMesh[i]->SetDither( true );

		m_pSkinMesh[i]->Render( &m_TransformList[i], &m_AC );

		m_pSkinMesh[i]->SetDither( false );
		m_pSkinMesh[i]->SetColor( Color::White );
		m_pSkinMesh[i]->SetAlpha( Color::ALPHA_MAX );
	}
	CDirectX11::SetDepth( true );
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CPlayer::Sprite3DRender_A()
{
	// エフェクトの位置の更新.
	const D3DXPOSITION3 PosList[] = {
		m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ),
		m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE )
	};
	const D3DXROTATION3 RotList[] = {
		m_TransformList[Mesh_RJetPack].Rotation,
		m_TransformList[Mesh_LJetPack].Rotation
	};
	for ( int i = 0; i < JetPack_Effect_Max; ++i ) {
		m_pEffectList[i]->SetPosition( PosList[(i + 1) % 2] );
		m_pEffectList[i]->SetRotation( RotList[(i + 1) % 2] );
	}

	// エフェクトの描画.
	for ( int i = 0; i < Effect_Max; ++i ) {
		m_pEffectList[i]->Render();
	}
	for ( auto& e : m_pLandingEffectList ) e->Render();

	m_pShadow->Render3D( &m_ShadowState );
}

//---------------------------.
// 当たり判定関数.
//---------------------------.
void CPlayer::Collision( CActor* pActor )
{
	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::ApartmentA &&
		 Tag != EObjectTag::ApartmentB &&
		 Tag != EObjectTag::Island &&
		 Tag != EObjectTag::PlayerIsland &&
		 Tag != EObjectTag::IslandObject ) return;

	IslandCollision(	pActor );	// 浮島の当たり判定.

	// 浮島の当たり判定を行わない場合抜ける.
	if ( ( Tag == EObjectTag::Island || Tag == EObjectTag::PlayerIsland || Tag == EObjectTag::IslandObject ) &&
		m_FlagList[IsIslandCollisionStop] ) return;

	JetPackEffectCollision( pActor );	// ジェットパックのエフェクト用の当たり判定.
	GroundCollision(		pActor );	// 地面の当たり判定.
	GroundCloseCollision(	pActor );	// 地面が近くにあるか調べる当たり判定.
	ShadowCollision(		pActor );	// 影用の当たり判定.
	CeilingCollision(		pActor );	// 天井の当たり判定.
	WallCollision(			pActor );	// 壁の当たり判定.
	IsPlayerHouseCollision( pActor );	// プレイヤーの家の当たり判定.
}

//---------------------------.
// 商品を渡す.
//---------------------------.
void CPlayer::FoodPassed()
{
	// スコアの加算.
	CScore::SetScore( m_pRuckSack->GetFoodState() );

	// リュックの中を空にする.
	m_pRuckSack->FoodPassed();

	// 渡すアニメーションに変更.
	m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( PizzPassed, &m_AC );

	// SEを再生.
	CSoundManager::PlaySE( FOODPASSED_SE );
}

//---------------------------.
// 向きベクトルの取得.
//---------------------------.
D3DXVECTOR3 CPlayer::GetDireVector()
{
	return m_VectorList[Dire];
}

//---------------------------.
// 店員/お客さんが見る位置を取得.
//---------------------------.
D3DXPOSITION3 CPlayer::GetLookPos()
{
	m_pSkinMesh[Mesh_Body]->SetTransform( m_TransformList[Mesh_Body] );

	// 座標を少し下にする.
	D3DXPOSITION3 LookPos = m_pSkinMesh[Mesh_Body]->GetPosFromBone( LOOK_POS_BONE );
	LookPos.y -= CONSTANT.LOOK_POS_ADD_Y;
	return LookPos;
}

//---------------------------.
// 着地しているかを取得.
//---------------------------.
bool CPlayer::GetIsLanding()
{
	return m_FlagList[IsHitGround];
}

//---------------------------.
// 浮島の近くにいるか取得.
//---------------------------.
bool CPlayer::GetIsIsland()
{
	return m_FlagList[IsIsland];
}

//---------------------------.
// 注視カメラを使用しているかを取得.
//---------------------------.
bool CPlayer::GetIsActiveGazingCam()
{
	return m_FlagList[IsActiveGazingCamera];
}

//---------------------------.
// 開始位置に設定.
//---------------------------.
void CPlayer::SetStartPosition( const D3DXPOSITION3& Pos )
{
	// 配置する場所の作成.
	m_StartPosition = Pos;
	m_StartPosition.x += 10.0f;
	m_StartPosition.y += CONSTANT.VERTICAL_RAY_LENGTH;

	// 座標の設定.
	m_TransformList[Mesh_Body].Position		= m_StartPosition;
	TransformUpdate();
}

//---------------------------.
//　商品を受け取る.
//---------------------------.
void CPlayer::SetNewFood( const int Money, int ShopNo )
{
	m_FlagList[IsGetFood] = true;

	// 商品を設定する.
	m_pRuckSack->SetNewFood( Money, ShopNo );

	// 受け取りアニメーションに変更.
	m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( PizzGet, &m_AC );

	// SEを再生.
	CSoundManager::PlaySE( GETFOOD_SE );
}

//---------------------------.
// 風の設定.
//---------------------------.
void CPlayer::SetWind( const D3DXVECTOR3& Vec )
{
	m_VectorList[Wind] = Vec;
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CPlayer::InitCollision()
{
	// 下方向のレイの初期化.
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Ray );
	m_pCollisions->GetCollision<CRay>()->SetVector( Math::_Y_VECTOR );
	m_pCollisions->GetCollision<CRay>()->SetLength( CONSTANT.VERTICAL_RAY_LENGTH );

	// レイの作成.
	for ( auto& r : m_pRayList ) r = std::make_unique<CRay>();

	// 地面に近いか調べる用のレイの初期化.
	m_pRayList[GroundClose]->SetVector( Math::_Y_VECTOR );
	m_pRayList[GroundClose]->SetLength( CONSTANT.GROUND_CLOSE_RAY_LENGTH );

	// 横方向のレイの初期化.
	m_pCollisions->InitCollision( ECollType::CrossRay );
	m_pCollisions->GetCollision<CCrossRay>()->SetLength( CONSTANT.SIDE_RAY_LENGTH );

	// 上方向のレイの初期化.
	m_pRayList[Ceiling]->SetVector( Math::Y_VECTOR );
	m_pRayList[Ceiling]->SetLength( CONSTANT.VERTICAL_RAY_LENGTH );

	// ジェットパックのエフェクト用のレイの初期化.
	m_pRayList[LJetPackEffect]->SetVector( Math::_Y_VECTOR );
	m_pRayList[RJetPackEffect]->SetVector( Math::_Y_VECTOR );
	m_pRayList[LJetPackEffect]->SetLength( CONSTANT.JETPACK_EFFECT_RAY_LENGTH );
	m_pRayList[RJetPackEffect]->SetLength( CONSTANT.JETPACK_EFFECT_RAY_LENGTH );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CPlayer::UpdateCollision()
{
	m_pCollisions->GetCollision<CRay>()->SetPosition( m_TransformList[Mesh_Body].Position );
	m_pCollisions->GetCollision<CCrossRay>()->SetPosition( m_TransformList[Mesh_Body].Position );
	m_pRayList[Ceiling]->SetPosition( m_TransformList[Mesh_Body].Position );
	m_pRayList[GroundClose]->SetPosition( m_TransformList[Mesh_Body].Position );


	m_pSkinMesh[Mesh_LJetPack]->SetTransform( m_TransformList[Mesh_LJetPack] );
	const D3DXPOSITION3& LStartPos	= m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETCENTER_BONE );
	const D3DXPOSITION3& LEndPos	= m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE );
	D3DXVECTOR3 LVector = LEndPos - LStartPos;
	D3DXVec3Normalize( &LVector, &LVector );
	m_pRayList[LJetPackEffect]->SetVector( LVector );
	m_pRayList[LJetPackEffect]->SetLength( CONSTANT.JETPACK_EFFECT_RAY_LENGTH * CXInput::GetLTrigger() );
	m_pRayList[LJetPackEffect]->SetPosition( m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ) );

	m_pSkinMesh[Mesh_RJetPack]->SetTransform( m_TransformList[Mesh_RJetPack] );
	const D3DXPOSITION3& RStartPos	= m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETCENTER_BONE );
	const D3DXPOSITION3& REndPos	= m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE );
	D3DXVECTOR3 RVector = REndPos - RStartPos;
	D3DXVec3Normalize( &RVector, &RVector );
	m_pRayList[RJetPackEffect]->SetVector( RVector );
	m_pRayList[RJetPackEffect]->SetLength( CONSTANT.JETPACK_EFFECT_RAY_LENGTH * CXInput::GetRTrigger() );
	m_pRayList[RJetPackEffect]->SetPosition( m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ) );
}

//---------------------------.
// 浮島の当たり判定.
//---------------------------.
void CPlayer::IslandCollision( CActor* pActor )
{
	if ( pActor->GetObjectTag() != EObjectTag::Island &&
		 pActor->GetObjectTag() != EObjectTag::PlayerIsland ) return;

	// 当たっているか.
	m_FlagList[IsIslandCollisionStop] = !Coll::IsSphereToPosition(
		pActor->GetCollision<CSphere>(),
		m_TransformList[Mesh_Body].Position );

	if ( m_FlagList[IsIslandCollisionStop] ) return;
	m_FlagList[IsIsland] = true;
}

//---------------------------.
// プレイヤーの家の着地の当たり判定.
//---------------------------.
void CPlayer::IsPlayerHouseCollision( CActor* pActor )
{
	if ( CTime::GetIsGameEnd()					== false ) return;
	if ( m_FlagList[IsIslandCollisionStop]				 ) return;
	if ( m_FlagList[IsHitPlayerIslandGround]	== false ) return;
	if ( m_pRuckSack->GetIsFoodHave()					 ) return;

	// 操作の停止を止める.
	CInput::SetIsUpdateStop( false );

	// シーン切り替え.
	CWind::StopWind();
	CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade, 0.01f );
}

//---------------------------.
// ジェットパックのエフェクトの当たり判定.
//---------------------------.
void CPlayer::JetPackEffectCollision( CActor* pActor )
{
	// 左のジェットパックが地面に当たっているか.
	D3DXPOSITION3 pIntersect = INIT_FLOAT3;
	if ( Coll::IsRayToMesh(
		m_pRayList[LJetPackEffect].get(),
		pActor->GetCollision<CMesh>(),
		nullptr, &pIntersect ) )
	{
		// エフェクトの再生.
		const D3DXPOSITION3& EffectPos = pIntersect + pActor->GetCollision<CMesh>()->GetPosition();
		LandingEffectPlay( EffectPos );
	}

	// 右のジェットパックが地面に当たっているか.
	pIntersect = INIT_FLOAT3;
	if ( Coll::IsRayToMesh(
		m_pRayList[RJetPackEffect].get(),
		pActor->GetCollision<CMesh>(),
		nullptr, &pIntersect ) )
	{
		// エフェクトの再生.
		const D3DXPOSITION3& EffectPos = pIntersect + pActor->GetCollision<CMesh>()->GetPosition();
		LandingEffectPlay( EffectPos );
	}
}

//---------------------------.
// 地面の当たり判定.
//---------------------------.
void CPlayer::GroundCollision( CActor* pActor )
{
	if ( m_FlagList[IsHitGround]		) return;

	// 当たっているか.
	D3DXPOSITION3 pIntersect = INIT_FLOAT3;
	m_FlagList[IsHitGround] = Coll::IsRayToMesh(
		m_pCollisions->GetCollision<CRay>(),
		pActor->GetCollision<CMesh>(),
		nullptr, &pIntersect );
	if ( m_FlagList[IsHitGround] == false ) return;
	m_FlagList[IsGroundClose] = false;

	// プレイヤーの浮島か.
	if ( pActor->GetObjectTag() == EObjectTag::PlayerIsland ) {
		m_FlagList[IsHitPlayerIslandGround] = true;
	}

	// 着地アニメーションに変更.
	if ( m_FlagList[IsControlStop] == false ) {
		m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( StandBy, &m_AC );
	}

	// 地面に着地.
	const float	HitY		= pIntersect.y + pActor->GetCollision<CMesh>()->GetPosition().y + m_pCollisions->GetCollision<CRay>()->GetLength();
	const bool	IsFriction	= Math::Round_n( HitY, 2 ) == Math::Round_n( m_TransformList[Mesh_Body].Position.y, 2 );
	m_TransformList[Mesh_Body].Position.y	= HitY;
	m_VectorList[NowMove].y					= INIT_FLOAT;
	m_pCollisions->GetCollision<CCrossRay>()->SetPosition( m_TransformList[Mesh_Body].Position );
	// エフェクトの再生.
	const D3DXPOSITION3& EffectPos = pIntersect + pActor->GetCollision<CMesh>()->GetPosition();
	LandingEffectPlay( EffectPos );

	// 地面摩擦.
	const float Power = IsFriction ? CONSTANT.GROUND_FRICTION : CONSTANT.GROUND_FRICTION / 2.0f;
	Friction( Power );

	// 着地SEの再生.
	if ( m_VerticalMoveSpeed >= CONSTANT.LANDING_SE_PLAY_SPEED ) {
		const EObjectTag Tag	= pActor->GetObjectTag();
		std::string		 SEName	= "";
		if ( Tag == EObjectTag::Island || Tag == EObjectTag::PlayerIsland ) SEName = GRASS_LANDING_SE;
		else																SEName = BUILDING_LANDING_SE;
		CSoundManager::PlaySE( SEName );
	}

	// 一定速度以上で着地したか.
	if ( m_FlagList[IsControlStop] ) return;
	if ( m_VerticalMoveSpeed >= CONSTANT.DEAD_GROUND_SPEED && m_FlagList[IsOldHitGround] == false ) {
		// 衝突エフェクトの再生.
		m_pEffectList[Hit]->Play( EffectPos );
		CSoundManager::PlaySE( "Hit" );

		// 操作を停止させる.
		m_FlagList[IsControlStop] = true;
		CInput::SetIsUpdateStop( true );

		// 移動ベクトルを無くす.
		m_VectorList[NowMove] = INIT_FLOAT3;
		m_VectorList[OldMove] = INIT_FLOAT3;
		m_MoveVectorList.clear();

		// カメラを衝突した時点で固定する.
		if ( m_FlagList[IsActiveGazingCamera] ) m_pGazingCamera->SetTarget( nullptr );
		else									m_pMainCamera->SetTarget( nullptr );

		// 治療費を発生させる.
		CScore::SetGameEnd();

		// 衝突アニメーションに変更する.
		m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( GroundCrash, &m_AC );
		m_pSkinMesh[Mesh_Body]->SetAnimSpeed( 0.02f );

		// ヒットストップの設定.
		CCameraManager::HitStop( 1.0f, true,
		// 終了時.
		[&] () {
			// 操作の停止を止める.
			CInput::SetIsUpdateStop( false );

			// シーン切り替え.
			CWind::StopWind();
			CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade, 0.01f );
		}, 
		// スローに変更時.
		[&] () {
			// SEの再生する.
			CSoundManager::SetSEPlaySpeed( "HitEnd", 500 );
			CSoundManager::PlaySE( "HitEnd" );
			CSoundManager::PlaySE( "Ambulance" );
		} );
	}
}

//---------------------------.
// 地面に近いかの当たり判定.
//---------------------------.
void CPlayer::GroundCloseCollision( CActor* pActor )
{
	if ( m_FlagList[IsControlStop]	) return;
	if ( m_FlagList[IsHitGround]	) return;
	if ( m_FlagList[IsGroundClose]	) return;

	// 当たっているか.
	m_FlagList[IsGroundClose] = Coll::IsRayToMesh(
		m_pRayList[GroundClose].get(),
		pActor->GetCollision<CMesh>() );
	if( m_FlagList[IsGroundClose] == false ) return;
}

//---------------------------.
// 影の当たり判定.
//---------------------------.
void CPlayer::ShadowCollision( CActor* pActor )
{
	// 当たっているか.
	float			Distance	= INIT_FLOAT;
	D3DXPOSITION3	pIntersect	= INIT_FLOAT3;
	if( Coll::IsRayToMesh(
		m_pRayList[GroundClose].get(),
		pActor->GetCollision<CMesh>(),
		&Distance, &pIntersect ) == false ) return;

	// 地面の位置を設定.
	const float	HitY = pIntersect.y + pActor->GetCollision<CMesh>()->GetPosition().y;
	if ( m_FlagList[IsDispShadow] ){
		if ( Distance < m_ShadowPosY.first ) m_ShadowPosY = std::make_pair( Distance, HitY );
		return;
	}
	m_FlagList[IsDispShadow]	= true;
	m_ShadowPosY				= std::make_pair( Distance, HitY );
}

//---------------------------.
// 天井の当たり判定.
//---------------------------.
void CPlayer::CeilingCollision( CActor* pActor )
{
	if ( m_FlagList[IsHitCeiling] ) return;

	// 当たっているか.
	D3DXPOSITION3 pIntersect = INIT_FLOAT3;
	m_FlagList[IsHitCeiling] = Coll::IsRayToMesh(
		m_pRayList[Ceiling].get(),
		pActor->GetCollision<CMesh>(),
		nullptr, &pIntersect, nullptr, true, CONSTANT.VERTICAL_RAY_LENGTH );
	if( m_FlagList[IsHitCeiling] == false ) return;

	// レイとモデルの座標の更新.
	const D3DXPOSITION3& RayPos = m_pRayList[Ceiling]->GetPosition();
	m_TransformList[Mesh_Body].Position.x -= ( m_TransformList[Mesh_Body].Position.x - RayPos.x ) / CONSTANT.CEILING_MOVE_SPEED;
	m_TransformList[Mesh_Body].Position.z -= ( m_TransformList[Mesh_Body].Position.z - RayPos.z ) / CONSTANT.CEILING_MOVE_SPEED;

	// 天井にぶつかった.
	const float	HitY = pActor->GetCollision<CMesh>()->GetPosition().y + pIntersect.y - CONSTANT.VERTICAL_RAY_LENGTH;
	m_TransformList[Mesh_Body].Position.y	= HitY;
	m_pCollisions->GetCollision<CCrossRay>()->SetPosition( m_TransformList[Mesh_Body].Position );
}

//---------------------------.
// 壁の当たり判定.
//---------------------------.
void CPlayer::WallCollision( CActor* pActor )
{
	// 当たっているか.
	std::vector<int> HitList;
	if ( Coll::IsCrossRayToWallMesh(
		m_pCollisions->GetCollision<CCrossRay>(),
		pActor->GetCollision<CMesh>(),
		&HitList ) == false ) return;

	// 壁にずれて進むように設定.
	const D3DXPOSITION3& RayPos = m_pCollisions->GetCollision<CCrossRay>()->GetPosition();
	m_TransformList[Mesh_Body].Position.x	= RayPos.x;
	m_TransformList[Mesh_Body].Position.z	= RayPos.z;

	// 壁に当たっている方向への力を無くす.
	for ( auto& h : HitList ) {
		const ECrossRayDire& Dire = static_cast<ECrossRayDire>( h );
		if ( Dire == ECrossRayDire::ZF && m_VectorList[NowMove].z < 0.0f ) m_VectorList[NowMove].z = 0.0f;
		if ( Dire == ECrossRayDire::ZB && m_VectorList[NowMove].z > 0.0f ) m_VectorList[NowMove].z = 0.0f;
		if ( Dire == ECrossRayDire::XL && m_VectorList[NowMove].x > 0.0f ) m_VectorList[NowMove].x = 0.0f;
		if ( Dire == ECrossRayDire::XR && m_VectorList[NowMove].x < 0.0f ) m_VectorList[NowMove].x = 0.0f;
	}

	// 操作を停止している場合は衝突判定を行わない.
	if ( m_FlagList[IsControlStop] ) {
		m_FlagList[IsHitWall] = true;
		return;
	}

	// 一定速度以上で衝突したか.
	if ( m_SideMoveSpeed >= CONSTANT.DEAD_WALL_SPEED && m_FlagList[IsHitWall] == false && m_FlagList[IsOldHitWall] == false ) {
		// エフェクトの用の回転を作成.
		D3DXROTATION3 EffectRot = INIT_FLOAT3;
		for ( auto& h : HitList ) {
			const ECrossRayDire& Dire = static_cast<ECrossRayDire>( h );
			if ( Dire == ECrossRayDire::ZF || Dire == ECrossRayDire::ZB ) EffectRot.x = Math::ToRadian( 90.0f );
			if ( Dire == ECrossRayDire::XL || Dire == ECrossRayDire::XR ) EffectRot.z = Math::ToRadian( 90.0f );
			break;
		}

		// 衝突エフェクトの再生.
		m_pEffectList[Hit]->Play( m_TransformList[Mesh_Body].Position, EffectRot );
		CSoundManager::PlaySE( "Hit" );

		// 操作を停止させる.
		m_FlagList[IsControlStop] = true;
		CInput::SetIsUpdateStop( true );

		// 移動ベクトルを無くす.
		m_VectorList[NowMove] = INIT_FLOAT3;
		m_VectorList[OldMove] = INIT_FLOAT3;
		m_MoveVectorList.clear();

		// カメラを衝突した時点で固定する.
		if ( m_FlagList[IsActiveGazingCamera] ) m_pGazingCamera->SetTarget( nullptr );
		else									m_pMainCamera->SetTarget( nullptr );

		// 治療費を発生させる.
		CScore::SetGameEnd();

		// 衝突アニメーションに変更する.
		m_pSkinMesh[Mesh_Body]->ChangeAnimSet( WallCrash, &m_AC );
		m_pSkinMesh[Mesh_Body]->SetAnimSpeed( 0.02f );

		// ヒットストップの設定.
		CCameraManager::HitStop( 1.0f, true,
		// 終了時.
		[&] () {
			// 操作の停止を止める.
			CInput::SetIsUpdateStop( false );

			// シーン切り替え.
			CWind::StopWind();
			CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade, 0.01f );
		}, 
		// スローに変更時.
		[&] () {
			// SEの再生する.
			CSoundManager::SetSEPlaySpeed( "HitEnd", 500 );
			CSoundManager::PlaySE( "HitEnd" );
			CSoundManager::PlaySE( "Ambulance" );
		} );
	}

	// 壁に当たった.
	m_FlagList[IsHitWall] = true;
}

//---------------------------.
// カメラの更新.
//---------------------------.
void CPlayer::CameraControl()
{
	// 操作可能でないなら処理を行わない.
	if ( m_FlagList[IsControlStop] ) return;

	// 渡しに行くべき客を取得.
	CCustomer* pGoHandingCustomer = CDeliveryManager::GetGoHandingCustomer();
	const bool IsJustSend = m_pOldCustomer != nullptr && pGoHandingCustomer == nullptr ? true : false;
	// 注視カメラが有効化されていて、お客さんに商品を渡し終えた瞬間なら.
	if ( m_FlagList[IsActiveGazingCamera] && IsJustSend ) {
		// 注視カメラからメインカメラに切り替える.
		SwitchToMain();
		return;
	}

	// カメラが切り替え中なら以後の処理を行わない.
	if ( m_pMainCamera->GetIsSwitching() || m_pGazingCamera->GetIsSwitching() )return;
	// Bボタンが押されていないなら以後の処理は行わない.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false )return;
	// 注視カメラが有効化されていて、Bボタンが押されたなら.
	if ( m_FlagList[IsActiveGazingCamera] ) {
		// 注視カメラからメインカメラに切り替える.
		SwitchToMain();
		return;
	}

	// 渡しに行くべき客がいないなら以後の処理は行わない.
	if ( pGoHandingCustomer == nullptr )return;
	// 渡しに行くべき客がいるときにBボタンが押されたなら.
	// メインカメラから注視カメラに切り替える.
	SwitchToGazing( *pGoHandingCustomer );
}

//---------------------------.
// 注視カメラからメインカメラに切り替える.
//---------------------------.
void CPlayer::SwitchToMain()
{
	// 注視カメラを無効化する.
	m_FlagList[IsActiveGazingCamera] = false;
	// 切り替えを開始する.
	m_pMainCamera->StartMove();
	// メインカメラに注視カメラの設定を渡す.
	m_pMainCamera->SetCameraState( m_pGazingCamera->GetCameraState() );
	m_pMainCamera->SetTarget( this );
	m_pMainCamera->SetCustomer( m_pGazingCamera->GetTarget() );
	// 使用するカメラをメインカメラに設定.
	CCameraManager::SetCamera( m_pMainCamera.get() );
}

//---------------------------.
// メインカメラから注視カメラに切り替える.
//---------------------------.
void CPlayer::SwitchToGazing( CActor& pCustmer )
{
	// 注視カメラを有効化する.
	m_FlagList[IsActiveGazingCamera] = true;
	// 切り替えを開始する.
	m_pGazingCamera->StartMove();
	// 注視カメラにメインカメラの設定を渡す.
	m_pGazingCamera->SetCameraState( m_pMainCamera->GetCameraState() );
	m_pGazingCamera->SetTarget( &pCustmer );
	m_pGazingCamera->SetRotX( m_pMainCamera->GetRotX() );
	// 使用するカメラを注視カメラに設定.
	CCameraManager::SetCamera( m_pGazingCamera.get() );
}

//---------------------------.
// トランスフォームの更新.
//---------------------------.
void CPlayer::TransformUpdate()
{
	// プレイヤー本体のモデルの座標を設定する.
	m_pSkinMesh[Mesh_Body]->SetTransform( m_TransformList[Mesh_Body] );

	// ジェットパックの位置の更新.
	m_TransformList[Mesh_LJetPack].Position = m_pSkinMesh[Mesh_Body]->GetPosFromBone( L_JETPACK_SETPOS_BONE );
	m_TransformList[Mesh_RJetPack].Position = m_pSkinMesh[Mesh_Body]->GetPosFromBone( R_JETPACK_SETPOS_BONE );

	// リュックの位置の更新.
	m_TransformList[Mesh_Bag].Position		= m_pSkinMesh[Mesh_Body]->GetPosFromBone( BAG_SETPOS_BONE );

	// 落下している場合リュックを少し上に移動させる.
	const float AddY = m_Transform.Position.y - m_TransformList[Mesh_Body].Position.y;
	m_TransformList[Mesh_Bag].Position.y += sinf( Math::ToRadian( m_CntList[GetFoodAng] ) );
	if ( AddY > 0.0f ) m_TransformList[Mesh_Bag].Position.y += AddY;

	// 外部で使用するトランスフォームをプレイヤー本体と同じにする.
	m_Transform	= m_TransformList[Mesh_Body];

	// ボーン座標を取得用のトランスフォームの更新.
	m_TransformList[Bone_Body].Position		= m_TransformList[Mesh_Body].Position;
	m_pSkinMesh[Bone_Body]->SetTransform( m_TransformList[Bone_Body] );
	m_TransformList[Bone_LJetPack].Position	= m_pSkinMesh[Bone_Body]->GetPosFromBone( L_JETPACK_SETPOS_BONE );
	m_TransformList[Bone_RJetPack].Position	= m_pSkinMesh[Bone_Body]->GetPosFromBone( R_JETPACK_SETPOS_BONE );
}

//---------------------------.
// リュックの色の設定.
//---------------------------.
void CPlayer::SetRuckSackColor()
{
	// ゲージの場所の取得.
	const float LR_Gage		= std::abs( m_pRuckSack->GetLRGauge() );
	const float UD_Gage		= std::abs( m_pRuckSack->GetUDGauge() );
	const float Color_Gage	= LR_Gage > UD_Gage ? LR_Gage : UD_Gage;

	// 白色から減らす最大の色.
	const D3DXCOLOR3 DecColorMax = { Color::RGB_( Color::COLOR_MIN, 0.5f, 0.5f ) };

	// ゲージの場所に応じてリュックの色を赤くしていく.
	m_pSkinMesh[Mesh_Bag]->SetColor( Color::White - DecColorMax * Color_Gage );
}

//---------------------------.
// ジェットパックの噴射.
//---------------------------.
void CPlayer::JetPackJetControl()
{
	if ( m_FlagList[IsControlStop] ) return;

	m_VectorList[Dire]	= m_TransformList[Mesh_Body].GetRotationVector();

	// トリガーの押し具合を取得.
	const float LTrigger	= CXInput::GetLTrigger();
	const float RTrigger	= CXInput::GetRTrigger();
	const float LPower		= LTrigger * CONSTANT.JETPACK_POWER;
	const float RPower		= RTrigger * CONSTANT.JETPACK_POWER;

	// 押し具合を保存.
	std::rotate( m_LJetPackInputList.begin(), m_LJetPackInputList.begin() + 1, m_LJetPackInputList.end() );
	std::rotate( m_RJetPackInputList.begin(), m_RJetPackInputList.begin() + 1, m_RJetPackInputList.end() );
	const int SetPos			= CONSTANT.JETPACK_INPUT_LIST_MAX - 1;
	m_LJetPackInputList[SetPos] = LTrigger;
	m_RJetPackInputList[SetPos] = RTrigger;

	// ボーンの位置を取得.
	m_pSkinMesh[Bone_Body]->SetTransform( m_TransformList[Bone_Body] );
	const D3DXPOSITION3& CenterPos = Math::Round_n( m_pSkinMesh[Bone_Body]->GetPosFromBone( JETPACK_CENTER_BONE ), 7 );

	// ジェットパックの開始音を再生するか.
	if ( m_LJetPackInputList[SetPos - 1] == 0.0f && m_LJetPackInputList[SetPos] > 0.0f ) {
		// SEの再生.
		CSoundManager::SetSelectNo( 0 );
		CSoundManager::SetSEVolume( JETPACK_START_SE, m_LJetPackInputList[SetPos] );
		CSoundManager::PlaySE( JETPACK_START_SE );
		CSoundManager::ResetSelectNo();
	}
	else if ( m_RJetPackInputList[SetPos - 1] == 0.0f && m_RJetPackInputList[SetPos] > 0.0f ) {
		// SEの再生.
		CSoundManager::SetSelectNo( 1 );
		CSoundManager::SetSEVolume( JETPACK_START_SE, m_RJetPackInputList[SetPos] );
		CSoundManager::PlaySE( JETPACK_START_SE );
		CSoundManager::ResetSelectNo();
	}

	// 左トリガーを押しているか.
	if ( LTrigger != 0.0f ) {
		if ( m_FlagList[IsLJetPackStop] == false ) {

			// ボーンの位置を取得.
			m_pSkinMesh[Bone_LJetPack]->SetTransform( m_TransformList[Bone_LJetPack] );
			const D3DXPOSITION3& LJetPackPos = Math::Round_n( m_pSkinMesh[Bone_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ), 7 );

			// 噴射ベクトルの作成.
			m_VectorList[LJetPackJet] = CenterPos - LJetPackPos;
			D3DXVec3Normalize( &m_VectorList[LJetPackJet], &m_VectorList[LJetPackJet] );

			// 噴射ベクトルの追加.
			m_MoveVectorList.emplace_back( m_VectorList[LJetPackJet] * LPower );

			// 左ジェットパックの炎エフェクトの開始.
			m_pEffectList[Left_Flame]->Play( m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ) );
			m_pEffectList[Left_Flame]->SetScale( LTrigger );

			// 左オーディオでSEの再生.
			CSoundManager::SetSelectNo( 0 );
			CSoundManager::SetSEVolume(	JETPACK_SE, LTrigger );
			CSoundManager::PlaySELoop(	JETPACK_SE );
			CSoundManager::ResetSelectNo();
		}
		// 左ジェットパックの煙エフェクトの開始.
		m_pEffectList[Left_Smoke]->Play( m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ) );
		m_pEffectList[Left_Smoke]->SetAlpha( LTrigger );
	}
	else {
		// 左ジェットパックのエフェクトの停止.
		m_pEffectList[Left_Flame]->Stop();
		m_pEffectList[Left_Smoke]->StopRoot();

		// 左オーディオでSEの停止.
		CSoundManager::SetSelectNo( 0 );
		CSoundManager::StopSE( JETPACK_SE, true );
		CSoundManager::ResetSelectNo();
	}

	// 右トリガーを押しているか.
	if ( RTrigger != 0.0f ) {
		if ( m_FlagList[IsRJetPackStop] == false ) {
			// ボーンの位置を取得.
			m_pSkinMesh[Bone_RJetPack]->SetTransform( m_TransformList[Bone_RJetPack] );
			const D3DXPOSITION3& RJetPackPos = Math::Round_n( m_pSkinMesh[Bone_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ), 7 );

			// 噴射ベクトルの作成.
			m_VectorList[RJetPackJet] = CenterPos - RJetPackPos;
			D3DXVec3Normalize( &m_VectorList[RJetPackJet], &m_VectorList[RJetPackJet] );

			// 噴射ベクトルの追加.
			m_MoveVectorList.emplace_back( m_VectorList[RJetPackJet] * RPower );

			// 右ジェットパックの炎エフェクトの開始.
			m_pEffectList[Right_Flame]->Play( m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ) );
			m_pEffectList[Right_Flame]->SetScale( RTrigger );

			// 右オーディオでSEの再生.
			CSoundManager::SetSelectNo( 1 );
			CSoundManager::SetSEVolume(	JETPACK_SE, RTrigger );
			CSoundManager::PlaySELoop(	JETPACK_SE );
			CSoundManager::ResetSelectNo();
		}
		// 右ジェットパックの煙エフェクトの開始.
		m_pEffectList[Right_Smoke]->Play( m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ) );
		m_pEffectList[Right_Smoke]->SetAlpha( RTrigger );
	}
	else {
		// 右ジェットパックのエフェクトの停止.
		m_pEffectList[Right_Flame]->Stop();
		m_pEffectList[Right_Smoke]->StopRoot();

		// 右オーディオでSEの停止.
		CSoundManager::SetSelectNo( 1 );
		CSoundManager::StopSE( JETPACK_SE, true );
		CSoundManager::ResetSelectNo();
	}

	// コントローラの振動.
	CXInput::SetVibration( ( LTrigger - 0.3f ) * CONSTANT.VIBRATION_POWER, RTrigger * CONSTANT.VIBRATION_POWER );
}

//---------------------------.
// ジェットパックの停止.
//---------------------------.
void CPlayer::JetPackStop()
{
	if ( m_FlagList[IsLJetPackStop] && m_FlagList[IsRJetPackStop] ) return;

	// どんなけトリガーの入力が切り替わったか調べる.
	int		LChangeCnt		= 0;
	int		RChangeCnt		= 0;
	bool	IsLUp			= m_LJetPackInputList[1] > m_LJetPackInputList[0];
	bool	IsRUp			= m_RJetPackInputList[1] > m_RJetPackInputList[0];
	for ( int Now = 2; Now < CONSTANT.JETPACK_INPUT_LIST_MAX; ++Now ) {
		const int	Old		= Now - 1;
		const float LDiff	= m_LJetPackInputList[Now] - m_LJetPackInputList[Old];
		const float RDiff	= m_RJetPackInputList[Now] - m_RJetPackInputList[Old];

		// 左のジェットパックの押し具合が変化したか.
		if ( m_FlagList[IsLJetPackStop] == false ) {
			if ( ( IsLUp && LDiff < 0.0f ) && std::abs( LDiff ) > CONSTANT.JETPACK_STOP_CNT_DIFF ) {
				LChangeCnt++;
				IsLUp = false;
			}
			if ( ( !IsLUp && LDiff > 0.0f ) && std::abs( LDiff ) > CONSTANT.JETPACK_STOP_CNT_DIFF ) {
				LChangeCnt++;
				IsLUp = true;
			}
		}

		// 右のジェットパックの押し具合が変化したか.
		if ( m_FlagList[IsRJetPackStop] == false ) {
			if ( ( IsRUp && RDiff < 0.0f ) && std::abs( RDiff ) > CONSTANT.JETPACK_STOP_CNT_DIFF ) {
				RChangeCnt++;
				IsRUp = false;
			}
			if ( ( !IsRUp && RDiff > 0.0f ) && std::abs( RDiff ) > CONSTANT.JETPACK_STOP_CNT_DIFF ) {
				RChangeCnt++;
				IsRUp = true;
			}
		}
	}

	// 回数に応じてジェットパックを膨らませる.
	const float LScale = STransform::NORMAL_SCALE + 0.03f * LChangeCnt;
	const float RScale = STransform::NORMAL_SCALE + 0.03f * RChangeCnt;
	m_TransformList[Mesh_LJetPack].Scale = { LScale, LScale, LScale };
	m_TransformList[Mesh_RJetPack].Scale = { RScale, RScale, RScale };

	// 一定回数以上切り替わっていた場合そのトリガーを停止させる.
	if ( LChangeCnt >= CONSTANT.STOP_CHANGE_NUM ) {
		// 左ジェットパックの爆発エフェクトの再生.
		m_pSkinMesh[Mesh_LJetPack]->SetTransform( m_TransformList[Bone_LJetPack] );
		m_pEffectList[Left_Explosion]->Play( m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ) );
		m_pEffectList[Left_Explosion]->SetScale( 0.2f );

		m_FlagList[IsLJetPackStop] = true;
		m_pEffectList[Left_Flame]->Stop();

		// 爆発SEの再生.
		CSoundManager::SetSelectNo( 0 );
		CSoundManager::PlaySE( EXPLOSION_SE );
		CSoundManager::ResetSelectNo();
	}
	if ( RChangeCnt >= CONSTANT.STOP_CHANGE_NUM ) {
		// 右ジェットパックの爆発エフェクトの再生.
		m_pSkinMesh[Mesh_RJetPack]->SetTransform( m_TransformList[Mesh_RJetPack] );
		m_pEffectList[Right_Explosion]->Play( m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ) );
		m_pEffectList[Right_Explosion]->SetScale( 0.2f );

		m_FlagList[IsRJetPackStop] = true;
		m_pEffectList[Right_Flame]->Stop();

		// 爆発SEの再生.
		CSoundManager::SetSelectNo( 1 );
		CSoundManager::PlaySE( EXPLOSION_SE );
		CSoundManager::ResetSelectNo();
	}
}

//---------------------------.
// ジェットパックの停止の修復.
//---------------------------.
void CPlayer::JetPackRepair()
{
	if ( m_FlagList[IsLJetPackStop] == false && m_FlagList[IsRJetPackStop] == false ) return;

	// 左ジェットパックの停止時間のカウントの追加.
	if ( m_FlagList[IsLJetPackStop] ) {
		m_CntList[LJetPackStopTime] += m_DeltaTime;

		// 左ジェットパックの停止の終了.
		if ( m_CntList[LJetPackStopTime] >= CONSTANT.JETPACK_STOP_TIME ) {
			m_CntList[LJetPackStopTime]	= INIT_FLOAT;
			m_FlagList[IsLJetPackStop]	= false;
		}
	}

	// 右ジェットパックの停止時間のカウントの追加.
	if ( m_FlagList[IsRJetPackStop] ) {
		m_CntList[RJetPackStopTime] += m_DeltaTime;

		// 右ジェットパックの停止の終了.
		if ( m_CntList[RJetPackStopTime] >= CONSTANT.JETPACK_STOP_TIME ) {
			m_CntList[RJetPackStopTime]	= INIT_FLOAT;
			m_FlagList[IsRJetPackStop]	= false;
		}
	}
}

//---------------------------.
// ジェットパックの回転.
//---------------------------.
void CPlayer::JetPackRotationControl()
{
	if ( m_FlagList[IsControlStop] ) return;

	const float	EndRotX = -Math::ToRadian( CONSTANT.JETPACK_ROT_END ) * CXInput::GetLThumbY();
	float		RotX	= m_TransformList[Mesh_LJetPack].Rotation.x;

	// 同じ回転値のため抜ける.
	if ( EndRotX == RotX ) return;

	// ジェットパックの回転.
	if ( EndRotX > RotX ) {
		RotX += Math::ToRadian( CONSTANT.JETPACK_ROT_SPEED );
		if ( EndRotX < RotX ) RotX = EndRotX;
	}
	else {
		RotX -= Math::ToRadian( CONSTANT.JETPACK_ROT_SPEED );
		if ( EndRotX > RotX ) RotX = EndRotX;
	}

	// 回転の反映.
	m_TransformList[Mesh_LJetPack].Rotation.x = RotX;
	m_TransformList[Mesh_RJetPack].Rotation.x = RotX;
	m_TransformList[Bone_LJetPack].Rotation.x = RotX;
	m_TransformList[Bone_RJetPack].Rotation.x = RotX;
}

//---------------------------.
// プレイヤーの移動.
//---------------------------.
void CPlayer::PlayerMove()
{
	D3DXVECTOR3* Position = &m_TransformList[Mesh_Body].Position;

	// 移動ベクトルでプレイヤーを動かす.
	*Position += m_VectorList[NowMove];

	// 風ベクトルでプレイヤーを動かす.
	*Position += m_VectorList[Wind];

	// マップ外か確認する.
	OffStageCheck();
}

//---------------------------.
// プレイヤーの回転.
//---------------------------.
void CPlayer::PlayerRotation()
{
	// 左右のジェットパックの回転の強さの取得
	const float LRotPower = m_TransformList[Bone_LJetPack].Rotation.x > 0.0f ?
		-D3DXVec3Length( &m_VectorList[LJetPackRot] ) :
		 D3DXVec3Length( &m_VectorList[LJetPackRot] );
	const float RRotPower = m_TransformList[Bone_RJetPack].Rotation.x > 0.0f ?
		 D3DXVec3Length( &m_VectorList[RJetPackRot] ) :
		-D3DXVec3Length( &m_VectorList[RJetPackRot] );

	// トリガーの押し具合を取得.
	const float LTrigger = CXInput::GetLTrigger() * CONSTANT.JETPACK_POWER;
	const float RTrigger = CXInput::GetRTrigger() * CONSTANT.JETPACK_POWER;

	// プレイヤーの回転.
	for ( auto& t : m_TransformList ) {
		D3DXROTATION3* pRot = &t.Rotation;
		pRot->y += LRotPower * LTrigger;
		pRot->y += RRotPower * RTrigger;

		// 回転値を範囲内に収める.
		if ( pRot->y < Math::RADIAN_MIN ) pRot->y += Math::RADIAN_MAX;
		if ( pRot->y > Math::RADIAN_MAX ) pRot->y -= Math::RADIAN_MAX;
	}

	// カメラに現在の回転値を渡す.
	m_pMainCamera->SetRotation( m_TransformList[Mesh_Body].Rotation );
}

//---------------------------.
// プレイヤーの傾きの更新.
//---------------------------.
void CPlayer::PlayerTiltUpdate()
{
	// 現在の傾きの取得.
	const D3DXROTATION3& Rot = { 0.0f, -m_TransformList[Bone_Body].Rotation.y, 0.0f };
	m_VectorList[Tilt] = Math::Round_n( Math::VectorRotation( m_VectorList[NowMove] + m_VectorList[Wind], Rot ), 5 );

	// 現在の傾きに応じた角度を求める.
	float RotZ = m_TransformList[Bone_Body].Rotation.z;
	RotZ -= m_VectorList[Tilt].x;
	const float Radian_End = Math::ToRadian( CONSTANT.END_TILE );
	if ( RotZ >=  Radian_End ) RotZ =  Radian_End;
	if ( RotZ <= -Radian_End ) RotZ = -Radian_End;

	// 現在の傾きに応じてプレイヤーを回転させる.
	for ( int i = 0; i < Mesh_Max; ++i ) {
		m_TransformList[i].Rotation.z = RotZ;
	}
}

//---------------------------.
// 移動ベクトルの作成.
//---------------------------.
void CPlayer::CreateMoveVec()
{
	// ベクトルリストのベクトルを全て合成する.
	D3DXVECTOR3 MoveVec = m_VectorList[NowMove] * CONSTANT.INERTIA_USAGE_RATE;
	for ( auto& Vec : m_MoveVectorList ) {
		MoveVec += Vec;
	}

	// 移動ベクトルとして保存.
	m_VectorList[OldMove] = m_VectorList[NowMove];
	m_VectorList[NowMove] = MoveVec;

	// ベクトルリストを初期化.
	m_MoveVectorList.clear();

	// 移動速度を求める用のベクトルの作成.
	D3DXVECTOR3 SideMoveVector		= m_VectorList[NowMove];
	D3DXVECTOR3 VerticalMoveVector	= m_VectorList[NowMove];
	SideMoveVector.y				= INIT_FLOAT;
	VerticalMoveVector.x			= INIT_FLOAT;
	VerticalMoveVector.z			= INIT_FLOAT;

	// 移動速度の設定.
	m_SideMoveSpeed		= D3DXVec3Length( &SideMoveVector );
	m_VerticalMoveSpeed	= D3DXVec3Length( &VerticalMoveVector );
}

//---------------------------.
// 左右のジェットパックの回転ベクトルの作成.
//---------------------------.
void CPlayer::CreateRotVec()
{
	// 回転ベクトルの作成.
	m_VectorList[LJetPackRot] = m_TransformList[Bone_LJetPack].Position -
		m_pSkinMesh[Bone_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE );
	m_VectorList[RJetPackRot] = m_TransformList[Bone_RJetPack].Position -
		m_pSkinMesh[Bone_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE );

	// y座標は使用しないため削除する.
	m_VectorList[LJetPackRot].y = INIT_FLOAT;
	m_VectorList[RJetPackRot].y = INIT_FLOAT;
}

//---------------------------.
// 重力の追加.
//---------------------------.
void CPlayer::AddGravity()
{
	// 重力の追加.
	m_MoveVectorList.emplace_back( GRAVITY_VECTOR * CONSTANT.GRAVITY_POWER );
}

//---------------------------.
// 加速度の上限/下限の設定.
//---------------------------.
void CPlayer::AccelerationClamp()
{
	const float Min = CONSTANT.ACCELERATION_MIN;
	const float Max = CONSTANT.ACCELERATION_MAX;
	m_VectorList[NowMove].y = std::clamp( m_VectorList[NowMove].y, Min, Max );
	m_VectorList[NowMove].x = std::clamp( m_VectorList[NowMove].x, Min, Max );
	m_VectorList[NowMove].z = std::clamp( m_VectorList[NowMove].z, Min, Max );
}

//---------------------------.
// 摩擦
//---------------------------.
void CPlayer::Friction( const float Value )
{
	D3DXVECTOR3 FrictionVec;
	D3DXVec3Normalize( &FrictionVec, &m_VectorList[NowMove] );

	if ( m_VectorList[NowMove].x > 0.0f ) {
		m_VectorList[NowMove].x -= FrictionVec.x * Value;
		if ( m_VectorList[NowMove].x < 0.0f ) m_VectorList[NowMove].x = 0.0f;
	}
	if ( m_VectorList[NowMove].x < 0.0f ) {
		m_VectorList[NowMove].x -= FrictionVec.x * Value;
		if ( m_VectorList[NowMove].x > 0.0f ) m_VectorList[NowMove].x = 0.0f;
	}
	if ( m_VectorList[NowMove].z > 0.0f ) {
		m_VectorList[NowMove].z -= FrictionVec.z * Value;
		if ( m_VectorList[NowMove].z < 0.0f ) m_VectorList[NowMove].z = 0.0f;
	}
	if ( m_VectorList[NowMove].z < 0.0f ) {
		m_VectorList[NowMove].z -= FrictionVec.z * Value;
		if ( m_VectorList[NowMove].z > 0.0f ) m_VectorList[NowMove].z = 0.0f;
	}
}

//---------------------------.
// 着地エフェクトの再生.
//---------------------------.
void CPlayer::LandingEffectPlay( const D3DXPOSITION3& Pos )
{
	if ( m_FlagList[IsHitGround] && m_LandingEndPos == Pos ) return;

	// エフェクトを再生するか.
	m_CntList[LandingEffect] += m_DeltaTime;
#ifdef _DEBUG
	if ( m_CntList[LandingEffect] < CONSTANT.LANDING_EFFECT_TIME * 5.0f ) return;
#else
	if ( m_CntList[LandingEffect] < CONSTANT.LANDING_EFFECT_TIME ) return;
#endif
	m_CntList[LandingEffect] = 0.0f;

	for ( auto& e : m_pLandingEffectList ) {
		if ( e->IsShow() ) continue;

		// ステータスの設定.
		e->Play( Pos );
		m_LandingEndPos = Pos;
		return;
	}

	// 新しく作成.
	m_pLandingEffectList.emplace_back( std::make_unique<CEffectManager>() );
	m_pLandingEffectList.back()->SetEffect( "Landing" );
	m_pLandingEffectList.back()->Play( Pos );
	m_LandingEndPos = Pos;
}

//---------------------------.
// 着地エフェクトの更新.
//---------------------------.
void CPlayer::LandingEffectUpdate()
{
	// エフェクトの削除.
	const int Size = static_cast<int>( m_pLandingEffectList.size() );
	for ( int i = Size - 1; i >= 0; --i ) {
		if ( m_pLandingEffectList[i]->IsShow() ) break;
		m_pLandingEffectList.pop_back();
	}
}

//---------------------------.
// 商品取得アニメーションの更新.
//---------------------------.
void CPlayer::GetFoodAnimUpdate()
{
	if ( m_FlagList[IsGetFood] == false ) return;

	// 商品取得アニメーションのカウントを増やす.
	m_CntList[GetFoodAng] += CONSTANT.GET_FOOD_ANIM_SPPED * m_DeltaTime;

	// 商品取得アニメーション終了.
	if ( m_CntList[GetFoodAng] >= GET_FOOD_ANIM_CNT_MAX ) {
		m_CntList[GetFoodAng]	= 0.0f;
		m_FlagList[IsGetFood]	= false;
	}

	// アニメーション用の拡縮を求める.
	m_TransformList[Mesh_Bag].Scale.y = STransform::NORMAL_SCALE + sinf( Math::ToRadian( m_CntList[GetFoodAng] ) ) / 2.0f;
}

//---------------------------.
// ステージ外か確認.
//---------------------------.
void CPlayer::OffStageCheck()
{
	if ( m_FlagList[IsControlStop] ) return;

	const D3DXPOSITION3& Position = m_TransformList[Mesh_Body].Position;
	const float			 Distance = sqrtf( Position.x * Position.x + Position.z * Position.z );

	//  ステージ外か.
	if ( ( Position.y	<= CONSTANT.DEAD_DOWN_POS_Y	) ||
		 ( Position.y	>= CONSTANT.DEAD_UP_POS_Y	) ||
		 ( Distance		>= CONSTANT.DEAD_RADIUS		) )
	{
		// 治療費を発生させる.
		CScore::SetGameEnd();

		// 操作を停止させる.
		CInput::SetIsUpdateStop( true );

		// 初期化.
		CWind::StopWind();
		m_FlagList[IsControlStop]	= true;
		m_FlagList[IsStageOut]		= true;
		m_CntList[StageOutTime]		= 0.0f;

		// カメラを停止させる.
		if ( m_FlagList[IsActiveGazingCamera] ) m_pGazingCamera->SetIsMoveStop( true );
		else									m_pMainCamera->SetIsMoveStop( true );

		// アニメーションの再生.
		m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( StageOut, &m_AC );

		// SEの再生.
		CSoundManager::PlaySE( "Ambulance" );
	}
}

//---------------------------.
// ステージ外に移動した時の更新.
//---------------------------.
void CPlayer::StageOutUpdate()
{
	if ( m_FlagList[IsStageOut] == false ) return;

	// 一定時間経過したらゲームクリアに移動させる.
	m_CntList[StageOutTime] += m_DeltaTime;
	if ( m_CntList[StageOutTime] >= CONSTANT.STAGE_OUT_TIME ) {
		CInput::SetIsUpdateStop( false );
		CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade, 0.01f );
	}
}

//---------------------------.
// 移動のSEの再生.
//---------------------------.
void CPlayer::PlayMoveSE()
{
	// 下に移動SEの再生.
	if ( m_VerticalMoveSpeed >= CONSTANT.DEAD_GROUND_SPEED ) {
		// 再生.
		if ( m_SEVolumeList[MoveDown] < 1.0f ) {
			m_SEVolumeList[MoveDown] += CONSTANT.ADD_SE_VOLUME * m_DeltaTime;
			if ( m_SEVolumeList[MoveDown] >= 1.0f ) m_SEVolumeList[MoveDown] = 1.0f;
			CSoundManager::SetSEVolume( MOVE_SE, m_SEVolumeList[MoveDown] );
		}
		CSoundManager::PlaySELoop( MOVE_SE );
	}
	else {
		// 停止.
		if ( m_SEVolumeList[MoveDown] > 0.0f ) {
			m_SEVolumeList[MoveDown] -= CONSTANT.ADD_SE_VOLUME * m_DeltaTime;
			if ( m_SEVolumeList[MoveDown] <= 0.0f ) {
				m_SEVolumeList[MoveDown] = 0.0f;
				CSoundManager::StopSE( MOVE_SE );
			}
			CSoundManager::SetSEVolume( MOVE_SE, m_SEVolumeList[MoveDown] );
		}
	}

	// 移動SEの再生.
	if ( m_SideMoveSpeed >= CONSTANT.DEAD_WALL_SPEED ) {
		// 再生.
		if ( m_SEVolumeList[Move] < 1.0f ) {
			m_SEVolumeList[Move] += CONSTANT.ADD_SE_VOLUME * m_DeltaTime;
			if ( m_SEVolumeList[Move] >= 1.0f ) m_SEVolumeList[Move] = 1.0f;
			CSoundManager::SetSEVolume( MOVE_SE, m_SEVolumeList[Move] );
		}
		CSoundManager::PlaySELoop( MOVE_SE );
	}
	else {
		// 停止.
		if ( m_SEVolumeList[Move] > 0.0f ) {
			m_SEVolumeList[Move] -= CONSTANT.ADD_SE_VOLUME * m_DeltaTime;
			if ( m_SEVolumeList[Move] <= 0.0f ) {
				m_SEVolumeList[Move] = 0.0f;
				CSoundManager::StopSE( MOVE_SE );
			}
			CSoundManager::SetSEVolume( MOVE_SE, m_SEVolumeList[Move] );
		}
	}
}