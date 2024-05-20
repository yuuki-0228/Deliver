#include "Customer.h"
#include "..\Employee\Employee.h"
#include "..\..\DeliveryManager\DeliveryManager.h"
#include "..\..\Player\Player.h"
#include "..\..\..\RuckSack\RuckSack.h"
#include "..\..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\Widget\UIWidget\GameMain\NotificationUI\NotificationUI.h"
#include "..\..\..\..\..\Collision\CollisionManager\CollisionManager.h"
#include "..\..\..\..\..\Collision\CollisionRender\CollisionRender.h"
#include "..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\Common\EffectManager\EffectManager.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Utility\ImGuiManager\MessageWindow\MessageWindow.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr float GAUGE_MAX		= 1.0f;	// 受け取りゲージの最大値.
	constexpr float GAUGE_MIN		= 0.0f;	// 受け取りゲージの最小値.
	constexpr float DELETE_TIME_MIN	= 0.0f;	// 削除タイムの最小値.
	constexpr float ANIM_CNT_MAX	= 1.0f;	// アニメーション用のカウントの最大値.
	constexpr float ANIM_CNT_MIN	= 0.0f;	// アニメーション用のカウントの最小値.
	constexpr int	BODY_NO_MIN		= 0;	// 体のテクスチャNoの最小値.
	constexpr int	BODY_NO_MAX		= 3;	// 体のテクスチャNoの最大値.
	constexpr int	HEAD_NO_MIN		= 0;	// 顔のテクスチャNoの最小値.
	constexpr int	HEAD_NO_MAX		= 4;	// 顔のテクスチャNoの最大値.

	constexpr char	HEAD_POS_BONE_NAME[]					= "customer_head";				// 頭の位置のボーン名.
	constexpr char	LEFT_HAND_POS_BONE_NAME[]				= "customer_hand_l";			// 左手の位置のボーン名.
	constexpr char	RIGHT_HAND_POS_BONE_NAME[]				= "customer_hand_r";			// 右手の位置のボーン名.
	constexpr char	LEFT_HAND_GET_PIZZA_POS_BONE_NAME[]		= "pizza_box_position2";		// 左手のピザ取得時の位置のボーン名.
	constexpr char	RIGHT_HAND_GET_PIZZA_POS_BONE_NAME[]	= "pizza_box_position1";		// 右手のピザ取得時の位置のボーン名.
	constexpr char	WAVEHANDS_CENTER_POS_BONE_NAME[]		= "customer_hand_position2";	// 手を振るアニメーションの中央の位置のボーン名.
	constexpr char	WAVEHANDS_START_POS_BONE_NAME[]			= "customer_hand_position2_1";	// 手を振るアニメーションの開始の位置のボーン名.
	constexpr char	WAVEHANDS_END_POS_BONE_NAME[]			= "customer_hand_position2_2";	// 手を振るアニメーションの終了の位置のボーン名.

	// スキンメッシュのNo.
	enum enMeshNo : unsigned char {
		Body,		// 本体.
		Head,		// 頭.
		LHand,		// 左手.
		RHand,		// 右手.

		Mesh_Max,	// メッシュの数.
	} typedef EMeshNo;

	// アニメーションのNo.
	enum enAnimationNo : unsigned char {
		NormalAnim,				// 通常時.
		ChangeNormalAnim,		// 通常時に変更.
		ChangeWaveHandsAnim,	// 手を振るアニメーションに変更.
		WaveHandsAnim,			// 手を振る.
		ChangeGetPizzaAnim,		// ピザの取得に変更.
		GetPizzaAnim,			// ピザの取得.
		GetPizzaAnimEnd,		// ピザの取得終了.

		AnimationNo_Max
	} typedef EAnimationNo;
}

CCustomer::CCustomer( const EObjectTag TypeTag )
	: m_pSkinMesh			()
	, m_TransformList		()
	, m_pGetEffect			()
	, m_pPizzaBox			( nullptr )
	, m_pPlayer				( nullptr )
	, m_PointLight			()
	, m_PizzaBoxTransform	()
	, m_Handle				( NO_LINK_HANDLE )
	, m_RightHandMoveVector	( INIT_FLOAT3 )
	, m_LeftHandMoveVector	( INIT_FLOAT3 )
	, m_RightHandsStartPos	( INIT_FLOAT3 )
	, m_LeftHandsStartPos	( INIT_FLOAT3 )
	, m_HeadRotMatrix		()
	, m_Type				( TypeTag )
	, m_Direction			( EDirection::None )
	, m_AddMoney			( INIT_FLOAT )
	, m_Gauge				( GAUGE_MIN )
	, m_GaugeDeleteTimeCnt	( DELETE_TIME_MIN )
	, m_Alpha				( Color::ALPHA_MAX )
	, m_DetachCoolTime		( INIT_FLOAT )
	, m_PizzaBoxAlpha		( Color::ALPHA_MIN )
	, m_AnimCnt				( INIT_FLOAT )
	, m_AnimationNo			( NormalAnim )
	, m_BodyNo				( INIT_INT )
	, m_HeadNo				( INIT_INT )
	, m_PizzaBoxNo			( INIT_INT )
	, m_IsHitPlayer			( false )
	, m_IsShortly			( false )
	, m_IsGoHandingCustomer	( false )
	, m_IsWaveHandsMoveUp	( false )
	, m_IsGetPizzaAnimEnd	( false )
{
}

CCustomer::~CCustomer()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CCustomer::Init()
{
	// メッシュの取得.
	m_pSkinMesh.resize( Mesh_Max );
	m_pSkinMesh[Body ]	= CMeshResource::GetSkin( "customer_body_s" );
	m_pSkinMesh[Head ]	= CMeshResource::GetSkin( "head_s" );
	m_pSkinMesh[LHand]	= CMeshResource::GetSkin( "hand_l_s" );
	m_pSkinMesh[RHand]	= CMeshResource::GetSkin( "hand_r_s" );
	m_pPizzaBox			= CMeshResource::GetSkin( "pizza_box_s" );

	// メッシュのNoの設定.
	m_BodyNo = CRandom::GetRand( BODY_NO_MIN, BODY_NO_MAX );
	m_HeadNo = CRandom::GetRand( HEAD_NO_MIN, HEAD_NO_MAX );

	// メッシュの座標の更新.
	m_TransformList.resize( Mesh_Max );
	m_TransformList[Body ]			= m_Transform;
	m_pSkinMesh[Body ]->SetTransform( m_TransformList[Body ] );
	m_TransformList[Head ]			= m_Transform;
	m_TransformList[LHand]			= m_Transform;
	m_TransformList[RHand]			= m_Transform;
	m_TransformList[Head ].Position = m_pSkinMesh[Body]->GetPosFromBone( HEAD_POS_BONE_NAME );
	m_TransformList[LHand].Position = m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	m_TransformList[RHand].Position = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );
	m_pSkinMesh[Head ]->SetTransform( m_TransformList[Head ] );
	m_pSkinMesh[LHand]->SetTransform( m_TransformList[LHand] );
	m_pSkinMesh[RHand]->SetTransform( m_TransformList[RHand] );

	// ピザ箱の取得.
	m_pPizzaBox		= CMeshResource::GetSkin( "pizza_box_s" );
	m_PizzaBoxAlpha = Color::ALPHA_MIN;

	// エフェクトの初期化.
	m_pGetEffect	= std::make_unique<CEffectManager>();
	m_pGetEffect->SetEffect( "Get" );

	// お客さんの追加.
	CDeliveryManager::AddEmployee( this, m_Type );

	// タグの設定.
	m_ObjectTag = EObjectTag::Customer;

	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CCustomer::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// 距離によるアルファフェードの更新.
	m_IsDisp = !IsScreenOut();
	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	AnimationUpdate();		// アニメーションの更新.
	DetachCoolTimeUpdate();	// 連続で紐づけされないようにするクールタイムの更新.

	// プレイヤーに当たっていない状態にする.
	m_IsHitPlayer = false;

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::BeAHit );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CCustomer::LateUpdate( const float& DeltaTime )
{
	if ( m_Handle == NO_LINK_HANDLE ) return;

	// ミニマップにアイコンを追加する.
	CMiniMapUI::PushCharacterIcon( this, m_Direction, m_Handle );

	// プレイヤーの方を向くように回転.
	if ( m_pPlayer != nullptr ) {
		const D3DXPOSITION3& PlayerPos = m_pPlayer->GetPosition();
		const D3DXPOSITION3& CustomerPos = m_Transform.Position;
		m_Transform.Rotation.y = atan2f( CustomerPos.x - PlayerPos.x, CustomerPos.z - PlayerPos.z );
		m_TransformList[Body].Rotation.y = m_Transform.Rotation.y;
		m_TransformList[Head].Rotation.y = m_Transform.Rotation.y;
		m_TransformList[LHand].Rotation.y = m_Transform.Rotation.y;
		m_TransformList[RHand].Rotation.y = m_Transform.Rotation.y;

		// 頭の回転行列の取得.
		m_HeadRotMatrix = Math::GetLookRotationMatrix( m_pPlayer->GetLookPos(), m_TransformList[Head].Position );
	}

	// 受け取りゲージを減少させる.
	if ( m_IsHitPlayer ) return;
	if ( m_Gauge > GAUGE_MIN ) {
		m_Gauge -= CONSTANT.GAUGE_DEC_SPEED * DeltaTime;
		if ( m_Gauge <= GAUGE_MIN ) m_Gauge = GAUGE_MIN;
	}

	// ゲージの削除カウントを減らしていく.
	if ( m_GaugeDeleteTimeCnt > DELETE_TIME_MIN ) {
		m_GaugeDeleteTimeCnt -= DeltaTime;

		// ゲージを完全に削除する.
		if ( m_GaugeDeleteTimeCnt <= DELETE_TIME_MIN ) {
			m_GaugeDeleteTimeCnt = DELETE_TIME_MIN;

			// 受け取りゲージを初期化する.
			m_Gauge = GAUGE_MIN;
		}
	}
}

//---------------------------.
// 描画.
//---------------------------.
void CCustomer::Render()
{
	if ( m_IsDisp == false ) return;

	TransformUpdate();

	m_pSkinMesh[Body ]->SetDither( true );
	m_pSkinMesh[Head ]->SetDither( true );
	m_pSkinMesh[LHand]->SetDither( true );
	m_pSkinMesh[RHand]->SetDither( true );
	m_pPizzaBox->SetDither( true );
	m_pSkinMesh[Body ]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pSkinMesh[Head ]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pSkinMesh[LHand]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pSkinMesh[RHand]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pPizzaBox->SetAlpha( m_PizzaBoxAlpha );
	m_pSkinMesh[Body ]->ChangeTexture(	0, m_BodyNo );
	m_pSkinMesh[Head ]->ChangeTexture(	0, m_HeadNo );
	m_pPizzaBox->ChangeTexture(			0, m_PizzaBoxNo - 1 );
	m_pSkinMesh[Body]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[Head]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[LHand]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[RHand]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[Body ]->Render( &m_TransformList[Body ] );
	if ( m_Handle == NO_LINK_HANDLE || m_IsShortly == false )
			m_pSkinMesh[Head]->Render( &m_TransformList[Head] );
	else	m_pSkinMesh[Head]->Render( &m_TransformList[Head], nullptr, &m_HeadRotMatrix );
	m_pSkinMesh[LHand]->Render( &m_TransformList[LHand] );
	m_pSkinMesh[RHand]->Render( &m_TransformList[RHand] );
	m_pPizzaBox->Render( &m_PizzaBoxTransform );
	m_pSkinMesh[Body ]->SetDither( false );
	m_pSkinMesh[Head ]->SetDither( false );
	m_pSkinMesh[LHand]->SetDither( false );
	m_pSkinMesh[RHand]->SetDither( false );
	m_pPizzaBox->SetDither( false );

	CCollisionRender::PushSphere( m_pCollisions->GetCollision<CSphere>() );

	if ( m_Handle				== NO_LINK_HANDLE	) return;
	if ( m_IsGoHandingCustomer	== false			) return;
	CCollisionRender::PushCylinder( m_pCollisions->GetCollision<CCylinder>() );
}

//---------------------------.
// スプライト(3D)の描画.
//---------------------------.
void CCustomer::Sprite3DRender_A()
{
	m_pGetEffect->Render();
}

//---------------------------.
// 当たり判定関数.
//---------------------------.
void CCustomer::Collision( CActor* pActor )
{
	if ( m_Handle == NO_LINK_HANDLE						) return;
	if ( pActor->GetObjectTag() != EObjectTag::Player	) return;
	if ( m_pPlayer == nullptr ) m_pPlayer = dynamic_cast<CPlayer*>( pActor );

	PlayerShortlyCollision( pActor );	// プレイヤーが近くにいるかの当たり判定.
	GetFoodCollision( pActor );			// 商品の受け取り用の当たり判定.
}

//---------------------------.
// 渡しに行くお客さんに設定.
//---------------------------.
void CCustomer::SetGoHandingCustomer()
{
	m_IsGoHandingCustomer	= true;
	m_ObjectTag				= EObjectTag::GoHandingCustomer;
}

//---------------------------.
// 紐づけの設定.
//---------------------------.
void CCustomer::AttachLink( const CLinking::Handle& handle )
{
#ifdef _DEBUG
	// 待ち時間に紐づけをしていないか確認.
	if ( m_DetachCoolTime > 0.0f ) {
		ErrorMessage( "待ち時間中に紐づけが発生しました" );
		return;
	}

	// 紐づけを上書きをしないか確認.
	if ( m_Handle != NO_LINK_HANDLE ) {
		ErrorMessage( "お客さんの紐づけの上書きが発生しました" );
		return;
	}
#endif

	// ピザ箱を透明にする.
	m_PizzaBoxAlpha = Color::ALPHA_MIN;

	// 初期化.
	m_AnimationNo			= NormalAnim;
	m_IsGetPizzaAnimEnd		= false;
	m_Handle				= handle;
	m_GaugeDeleteTimeCnt	= DELETE_TIME_MIN;
}

//---------------------------.
// 紐づけの破棄.
//---------------------------.
void CCustomer::DetachLink()
{
	m_Handle			= NO_LINK_HANDLE;
	m_DetachCoolTime	= CONSTANT.DETACH_COOL_TIME;
	m_pCollisions->GetCollision<CCylinder>()->SetFlagOff();
}

//---------------------------.
// 通常時に変更.
//---------------------------.
void CCustomer::ChangeNormal()
{
	m_AnimationNo = ChangeNormalAnim;

	// ボーン座標の取得.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftEndPos		= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& RightEndPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );

	// 開始座標の保存.
	m_LeftHandsStartPos		= m_TransformList[LHand].Position;
	m_RightHandsStartPos	= m_TransformList[RHand].Position;

	// 移動ベクトルの作成.
	m_LeftHandMoveVector	= LeftEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector	= RightEndPos - m_RightHandsStartPos;

	// 初期化.
	m_AnimCnt = ANIM_CNT_MIN;
}

//---------------------------.
// 手を振るアニメーションに変更.
//---------------------------.
void CCustomer::ChangeWaveHands()
{
	m_AnimationNo = ChangeWaveHandsAnim;

	// ボーン座標の取得.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& StartPos	= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& EndPos		= m_pSkinMesh[Body]->GetPosFromBone( WAVEHANDS_END_POS_BONE_NAME );

	// 移動ベクトルの作成.
	m_LeftHandMoveVector	= EndPos - StartPos;

	// 初期化.
	m_AnimCnt				= ANIM_CNT_MIN;
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CCustomer::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( CONSTANT.SPHERE_SIZE );

	m_pCollisions->InitCollision( ECollType::Cylinder );
	m_pCollisions->GetCollision<CCylinder>()->SetRadius( CONSTANT.CYLINDER_RADIUS );
	m_pCollisions->GetCollision<CCylinder>()->SetHeight( CONSTANT.CYLINDER_HEIGHT );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CCustomer::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetTransform( m_Transform );

	STransform CylinderTransform = m_Transform;
	CylinderTransform.Position.y += CONSTANT.CYLINDER_ADD_POS_Y;
	m_pCollisions->GetCollision<CCylinder>()->SetTransform( CylinderTransform );
}

//---------------------------.
// プレイヤーが近くにいるかの当たり判定.
//---------------------------.
void CCustomer::PlayerShortlyCollision( CActor* pActor )
{
	// 当たっているか.
	const D3DXPOSITION3& PlayerPos = pActor->GetPosition();
	m_IsShortly = Coll::IsSphereToPosition(
		m_pCollisions->GetCollision<CSphere>(),
		PlayerPos );
}

//---------------------------.
// 商品の受け取り用の当たり判定.
//---------------------------.
void CCustomer::GetFoodCollision( CActor* pActor )
{
	if ( m_IsShortly == false ) return;

	// 渡しに行くお客さんかどうか.
	if ( m_IsGoHandingCustomer == false ) return;

	// 当たっているか.
	if ( Coll::IsCylinderToPosition(
		m_pCollisions->GetCollision<CCylinder>(),
		pActor->GetPosition() ) == false ) return;

	// プレイヤーに当たった.
	m_IsHitPlayer			= true;
	m_GaugeDeleteTimeCnt	= CONSTANT.GAUGE_DELETE_TIME;
	
	// 受け取りゲージの移動速度の設定.
	if ( m_Gauge >= GAUGE_MAX ) return;
	float GaugeSpeed = CONSTANT.GAUGE_SPEED * m_DeltaTime;

	// プレイヤーが着地している場合ゲージ速度をあげる.
	if ( m_pPlayer->GetIsLanding() ) GaugeSpeed *= CONSTANT.GAUGE_SPEED_UP;

	// プレイヤーがお客さんの方を向いているか.
	const D3DXVECTOR3&	CustomerRotVec		= m_Transform.GetRotationVector();
	const float			CustomerRotLength	= D3DXVec3Length( &CustomerRotVec );
	const D3DXVECTOR3&	SumRotVec			= CustomerRotVec + pActor->GetTransform().GetRotationVector();
	const float			SumRotLength		= D3DXVec3Length( &SumRotVec );
	if ( SumRotLength <= CustomerRotLength ) GaugeSpeed *= CONSTANT.GAUGE_SPEED_UP;

	// 受け取りゲージを増やす.
	m_Gauge += GaugeSpeed;

	// お客さんに商品を渡す.
	if ( m_Gauge >= GAUGE_MAX ) {
		m_Gauge = GAUGE_MIN;
		const SFoodState FoodState = m_pPlayer->GetFoodState();
		m_pPlayer->FoodPassed();

		// 新しいお客さんとお店を紐づける.
		CLinking::Link();
		CMessageWindow::PushMessage( u8"商品を渡しました。" );

		// 紐づけの解除.
		CLinking::SetIsGoToPass( m_Handle, false );
		CLinking::LinkEnd( m_Handle );

		// 普通のお客さんに変更.
		m_IsGoHandingCustomer	= false;
		m_ObjectTag				= EObjectTag::Customer;

		// エフェクトの再生.
		m_pGetEffect->Play( m_Transform.Position );

		// 通知の送る.
		PushNotification( FoodState );

		// ピザ箱の変更.
		m_PizzaBoxNo = FoodState.ShopNo;

		// ピザの取得アニメーションに変更.
		ChangeGetPizza();
	}
}

//---------------------------.
// トランスフォームの更新.
//---------------------------.
void CCustomer::TransformUpdate()
{
	m_TransformList[Body ]			= m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	if ( m_Handle == NO_LINK_HANDLE ) m_TransformList[Head] = m_Transform;
	m_TransformList[Head ].Position = m_pSkinMesh[Body]->GetPosFromBone( HEAD_POS_BONE_NAME );

	if ( m_AnimationNo != NormalAnim ) return;
	m_TransformList[LHand]			= m_Transform;
	m_TransformList[LHand].Position = m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	m_TransformList[RHand].Position = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );
}

//---------------------------.
// アニメーションの更新.
//---------------------------.
void CCustomer::AnimationUpdate()
{
	// 対応したアニメーションを行う.
	switch ( m_AnimationNo ) {
	case ChangeNormalAnim:		ChangeNormalAnimUpdate();		break;
	case ChangeWaveHandsAnim:	ChangeWaveHandsAnimUpdate();	break;
	case WaveHandsAnim:			WaveHandsAnimUpdate();			break;
	case ChangeGetPizzaAnim:	ChangeGetPizzaAnimUpdate();		break;
	case GetPizzaAnim:			GetPizzaAnimUpdate();			break;
	case GetPizzaAnimEnd:		GetPizzaAnimEndUpdate();		break;
	default:													break;
	}
}

//---------------------------.
// 通知を送る.
//---------------------------.
void CCustomer::PushNotification( const SFoodState& FoodState )
{
	ConstantStruct::SRuckSack RuckSackConstant = CConstant::GetRuckSack();
	bool IsHot		= FoodState.Temperature	> RuckSackConstant.MAX_TEMPERATURE	/ 2.0f;
	bool IsMessy	= FoodState.Discount	> RuckSackConstant.MAX_DISCOUNT		/ 2.0f;

	// アツアツの時の通知を送る.
	if ( IsHot ) {
		const int MsgNo	= CRandom::GetRand( 0, CONSTANT.HOT_MESSAGE_SIZE - 1 );
		CNotificationUI::PushMessage( EMessageType::Evaluation, CONSTANT.HOT_MESSAGE[MsgNo] );
		return;
	}

	// ぐちゃぐちゃの時の通知を送る.
	if ( IsMessy ) {
		const int MsgNo	= CRandom::GetRand( 0, CONSTANT.MESSY_MESSAGE_SIZE - 1 );
		CNotificationUI::PushMessage( EMessageType::Evaluation, CONSTANT.MESSY_MESSAGE[MsgNo] );
		return;
	}

	// 通常時の通知を送る.
	const int MsgNo = CRandom::GetRand( 0, CONSTANT.NORMAL_MESSAGE_SIZE - 1 );
	CNotificationUI::PushMessage( EMessageType::Evaluation, CONSTANT.NORMAL_MESSAGE[MsgNo] );
}

//---------------------------.
// 連続で紐づけされないようにするクールタイムの更新.
//---------------------------.
void CCustomer::DetachCoolTimeUpdate()
{
	if ( m_DetachCoolTime <= 0.0f ) return;

	m_DetachCoolTime -= m_DeltaTime;
	if ( m_DetachCoolTime <= 0.0f ) m_DetachCoolTime = 0.0f;
}

//---------------------------.
// ピザの取得アニメーションに変更.
//---------------------------.
void CCustomer::ChangeGetPizza()
{
	m_AnimationNo = ChangeGetPizzaAnim;

	// 現在の手の位置を保存.
	m_LeftHandsStartPos		= m_TransformList[LHand].Position;
	m_RightHandsStartPos	= m_TransformList[RHand].Position;
	
	// 初期化.
	m_AnimCnt = ANIM_CNT_MIN;
}

//---------------------------.
// 通常時に変更の更新.
//---------------------------.
void CCustomer::ChangeNormalAnimUpdate()
{
	// ボーン座標の取得.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftEndPos		= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& RightEndPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );

	// 移動ベクトルの作成.
	m_LeftHandMoveVector	= LeftEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector	= RightEndPos - m_RightHandsStartPos;

	// アニメーション用のカウントを加算.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// 座標の更新.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( m_LeftHandMoveVector  * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( m_RightHandMoveVector * m_AnimCnt );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MAX;

		// 座標を合わせる.
		m_TransformList[LHand].Position = LeftEndPos;
		m_TransformList[RHand].Position = RightEndPos;

		// アニメーション終了.
		m_IsGetPizzaAnimEnd = true;
	}
}

//---------------------------.
// 手を振るアニメーションに変更の更新.
//---------------------------.
void CCustomer::ChangeWaveHandsAnimUpdate()
{
	// ボーン座標の取得.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& StartPos	= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& EndPos		= m_pSkinMesh[Body]->GetPosFromBone( WAVEHANDS_END_POS_BONE_NAME );

	// 移動ベクトルの作成.
	m_LeftHandMoveVector = EndPos - StartPos;

	// アニメーション用のカウントを加算.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// 座標の更新.
	m_TransformList[LHand].Position = StartPos + ( m_LeftHandMoveVector * m_AnimCnt );
	m_TransformList[RHand].Position = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MIN;

		// 座標を合わせる.
		m_TransformList[LHand].Position = EndPos;

		// 手を振るアニメーションに変更.
		m_AnimationNo					= WaveHandsAnim;
		m_IsWaveHandsMoveUp				= true;
	}
}

//---------------------------.
// 手を振るアニメーションの更新.
//---------------------------.
void CCustomer::WaveHandsAnimUpdate()
{
	// 移動が終了している場合移動方向を変更する.
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MIN;
		m_IsWaveHandsMoveUp = !m_IsWaveHandsMoveUp;
	}

	// ボーン座標の取得.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& StartPos	= m_pSkinMesh[Body]->GetPosFromBone( WAVEHANDS_START_POS_BONE_NAME );
	const D3DXPOSITION3& EndPos		= m_pSkinMesh[Body]->GetPosFromBone( WAVEHANDS_END_POS_BONE_NAME );

	// ベクトルの更新.
	D3DXPOSITION3 HandsEndPos = INIT_FLOAT3;
	if ( m_IsWaveHandsMoveUp ) {
		m_LeftHandsStartPos		= EndPos;
		HandsEndPos				= StartPos;
		m_LeftHandMoveVector	= StartPos - EndPos;
	}
	else {
		m_LeftHandsStartPos		= StartPos;
		HandsEndPos				= EndPos;
		m_LeftHandMoveVector	= EndPos - StartPos;
	}

	// アニメーション用のカウントを加算.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// 座標の更新.
	m_TransformList[LHand].Position = m_LeftHandsStartPos + ( m_LeftHandMoveVector * m_AnimCnt );
	m_TransformList[RHand].Position = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MAX;

		// 座標を合わせる.
		m_TransformList[LHand].Position = HandsEndPos;
	}
}

//---------------------------.
// ピザの取得アニメーションに変更の更新.
//---------------------------.
void CCustomer::ChangeGetPizzaAnimUpdate()
{
	// ボーン座標の取得.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftHandEndPos  = m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_GET_PIZZA_POS_BONE_NAME );
	const D3DXPOSITION3& RightHandEndPos = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_GET_PIZZA_POS_BONE_NAME );

	// ベクトルの作成.
	m_LeftHandMoveVector  = LeftHandEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector = RightHandEndPos - m_RightHandsStartPos;

	// アニメーション用のカウントを加算.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// 座標の更新.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( m_LeftHandMoveVector  * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( m_RightHandMoveVector * m_AnimCnt );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MIN;

		// 座標を合わせる.
		m_TransformList[LHand].Position = LeftHandEndPos;
		m_TransformList[RHand].Position = RightHandEndPos;
		m_LeftHandsStartPos				= m_TransformList[LHand].Position;
		m_RightHandsStartPos			= m_TransformList[RHand].Position;

		// ピザを取得アニメーションに変更.
		m_AnimationNo = GetPizzaAnim;
	}
}

//---------------------------.
// ピザの取得アニメーションの更新.
//---------------------------.
void CCustomer::GetPizzaAnimUpdate()
{
	// ボーン座標の取得.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& StartPos	= m_pSkinMesh[Body]->GetPosFromBone( HEAD_POS_BONE_NAME );

	// プレイヤーのまでの移動ベクトルの作成.
	const D3DXPOSITION3& EndPos		= m_pPlayer->GetLookPos();
	const D3DXVECTOR3&	 MoveVector	= EndPos - StartPos;

	// アニメーション用のカウントを加算.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// 座標の更新.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( MoveVector * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( MoveVector * m_AnimCnt );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MIN;

		// 手の位置のボーン座標の取得.
		const D3DXPOSITION3& LeftHandPos	= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_GET_PIZZA_POS_BONE_NAME );
		const D3DXPOSITION3& RightHandPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_GET_PIZZA_POS_BONE_NAME );
		
		// 座標を合わせる.
		m_TransformList[LHand].Position = LeftHandPos  + MoveVector;
		m_TransformList[RHand].Position = RightHandPos + MoveVector;
		m_LeftHandsStartPos				= m_TransformList[LHand].Position;
		m_RightHandsStartPos			= m_TransformList[RHand].Position;

		// 通常に戻すアニメーションに変更.
		m_AnimationNo = GetPizzaAnimEnd;
	}
}

//---------------------------.
// ピザの取得終了アニメーションの更新.
//---------------------------.
void CCustomer::GetPizzaAnimEndUpdate()
{
	// ボーン座標の取得.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftHandEndPos		= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_GET_PIZZA_POS_BONE_NAME );
	const D3DXPOSITION3& RightHandEndPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_GET_PIZZA_POS_BONE_NAME );

	// ベクトルの作成.
	m_LeftHandMoveVector	= LeftHandEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector	= RightHandEndPos - m_RightHandsStartPos;

	// アニメーション用のカウントを加算.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// ピザ箱を不透明にしていく.
	if ( m_PizzaBoxAlpha < Color::ALPHA_MAX ) {
		m_PizzaBoxAlpha = m_AnimCnt * 3.0f;
		if ( m_PizzaBoxAlpha >= Color::ALPHA_MAX ) m_PizzaBoxAlpha = Color::ALPHA_MAX;
	}

	// 座標の更新.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( m_LeftHandMoveVector  * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( m_RightHandMoveVector * m_AnimCnt );
	m_PizzaBoxTransform.Position	= m_TransformList[RHand].Position;
	m_PizzaBoxTransform.Rotation	= m_TransformList[Body].Rotation;
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MAX;

		// 座標を合わせる.
		m_TransformList[LHand].Position = LeftHandEndPos;
		m_TransformList[RHand].Position = RightHandEndPos;

		// ピザ箱を不透明にする.
		m_PizzaBoxAlpha = Color::ALPHA_MAX;

		// アニメーション終了.
		m_IsGetPizzaAnimEnd = true;
	}
}
