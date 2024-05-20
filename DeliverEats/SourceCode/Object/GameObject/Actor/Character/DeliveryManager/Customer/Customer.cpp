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
	constexpr float GAUGE_MAX		= 1.0f;	// �󂯎��Q�[�W�̍ő�l.
	constexpr float GAUGE_MIN		= 0.0f;	// �󂯎��Q�[�W�̍ŏ��l.
	constexpr float DELETE_TIME_MIN	= 0.0f;	// �폜�^�C���̍ŏ��l.
	constexpr float ANIM_CNT_MAX	= 1.0f;	// �A�j���[�V�����p�̃J�E���g�̍ő�l.
	constexpr float ANIM_CNT_MIN	= 0.0f;	// �A�j���[�V�����p�̃J�E���g�̍ŏ��l.
	constexpr int	BODY_NO_MIN		= 0;	// �̂̃e�N�X�`��No�̍ŏ��l.
	constexpr int	BODY_NO_MAX		= 3;	// �̂̃e�N�X�`��No�̍ő�l.
	constexpr int	HEAD_NO_MIN		= 0;	// ��̃e�N�X�`��No�̍ŏ��l.
	constexpr int	HEAD_NO_MAX		= 4;	// ��̃e�N�X�`��No�̍ő�l.

	constexpr char	HEAD_POS_BONE_NAME[]					= "customer_head";				// ���̈ʒu�̃{�[����.
	constexpr char	LEFT_HAND_POS_BONE_NAME[]				= "customer_hand_l";			// ����̈ʒu�̃{�[����.
	constexpr char	RIGHT_HAND_POS_BONE_NAME[]				= "customer_hand_r";			// �E��̈ʒu�̃{�[����.
	constexpr char	LEFT_HAND_GET_PIZZA_POS_BONE_NAME[]		= "pizza_box_position2";		// ����̃s�U�擾���̈ʒu�̃{�[����.
	constexpr char	RIGHT_HAND_GET_PIZZA_POS_BONE_NAME[]	= "pizza_box_position1";		// �E��̃s�U�擾���̈ʒu�̃{�[����.
	constexpr char	WAVEHANDS_CENTER_POS_BONE_NAME[]		= "customer_hand_position2";	// ���U��A�j���[�V�����̒����̈ʒu�̃{�[����.
	constexpr char	WAVEHANDS_START_POS_BONE_NAME[]			= "customer_hand_position2_1";	// ���U��A�j���[�V�����̊J�n�̈ʒu�̃{�[����.
	constexpr char	WAVEHANDS_END_POS_BONE_NAME[]			= "customer_hand_position2_2";	// ���U��A�j���[�V�����̏I���̈ʒu�̃{�[����.

	// �X�L�����b�V����No.
	enum enMeshNo : unsigned char {
		Body,		// �{��.
		Head,		// ��.
		LHand,		// ����.
		RHand,		// �E��.

		Mesh_Max,	// ���b�V���̐�.
	} typedef EMeshNo;

	// �A�j���[�V������No.
	enum enAnimationNo : unsigned char {
		NormalAnim,				// �ʏ펞.
		ChangeNormalAnim,		// �ʏ펞�ɕύX.
		ChangeWaveHandsAnim,	// ���U��A�j���[�V�����ɕύX.
		WaveHandsAnim,			// ���U��.
		ChangeGetPizzaAnim,		// �s�U�̎擾�ɕύX.
		GetPizzaAnim,			// �s�U�̎擾.
		GetPizzaAnimEnd,		// �s�U�̎擾�I��.

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
// ������.
//---------------------------.
bool CCustomer::Init()
{
	// ���b�V���̎擾.
	m_pSkinMesh.resize( Mesh_Max );
	m_pSkinMesh[Body ]	= CMeshResource::GetSkin( "customer_body_s" );
	m_pSkinMesh[Head ]	= CMeshResource::GetSkin( "head_s" );
	m_pSkinMesh[LHand]	= CMeshResource::GetSkin( "hand_l_s" );
	m_pSkinMesh[RHand]	= CMeshResource::GetSkin( "hand_r_s" );
	m_pPizzaBox			= CMeshResource::GetSkin( "pizza_box_s" );

	// ���b�V����No�̐ݒ�.
	m_BodyNo = CRandom::GetRand( BODY_NO_MIN, BODY_NO_MAX );
	m_HeadNo = CRandom::GetRand( HEAD_NO_MIN, HEAD_NO_MAX );

	// ���b�V���̍��W�̍X�V.
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

	// �s�U���̎擾.
	m_pPizzaBox		= CMeshResource::GetSkin( "pizza_box_s" );
	m_PizzaBoxAlpha = Color::ALPHA_MIN;

	// �G�t�F�N�g�̏�����.
	m_pGetEffect	= std::make_unique<CEffectManager>();
	m_pGetEffect->SetEffect( "Get" );

	// ���q����̒ǉ�.
	CDeliveryManager::AddEmployee( this, m_Type );

	// �^�O�̐ݒ�.
	m_ObjectTag = EObjectTag::Customer;

	InitCollision();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CCustomer::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
	m_IsDisp = !IsScreenOut();
	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	AnimationUpdate();		// �A�j���[�V�����̍X�V.
	DetachCoolTimeUpdate();	// �A���ŕR�Â�����Ȃ��悤�ɂ���N�[���^�C���̍X�V.

	// �v���C���[�ɓ������Ă��Ȃ���Ԃɂ���.
	m_IsHitPlayer = false;

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::BeAHit );
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CCustomer::LateUpdate( const float& DeltaTime )
{
	if ( m_Handle == NO_LINK_HANDLE ) return;

	// �~�j�}�b�v�ɃA�C�R����ǉ�����.
	CMiniMapUI::PushCharacterIcon( this, m_Direction, m_Handle );

	// �v���C���[�̕��������悤�ɉ�].
	if ( m_pPlayer != nullptr ) {
		const D3DXPOSITION3& PlayerPos = m_pPlayer->GetPosition();
		const D3DXPOSITION3& CustomerPos = m_Transform.Position;
		m_Transform.Rotation.y = atan2f( CustomerPos.x - PlayerPos.x, CustomerPos.z - PlayerPos.z );
		m_TransformList[Body].Rotation.y = m_Transform.Rotation.y;
		m_TransformList[Head].Rotation.y = m_Transform.Rotation.y;
		m_TransformList[LHand].Rotation.y = m_Transform.Rotation.y;
		m_TransformList[RHand].Rotation.y = m_Transform.Rotation.y;

		// ���̉�]�s��̎擾.
		m_HeadRotMatrix = Math::GetLookRotationMatrix( m_pPlayer->GetLookPos(), m_TransformList[Head].Position );
	}

	// �󂯎��Q�[�W������������.
	if ( m_IsHitPlayer ) return;
	if ( m_Gauge > GAUGE_MIN ) {
		m_Gauge -= CONSTANT.GAUGE_DEC_SPEED * DeltaTime;
		if ( m_Gauge <= GAUGE_MIN ) m_Gauge = GAUGE_MIN;
	}

	// �Q�[�W�̍폜�J�E���g�����炵�Ă���.
	if ( m_GaugeDeleteTimeCnt > DELETE_TIME_MIN ) {
		m_GaugeDeleteTimeCnt -= DeltaTime;

		// �Q�[�W�����S�ɍ폜����.
		if ( m_GaugeDeleteTimeCnt <= DELETE_TIME_MIN ) {
			m_GaugeDeleteTimeCnt = DELETE_TIME_MIN;

			// �󂯎��Q�[�W������������.
			m_Gauge = GAUGE_MIN;
		}
	}
}

//---------------------------.
// �`��.
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
// �X�v���C�g(3D)�̕`��.
//---------------------------.
void CCustomer::Sprite3DRender_A()
{
	m_pGetEffect->Render();
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CCustomer::Collision( CActor* pActor )
{
	if ( m_Handle == NO_LINK_HANDLE						) return;
	if ( pActor->GetObjectTag() != EObjectTag::Player	) return;
	if ( m_pPlayer == nullptr ) m_pPlayer = dynamic_cast<CPlayer*>( pActor );

	PlayerShortlyCollision( pActor );	// �v���C���[���߂��ɂ��邩�̓����蔻��.
	GetFoodCollision( pActor );			// ���i�̎󂯎��p�̓����蔻��.
}

//---------------------------.
// �n���ɍs�����q����ɐݒ�.
//---------------------------.
void CCustomer::SetGoHandingCustomer()
{
	m_IsGoHandingCustomer	= true;
	m_ObjectTag				= EObjectTag::GoHandingCustomer;
}

//---------------------------.
// �R�Â��̐ݒ�.
//---------------------------.
void CCustomer::AttachLink( const CLinking::Handle& handle )
{
#ifdef _DEBUG
	// �҂����ԂɕR�Â������Ă��Ȃ����m�F.
	if ( m_DetachCoolTime > 0.0f ) {
		ErrorMessage( "�҂����Ԓ��ɕR�Â����������܂���" );
		return;
	}

	// �R�Â����㏑�������Ȃ����m�F.
	if ( m_Handle != NO_LINK_HANDLE ) {
		ErrorMessage( "���q����̕R�Â��̏㏑�����������܂���" );
		return;
	}
#endif

	// �s�U���𓧖��ɂ���.
	m_PizzaBoxAlpha = Color::ALPHA_MIN;

	// ������.
	m_AnimationNo			= NormalAnim;
	m_IsGetPizzaAnimEnd		= false;
	m_Handle				= handle;
	m_GaugeDeleteTimeCnt	= DELETE_TIME_MIN;
}

//---------------------------.
// �R�Â��̔j��.
//---------------------------.
void CCustomer::DetachLink()
{
	m_Handle			= NO_LINK_HANDLE;
	m_DetachCoolTime	= CONSTANT.DETACH_COOL_TIME;
	m_pCollisions->GetCollision<CCylinder>()->SetFlagOff();
}

//---------------------------.
// �ʏ펞�ɕύX.
//---------------------------.
void CCustomer::ChangeNormal()
{
	m_AnimationNo = ChangeNormalAnim;

	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftEndPos		= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& RightEndPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );

	// �J�n���W�̕ۑ�.
	m_LeftHandsStartPos		= m_TransformList[LHand].Position;
	m_RightHandsStartPos	= m_TransformList[RHand].Position;

	// �ړ��x�N�g���̍쐬.
	m_LeftHandMoveVector	= LeftEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector	= RightEndPos - m_RightHandsStartPos;

	// ������.
	m_AnimCnt = ANIM_CNT_MIN;
}

//---------------------------.
// ���U��A�j���[�V�����ɕύX.
//---------------------------.
void CCustomer::ChangeWaveHands()
{
	m_AnimationNo = ChangeWaveHandsAnim;

	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& StartPos	= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& EndPos		= m_pSkinMesh[Body]->GetPosFromBone( WAVEHANDS_END_POS_BONE_NAME );

	// �ړ��x�N�g���̍쐬.
	m_LeftHandMoveVector	= EndPos - StartPos;

	// ������.
	m_AnimCnt				= ANIM_CNT_MIN;
}

//---------------------------.
// �����蔻��̏�����.
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
// �����蔻��̍X�V.
//---------------------------.
void CCustomer::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetTransform( m_Transform );

	STransform CylinderTransform = m_Transform;
	CylinderTransform.Position.y += CONSTANT.CYLINDER_ADD_POS_Y;
	m_pCollisions->GetCollision<CCylinder>()->SetTransform( CylinderTransform );
}

//---------------------------.
// �v���C���[���߂��ɂ��邩�̓����蔻��.
//---------------------------.
void CCustomer::PlayerShortlyCollision( CActor* pActor )
{
	// �������Ă��邩.
	const D3DXPOSITION3& PlayerPos = pActor->GetPosition();
	m_IsShortly = Coll::IsSphereToPosition(
		m_pCollisions->GetCollision<CSphere>(),
		PlayerPos );
}

//---------------------------.
// ���i�̎󂯎��p�̓����蔻��.
//---------------------------.
void CCustomer::GetFoodCollision( CActor* pActor )
{
	if ( m_IsShortly == false ) return;

	// �n���ɍs�����q���񂩂ǂ���.
	if ( m_IsGoHandingCustomer == false ) return;

	// �������Ă��邩.
	if ( Coll::IsCylinderToPosition(
		m_pCollisions->GetCollision<CCylinder>(),
		pActor->GetPosition() ) == false ) return;

	// �v���C���[�ɓ�������.
	m_IsHitPlayer			= true;
	m_GaugeDeleteTimeCnt	= CONSTANT.GAUGE_DELETE_TIME;
	
	// �󂯎��Q�[�W�̈ړ����x�̐ݒ�.
	if ( m_Gauge >= GAUGE_MAX ) return;
	float GaugeSpeed = CONSTANT.GAUGE_SPEED * m_DeltaTime;

	// �v���C���[�����n���Ă���ꍇ�Q�[�W���x��������.
	if ( m_pPlayer->GetIsLanding() ) GaugeSpeed *= CONSTANT.GAUGE_SPEED_UP;

	// �v���C���[�����q����̕��������Ă��邩.
	const D3DXVECTOR3&	CustomerRotVec		= m_Transform.GetRotationVector();
	const float			CustomerRotLength	= D3DXVec3Length( &CustomerRotVec );
	const D3DXVECTOR3&	SumRotVec			= CustomerRotVec + pActor->GetTransform().GetRotationVector();
	const float			SumRotLength		= D3DXVec3Length( &SumRotVec );
	if ( SumRotLength <= CustomerRotLength ) GaugeSpeed *= CONSTANT.GAUGE_SPEED_UP;

	// �󂯎��Q�[�W�𑝂₷.
	m_Gauge += GaugeSpeed;

	// ���q����ɏ��i��n��.
	if ( m_Gauge >= GAUGE_MAX ) {
		m_Gauge = GAUGE_MIN;
		const SFoodState FoodState = m_pPlayer->GetFoodState();
		m_pPlayer->FoodPassed();

		// �V�������q����Ƃ��X��R�Â���.
		CLinking::Link();
		CMessageWindow::PushMessage( u8"���i��n���܂����B" );

		// �R�Â��̉���.
		CLinking::SetIsGoToPass( m_Handle, false );
		CLinking::LinkEnd( m_Handle );

		// ���ʂ̂��q����ɕύX.
		m_IsGoHandingCustomer	= false;
		m_ObjectTag				= EObjectTag::Customer;

		// �G�t�F�N�g�̍Đ�.
		m_pGetEffect->Play( m_Transform.Position );

		// �ʒm�̑���.
		PushNotification( FoodState );

		// �s�U���̕ύX.
		m_PizzaBoxNo = FoodState.ShopNo;

		// �s�U�̎擾�A�j���[�V�����ɕύX.
		ChangeGetPizza();
	}
}

//---------------------------.
// �g�����X�t�H�[���̍X�V.
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
// �A�j���[�V�����̍X�V.
//---------------------------.
void CCustomer::AnimationUpdate()
{
	// �Ή������A�j���[�V�������s��.
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
// �ʒm�𑗂�.
//---------------------------.
void CCustomer::PushNotification( const SFoodState& FoodState )
{
	ConstantStruct::SRuckSack RuckSackConstant = CConstant::GetRuckSack();
	bool IsHot		= FoodState.Temperature	> RuckSackConstant.MAX_TEMPERATURE	/ 2.0f;
	bool IsMessy	= FoodState.Discount	> RuckSackConstant.MAX_DISCOUNT		/ 2.0f;

	// �A�c�A�c�̎��̒ʒm�𑗂�.
	if ( IsHot ) {
		const int MsgNo	= CRandom::GetRand( 0, CONSTANT.HOT_MESSAGE_SIZE - 1 );
		CNotificationUI::PushMessage( EMessageType::Evaluation, CONSTANT.HOT_MESSAGE[MsgNo] );
		return;
	}

	// �����Ⴎ����̎��̒ʒm�𑗂�.
	if ( IsMessy ) {
		const int MsgNo	= CRandom::GetRand( 0, CONSTANT.MESSY_MESSAGE_SIZE - 1 );
		CNotificationUI::PushMessage( EMessageType::Evaluation, CONSTANT.MESSY_MESSAGE[MsgNo] );
		return;
	}

	// �ʏ펞�̒ʒm�𑗂�.
	const int MsgNo = CRandom::GetRand( 0, CONSTANT.NORMAL_MESSAGE_SIZE - 1 );
	CNotificationUI::PushMessage( EMessageType::Evaluation, CONSTANT.NORMAL_MESSAGE[MsgNo] );
}

//---------------------------.
// �A���ŕR�Â�����Ȃ��悤�ɂ���N�[���^�C���̍X�V.
//---------------------------.
void CCustomer::DetachCoolTimeUpdate()
{
	if ( m_DetachCoolTime <= 0.0f ) return;

	m_DetachCoolTime -= m_DeltaTime;
	if ( m_DetachCoolTime <= 0.0f ) m_DetachCoolTime = 0.0f;
}

//---------------------------.
// �s�U�̎擾�A�j���[�V�����ɕύX.
//---------------------------.
void CCustomer::ChangeGetPizza()
{
	m_AnimationNo = ChangeGetPizzaAnim;

	// ���݂̎�̈ʒu��ۑ�.
	m_LeftHandsStartPos		= m_TransformList[LHand].Position;
	m_RightHandsStartPos	= m_TransformList[RHand].Position;
	
	// ������.
	m_AnimCnt = ANIM_CNT_MIN;
}

//---------------------------.
// �ʏ펞�ɕύX�̍X�V.
//---------------------------.
void CCustomer::ChangeNormalAnimUpdate()
{
	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftEndPos		= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& RightEndPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );

	// �ړ��x�N�g���̍쐬.
	m_LeftHandMoveVector	= LeftEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector	= RightEndPos - m_RightHandsStartPos;

	// �A�j���[�V�����p�̃J�E���g�����Z.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// ���W�̍X�V.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( m_LeftHandMoveVector  * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( m_RightHandMoveVector * m_AnimCnt );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MAX;

		// ���W�����킹��.
		m_TransformList[LHand].Position = LeftEndPos;
		m_TransformList[RHand].Position = RightEndPos;

		// �A�j���[�V�����I��.
		m_IsGetPizzaAnimEnd = true;
	}
}

//---------------------------.
// ���U��A�j���[�V�����ɕύX�̍X�V.
//---------------------------.
void CCustomer::ChangeWaveHandsAnimUpdate()
{
	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& StartPos	= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& EndPos		= m_pSkinMesh[Body]->GetPosFromBone( WAVEHANDS_END_POS_BONE_NAME );

	// �ړ��x�N�g���̍쐬.
	m_LeftHandMoveVector = EndPos - StartPos;

	// �A�j���[�V�����p�̃J�E���g�����Z.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// ���W�̍X�V.
	m_TransformList[LHand].Position = StartPos + ( m_LeftHandMoveVector * m_AnimCnt );
	m_TransformList[RHand].Position = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MIN;

		// ���W�����킹��.
		m_TransformList[LHand].Position = EndPos;

		// ���U��A�j���[�V�����ɕύX.
		m_AnimationNo					= WaveHandsAnim;
		m_IsWaveHandsMoveUp				= true;
	}
}

//---------------------------.
// ���U��A�j���[�V�����̍X�V.
//---------------------------.
void CCustomer::WaveHandsAnimUpdate()
{
	// �ړ����I�����Ă���ꍇ�ړ�������ύX����.
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MIN;
		m_IsWaveHandsMoveUp = !m_IsWaveHandsMoveUp;
	}

	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& StartPos	= m_pSkinMesh[Body]->GetPosFromBone( WAVEHANDS_START_POS_BONE_NAME );
	const D3DXPOSITION3& EndPos		= m_pSkinMesh[Body]->GetPosFromBone( WAVEHANDS_END_POS_BONE_NAME );

	// �x�N�g���̍X�V.
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

	// �A�j���[�V�����p�̃J�E���g�����Z.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// ���W�̍X�V.
	m_TransformList[LHand].Position = m_LeftHandsStartPos + ( m_LeftHandMoveVector * m_AnimCnt );
	m_TransformList[RHand].Position = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MAX;

		// ���W�����킹��.
		m_TransformList[LHand].Position = HandsEndPos;
	}
}

//---------------------------.
// �s�U�̎擾�A�j���[�V�����ɕύX�̍X�V.
//---------------------------.
void CCustomer::ChangeGetPizzaAnimUpdate()
{
	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftHandEndPos  = m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_GET_PIZZA_POS_BONE_NAME );
	const D3DXPOSITION3& RightHandEndPos = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_GET_PIZZA_POS_BONE_NAME );

	// �x�N�g���̍쐬.
	m_LeftHandMoveVector  = LeftHandEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector = RightHandEndPos - m_RightHandsStartPos;

	// �A�j���[�V�����p�̃J�E���g�����Z.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// ���W�̍X�V.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( m_LeftHandMoveVector  * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( m_RightHandMoveVector * m_AnimCnt );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MIN;

		// ���W�����킹��.
		m_TransformList[LHand].Position = LeftHandEndPos;
		m_TransformList[RHand].Position = RightHandEndPos;
		m_LeftHandsStartPos				= m_TransformList[LHand].Position;
		m_RightHandsStartPos			= m_TransformList[RHand].Position;

		// �s�U���擾�A�j���[�V�����ɕύX.
		m_AnimationNo = GetPizzaAnim;
	}
}

//---------------------------.
// �s�U�̎擾�A�j���[�V�����̍X�V.
//---------------------------.
void CCustomer::GetPizzaAnimUpdate()
{
	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& StartPos	= m_pSkinMesh[Body]->GetPosFromBone( HEAD_POS_BONE_NAME );

	// �v���C���[�̂܂ł̈ړ��x�N�g���̍쐬.
	const D3DXPOSITION3& EndPos		= m_pPlayer->GetLookPos();
	const D3DXVECTOR3&	 MoveVector	= EndPos - StartPos;

	// �A�j���[�V�����p�̃J�E���g�����Z.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// ���W�̍X�V.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( MoveVector * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( MoveVector * m_AnimCnt );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MIN;

		// ��̈ʒu�̃{�[�����W�̎擾.
		const D3DXPOSITION3& LeftHandPos	= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_GET_PIZZA_POS_BONE_NAME );
		const D3DXPOSITION3& RightHandPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_GET_PIZZA_POS_BONE_NAME );
		
		// ���W�����킹��.
		m_TransformList[LHand].Position = LeftHandPos  + MoveVector;
		m_TransformList[RHand].Position = RightHandPos + MoveVector;
		m_LeftHandsStartPos				= m_TransformList[LHand].Position;
		m_RightHandsStartPos			= m_TransformList[RHand].Position;

		// �ʏ�ɖ߂��A�j���[�V�����ɕύX.
		m_AnimationNo = GetPizzaAnimEnd;
	}
}

//---------------------------.
// �s�U�̎擾�I���A�j���[�V�����̍X�V.
//---------------------------.
void CCustomer::GetPizzaAnimEndUpdate()
{
	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftHandEndPos		= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_GET_PIZZA_POS_BONE_NAME );
	const D3DXPOSITION3& RightHandEndPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_GET_PIZZA_POS_BONE_NAME );

	// �x�N�g���̍쐬.
	m_LeftHandMoveVector	= LeftHandEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector	= RightHandEndPos - m_RightHandsStartPos;

	// �A�j���[�V�����p�̃J�E���g�����Z.
	m_AnimCnt += CONSTANT.WAVEHANDS_ANIMATION_SPEED * m_DeltaTime;

	// �s�U����s�����ɂ��Ă���.
	if ( m_PizzaBoxAlpha < Color::ALPHA_MAX ) {
		m_PizzaBoxAlpha = m_AnimCnt * 3.0f;
		if ( m_PizzaBoxAlpha >= Color::ALPHA_MAX ) m_PizzaBoxAlpha = Color::ALPHA_MAX;
	}

	// ���W�̍X�V.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( m_LeftHandMoveVector  * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( m_RightHandMoveVector * m_AnimCnt );
	m_PizzaBoxTransform.Position	= m_TransformList[RHand].Position;
	m_PizzaBoxTransform.Rotation	= m_TransformList[Body].Rotation;
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MAX;

		// ���W�����킹��.
		m_TransformList[LHand].Position = LeftHandEndPos;
		m_TransformList[RHand].Position = RightHandEndPos;

		// �s�U����s�����ɂ���.
		m_PizzaBoxAlpha = Color::ALPHA_MAX;

		// �A�j���[�V�����I��.
		m_IsGetPizzaAnimEnd = true;
	}
}
