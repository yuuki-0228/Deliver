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
	const D3DXVECTOR3	GRAVITY_VECTOR				= { 0.0f, -1.0f, 0.0f };	// �d�̓x�N�g��.
	constexpr float		GET_FOOD_ANIM_CNT_MAX		= 180.0f;					// ���i�擾�A�j���[�V�����̃J�E���g�̏��.

	constexpr char		JETPACK_CENTER_BONE[]		= "p_jet_center";			// �W�F�b�g�p�b�N�̒��S�̈ʒu�̃{�[����[Body].
	constexpr char		L_JETPACK_JETCENTER_BONE[]	= "jetpack_l_center";		// ���W�F�b�g�p�b�N�̕��ˌ��̃{�[����[LJetPack].
	constexpr char		L_JETPACK_JETPOS_BONE[]		= "jetpack_l_jet_place";	// ���W�F�b�g�p�b�N�̕��ˌ��̃{�[����[LJetPack].
	constexpr char		L_JETPACK_SETPOS_BONE[]		= "p_jet_l";				// ���W�F�b�g�p�b�N�̐ݒu�ꏊ�̃{�[����[Body].
	constexpr char		R_JETPACK_JETCENTER_BONE[]	= "jetpack_r_center";		// �E�W�F�b�g�p�b�N�̕��ˌ��̃{�[����[RJetPack].
	constexpr char		R_JETPACK_JETPOS_BONE[]		= "jetpack_r_jet_place";	// �E�W�F�b�g�p�b�N�̕��ˌ��̃{�[����[RJetPack].
	constexpr char		R_JETPACK_SETPOS_BONE[]		= "p_jet_r";				// ���W�F�b�g�p�b�N�̐ݒu�ꏊ�̃{�[����[Body].
	constexpr char		BAG_SETPOS_BONE[]			= "p_jet_center";			// �����b�N�̐ݒu�ꏊ�̃{�[����[Bag].
	constexpr char		LOOK_POS_BONE[]				= "neck";					// �X��/���q���񂪌���ʒu.

	constexpr char		JETPACK_START_SE[]			= "JetPackStart";			// �W�F�b�g�p�b�N�̊J�n��.
	constexpr char		JETPACK_SE[]				= "JetPack";				// �W�F�b�g�p�b�N�̕��ˉ�.
	constexpr char		MOVE_SE[]					= "Move";					// �ړ�.
	constexpr char		BUILDING_LANDING_SE[]		= "BuildingLanding";		// �����ɒ��n.
	constexpr char		GRASS_LANDING_SE[]			= "GrassLanding";			// ���ɒ��n.
	constexpr char		EXPLOSION_SE[]				= "Explosion";				// ����.
	constexpr char		FOODPASSED_SE[]				= "FoodPassed";				// ���i��n����.
	constexpr char		GETFOOD_SE[]				= "GetFood";				// ���i���������.
	constexpr char		PIZZA_KILN_SE[]				= "PizzaKiln";				// �s�U�qSE.

	// �X�L�����b�V����No.
	enum enMeshNo : unsigned char {
		Mesh_Body,					// �\������v���C���[�{��.
		Mesh_LJetPack,				// �\�����鍶�W�F�b�g�p�b�N.
		Mesh_RJetPack,				// �\������E�W�F�b�g�p�b�N.
		Mesh_Bag,					// �\�����郊���b�N.

		Mesh_Max,					// �\�����郁�b�V���̐�.

		Bone_Body = Mesh_Max,		// �{�[�����W�̎擾�Ŏg�p����v���C���[�{��.
		Bone_LJetPack,				// �{�[�����W�̎擾�Ŏg�p���鍶�W�F�b�g�p�b�N.
		Bone_RJetPack,				// �{�[�����W�̎擾�Ŏg�p����E�W�F�b�g�p�b�N.

		MeshList_Max,				// ���b�V���̌�.
	} typedef EMeshNo;

	// �G�t�F�N�gNo.
	enum enEffectNo : unsigned char {
		Left_Smoke,					// ���̉��G�t�F�N�g.
		Right_Smoke,				// �E�̉��G�t�F�N�g.
		Left_Flame,					// ���̉��G�t�F�N�g.
		Right_Flame,				// �E�̉��G�t�F�N�g.
		Left_Explosion,				// ���̔����G�t�F�N�g.
		Right_Explosion,			// �E�̔����G�t�F�N�g.

		JetPack_Effect_Max,			// �W�F�b�g�p�b�N�֘A�̃G�t�F�N�g�̌�.

		Hit = JetPack_Effect_Max,	// �Փ˃G�t�F�N�g.

		Effect_Max					// �G�t�F�N�g�̌�.
	} typedef EEffectNo;

	// �x�N�g��No.
	enum enVectorNo : unsigned char {
		Wind,						// ���x�N�g��.
		Dire,						// �����x�N�g��.
		Tilt,						// �X���x�N�g��.
		OldMove,					// �O��̈ړ��x�N�g��.
		NowMove,					// ����̈ړ��x�N�g��.
		LJetPackRot,				// ���W�F�b�g�p�b�N�̉�]�x�N�g��.
		RJetPackRot,				// �E�W�F�b�g�p�b�N�̉�]�x�N�g��.
		LJetPackJet,				// ���W�F�b�g�p�b�N�̕��˃x�N�g��
		RJetPackJet,				// �E�W�F�b�g�p�b�N�̕��˃x�N�g��
		
		Vector_Max					// �G�t�F�N�g�̌�.
	} typedef EVectorNo;

	// �J�E���gNo.
	enum enCntNo : unsigned char {
		LandingEffect,				// ���n�G�t�F�N�g���Đ����Ă����p�̃J�E���g.
		LandingAng,					// ���n�A�j���[�V�����p�̊p�x�J�E���g.
		GetFoodAng,					// ���i�擾�A�j���[�V�����p�̊p�x�J�E���g.
		LJetPackStopTime,			// ���W�F�b�g�p�b�N���~�����鎞�ԗp�̃J�E���g.
		RJetPackStopTime,			// �E�W�F�b�g�p�b�N���~�����鎞�ԗp�̃J�E���g.
		StageOutTime,				// �X�e�[�W�O�ɏo�����ɃV�[����؂�ւ���܂ł̎��ԗp�̃J�E���g.

		Cnt_Max						// �J�E���g�̌�.
	} typedef ECntNo;

	// �t���ONo.
	enum enFlagNo : unsigned char
	{
		IsHitGround,				// �n�ʂɓ���������.
		IsHitPlayerIslandGround,	// �v���C���[�̕����̒n�ʂɓ���������.
		IsGroundClose,				// �n�ʂ��߂��ɂ��邩.
		IsHitCeiling,				// �V��ɓ���������.
		IsHitWall,					// �ǂɓ���������.
		IsOldHitGround,				// �O�̃t���[���Œn�ʂɓ���������.
		IsOldHitCeiling,			// �O�̃t���[���œV��ɓ���������.
		IsOldHitWall,				// �O�̃t���[���ŕǂɓ���������.
		IsGetFood,					// ���i���擾������.
		IsIslandCollisionStop,		// �����̓����蔻����~�����邩.
		IsIsland,					// �����̎��ӂɂ��邩.
		IsLJetPackStop,				// ���W�F�b�g�p�b�N���~�����邩.
		IsRJetPackStop,				// �E�W�F�b�g�p�b�N���~�����邩.
		IsActiveGazingCamera,		// �����J�������g�p����Ă��邩.
		IsControlStop,				// ������~�����邩.
		IsPushGoHomeNotification,	// �A��ʒm�𑗂�����.
		IsDispShadow,				// �e��\�������邩.
		IsStageOut,					// �X�e�[�W�O�Ɉړ�������.

		Flag_Max					// �t���O�̌�.
	} typedef EFlagNo;

	// ���CNo.
	enum enRayNo : unsigned char {
		Ceiling,					// �V��p.
		GroundClose,				// �n�ʂɋ߂������ׂ�p.
		LJetPackEffect,				// ���̃W�F�b�g�p�b�N�̃G�t�F�N�g�p.
		RJetPackEffect,				// �E�̃W�F�b�g�p�b�N�̃G�t�F�N�g�p.

		Ray_Max						// ���C�̌�.
	} typedef ERayNo;

	// �v���C���[�̃A�j���[�V����No.
	enum enAnimationNo : unsigned char {
		StandBy,					// �ҋ@.
		Fly,						// ��ԃA�j���[�V����.
		Landing,					// ���n�A�j���[�V����.
		WallCrash,					// �ǂɏՓ˃A�j���[�V����.
		GroundCrash,				// �n�ʂɏՓ˃A�j���[�V����.
		StageOut,					// �X�e�[�W�̊O�Ɉړ������Ƃ��̃A�j���[�V����.
		PizzGet,					// �s�U���󂯎�鎞�̃A�j���[�V����.
		PizzPassed,					// �s�U��n�����̃A�j���[�V����.
	} typedef EAnimationNo;

	// SE���X�g.
	enum enSENo : unsigned char {
		Move,						// �ړ�SE.
		MoveDown,					// ���Ɉړ�SE.

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
// ������.
//---------------------------.
bool CPlayer::Init()
{
	// �J�����̐ݒ�.
	m_pMainCamera = std::make_unique<CMainCamera>();
	m_pMainCamera->SetTarget( this );
	CCameraManager::SetCamera( m_pMainCamera.get() );
	
	// �����J�����̐ݒ�.
	m_pGazingCamera = std::make_unique<CGazingCamera>();
	m_pGazingCamera->SetPlayer( this );

	// ���i�̐ݒ�.
	m_pRuckSack = std::make_unique<CRuckSack>();
	m_pRuckSack->SetTilt( &m_Transform.Rotation.z );
	m_pRuckSack->Init();

	// �n���ɍs�����q����̕����̐ݒ�.
	m_pCustomerDire = std::make_unique<CCustomerDire>();
	m_pCustomerDire->SetPlayer( this );
	m_pCustomerDire->Init();

	// ���b�V���̎擾.
	m_pSkinMesh[Mesh_Body]		= CMeshResource::GetSkin( "player_s", &m_AC );
	m_pSkinMesh[Mesh_LJetPack]	= CMeshResource::GetSkin( "l_jet_s" );
	m_pSkinMesh[Mesh_RJetPack]	= CMeshResource::GetSkin( "r_jet_s" );
	m_pSkinMesh[Mesh_Bag]		= CMeshResource::GetSkin( "p_bag_s" );
	m_pSkinMesh[Bone_Body]		= m_pSkinMesh[Mesh_Body];
	m_pSkinMesh[Bone_LJetPack]	= m_pSkinMesh[Mesh_LJetPack];
	m_pSkinMesh[Bone_RJetPack]	= m_pSkinMesh[Mesh_RJetPack];
	m_pSkinMesh[Mesh_Body]->SetAnimSpeed( 0.001f );

	// �G�t�F�N�g�̐ݒ�.
	for( auto& e : m_pEffectList ) e = std::make_unique<CEffectManager>();
	m_pEffectList[Left_Flame]->SetEffect( "Flame" );
	m_pEffectList[Left_Smoke]->SetEffect( "Smoke" );
	m_pEffectList[Left_Explosion]->SetEffect( "Explosion" );
	m_pEffectList[Right_Flame]->SetEffect( "Flame" );
	m_pEffectList[Right_Smoke]->SetEffect( "Smoke" );
	m_pEffectList[Right_Explosion]->SetEffect( "Explosion" );
	m_pEffectList[Hit]->SetEffect( "Hit" );

	// �e�̉摜�̎擾.
	m_pShadow = CSpriteResource::GetSprite( "RoundShadow", &m_ShadowState );
	m_ShadowState.Transform.Rotation.x	= Math::ToRadian( 90.0f );
	m_ShadowState.Transform.Scale		= { 0.35f, 0.35f, 0.35f };

	// �����̓����蔻����~������.
	m_FlagList[IsHitGround] = true;

	// �W�F�b�g�p�b�N�̓��̓��X�g�̐ݒ�.
	m_LJetPackInputList.resize( CONSTANT.JETPACK_INPUT_LIST_MAX );
	m_RJetPackInputList.resize( CONSTANT.JETPACK_INPUT_LIST_MAX );

	// �����̐ݒ�.
	for ( auto& t : m_TransformList ) {
		t.Rotation.y = Math::ToRadian( -90.0f );
	}

	// �v���C���[�̐ݒ�.
	CDeliveryManager::SetPlayer( this );

	// �^�O�̐ݒ�.
	m_ObjectTag = EObjectTag::Player;

#ifndef _DEBUG
	// �R���g���[���̐U�����g�p����.
	CXInput::SetIsVibration( true );
#endif	// #ifndef _DEBUG.

	// �~�j�}�b�v�Ƀv���C���[�̃g�����X�t�H�[����ݒ�.
	CMiniMapUI::SetPlayer( this );

	// �I�[�f�B�I�̐ݒ�.
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
// �X�V.
//---------------------------.
void CPlayer::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	JetPackRotationControl();				// �W�F�b�g�p�b�N�̉�].
	JetPackJetControl();					// �W�F�b�g�p�b�N�̕���.
	JetPackStop();							// �W�F�b�g�p�b�N�̒�~.
	JetPackRepair();						// �W�F�b�g�p�b�N�̒�~�̏C��.

	AddGravity();							// �d�͂�ǉ�����.

	CreateMoveVec();						// �ړ��x�N�g���̍쐬.
	CreateRotVec();							// ���E�̃W�F�b�g�p�b�N�̉�]�x�N�g���̍쐬.

	AccelerationClamp();					// �����x�̏��/�����̐ݒ�.
	Friction( CONSTANT.AIR_FRICTION );		// ��C���C.

	PlayerMove();							// �v���C���[�̈ړ�.
	PlayerRotation();						// �v���C���[�̉�].

	PlayerTiltUpdate();						// �v���C���[�̌X���̍X�V.

	LandingEffectUpdate();					// ���n�G�t�F�N�g�̍X�V.
	GetFoodAnimUpdate();					// ���i�擾�A�j���[�V�����̍X�V.

	StageOutUpdate();						// �X�e�[�W�O�Ɉړ��������̍X�V.
	PlayMoveSE();							// �ړ���SE�̍Đ�.

	m_pRuckSack->Update( DeltaTime );		// �����b�N�̍X�V.
	m_pCustomerDire->Update( DeltaTime );	// �n���ɍs�����q����̕����̍X�V.

	m_FlagList[IsIsland] = false;

	UpdateCollision();
	CActorCollisionManager::PushObject( this );
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CPlayer::LateUpdate( const float& DeltaTime )
{
	// �����J�����̒����_��ύX.
	CCustomer* pGoHandingCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( pGoHandingCustomer != nullptr ) {
		m_pGazingCamera->SetTarget( pGoHandingCustomer );
	}

	// �J�����̍X�V.
	CameraControl();
	// 1F�O�̓n���ɍs���ׂ��q��ۑ�.
	m_pOldCustomer = pGoHandingCustomer;

	// �A�j���[�V�����̕ύX.
	if ( m_FlagList[IsControlStop] == false ) {
		// �n�ʂ��߂��̎��̃A�j���[�V�����ɕύX.
		if ( m_FlagList[IsGroundClose] ) {
			m_pSkinMesh[Mesh_Body]->ChangeAnimBlend( Landing, &m_AC );
		}
		// �����ł��鎞�̃A�j���[�V�����ɕύX.
		else if ( m_FlagList[IsHitGround] == false ) {
			m_pSkinMesh[Mesh_Body]->ChangeAnimBlend( Fly, &m_AC );
		}
	}

	// �e�̍X�V.
	if ( m_FlagList[IsDispShadow] ) {
		m_ShadowState.IsDisp = true;

		// �ʒu�̍X�V.
		m_ShadowState.Transform.Position	= m_TransformList[Mesh_Body].Position;
		m_ShadowState.Transform.Position.y	= m_ShadowPosY.second + 0.1f;

		// �A���t�@�l�̍X�V.
		const float DistanceRate	= m_ShadowPosY.first / CONSTANT.SHADOW_DISP_DISTANCE;
		m_ShadowState.Color.w		= 1.0f - DistanceRate;

		// �g�k.
		m_ShadowState.Transform.Scale = CONSTANT.SHADOW_MIN_SCALE + CONSTANT.SHADOW_ADD_ACALE * DistanceRate;
	}
	else m_ShadowState.IsDisp = false;

	// �t���O�̏�����.
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
// �f�o�b�N�̍X�V.
//---------------------------.
void CPlayer::DebugUpdate( const float& DeltaTime )
{
	CDebugWindow::PushProc( "Player",
	[&](){
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"���W        : %5f, %5f, %5f",	m_Transform.Position.x, m_Transform.Position.y, m_Transform.Position.z );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"�����x�N�g�� : %5f, %5f, %5f",	m_VectorList[Dire].x,		m_VectorList[Dire].y,		m_VectorList[Dire].z );
		ImGui::Text( u8"�X���x�N�g�� : %5f, %5f, %5f",	m_VectorList[Tilt].x,		m_VectorList[Tilt].y,		m_VectorList[Tilt].z );
		ImGui::Text( u8"�ړ��x�N�g�� : %5f, %5f, %5f",	m_VectorList[NowMove].x,	m_VectorList[NowMove].y,	m_VectorList[NowMove].z );
		ImGui::Text( u8" �� �x�N�g�� : %5f, %5f, %5f",	m_VectorList[Wind].x,		m_VectorList[Wind].y,		m_VectorList[Wind].z );
		ImGui::Text( u8"�ړ����x(���E): %5f",				m_SideMoveSpeed );
		ImGui::Text( u8"�ړ����x(�㉺): %5f",				m_VerticalMoveSpeed );
		ImGui::Text( u8"------------------------------" );
	} );
	m_pRuckSack->DebugUpdate( DeltaTime );
}

//---------------------------.
// �`��.
//---------------------------.
void CPlayer::Render()
{
	TransformUpdate();

	// �v���C���[�̉e�̕`��.
	ShadowRender();

	// �v���C���[�̕`��.
	SetRuckSackColor();
	for ( int i = 0; i < Mesh_Max; ++i ) {
		m_pSkinMesh[i]->Render( &m_TransformList[i], &m_AC );
	}

	// �n���ɍs�����q����̕����̕`��.
	m_pCustomerDire->Render();

	CCollisionRender::PushRay( m_pCollisions->GetCollision<CRay>() );
	CCollisionRender::PushRay( m_pCollisions->GetCollision<CCrossRay>() );
	for( auto& r : m_pRayList ) CCollisionRender::PushRay( r.get() );
	CCollisionRender::PushPoint( m_Transform.Position );
}

//---------------------------.
// �e�̕`��.
//---------------------------.
void CPlayer::ShadowRender()
{
	// �v���C���[�����F�̔������ŕ`��.
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
// �X�v���C�g(3D)�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CPlayer::Sprite3DRender_A()
{
	// �G�t�F�N�g�̈ʒu�̍X�V.
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

	// �G�t�F�N�g�̕`��.
	for ( int i = 0; i < Effect_Max; ++i ) {
		m_pEffectList[i]->Render();
	}
	for ( auto& e : m_pLandingEffectList ) e->Render();

	m_pShadow->Render3D( &m_ShadowState );
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CPlayer::Collision( CActor* pActor )
{
	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::ApartmentA &&
		 Tag != EObjectTag::ApartmentB &&
		 Tag != EObjectTag::Island &&
		 Tag != EObjectTag::PlayerIsland &&
		 Tag != EObjectTag::IslandObject ) return;

	IslandCollision(	pActor );	// �����̓����蔻��.

	// �����̓����蔻����s��Ȃ��ꍇ������.
	if ( ( Tag == EObjectTag::Island || Tag == EObjectTag::PlayerIsland || Tag == EObjectTag::IslandObject ) &&
		m_FlagList[IsIslandCollisionStop] ) return;

	JetPackEffectCollision( pActor );	// �W�F�b�g�p�b�N�̃G�t�F�N�g�p�̓����蔻��.
	GroundCollision(		pActor );	// �n�ʂ̓����蔻��.
	GroundCloseCollision(	pActor );	// �n�ʂ��߂��ɂ��邩���ׂ铖���蔻��.
	ShadowCollision(		pActor );	// �e�p�̓����蔻��.
	CeilingCollision(		pActor );	// �V��̓����蔻��.
	WallCollision(			pActor );	// �ǂ̓����蔻��.
	IsPlayerHouseCollision( pActor );	// �v���C���[�̉Ƃ̓����蔻��.
}

//---------------------------.
// ���i��n��.
//---------------------------.
void CPlayer::FoodPassed()
{
	// �X�R�A�̉��Z.
	CScore::SetScore( m_pRuckSack->GetFoodState() );

	// �����b�N�̒�����ɂ���.
	m_pRuckSack->FoodPassed();

	// �n���A�j���[�V�����ɕύX.
	m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( PizzPassed, &m_AC );

	// SE���Đ�.
	CSoundManager::PlaySE( FOODPASSED_SE );
}

//---------------------------.
// �����x�N�g���̎擾.
//---------------------------.
D3DXVECTOR3 CPlayer::GetDireVector()
{
	return m_VectorList[Dire];
}

//---------------------------.
// �X��/���q���񂪌���ʒu���擾.
//---------------------------.
D3DXPOSITION3 CPlayer::GetLookPos()
{
	m_pSkinMesh[Mesh_Body]->SetTransform( m_TransformList[Mesh_Body] );

	// ���W���������ɂ���.
	D3DXPOSITION3 LookPos = m_pSkinMesh[Mesh_Body]->GetPosFromBone( LOOK_POS_BONE );
	LookPos.y -= CONSTANT.LOOK_POS_ADD_Y;
	return LookPos;
}

//---------------------------.
// ���n���Ă��邩���擾.
//---------------------------.
bool CPlayer::GetIsLanding()
{
	return m_FlagList[IsHitGround];
}

//---------------------------.
// �����̋߂��ɂ��邩�擾.
//---------------------------.
bool CPlayer::GetIsIsland()
{
	return m_FlagList[IsIsland];
}

//---------------------------.
// �����J�������g�p���Ă��邩���擾.
//---------------------------.
bool CPlayer::GetIsActiveGazingCam()
{
	return m_FlagList[IsActiveGazingCamera];
}

//---------------------------.
// �J�n�ʒu�ɐݒ�.
//---------------------------.
void CPlayer::SetStartPosition( const D3DXPOSITION3& Pos )
{
	// �z�u����ꏊ�̍쐬.
	m_StartPosition = Pos;
	m_StartPosition.x += 10.0f;
	m_StartPosition.y += CONSTANT.VERTICAL_RAY_LENGTH;

	// ���W�̐ݒ�.
	m_TransformList[Mesh_Body].Position		= m_StartPosition;
	TransformUpdate();
}

//---------------------------.
//�@���i���󂯎��.
//---------------------------.
void CPlayer::SetNewFood( const int Money, int ShopNo )
{
	m_FlagList[IsGetFood] = true;

	// ���i��ݒ肷��.
	m_pRuckSack->SetNewFood( Money, ShopNo );

	// �󂯎��A�j���[�V�����ɕύX.
	m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( PizzGet, &m_AC );

	// SE���Đ�.
	CSoundManager::PlaySE( GETFOOD_SE );
}

//---------------------------.
// ���̐ݒ�.
//---------------------------.
void CPlayer::SetWind( const D3DXVECTOR3& Vec )
{
	m_VectorList[Wind] = Vec;
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CPlayer::InitCollision()
{
	// �������̃��C�̏�����.
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Ray );
	m_pCollisions->GetCollision<CRay>()->SetVector( Math::_Y_VECTOR );
	m_pCollisions->GetCollision<CRay>()->SetLength( CONSTANT.VERTICAL_RAY_LENGTH );

	// ���C�̍쐬.
	for ( auto& r : m_pRayList ) r = std::make_unique<CRay>();

	// �n�ʂɋ߂������ׂ�p�̃��C�̏�����.
	m_pRayList[GroundClose]->SetVector( Math::_Y_VECTOR );
	m_pRayList[GroundClose]->SetLength( CONSTANT.GROUND_CLOSE_RAY_LENGTH );

	// �������̃��C�̏�����.
	m_pCollisions->InitCollision( ECollType::CrossRay );
	m_pCollisions->GetCollision<CCrossRay>()->SetLength( CONSTANT.SIDE_RAY_LENGTH );

	// ������̃��C�̏�����.
	m_pRayList[Ceiling]->SetVector( Math::Y_VECTOR );
	m_pRayList[Ceiling]->SetLength( CONSTANT.VERTICAL_RAY_LENGTH );

	// �W�F�b�g�p�b�N�̃G�t�F�N�g�p�̃��C�̏�����.
	m_pRayList[LJetPackEffect]->SetVector( Math::_Y_VECTOR );
	m_pRayList[RJetPackEffect]->SetVector( Math::_Y_VECTOR );
	m_pRayList[LJetPackEffect]->SetLength( CONSTANT.JETPACK_EFFECT_RAY_LENGTH );
	m_pRayList[RJetPackEffect]->SetLength( CONSTANT.JETPACK_EFFECT_RAY_LENGTH );
}

//---------------------------.
// �����蔻��̍X�V.
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
// �����̓����蔻��.
//---------------------------.
void CPlayer::IslandCollision( CActor* pActor )
{
	if ( pActor->GetObjectTag() != EObjectTag::Island &&
		 pActor->GetObjectTag() != EObjectTag::PlayerIsland ) return;

	// �������Ă��邩.
	m_FlagList[IsIslandCollisionStop] = !Coll::IsSphereToPosition(
		pActor->GetCollision<CSphere>(),
		m_TransformList[Mesh_Body].Position );

	if ( m_FlagList[IsIslandCollisionStop] ) return;
	m_FlagList[IsIsland] = true;
}

//---------------------------.
// �v���C���[�̉Ƃ̒��n�̓����蔻��.
//---------------------------.
void CPlayer::IsPlayerHouseCollision( CActor* pActor )
{
	if ( CTime::GetIsGameEnd()					== false ) return;
	if ( m_FlagList[IsIslandCollisionStop]				 ) return;
	if ( m_FlagList[IsHitPlayerIslandGround]	== false ) return;
	if ( m_pRuckSack->GetIsFoodHave()					 ) return;

	// ����̒�~���~�߂�.
	CInput::SetIsUpdateStop( false );

	// �V�[���؂�ւ�.
	CWind::StopWind();
	CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade, 0.01f );
}

//---------------------------.
// �W�F�b�g�p�b�N�̃G�t�F�N�g�̓����蔻��.
//---------------------------.
void CPlayer::JetPackEffectCollision( CActor* pActor )
{
	// ���̃W�F�b�g�p�b�N���n�ʂɓ������Ă��邩.
	D3DXPOSITION3 pIntersect = INIT_FLOAT3;
	if ( Coll::IsRayToMesh(
		m_pRayList[LJetPackEffect].get(),
		pActor->GetCollision<CMesh>(),
		nullptr, &pIntersect ) )
	{
		// �G�t�F�N�g�̍Đ�.
		const D3DXPOSITION3& EffectPos = pIntersect + pActor->GetCollision<CMesh>()->GetPosition();
		LandingEffectPlay( EffectPos );
	}

	// �E�̃W�F�b�g�p�b�N���n�ʂɓ������Ă��邩.
	pIntersect = INIT_FLOAT3;
	if ( Coll::IsRayToMesh(
		m_pRayList[RJetPackEffect].get(),
		pActor->GetCollision<CMesh>(),
		nullptr, &pIntersect ) )
	{
		// �G�t�F�N�g�̍Đ�.
		const D3DXPOSITION3& EffectPos = pIntersect + pActor->GetCollision<CMesh>()->GetPosition();
		LandingEffectPlay( EffectPos );
	}
}

//---------------------------.
// �n�ʂ̓����蔻��.
//---------------------------.
void CPlayer::GroundCollision( CActor* pActor )
{
	if ( m_FlagList[IsHitGround]		) return;

	// �������Ă��邩.
	D3DXPOSITION3 pIntersect = INIT_FLOAT3;
	m_FlagList[IsHitGround] = Coll::IsRayToMesh(
		m_pCollisions->GetCollision<CRay>(),
		pActor->GetCollision<CMesh>(),
		nullptr, &pIntersect );
	if ( m_FlagList[IsHitGround] == false ) return;
	m_FlagList[IsGroundClose] = false;

	// �v���C���[�̕�����.
	if ( pActor->GetObjectTag() == EObjectTag::PlayerIsland ) {
		m_FlagList[IsHitPlayerIslandGround] = true;
	}

	// ���n�A�j���[�V�����ɕύX.
	if ( m_FlagList[IsControlStop] == false ) {
		m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( StandBy, &m_AC );
	}

	// �n�ʂɒ��n.
	const float	HitY		= pIntersect.y + pActor->GetCollision<CMesh>()->GetPosition().y + m_pCollisions->GetCollision<CRay>()->GetLength();
	const bool	IsFriction	= Math::Round_n( HitY, 2 ) == Math::Round_n( m_TransformList[Mesh_Body].Position.y, 2 );
	m_TransformList[Mesh_Body].Position.y	= HitY;
	m_VectorList[NowMove].y					= INIT_FLOAT;
	m_pCollisions->GetCollision<CCrossRay>()->SetPosition( m_TransformList[Mesh_Body].Position );
	// �G�t�F�N�g�̍Đ�.
	const D3DXPOSITION3& EffectPos = pIntersect + pActor->GetCollision<CMesh>()->GetPosition();
	LandingEffectPlay( EffectPos );

	// �n�ʖ��C.
	const float Power = IsFriction ? CONSTANT.GROUND_FRICTION : CONSTANT.GROUND_FRICTION / 2.0f;
	Friction( Power );

	// ���nSE�̍Đ�.
	if ( m_VerticalMoveSpeed >= CONSTANT.LANDING_SE_PLAY_SPEED ) {
		const EObjectTag Tag	= pActor->GetObjectTag();
		std::string		 SEName	= "";
		if ( Tag == EObjectTag::Island || Tag == EObjectTag::PlayerIsland ) SEName = GRASS_LANDING_SE;
		else																SEName = BUILDING_LANDING_SE;
		CSoundManager::PlaySE( SEName );
	}

	// ��葬�x�ȏ�Œ��n������.
	if ( m_FlagList[IsControlStop] ) return;
	if ( m_VerticalMoveSpeed >= CONSTANT.DEAD_GROUND_SPEED && m_FlagList[IsOldHitGround] == false ) {
		// �Փ˃G�t�F�N�g�̍Đ�.
		m_pEffectList[Hit]->Play( EffectPos );
		CSoundManager::PlaySE( "Hit" );

		// ������~������.
		m_FlagList[IsControlStop] = true;
		CInput::SetIsUpdateStop( true );

		// �ړ��x�N�g���𖳂���.
		m_VectorList[NowMove] = INIT_FLOAT3;
		m_VectorList[OldMove] = INIT_FLOAT3;
		m_MoveVectorList.clear();

		// �J�������Փ˂������_�ŌŒ肷��.
		if ( m_FlagList[IsActiveGazingCamera] ) m_pGazingCamera->SetTarget( nullptr );
		else									m_pMainCamera->SetTarget( nullptr );

		// ���Ô�𔭐�������.
		CScore::SetGameEnd();

		// �Փ˃A�j���[�V�����ɕύX����.
		m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( GroundCrash, &m_AC );
		m_pSkinMesh[Mesh_Body]->SetAnimSpeed( 0.02f );

		// �q�b�g�X�g�b�v�̐ݒ�.
		CCameraManager::HitStop( 1.0f, true,
		// �I����.
		[&] () {
			// ����̒�~���~�߂�.
			CInput::SetIsUpdateStop( false );

			// �V�[���؂�ւ�.
			CWind::StopWind();
			CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade, 0.01f );
		}, 
		// �X���[�ɕύX��.
		[&] () {
			// SE�̍Đ�����.
			CSoundManager::SetSEPlaySpeed( "HitEnd", 500 );
			CSoundManager::PlaySE( "HitEnd" );
			CSoundManager::PlaySE( "Ambulance" );
		} );
	}
}

//---------------------------.
// �n�ʂɋ߂����̓����蔻��.
//---------------------------.
void CPlayer::GroundCloseCollision( CActor* pActor )
{
	if ( m_FlagList[IsControlStop]	) return;
	if ( m_FlagList[IsHitGround]	) return;
	if ( m_FlagList[IsGroundClose]	) return;

	// �������Ă��邩.
	m_FlagList[IsGroundClose] = Coll::IsRayToMesh(
		m_pRayList[GroundClose].get(),
		pActor->GetCollision<CMesh>() );
	if( m_FlagList[IsGroundClose] == false ) return;
}

//---------------------------.
// �e�̓����蔻��.
//---------------------------.
void CPlayer::ShadowCollision( CActor* pActor )
{
	// �������Ă��邩.
	float			Distance	= INIT_FLOAT;
	D3DXPOSITION3	pIntersect	= INIT_FLOAT3;
	if( Coll::IsRayToMesh(
		m_pRayList[GroundClose].get(),
		pActor->GetCollision<CMesh>(),
		&Distance, &pIntersect ) == false ) return;

	// �n�ʂ̈ʒu��ݒ�.
	const float	HitY = pIntersect.y + pActor->GetCollision<CMesh>()->GetPosition().y;
	if ( m_FlagList[IsDispShadow] ){
		if ( Distance < m_ShadowPosY.first ) m_ShadowPosY = std::make_pair( Distance, HitY );
		return;
	}
	m_FlagList[IsDispShadow]	= true;
	m_ShadowPosY				= std::make_pair( Distance, HitY );
}

//---------------------------.
// �V��̓����蔻��.
//---------------------------.
void CPlayer::CeilingCollision( CActor* pActor )
{
	if ( m_FlagList[IsHitCeiling] ) return;

	// �������Ă��邩.
	D3DXPOSITION3 pIntersect = INIT_FLOAT3;
	m_FlagList[IsHitCeiling] = Coll::IsRayToMesh(
		m_pRayList[Ceiling].get(),
		pActor->GetCollision<CMesh>(),
		nullptr, &pIntersect, nullptr, true, CONSTANT.VERTICAL_RAY_LENGTH );
	if( m_FlagList[IsHitCeiling] == false ) return;

	// ���C�ƃ��f���̍��W�̍X�V.
	const D3DXPOSITION3& RayPos = m_pRayList[Ceiling]->GetPosition();
	m_TransformList[Mesh_Body].Position.x -= ( m_TransformList[Mesh_Body].Position.x - RayPos.x ) / CONSTANT.CEILING_MOVE_SPEED;
	m_TransformList[Mesh_Body].Position.z -= ( m_TransformList[Mesh_Body].Position.z - RayPos.z ) / CONSTANT.CEILING_MOVE_SPEED;

	// �V��ɂԂ�����.
	const float	HitY = pActor->GetCollision<CMesh>()->GetPosition().y + pIntersect.y - CONSTANT.VERTICAL_RAY_LENGTH;
	m_TransformList[Mesh_Body].Position.y	= HitY;
	m_pCollisions->GetCollision<CCrossRay>()->SetPosition( m_TransformList[Mesh_Body].Position );
}

//---------------------------.
// �ǂ̓����蔻��.
//---------------------------.
void CPlayer::WallCollision( CActor* pActor )
{
	// �������Ă��邩.
	std::vector<int> HitList;
	if ( Coll::IsCrossRayToWallMesh(
		m_pCollisions->GetCollision<CCrossRay>(),
		pActor->GetCollision<CMesh>(),
		&HitList ) == false ) return;

	// �ǂɂ���Đi�ނ悤�ɐݒ�.
	const D3DXPOSITION3& RayPos = m_pCollisions->GetCollision<CCrossRay>()->GetPosition();
	m_TransformList[Mesh_Body].Position.x	= RayPos.x;
	m_TransformList[Mesh_Body].Position.z	= RayPos.z;

	// �ǂɓ������Ă�������ւ̗͂𖳂���.
	for ( auto& h : HitList ) {
		const ECrossRayDire& Dire = static_cast<ECrossRayDire>( h );
		if ( Dire == ECrossRayDire::ZF && m_VectorList[NowMove].z < 0.0f ) m_VectorList[NowMove].z = 0.0f;
		if ( Dire == ECrossRayDire::ZB && m_VectorList[NowMove].z > 0.0f ) m_VectorList[NowMove].z = 0.0f;
		if ( Dire == ECrossRayDire::XL && m_VectorList[NowMove].x > 0.0f ) m_VectorList[NowMove].x = 0.0f;
		if ( Dire == ECrossRayDire::XR && m_VectorList[NowMove].x < 0.0f ) m_VectorList[NowMove].x = 0.0f;
	}

	// ������~���Ă���ꍇ�͏Փ˔�����s��Ȃ�.
	if ( m_FlagList[IsControlStop] ) {
		m_FlagList[IsHitWall] = true;
		return;
	}

	// ��葬�x�ȏ�ŏՓ˂�����.
	if ( m_SideMoveSpeed >= CONSTANT.DEAD_WALL_SPEED && m_FlagList[IsHitWall] == false && m_FlagList[IsOldHitWall] == false ) {
		// �G�t�F�N�g�̗p�̉�]���쐬.
		D3DXROTATION3 EffectRot = INIT_FLOAT3;
		for ( auto& h : HitList ) {
			const ECrossRayDire& Dire = static_cast<ECrossRayDire>( h );
			if ( Dire == ECrossRayDire::ZF || Dire == ECrossRayDire::ZB ) EffectRot.x = Math::ToRadian( 90.0f );
			if ( Dire == ECrossRayDire::XL || Dire == ECrossRayDire::XR ) EffectRot.z = Math::ToRadian( 90.0f );
			break;
		}

		// �Փ˃G�t�F�N�g�̍Đ�.
		m_pEffectList[Hit]->Play( m_TransformList[Mesh_Body].Position, EffectRot );
		CSoundManager::PlaySE( "Hit" );

		// ������~������.
		m_FlagList[IsControlStop] = true;
		CInput::SetIsUpdateStop( true );

		// �ړ��x�N�g���𖳂���.
		m_VectorList[NowMove] = INIT_FLOAT3;
		m_VectorList[OldMove] = INIT_FLOAT3;
		m_MoveVectorList.clear();

		// �J�������Փ˂������_�ŌŒ肷��.
		if ( m_FlagList[IsActiveGazingCamera] ) m_pGazingCamera->SetTarget( nullptr );
		else									m_pMainCamera->SetTarget( nullptr );

		// ���Ô�𔭐�������.
		CScore::SetGameEnd();

		// �Փ˃A�j���[�V�����ɕύX����.
		m_pSkinMesh[Mesh_Body]->ChangeAnimSet( WallCrash, &m_AC );
		m_pSkinMesh[Mesh_Body]->SetAnimSpeed( 0.02f );

		// �q�b�g�X�g�b�v�̐ݒ�.
		CCameraManager::HitStop( 1.0f, true,
		// �I����.
		[&] () {
			// ����̒�~���~�߂�.
			CInput::SetIsUpdateStop( false );

			// �V�[���؂�ւ�.
			CWind::StopWind();
			CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade, 0.01f );
		}, 
		// �X���[�ɕύX��.
		[&] () {
			// SE�̍Đ�����.
			CSoundManager::SetSEPlaySpeed( "HitEnd", 500 );
			CSoundManager::PlaySE( "HitEnd" );
			CSoundManager::PlaySE( "Ambulance" );
		} );
	}

	// �ǂɓ�������.
	m_FlagList[IsHitWall] = true;
}

//---------------------------.
// �J�����̍X�V.
//---------------------------.
void CPlayer::CameraControl()
{
	// ����\�łȂ��Ȃ珈�����s��Ȃ�.
	if ( m_FlagList[IsControlStop] ) return;

	// �n���ɍs���ׂ��q���擾.
	CCustomer* pGoHandingCustomer = CDeliveryManager::GetGoHandingCustomer();
	const bool IsJustSend = m_pOldCustomer != nullptr && pGoHandingCustomer == nullptr ? true : false;
	// �����J�������L��������Ă��āA���q����ɏ��i��n���I�����u�ԂȂ�.
	if ( m_FlagList[IsActiveGazingCamera] && IsJustSend ) {
		// �����J�������烁�C���J�����ɐ؂�ւ���.
		SwitchToMain();
		return;
	}

	// �J�������؂�ւ����Ȃ�Ȍ�̏������s��Ȃ�.
	if ( m_pMainCamera->GetIsSwitching() || m_pGazingCamera->GetIsSwitching() )return;
	// B�{�^����������Ă��Ȃ��Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false )return;
	// �����J�������L��������Ă��āAB�{�^���������ꂽ�Ȃ�.
	if ( m_FlagList[IsActiveGazingCamera] ) {
		// �����J�������烁�C���J�����ɐ؂�ւ���.
		SwitchToMain();
		return;
	}

	// �n���ɍs���ׂ��q�����Ȃ��Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( pGoHandingCustomer == nullptr )return;
	// �n���ɍs���ׂ��q������Ƃ���B�{�^���������ꂽ�Ȃ�.
	// ���C���J�������璍���J�����ɐ؂�ւ���.
	SwitchToGazing( *pGoHandingCustomer );
}

//---------------------------.
// �����J�������烁�C���J�����ɐ؂�ւ���.
//---------------------------.
void CPlayer::SwitchToMain()
{
	// �����J�����𖳌�������.
	m_FlagList[IsActiveGazingCamera] = false;
	// �؂�ւ����J�n����.
	m_pMainCamera->StartMove();
	// ���C���J�����ɒ����J�����̐ݒ��n��.
	m_pMainCamera->SetCameraState( m_pGazingCamera->GetCameraState() );
	m_pMainCamera->SetTarget( this );
	m_pMainCamera->SetCustomer( m_pGazingCamera->GetTarget() );
	// �g�p����J���������C���J�����ɐݒ�.
	CCameraManager::SetCamera( m_pMainCamera.get() );
}

//---------------------------.
// ���C���J�������璍���J�����ɐ؂�ւ���.
//---------------------------.
void CPlayer::SwitchToGazing( CActor& pCustmer )
{
	// �����J������L��������.
	m_FlagList[IsActiveGazingCamera] = true;
	// �؂�ւ����J�n����.
	m_pGazingCamera->StartMove();
	// �����J�����Ƀ��C���J�����̐ݒ��n��.
	m_pGazingCamera->SetCameraState( m_pMainCamera->GetCameraState() );
	m_pGazingCamera->SetTarget( &pCustmer );
	m_pGazingCamera->SetRotX( m_pMainCamera->GetRotX() );
	// �g�p����J�����𒍎��J�����ɐݒ�.
	CCameraManager::SetCamera( m_pGazingCamera.get() );
}

//---------------------------.
// �g�����X�t�H�[���̍X�V.
//---------------------------.
void CPlayer::TransformUpdate()
{
	// �v���C���[�{�̂̃��f���̍��W��ݒ肷��.
	m_pSkinMesh[Mesh_Body]->SetTransform( m_TransformList[Mesh_Body] );

	// �W�F�b�g�p�b�N�̈ʒu�̍X�V.
	m_TransformList[Mesh_LJetPack].Position = m_pSkinMesh[Mesh_Body]->GetPosFromBone( L_JETPACK_SETPOS_BONE );
	m_TransformList[Mesh_RJetPack].Position = m_pSkinMesh[Mesh_Body]->GetPosFromBone( R_JETPACK_SETPOS_BONE );

	// �����b�N�̈ʒu�̍X�V.
	m_TransformList[Mesh_Bag].Position		= m_pSkinMesh[Mesh_Body]->GetPosFromBone( BAG_SETPOS_BONE );

	// �������Ă���ꍇ�����b�N��������Ɉړ�������.
	const float AddY = m_Transform.Position.y - m_TransformList[Mesh_Body].Position.y;
	m_TransformList[Mesh_Bag].Position.y += sinf( Math::ToRadian( m_CntList[GetFoodAng] ) );
	if ( AddY > 0.0f ) m_TransformList[Mesh_Bag].Position.y += AddY;

	// �O���Ŏg�p����g�����X�t�H�[�����v���C���[�{�̂Ɠ����ɂ���.
	m_Transform	= m_TransformList[Mesh_Body];

	// �{�[�����W���擾�p�̃g�����X�t�H�[���̍X�V.
	m_TransformList[Bone_Body].Position		= m_TransformList[Mesh_Body].Position;
	m_pSkinMesh[Bone_Body]->SetTransform( m_TransformList[Bone_Body] );
	m_TransformList[Bone_LJetPack].Position	= m_pSkinMesh[Bone_Body]->GetPosFromBone( L_JETPACK_SETPOS_BONE );
	m_TransformList[Bone_RJetPack].Position	= m_pSkinMesh[Bone_Body]->GetPosFromBone( R_JETPACK_SETPOS_BONE );
}

//---------------------------.
// �����b�N�̐F�̐ݒ�.
//---------------------------.
void CPlayer::SetRuckSackColor()
{
	// �Q�[�W�̏ꏊ�̎擾.
	const float LR_Gage		= std::abs( m_pRuckSack->GetLRGauge() );
	const float UD_Gage		= std::abs( m_pRuckSack->GetUDGauge() );
	const float Color_Gage	= LR_Gage > UD_Gage ? LR_Gage : UD_Gage;

	// ���F���猸�炷�ő�̐F.
	const D3DXCOLOR3 DecColorMax = { Color::RGB_( Color::COLOR_MIN, 0.5f, 0.5f ) };

	// �Q�[�W�̏ꏊ�ɉ����ă����b�N�̐F��Ԃ����Ă���.
	m_pSkinMesh[Mesh_Bag]->SetColor( Color::White - DecColorMax * Color_Gage );
}

//---------------------------.
// �W�F�b�g�p�b�N�̕���.
//---------------------------.
void CPlayer::JetPackJetControl()
{
	if ( m_FlagList[IsControlStop] ) return;

	m_VectorList[Dire]	= m_TransformList[Mesh_Body].GetRotationVector();

	// �g���K�[�̉�������擾.
	const float LTrigger	= CXInput::GetLTrigger();
	const float RTrigger	= CXInput::GetRTrigger();
	const float LPower		= LTrigger * CONSTANT.JETPACK_POWER;
	const float RPower		= RTrigger * CONSTANT.JETPACK_POWER;

	// �������ۑ�.
	std::rotate( m_LJetPackInputList.begin(), m_LJetPackInputList.begin() + 1, m_LJetPackInputList.end() );
	std::rotate( m_RJetPackInputList.begin(), m_RJetPackInputList.begin() + 1, m_RJetPackInputList.end() );
	const int SetPos			= CONSTANT.JETPACK_INPUT_LIST_MAX - 1;
	m_LJetPackInputList[SetPos] = LTrigger;
	m_RJetPackInputList[SetPos] = RTrigger;

	// �{�[���̈ʒu���擾.
	m_pSkinMesh[Bone_Body]->SetTransform( m_TransformList[Bone_Body] );
	const D3DXPOSITION3& CenterPos = Math::Round_n( m_pSkinMesh[Bone_Body]->GetPosFromBone( JETPACK_CENTER_BONE ), 7 );

	// �W�F�b�g�p�b�N�̊J�n�����Đ����邩.
	if ( m_LJetPackInputList[SetPos - 1] == 0.0f && m_LJetPackInputList[SetPos] > 0.0f ) {
		// SE�̍Đ�.
		CSoundManager::SetSelectNo( 0 );
		CSoundManager::SetSEVolume( JETPACK_START_SE, m_LJetPackInputList[SetPos] );
		CSoundManager::PlaySE( JETPACK_START_SE );
		CSoundManager::ResetSelectNo();
	}
	else if ( m_RJetPackInputList[SetPos - 1] == 0.0f && m_RJetPackInputList[SetPos] > 0.0f ) {
		// SE�̍Đ�.
		CSoundManager::SetSelectNo( 1 );
		CSoundManager::SetSEVolume( JETPACK_START_SE, m_RJetPackInputList[SetPos] );
		CSoundManager::PlaySE( JETPACK_START_SE );
		CSoundManager::ResetSelectNo();
	}

	// ���g���K�[�������Ă��邩.
	if ( LTrigger != 0.0f ) {
		if ( m_FlagList[IsLJetPackStop] == false ) {

			// �{�[���̈ʒu���擾.
			m_pSkinMesh[Bone_LJetPack]->SetTransform( m_TransformList[Bone_LJetPack] );
			const D3DXPOSITION3& LJetPackPos = Math::Round_n( m_pSkinMesh[Bone_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ), 7 );

			// ���˃x�N�g���̍쐬.
			m_VectorList[LJetPackJet] = CenterPos - LJetPackPos;
			D3DXVec3Normalize( &m_VectorList[LJetPackJet], &m_VectorList[LJetPackJet] );

			// ���˃x�N�g���̒ǉ�.
			m_MoveVectorList.emplace_back( m_VectorList[LJetPackJet] * LPower );

			// ���W�F�b�g�p�b�N�̉��G�t�F�N�g�̊J�n.
			m_pEffectList[Left_Flame]->Play( m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ) );
			m_pEffectList[Left_Flame]->SetScale( LTrigger );

			// ���I�[�f�B�I��SE�̍Đ�.
			CSoundManager::SetSelectNo( 0 );
			CSoundManager::SetSEVolume(	JETPACK_SE, LTrigger );
			CSoundManager::PlaySELoop(	JETPACK_SE );
			CSoundManager::ResetSelectNo();
		}
		// ���W�F�b�g�p�b�N�̉��G�t�F�N�g�̊J�n.
		m_pEffectList[Left_Smoke]->Play( m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ) );
		m_pEffectList[Left_Smoke]->SetAlpha( LTrigger );
	}
	else {
		// ���W�F�b�g�p�b�N�̃G�t�F�N�g�̒�~.
		m_pEffectList[Left_Flame]->Stop();
		m_pEffectList[Left_Smoke]->StopRoot();

		// ���I�[�f�B�I��SE�̒�~.
		CSoundManager::SetSelectNo( 0 );
		CSoundManager::StopSE( JETPACK_SE, true );
		CSoundManager::ResetSelectNo();
	}

	// �E�g���K�[�������Ă��邩.
	if ( RTrigger != 0.0f ) {
		if ( m_FlagList[IsRJetPackStop] == false ) {
			// �{�[���̈ʒu���擾.
			m_pSkinMesh[Bone_RJetPack]->SetTransform( m_TransformList[Bone_RJetPack] );
			const D3DXPOSITION3& RJetPackPos = Math::Round_n( m_pSkinMesh[Bone_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ), 7 );

			// ���˃x�N�g���̍쐬.
			m_VectorList[RJetPackJet] = CenterPos - RJetPackPos;
			D3DXVec3Normalize( &m_VectorList[RJetPackJet], &m_VectorList[RJetPackJet] );

			// ���˃x�N�g���̒ǉ�.
			m_MoveVectorList.emplace_back( m_VectorList[RJetPackJet] * RPower );

			// �E�W�F�b�g�p�b�N�̉��G�t�F�N�g�̊J�n.
			m_pEffectList[Right_Flame]->Play( m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ) );
			m_pEffectList[Right_Flame]->SetScale( RTrigger );

			// �E�I�[�f�B�I��SE�̍Đ�.
			CSoundManager::SetSelectNo( 1 );
			CSoundManager::SetSEVolume(	JETPACK_SE, RTrigger );
			CSoundManager::PlaySELoop(	JETPACK_SE );
			CSoundManager::ResetSelectNo();
		}
		// �E�W�F�b�g�p�b�N�̉��G�t�F�N�g�̊J�n.
		m_pEffectList[Right_Smoke]->Play( m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ) );
		m_pEffectList[Right_Smoke]->SetAlpha( RTrigger );
	}
	else {
		// �E�W�F�b�g�p�b�N�̃G�t�F�N�g�̒�~.
		m_pEffectList[Right_Flame]->Stop();
		m_pEffectList[Right_Smoke]->StopRoot();

		// �E�I�[�f�B�I��SE�̒�~.
		CSoundManager::SetSelectNo( 1 );
		CSoundManager::StopSE( JETPACK_SE, true );
		CSoundManager::ResetSelectNo();
	}

	// �R���g���[���̐U��.
	CXInput::SetVibration( ( LTrigger - 0.3f ) * CONSTANT.VIBRATION_POWER, RTrigger * CONSTANT.VIBRATION_POWER );
}

//---------------------------.
// �W�F�b�g�p�b�N�̒�~.
//---------------------------.
void CPlayer::JetPackStop()
{
	if ( m_FlagList[IsLJetPackStop] && m_FlagList[IsRJetPackStop] ) return;

	// �ǂ�Ȃ��g���K�[�̓��͂��؂�ւ���������ׂ�.
	int		LChangeCnt		= 0;
	int		RChangeCnt		= 0;
	bool	IsLUp			= m_LJetPackInputList[1] > m_LJetPackInputList[0];
	bool	IsRUp			= m_RJetPackInputList[1] > m_RJetPackInputList[0];
	for ( int Now = 2; Now < CONSTANT.JETPACK_INPUT_LIST_MAX; ++Now ) {
		const int	Old		= Now - 1;
		const float LDiff	= m_LJetPackInputList[Now] - m_LJetPackInputList[Old];
		const float RDiff	= m_RJetPackInputList[Now] - m_RJetPackInputList[Old];

		// ���̃W�F�b�g�p�b�N�̉�������ω�������.
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

		// �E�̃W�F�b�g�p�b�N�̉�������ω�������.
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

	// �񐔂ɉ����ăW�F�b�g�p�b�N��c��܂���.
	const float LScale = STransform::NORMAL_SCALE + 0.03f * LChangeCnt;
	const float RScale = STransform::NORMAL_SCALE + 0.03f * RChangeCnt;
	m_TransformList[Mesh_LJetPack].Scale = { LScale, LScale, LScale };
	m_TransformList[Mesh_RJetPack].Scale = { RScale, RScale, RScale };

	// ���񐔈ȏ�؂�ւ���Ă����ꍇ���̃g���K�[���~������.
	if ( LChangeCnt >= CONSTANT.STOP_CHANGE_NUM ) {
		// ���W�F�b�g�p�b�N�̔����G�t�F�N�g�̍Đ�.
		m_pSkinMesh[Mesh_LJetPack]->SetTransform( m_TransformList[Bone_LJetPack] );
		m_pEffectList[Left_Explosion]->Play( m_pSkinMesh[Mesh_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE ) );
		m_pEffectList[Left_Explosion]->SetScale( 0.2f );

		m_FlagList[IsLJetPackStop] = true;
		m_pEffectList[Left_Flame]->Stop();

		// ����SE�̍Đ�.
		CSoundManager::SetSelectNo( 0 );
		CSoundManager::PlaySE( EXPLOSION_SE );
		CSoundManager::ResetSelectNo();
	}
	if ( RChangeCnt >= CONSTANT.STOP_CHANGE_NUM ) {
		// �E�W�F�b�g�p�b�N�̔����G�t�F�N�g�̍Đ�.
		m_pSkinMesh[Mesh_RJetPack]->SetTransform( m_TransformList[Mesh_RJetPack] );
		m_pEffectList[Right_Explosion]->Play( m_pSkinMesh[Mesh_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE ) );
		m_pEffectList[Right_Explosion]->SetScale( 0.2f );

		m_FlagList[IsRJetPackStop] = true;
		m_pEffectList[Right_Flame]->Stop();

		// ����SE�̍Đ�.
		CSoundManager::SetSelectNo( 1 );
		CSoundManager::PlaySE( EXPLOSION_SE );
		CSoundManager::ResetSelectNo();
	}
}

//---------------------------.
// �W�F�b�g�p�b�N�̒�~�̏C��.
//---------------------------.
void CPlayer::JetPackRepair()
{
	if ( m_FlagList[IsLJetPackStop] == false && m_FlagList[IsRJetPackStop] == false ) return;

	// ���W�F�b�g�p�b�N�̒�~���Ԃ̃J�E���g�̒ǉ�.
	if ( m_FlagList[IsLJetPackStop] ) {
		m_CntList[LJetPackStopTime] += m_DeltaTime;

		// ���W�F�b�g�p�b�N�̒�~�̏I��.
		if ( m_CntList[LJetPackStopTime] >= CONSTANT.JETPACK_STOP_TIME ) {
			m_CntList[LJetPackStopTime]	= INIT_FLOAT;
			m_FlagList[IsLJetPackStop]	= false;
		}
	}

	// �E�W�F�b�g�p�b�N�̒�~���Ԃ̃J�E���g�̒ǉ�.
	if ( m_FlagList[IsRJetPackStop] ) {
		m_CntList[RJetPackStopTime] += m_DeltaTime;

		// �E�W�F�b�g�p�b�N�̒�~�̏I��.
		if ( m_CntList[RJetPackStopTime] >= CONSTANT.JETPACK_STOP_TIME ) {
			m_CntList[RJetPackStopTime]	= INIT_FLOAT;
			m_FlagList[IsRJetPackStop]	= false;
		}
	}
}

//---------------------------.
// �W�F�b�g�p�b�N�̉�].
//---------------------------.
void CPlayer::JetPackRotationControl()
{
	if ( m_FlagList[IsControlStop] ) return;

	const float	EndRotX = -Math::ToRadian( CONSTANT.JETPACK_ROT_END ) * CXInput::GetLThumbY();
	float		RotX	= m_TransformList[Mesh_LJetPack].Rotation.x;

	// ������]�l�̂��ߔ�����.
	if ( EndRotX == RotX ) return;

	// �W�F�b�g�p�b�N�̉�].
	if ( EndRotX > RotX ) {
		RotX += Math::ToRadian( CONSTANT.JETPACK_ROT_SPEED );
		if ( EndRotX < RotX ) RotX = EndRotX;
	}
	else {
		RotX -= Math::ToRadian( CONSTANT.JETPACK_ROT_SPEED );
		if ( EndRotX > RotX ) RotX = EndRotX;
	}

	// ��]�̔��f.
	m_TransformList[Mesh_LJetPack].Rotation.x = RotX;
	m_TransformList[Mesh_RJetPack].Rotation.x = RotX;
	m_TransformList[Bone_LJetPack].Rotation.x = RotX;
	m_TransformList[Bone_RJetPack].Rotation.x = RotX;
}

//---------------------------.
// �v���C���[�̈ړ�.
//---------------------------.
void CPlayer::PlayerMove()
{
	D3DXVECTOR3* Position = &m_TransformList[Mesh_Body].Position;

	// �ړ��x�N�g���Ńv���C���[�𓮂���.
	*Position += m_VectorList[NowMove];

	// ���x�N�g���Ńv���C���[�𓮂���.
	*Position += m_VectorList[Wind];

	// �}�b�v�O���m�F����.
	OffStageCheck();
}

//---------------------------.
// �v���C���[�̉�].
//---------------------------.
void CPlayer::PlayerRotation()
{
	// ���E�̃W�F�b�g�p�b�N�̉�]�̋����̎擾
	const float LRotPower = m_TransformList[Bone_LJetPack].Rotation.x > 0.0f ?
		-D3DXVec3Length( &m_VectorList[LJetPackRot] ) :
		 D3DXVec3Length( &m_VectorList[LJetPackRot] );
	const float RRotPower = m_TransformList[Bone_RJetPack].Rotation.x > 0.0f ?
		 D3DXVec3Length( &m_VectorList[RJetPackRot] ) :
		-D3DXVec3Length( &m_VectorList[RJetPackRot] );

	// �g���K�[�̉�������擾.
	const float LTrigger = CXInput::GetLTrigger() * CONSTANT.JETPACK_POWER;
	const float RTrigger = CXInput::GetRTrigger() * CONSTANT.JETPACK_POWER;

	// �v���C���[�̉�].
	for ( auto& t : m_TransformList ) {
		D3DXROTATION3* pRot = &t.Rotation;
		pRot->y += LRotPower * LTrigger;
		pRot->y += RRotPower * RTrigger;

		// ��]�l��͈͓��Ɏ��߂�.
		if ( pRot->y < Math::RADIAN_MIN ) pRot->y += Math::RADIAN_MAX;
		if ( pRot->y > Math::RADIAN_MAX ) pRot->y -= Math::RADIAN_MAX;
	}

	// �J�����Ɍ��݂̉�]�l��n��.
	m_pMainCamera->SetRotation( m_TransformList[Mesh_Body].Rotation );
}

//---------------------------.
// �v���C���[�̌X���̍X�V.
//---------------------------.
void CPlayer::PlayerTiltUpdate()
{
	// ���݂̌X���̎擾.
	const D3DXROTATION3& Rot = { 0.0f, -m_TransformList[Bone_Body].Rotation.y, 0.0f };
	m_VectorList[Tilt] = Math::Round_n( Math::VectorRotation( m_VectorList[NowMove] + m_VectorList[Wind], Rot ), 5 );

	// ���݂̌X���ɉ������p�x�����߂�.
	float RotZ = m_TransformList[Bone_Body].Rotation.z;
	RotZ -= m_VectorList[Tilt].x;
	const float Radian_End = Math::ToRadian( CONSTANT.END_TILE );
	if ( RotZ >=  Radian_End ) RotZ =  Radian_End;
	if ( RotZ <= -Radian_End ) RotZ = -Radian_End;

	// ���݂̌X���ɉ����ăv���C���[����]������.
	for ( int i = 0; i < Mesh_Max; ++i ) {
		m_TransformList[i].Rotation.z = RotZ;
	}
}

//---------------------------.
// �ړ��x�N�g���̍쐬.
//---------------------------.
void CPlayer::CreateMoveVec()
{
	// �x�N�g�����X�g�̃x�N�g����S�č�������.
	D3DXVECTOR3 MoveVec = m_VectorList[NowMove] * CONSTANT.INERTIA_USAGE_RATE;
	for ( auto& Vec : m_MoveVectorList ) {
		MoveVec += Vec;
	}

	// �ړ��x�N�g���Ƃ��ĕۑ�.
	m_VectorList[OldMove] = m_VectorList[NowMove];
	m_VectorList[NowMove] = MoveVec;

	// �x�N�g�����X�g��������.
	m_MoveVectorList.clear();

	// �ړ����x�����߂�p�̃x�N�g���̍쐬.
	D3DXVECTOR3 SideMoveVector		= m_VectorList[NowMove];
	D3DXVECTOR3 VerticalMoveVector	= m_VectorList[NowMove];
	SideMoveVector.y				= INIT_FLOAT;
	VerticalMoveVector.x			= INIT_FLOAT;
	VerticalMoveVector.z			= INIT_FLOAT;

	// �ړ����x�̐ݒ�.
	m_SideMoveSpeed		= D3DXVec3Length( &SideMoveVector );
	m_VerticalMoveSpeed	= D3DXVec3Length( &VerticalMoveVector );
}

//---------------------------.
// ���E�̃W�F�b�g�p�b�N�̉�]�x�N�g���̍쐬.
//---------------------------.
void CPlayer::CreateRotVec()
{
	// ��]�x�N�g���̍쐬.
	m_VectorList[LJetPackRot] = m_TransformList[Bone_LJetPack].Position -
		m_pSkinMesh[Bone_LJetPack]->GetPosFromBone( L_JETPACK_JETPOS_BONE );
	m_VectorList[RJetPackRot] = m_TransformList[Bone_RJetPack].Position -
		m_pSkinMesh[Bone_RJetPack]->GetPosFromBone( R_JETPACK_JETPOS_BONE );

	// y���W�͎g�p���Ȃ����ߍ폜����.
	m_VectorList[LJetPackRot].y = INIT_FLOAT;
	m_VectorList[RJetPackRot].y = INIT_FLOAT;
}

//---------------------------.
// �d�͂̒ǉ�.
//---------------------------.
void CPlayer::AddGravity()
{
	// �d�͂̒ǉ�.
	m_MoveVectorList.emplace_back( GRAVITY_VECTOR * CONSTANT.GRAVITY_POWER );
}

//---------------------------.
// �����x�̏��/�����̐ݒ�.
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
// ���C
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
// ���n�G�t�F�N�g�̍Đ�.
//---------------------------.
void CPlayer::LandingEffectPlay( const D3DXPOSITION3& Pos )
{
	if ( m_FlagList[IsHitGround] && m_LandingEndPos == Pos ) return;

	// �G�t�F�N�g���Đ����邩.
	m_CntList[LandingEffect] += m_DeltaTime;
#ifdef _DEBUG
	if ( m_CntList[LandingEffect] < CONSTANT.LANDING_EFFECT_TIME * 5.0f ) return;
#else
	if ( m_CntList[LandingEffect] < CONSTANT.LANDING_EFFECT_TIME ) return;
#endif
	m_CntList[LandingEffect] = 0.0f;

	for ( auto& e : m_pLandingEffectList ) {
		if ( e->IsShow() ) continue;

		// �X�e�[�^�X�̐ݒ�.
		e->Play( Pos );
		m_LandingEndPos = Pos;
		return;
	}

	// �V�����쐬.
	m_pLandingEffectList.emplace_back( std::make_unique<CEffectManager>() );
	m_pLandingEffectList.back()->SetEffect( "Landing" );
	m_pLandingEffectList.back()->Play( Pos );
	m_LandingEndPos = Pos;
}

//---------------------------.
// ���n�G�t�F�N�g�̍X�V.
//---------------------------.
void CPlayer::LandingEffectUpdate()
{
	// �G�t�F�N�g�̍폜.
	const int Size = static_cast<int>( m_pLandingEffectList.size() );
	for ( int i = Size - 1; i >= 0; --i ) {
		if ( m_pLandingEffectList[i]->IsShow() ) break;
		m_pLandingEffectList.pop_back();
	}
}

//---------------------------.
// ���i�擾�A�j���[�V�����̍X�V.
//---------------------------.
void CPlayer::GetFoodAnimUpdate()
{
	if ( m_FlagList[IsGetFood] == false ) return;

	// ���i�擾�A�j���[�V�����̃J�E���g�𑝂₷.
	m_CntList[GetFoodAng] += CONSTANT.GET_FOOD_ANIM_SPPED * m_DeltaTime;

	// ���i�擾�A�j���[�V�����I��.
	if ( m_CntList[GetFoodAng] >= GET_FOOD_ANIM_CNT_MAX ) {
		m_CntList[GetFoodAng]	= 0.0f;
		m_FlagList[IsGetFood]	= false;
	}

	// �A�j���[�V�����p�̊g�k�����߂�.
	m_TransformList[Mesh_Bag].Scale.y = STransform::NORMAL_SCALE + sinf( Math::ToRadian( m_CntList[GetFoodAng] ) ) / 2.0f;
}

//---------------------------.
// �X�e�[�W�O���m�F.
//---------------------------.
void CPlayer::OffStageCheck()
{
	if ( m_FlagList[IsControlStop] ) return;

	const D3DXPOSITION3& Position = m_TransformList[Mesh_Body].Position;
	const float			 Distance = sqrtf( Position.x * Position.x + Position.z * Position.z );

	//  �X�e�[�W�O��.
	if ( ( Position.y	<= CONSTANT.DEAD_DOWN_POS_Y	) ||
		 ( Position.y	>= CONSTANT.DEAD_UP_POS_Y	) ||
		 ( Distance		>= CONSTANT.DEAD_RADIUS		) )
	{
		// ���Ô�𔭐�������.
		CScore::SetGameEnd();

		// ������~������.
		CInput::SetIsUpdateStop( true );

		// ������.
		CWind::StopWind();
		m_FlagList[IsControlStop]	= true;
		m_FlagList[IsStageOut]		= true;
		m_CntList[StageOutTime]		= 0.0f;

		// �J�������~������.
		if ( m_FlagList[IsActiveGazingCamera] ) m_pGazingCamera->SetIsMoveStop( true );
		else									m_pMainCamera->SetIsMoveStop( true );

		// �A�j���[�V�����̍Đ�.
		m_pSkinMesh[Mesh_Body]->ChangeAnimBlend_Skip( StageOut, &m_AC );

		// SE�̍Đ�.
		CSoundManager::PlaySE( "Ambulance" );
	}
}

//---------------------------.
// �X�e�[�W�O�Ɉړ��������̍X�V.
//---------------------------.
void CPlayer::StageOutUpdate()
{
	if ( m_FlagList[IsStageOut] == false ) return;

	// ��莞�Ԍo�߂�����Q�[���N���A�Ɉړ�������.
	m_CntList[StageOutTime] += m_DeltaTime;
	if ( m_CntList[StageOutTime] >= CONSTANT.STAGE_OUT_TIME ) {
		CInput::SetIsUpdateStop( false );
		CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade, 0.01f );
	}
}

//---------------------------.
// �ړ���SE�̍Đ�.
//---------------------------.
void CPlayer::PlayMoveSE()
{
	// ���Ɉړ�SE�̍Đ�.
	if ( m_VerticalMoveSpeed >= CONSTANT.DEAD_GROUND_SPEED ) {
		// �Đ�.
		if ( m_SEVolumeList[MoveDown] < 1.0f ) {
			m_SEVolumeList[MoveDown] += CONSTANT.ADD_SE_VOLUME * m_DeltaTime;
			if ( m_SEVolumeList[MoveDown] >= 1.0f ) m_SEVolumeList[MoveDown] = 1.0f;
			CSoundManager::SetSEVolume( MOVE_SE, m_SEVolumeList[MoveDown] );
		}
		CSoundManager::PlaySELoop( MOVE_SE );
	}
	else {
		// ��~.
		if ( m_SEVolumeList[MoveDown] > 0.0f ) {
			m_SEVolumeList[MoveDown] -= CONSTANT.ADD_SE_VOLUME * m_DeltaTime;
			if ( m_SEVolumeList[MoveDown] <= 0.0f ) {
				m_SEVolumeList[MoveDown] = 0.0f;
				CSoundManager::StopSE( MOVE_SE );
			}
			CSoundManager::SetSEVolume( MOVE_SE, m_SEVolumeList[MoveDown] );
		}
	}

	// �ړ�SE�̍Đ�.
	if ( m_SideMoveSpeed >= CONSTANT.DEAD_WALL_SPEED ) {
		// �Đ�.
		if ( m_SEVolumeList[Move] < 1.0f ) {
			m_SEVolumeList[Move] += CONSTANT.ADD_SE_VOLUME * m_DeltaTime;
			if ( m_SEVolumeList[Move] >= 1.0f ) m_SEVolumeList[Move] = 1.0f;
			CSoundManager::SetSEVolume( MOVE_SE, m_SEVolumeList[Move] );
		}
		CSoundManager::PlaySELoop( MOVE_SE );
	}
	else {
		// ��~.
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