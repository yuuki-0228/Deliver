#include "Employee.h"
#include "..\Customer\Customer.h"
#include "..\..\DeliveryManager\DeliveryManager.h"
#include "..\..\Player\Player.h"
#include "..\..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
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
	constexpr float DELETE_TIME_MIN = 0.0f;	// �폜�^�C���̍ŏ��l.
	constexpr float ANIM_CNT_MAX	= 1.0f;	// �A�j���[�V�����p�̃J�E���g�̍ő�l.
	constexpr float ANIM_CNT_MIN	= 0.0f;	// �A�j���[�V�����p�̃J�E���g�̍ŏ��l.
	constexpr int	HEAD_NO_MIN		= 0;	// ��̃e�N�X�`��No�̍ŏ��l.
	constexpr int	HEAD_NO_MAX		= 4;	// ��̃e�N�X�`��No�̍ő�l.

	constexpr char	HEAD_POS_BONE_NAME[]					= "employee_head";				// ���̈ʒu�̃{�[����.
	constexpr char	LEFT_HAND_POS_BONE_NAME[]				= "employee_hand_l";			// ����̈ʒu�̃{�[����.
	constexpr char	RIGHT_HAND_POS_BONE_NAME[]				= "employee_hand_r";			// �E��̈ʒu�̃{�[����.
	constexpr char	LEFT_HAND_GET_PIZZA_POS_BONE_NAME[]		= "pizza_box_position2";		// ����̃s�U�擾���̈ʒu�̃{�[����.
	constexpr char	RIGHT_HAND_GET_PIZZA_POS_BONE_NAME[]	= "pizza_box_position1";		// �E��̃s�U�擾���̈ʒu�̃{�[����.
	constexpr char	WAVEHANDS_CENTER_POS_BONE_NAME[]		= "employee_hand_position2";	// ���U��A�j���[�V�����̒����̈ʒu�̃{�[����.
	constexpr char	WAVEHANDS_START_POS_BONE_NAME[]			= "employee_hand_position2_1";	// ���U��A�j���[�V�����̊J�n�̈ʒu�̃{�[����.
	constexpr char	WAVEHANDS_END_POS_BONE_NAME[]			= "employee_hand_position2_2";	// ���U��A�j���[�V�����̏I���̈ʒu�̃{�[����.

	// �X�L�����b�V����No.
	enum enMeshNo : unsigned char {
		Body,		// �{��.
		Head,		// ��.
		Cap,		// �X�q.
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
		ChangeGetPizzaAnim,		// �s�U����n���ɕύX.
		PizzaPassedAnim,		// �s�U����n��.
		PizzaPassedAnimEnd,		// �s�U����n������ɒʏ�ɕύX.
		ChangeHavePizzaAnim,	// �s�U�������ɕύX.

		AnimationNo_Max
	} typedef EAnimationNo;
}

CEmployee::CEmployee( const EObjectTag TypeTag, int ShopNo )
	: m_pSkinMesh				()
	, m_TransformList			()
	, m_pHandOverEffect			( nullptr )
	, m_pPizzaBox				( nullptr )
	, m_pPlayer					( nullptr )
	, m_PointLight				()
	, m_PizzaBoxTransform		()
	, m_Handle					( NO_LINK_HANDLE )
	, m_RightHandMoveVector		( INIT_FLOAT3 )
	, m_LeftHandMoveVector		( INIT_FLOAT3 )
	, m_RightHandsStartPos		( INIT_FLOAT3 )
	, m_LeftHandsStartPos		( INIT_FLOAT3 )
	, m_HeadRotMatrix			()
	, m_Type					( TypeTag )
	, m_Direction				( EDirection::None )
	, m_AddMoney				( INIT_FLOAT )
	, m_Gauge					( GAUGE_MIN )
	, m_GaugeDeleteTimeCnt		( DELETE_TIME_MIN )
	, m_Alpha					( Color::ALPHA_MAX )
	, m_DetachCoolTime			( INIT_FLOAT )
	, m_PizzaBoxAlpha			( Color::ALPHA_MIN )
	, m_AnimCnt					( INIT_FLOAT )
	, m_AnimationNo				( INIT_INT )
	, m_ShopNo					( ShopNo )
	, m_Money					( INIT_INT )
	, m_HeadNo					( INIT_INT )
	, m_PizzaBoxNo				( INIT_INT )
	, m_IsHitPlayer				( false )
	, m_IsShortly				( false )
	, m_IsPassed				( false )
	, m_IsGetFoodEmployee		( false )
	, m_IsWaveHandsMoveUp		( false )
	, m_IsPizzaPassedAnimEnd	( false )
	, m_IsPizzaBoxAlpha			( false )
	, m_IsLookPlayer			( false )
{
}

CEmployee::~CEmployee()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CEmployee::Init()
{
	// ���b�V���̎擾.
	m_pSkinMesh.resize( Mesh_Max );
	m_pSkinMesh[Body ] = CMeshResource::GetSkin( "employee_body_s" );
	m_pSkinMesh[Head ] = CMeshResource::GetSkin( "head_s" );
	m_pSkinMesh[Cap  ] = CMeshResource::GetSkin( "employee_cap_s" );
	m_pSkinMesh[LHand] = CMeshResource::GetSkin( "hand_l_s" );
	m_pSkinMesh[RHand] = CMeshResource::GetSkin( "hand_r_s" );

	// ���b�V����No�̐ݒ�.
	m_HeadNo = CRandom::GetRand( HEAD_NO_MIN, HEAD_NO_MAX );

	// ���b�V���̍��W�̍X�V.
	m_TransformList.resize( Mesh_Max );
	m_TransformList[Body ]			= m_Transform;
	m_pSkinMesh[Body ]->SetTransform( m_TransformList[Body ] );
	m_TransformList[Head ]			= m_Transform;
	m_TransformList[Cap ]			= m_Transform;
	m_TransformList[LHand]			= m_Transform;
	m_TransformList[RHand]			= m_Transform;
	m_TransformList[Head ].Position = m_pSkinMesh[Body]->GetPosFromBone( HEAD_POS_BONE_NAME );
	m_TransformList[Cap  ].Position = m_pSkinMesh[Body]->GetPosFromBone( HEAD_POS_BONE_NAME );
	m_TransformList[LHand].Position = m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	m_TransformList[RHand].Position = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );
	m_pSkinMesh[Head ]->SetTransform( m_TransformList[Head ] );
	m_pSkinMesh[Cap  ]->SetTransform( m_TransformList[Cap  ] );
	m_pSkinMesh[LHand]->SetTransform( m_TransformList[LHand] );
	m_pSkinMesh[RHand]->SetTransform( m_TransformList[RHand] );

	// �s�U���̎擾.
	m_pPizzaBox = CMeshResource::GetSkin( "pizza_box_s" );

	// �G�t�F�N�g�̏�����.
	m_pHandOverEffect = std::make_unique<CEffectManager>();
	m_pHandOverEffect->SetEffect( "Get" );

	// �X���̒ǉ�.
	CDeliveryManager::AddEmployee( this, m_Type );

	// �^�O�̐ݒ�.
	m_ObjectTag = EObjectTag::Employee;

	InitCollision();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CEmployee::Update( const float& DeltaTime )
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
void CEmployee::LateUpdate( const float& DeltaTime )
{
	if ( m_Handle == NO_LINK_HANDLE ) {
		m_IsGetFoodEmployee = false;
		return;
	}

	// �~�j�}�b�v�ɃA�C�R����ǉ�����.
	CMiniMapUI::PushCharacterIcon( this, m_Direction, m_Handle );

	// �v���C���[�̂ق�������.
	if ( m_IsLookPlayer && m_pPlayer != nullptr ) {
		// �v���C���[�̕�������.
		const D3DXPOSITION3& PlayerPos		= m_pPlayer->GetPosition();
		const D3DXPOSITION3& EmployeePos	= m_Transform.Position;
		m_Transform.Rotation.y = atan2f( EmployeePos.x - PlayerPos.x, EmployeePos.z - PlayerPos.z );
		m_TransformList[Body ].Rotation.y	= m_Transform.Rotation.y;
		m_TransformList[Head ].Rotation.y	= m_Transform.Rotation.y;
		m_TransformList[Cap  ].Rotation.y	= m_Transform.Rotation.y;
		m_TransformList[LHand].Rotation.y	= m_Transform.Rotation.y;
		m_TransformList[RHand].Rotation.y	= m_Transform.Rotation.y;

		// ���̉�]�s��̎擾.
		m_HeadRotMatrix = Math::GetLookRotationMatrix( m_pPlayer->GetLookPos(), m_TransformList[Head].Position );
	}
	if ( m_IsPassed					) return;
	if ( m_IsHitPlayer				) return;

	// �󂯎��Q�[�W������������.
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
			m_Gauge				= GAUGE_MIN;
			m_IsGetFoodEmployee = false;
		}
	}

	// �R�Â��̉����̃J�E���g���ĊJ����.
	if ( m_IsShortly ) return;
	if ( m_GaugeDeleteTimeCnt == DELETE_TIME_MIN )
		CLinking::SetIsLinkEndStop( m_Handle, false );
}

//---------------------------.
// �`��.
//---------------------------.
void CEmployee::Render()
{
	if ( m_IsDisp == false ) return;

	TransformUpdate();

	m_pSkinMesh[Body ]->SetDither( true );
	m_pSkinMesh[Head ]->SetDither( true );
	m_pSkinMesh[Cap  ]->SetDither( true );
	m_pSkinMesh[LHand]->SetDither( true );
	m_pSkinMesh[RHand]->SetDither( true );
	m_pPizzaBox->SetDither( true );
	m_pSkinMesh[Body ]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pSkinMesh[Head ]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pSkinMesh[Cap  ]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pSkinMesh[LHand]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pSkinMesh[RHand]->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pPizzaBox->SetAlpha( m_PizzaBoxAlpha );
	m_pSkinMesh[Body ]->ChangeTexture(	0, m_ShopNo - 1 );
	m_pSkinMesh[Head ]->ChangeTexture(	0, m_HeadNo );
	m_pSkinMesh[Cap  ]->ChangeTexture(	0, m_ShopNo - 1 );
	m_pPizzaBox->ChangeTexture(			0, m_ShopNo - 1 );
	m_pSkinMesh[Body]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[Head]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[Cap]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[LHand]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[RHand]->SetMeshPointLight( m_PointLight );
	m_pSkinMesh[Body ]->Render( &m_TransformList[Body ] );
	if ( m_Handle == NO_LINK_HANDLE || m_IsLookPlayer == false ) {
		m_pSkinMesh[Head]->Render( &m_TransformList[Head] );
		m_pSkinMesh[Cap ]->Render( &m_TransformList[Cap ] );
	}
	else {
		m_pSkinMesh[Head]->Render( &m_TransformList[Head], nullptr, &m_HeadRotMatrix );
		m_pSkinMesh[Cap ]->Render( &m_TransformList[Cap ], nullptr, &m_HeadRotMatrix );
	}
	m_pSkinMesh[LHand]->Render( &m_TransformList[LHand] );
	m_pSkinMesh[RHand]->Render( &m_TransformList[RHand] );
	m_pPizzaBox->Render( &m_PizzaBoxTransform );
	m_pSkinMesh[Body ]->SetDither( false );
	m_pSkinMesh[Head ]->SetDither( false );
	m_pSkinMesh[Cap  ]->SetDither( false );
	m_pSkinMesh[LHand]->SetDither( false );
	m_pSkinMesh[RHand]->SetDither( false );
	m_pPizzaBox->SetDither( false );

	CCollisionRender::PushSphere( m_pCollisions->GetCollision<CSphere>() );

	if ( m_Handle	== NO_LINK_HANDLE	) return;
	if ( m_IsPassed == true				) return;
	CCollisionRender::PushCylinder( m_pCollisions->GetCollision<CCylinder>() );
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//---------------------------.
void CEmployee::Sprite3DRender_A()
{
	m_pHandOverEffect->Render();
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CEmployee::Collision( CActor* pActor )
{
	if ( pActor->GetObjectTag() != EObjectTag::Player	) return;
	if ( m_pPlayer == nullptr ) m_pPlayer = dynamic_cast<CPlayer*>( pActor );

	// �v���C���[���߂��ɂ��邩�̓����蔻��.
	PlayerShortlyCollision( pActor );

	if ( m_Handle == NO_LINK_HANDLE ) return;

	// ���i��n���p�̓����蔻��.
	if ( m_IsPassed == true ) return;
	HandOverFoodCollision( pActor );
}

//---------------------------.
// �R�Â��̐ݒ�.
//---------------------------.
void CEmployee::AttachLink( const CLinking::Handle& handle )
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
	m_IsPassed				= false;
	m_Handle				= handle;
	m_GaugeDeleteTimeCnt	= DELETE_TIME_MIN;

	// �Œ�����炦�邨���̐ݒ�.
	m_Money = GetMoney();
	if ( m_Money >= CONSTANT.MONEY_MAX ) m_Money = CONSTANT.MONEY_MAX - 1;
}

//---------------------------.
// �R�Â��̔j��.
//---------------------------.
void CEmployee::DetachLink()
{
	m_Handle			= NO_LINK_HANDLE;
	m_DetachCoolTime	= CONSTANT.DETACH_COOL_TIME;
	m_pCollisions->GetCollision<CCylinder>()->SetFlagOff();
}

//---------------------------.
// �ʏ펞�ɕύX.
//---------------------------.
void CEmployee::ChangeNormal()
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

	// �s�U�����A���t�@�������邩.
	if ( m_PizzaBoxAlpha > Color::ALPHA_MIN ) {
		m_IsPizzaBoxAlpha = true;
	}
}

//---------------------------.
// ���U��A�j���[�V�����ɕύX.
//---------------------------.
void CEmployee::ChangeWaveHands()
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
// �s�U�������A�j���[�V�����ɕύX.
//---------------------------.
void CEmployee::ChangeHavePizaBox()
{
	m_AnimationNo = ChangeHavePizzaAnim;

	// �J�n���W�̕ۑ�.
	m_LeftHandsStartPos		= m_TransformList[LHand].Position;
	m_RightHandsStartPos	= m_TransformList[RHand].Position;

	// ������.
	m_AnimCnt				= ANIM_CNT_MIN;
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CEmployee::InitCollision()
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
void CEmployee::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetTransform( m_Transform );

	STransform CylinderTransform = m_Transform;
	CylinderTransform.Position.y += CONSTANT.CYLINDER_ADD_POS_Y;
	m_pCollisions->GetCollision<CCylinder>()->SetTransform( CylinderTransform );
}

//---------------------------.
// �v���C���[���߂��ɂ��邩�̓����蔻��.
//---------------------------.
void CEmployee::PlayerShortlyCollision( CActor* pActor )
{
	// �������Ă��邩.
	const D3DXPOSITION3& PlayerPos = pActor->GetPosition();
	m_IsShortly = Coll::IsSphereToPosition(
		m_pCollisions->GetCollision<CSphere>(),
		PlayerPos );
	if ( m_IsShortly == false			) return;
	if ( m_Handle	 == NO_LINK_HANDLE	) return;

	// �R�Â��̉����̃J�E���g���~����.
	CLinking::SetIsLinkEndStop( m_Handle, true );
}

//---------------------------.
// ���i��n���p�̓����蔻��.
//---------------------------.
void CEmployee::HandOverFoodCollision( CActor* pActor )
{
	if ( m_IsShortly == false ) return;

	// ���i�������Ă���ꍇ�͏����͍s��Ȃ�.
	if ( m_pPlayer->GetIsFoodHave() ) return;

	// �������Ă��邩.
	if ( Coll::IsCylinderToPosition(
		m_pCollisions->GetCollision<CCylinder>(),
		pActor->GetPosition() ) == false ) return;

	// �v���C���[�ɓ�������.
	m_IsHitPlayer			= true;
	m_IsGetFoodEmployee		= true;
	m_GaugeDeleteTimeCnt	= CONSTANT.GAUGE_DELETE_TIME;

	// �󂯎��Q�[�W�̈ړ����x�̐ݒ�.
	if ( m_Gauge >= GAUGE_MAX ) return;
	float GaugeSpeed = CONSTANT.GAUGE_SPEED * m_DeltaTime;

	// �v���C���[�����n���Ă���ꍇ�Q�[�W���x��������.
	if ( m_pPlayer->GetIsLanding() ) GaugeSpeed *= CONSTANT.GAUGE_SPEED_UP;

	// �v���C���[���X���̕��������Ă��邩.
	const D3DXVECTOR3&	EmployeeRotVec		= m_Transform.GetRotationVector();
	const float			EmployeeRotLength	= D3DXVec3Length( &EmployeeRotVec );
	const D3DXVECTOR3&	SumRotVec			= EmployeeRotVec + m_pPlayer->GetTransform().GetRotationVector();
	const float			SumRotLength		= D3DXVec3Length( &SumRotVec );
	if ( SumRotLength <= EmployeeRotLength ) GaugeSpeed *= CONSTANT.GAUGE_SPEED_UP;

	// �󂯎��Q�[�W�𑝂₷.
	m_Gauge += GaugeSpeed;

	// �v���C���[�ɏ��i��n��.
	if ( m_Gauge >= GAUGE_MAX ) {
		m_Gauge				= GAUGE_MIN;
		m_IsPassed			= true;
		m_IsGetFoodEmployee = false;
		m_pPlayer->SetNewFood( m_Money, m_ShopNo );

		// ���q����ɓn���ɍs�����q����ɂȂ������Ƃ�������.
		CLinking::GetCustomer( m_Handle )->SetGoHandingCustomer();
		CLinking::SetIsGoToPass( m_Handle, true );

		// �G�t�F�N�g�̍Đ�.
		m_pHandOverEffect->Play( m_pPlayer->GetPosition() );

		CMessageWindow::PushMessage( u8"���i���󂯎��܂����B" );

		// �s�U����n��.
		ChangeGetPizza();
	}
}

//---------------------------.
// �g�����X�t�H�[���̍X�V.
//---------------------------.
void CEmployee::TransformUpdate()
{
	m_TransformList[Body ]			= m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	if ( m_Handle == NO_LINK_HANDLE || m_IsLookPlayer == false ) {
		m_TransformList[Head] = m_Transform;
		m_TransformList[Cap ] = m_Transform;
	}
	m_TransformList[Head ].Position = m_pSkinMesh[Body]->GetPosFromBone( HEAD_POS_BONE_NAME );
	m_TransformList[Cap  ].Position = m_pSkinMesh[Body]->GetPosFromBone( HEAD_POS_BONE_NAME );

	if ( m_AnimationNo != NormalAnim ) return;
	m_TransformList[LHand]			= m_Transform;
	m_TransformList[LHand].Position = m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	m_TransformList[RHand].Position = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );
}

//---------------------------.
// �A�j���[�V�����̍X�V.
//---------------------------.
void CEmployee::AnimationUpdate()
{
	// �Ή������A�j���[�V�������s��.
	switch ( m_AnimationNo ) {
	case ChangeNormalAnim:		ChangeNormalAnimUpdate();		break;
	case ChangeWaveHandsAnim:	ChangeWaveHandsAnimUpdate();	break;
	case WaveHandsAnim:			WaveHandsAnimUpdate();			break;
	case ChangeGetPizzaAnim:	ChangeGetPizzaAnimUpdate();		break;
	case PizzaPassedAnim:		PizzaPassedAnimUpdate();		break;
	case PizzaPassedAnimEnd:	PizzaPassedAnimEndUpdate();		break;
	case ChangeHavePizzaAnim:	ChangeHavePizzaAnimUpdate();	break;
	default:													break;
	}
}

//---------------------------.
// �l�i���擾.
//---------------------------.
int CEmployee::GetMoney()
{
	// �}���V�����^�C�v�ɂ��ǉ��̂���.
	const float			ApartmentAddMoney	= m_AddMoney + CLinking::GetCustomer( m_Handle )->GetAddMoney();

	// ���X�̍����ɂ��ǉ��̂���.
	const float			E_HightInterval		= m_Transform.Position.y / CONSTANT.HIGHT_MONEY_INTERVAL;
	const float			E_HightAddMoney		= CONSTANT.HIGHT_ADD_MONEY * E_HightInterval;

	// ���q����̍����ɂ��ǉ��̂���.
	const D3DXVECTOR3&	CustomerPos			= CLinking::GetCustomer( m_Handle )->GetPosition();
	const float			C_HightInterval		= CustomerPos.y / CONSTANT.HIGHT_MONEY_INTERVAL;
	const float			C_HightAddMoney		= CONSTANT.HIGHT_ADD_MONEY * C_HightInterval;

	// ���q����Ƃ̋����ɂ��ǉ��̂���.
	const D3DXVECTOR3&	Vector				= CustomerPos - m_Transform.Position;
	const float			Distance			= D3DXVec3Length( &Vector );
	const float			DistanceInterval	= Distance / CONSTANT.DISTANCE_MONEY_INTERVAL;

	// �S�Čv�Z����������Ԃ�.
	const float			Money				= CONSTANT.MONEY + ApartmentAddMoney + ( E_HightAddMoney + C_HightAddMoney ) * DistanceInterval;
	return static_cast<int>( Money );
}

//---------------------------.
// �A���ŕR�Â�����Ȃ��悤�ɂ���N�[���^�C���̍X�V.
//---------------------------.
void CEmployee::DetachCoolTimeUpdate()
{
	if ( m_DetachCoolTime <= 0.0f ) return;

	m_DetachCoolTime -= m_DeltaTime;
	if ( m_DetachCoolTime <= 0.0f ) m_DetachCoolTime = 0.0f;
}

//---------------------------.
// �s�U�̎擾�A�j���[�V�����ɕύX.
//---------------------------.
void CEmployee::ChangeGetPizza()
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
void CEmployee::ChangeNormalAnimUpdate()
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

	// �s�U���𓧖��ɂ��Ă���.
	if ( m_IsPizzaBoxAlpha && m_PizzaBoxAlpha > Color::ALPHA_MIN ) {
		m_PizzaBoxAlpha = 1.0f - m_AnimCnt;
		if ( m_PizzaBoxAlpha <= Color::ALPHA_MIN ) {
			m_PizzaBoxAlpha		= Color::ALPHA_MIN;
			m_IsPizzaBoxAlpha	= false;
		}
	}

	// ���W�̍X�V.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( m_LeftHandMoveVector  * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( m_RightHandMoveVector * m_AnimCnt );
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		m_AnimCnt = ANIM_CNT_MAX;

		// ���W�����킹��.
		m_TransformList[LHand].Position = LeftEndPos;
		m_TransformList[RHand].Position = RightEndPos;

		// �A�j���[�V�����I��.
		m_IsPizzaPassedAnimEnd = true;
	}
}

//---------------------------.
// ���U��A�j���[�V�����ɕύX�̍X�V.
//---------------------------.
void CEmployee::ChangeWaveHandsAnimUpdate()
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
void CEmployee::WaveHandsAnimUpdate()
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
// �s�U��n���A�j���[�V�����ɕύX�̍X�V.
//---------------------------.
void CEmployee::ChangeGetPizzaAnimUpdate()
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
		m_AnimCnt = ANIM_CNT_MIN;

		// ���W�����킹��.
		m_TransformList[LHand].Position = LeftHandEndPos;
		m_TransformList[RHand].Position = RightHandEndPos;
		m_LeftHandsStartPos				= m_TransformList[LHand].Position;
		m_RightHandsStartPos			= m_TransformList[RHand].Position;

		// �s�U����s�����ɂ���.
		m_PizzaBoxAlpha = Color::ALPHA_MAX;

		// �s�U���擾�A�j���[�V�����ɕύX.
		m_AnimationNo = PizzaPassedAnim;
	}
}

//---------------------------.
// �s�U��n���A�j���[�V�����̍X�V.
//---------------------------.
void CEmployee::PizzaPassedAnimUpdate()
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

	// �s�U���𓧖��ɂ��Ă���.
	if ( m_PizzaBoxAlpha > Color::ALPHA_MIN ) {
		m_PizzaBoxAlpha = 1.0f - m_AnimCnt;
		if ( m_PizzaBoxAlpha <= Color::ALPHA_MIN ) m_PizzaBoxAlpha = Color::ALPHA_MIN;
	}

	// ���W�̍X�V.
	m_TransformList[LHand].Position = m_LeftHandsStartPos  + ( MoveVector * m_AnimCnt );
	m_TransformList[RHand].Position = m_RightHandsStartPos + ( MoveVector * m_AnimCnt );
	m_PizzaBoxTransform.Position	= m_TransformList[RHand].Position;
	m_PizzaBoxTransform.Rotation	= m_TransformList[Body].Rotation;
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

		// �s�U���𓧖��ɂ���.
		m_PizzaBoxAlpha = Color::ALPHA_MIN;

		// �ʏ�ɖ߂��A�j���[�V�����ɕύX.
		m_AnimationNo = PizzaPassedAnimEnd;
	}
}

//---------------------------.
// �s�U��n���I������A�j���[�V�����̍X�V.
//---------------------------.
void CEmployee::PizzaPassedAnimEndUpdate()
{
	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftHandEndPos		= m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_POS_BONE_NAME );
	const D3DXPOSITION3& RightHandEndPos	= m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_POS_BONE_NAME );

	// �x�N�g���̍쐬.
	m_LeftHandMoveVector	= LeftHandEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector	= RightHandEndPos - m_RightHandsStartPos;

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

		// ���U��A�j���[�V�����ɕύX.
		m_IsPizzaPassedAnimEnd = true;
		ChangeWaveHands();
	}
}

//---------------------------.
// �s�U�������A�j���[�V�����̍X�V.
//---------------------------.
void CEmployee::ChangeHavePizzaAnimUpdate()
{
	// �{�[�����W�̎擾.
	m_TransformList[Body] = m_Transform;
	m_pSkinMesh[Body]->SetTransform( m_TransformList[Body] );
	const D3DXPOSITION3& LeftHandEndPos  = m_pSkinMesh[Body]->GetPosFromBone( LEFT_HAND_GET_PIZZA_POS_BONE_NAME );
	const D3DXPOSITION3& RightHandEndPos = m_pSkinMesh[Body]->GetPosFromBone( RIGHT_HAND_GET_PIZZA_POS_BONE_NAME );

	// �A�j���[�V�������I����Ă��邩.
	if ( m_AnimCnt >= ANIM_CNT_MAX ) {
		// ���W�����킹��.
		m_TransformList[LHand].Position = LeftHandEndPos;
		m_TransformList[RHand].Position = RightHandEndPos;

		// �s�U����s�����ɂ���.
		m_PizzaBoxAlpha = Color::ALPHA_MAX;
		return;
	}

	// �x�N�g���̍쐬.
	m_LeftHandMoveVector  = LeftHandEndPos  - m_LeftHandsStartPos;
	m_RightHandMoveVector = RightHandEndPos - m_RightHandsStartPos;

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
	}
}
