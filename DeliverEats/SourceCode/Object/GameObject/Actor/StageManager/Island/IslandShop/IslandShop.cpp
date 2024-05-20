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
	constexpr float DROT_MAX			= 180.0f;	// ��]�̍ő�l.
	constexpr float MOVE_MIN			= 0.0f;		// �ړ��̍ŏ��l.
	constexpr float MOVE_MAX			= 1.0f;		// �ړ��̍ő�l.
	constexpr float DOOR_MOVE_MIN		= 0.0f;		// ���̈ړ��̍ŏ��l.
	constexpr float DOOR_MOVE_MAX		= 1.0f;		// ���̈ړ��̍ő�l.
	constexpr float INVERSION_DROT		= 180.0f;	// �p�x�𔽓]������p�x(�x).

	constexpr char SMOKE_POS_BONE_NAME[]			= "i_shop_smoke";			// ���̈ʒu�̃{�[����.
	constexpr char ICON_POS_BONE_NAME[]				= "i_s_hukidashi";			// �A�C�R���̈ʒu�̃{�[����.
	constexpr char EMPLOYEE_START_POS_BONE_NAME[]	= "i_employee_position1";	// �X���̊J�n�̈ʒu�̃{�[����.
	constexpr char EMPLOYEE_END_POS_BONE_NAME[]		= "i_employee_position2";	// �X���̏I���̈ʒu�̃{�[����.
	constexpr char DOOR_START_POS_BONE_NAME[]		= "p_door_position1";		// ���̊J�n���̃{�[����.
	constexpr char DOOR_END_POS_BONE_NAME[]			= "p_door_position2";		// ���̏I�����̃{�[����.
	constexpr char SHOP_OPEN_SE[]					= "ShopOpen";				// �����J��������SE.
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
// ������.
//---------------------------.
bool CIslandShop::Init()
{
	// ���X�̃��b�V���̎擾.
	m_pShopMesh = CMeshResource::GetSkin( "island_shop_s" );
	m_pShopMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pShopMesh->SetTransform( m_Transform );

	// �A�C�R���̎擾.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pShopMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::Employee );

	// ���̏�����.
	m_pSmokeManager					= std::make_unique<CSmokeManager>();
	m_SmokePlayPosition				= m_pShopMesh->GetPosFromBone( SMOKE_POS_BONE_NAME );
	m_pSmokeManager->Init();

	// ���W�̎擾.
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

	// ���̃��b�V���̎擾.
	m_pDoorMesh = CMeshResource::GetSkin( "island_pizza_door_s" );
	m_pDoorMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pDoorMesh->SetTransform( m_DoorTransform );

	// �Ŕ̏�����.
	m_pSignboard					= std::make_unique<CSignboard>();
	m_pSignboard->SetTransform( m_Transform );
	m_pSignboard->SetShopNo( m_ShopNo );
	m_pSignboard->Init();

	// �X���̏�����.
	m_pEmployee						= std::make_unique<CEmployee>( EObjectTag::Island, m_ShopNo );
	m_EmployeeTransform.Rotation	= { 0.0f, Math::ToRadian( -90.0f ), 0.0f };
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetDirection( EDirection::Down );
	m_pEmployee->SetIsLookPlayer( true );
	m_pEmployee->Init();

	// �|�C���g���C�g�̐ݒ�.
	m_PointLight.Brightness		= 1.0f;
	m_EmployeeLight.Brightness	= INIT_FLOAT;

	// �^�O�̐ݒ�.
	m_ObjectTag = EObjectTag::IslandObject;

	InitCollision();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CIslandShop::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector = m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance = D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.0f;

	PlaySmoke();							// ���̍Đ�.
	m_pSmokeManager->Update( DeltaTime );	// ���̍X�V.
	
	EmployeeDispMove();						// �X����\�����邽�߂̈ړ�.
	EmployeeHideMove();						// �X�����\���ɂ��邽�߂̈ړ�.
	m_pSignboard->Update( DeltaTime );		// �Ŕ̍X�V.
	m_pEmployee->Update( DeltaTime );		// �X���̍X�V.

	IconUpdate();							// �A�C�R���̍X�V.
	CameraAlphaUpdate();					// �J�����ɂ��A���t�@����.
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CIslandShop::LateUpdate( const float& DeltaTime )
{
	m_pEmployee->LateUpdate( DeltaTime );

	// �X�����v���C���[�̕�������悤�ɉ�]������.
	EmployeePlayerLook();
}

//---------------------------.
// �`��.
//---------------------------.
void CIslandShop::Render()
{
	if ( m_IsDisp == false ) return;

	// ���X�̕`��.
	m_pShopMesh->SetDither( true );
	m_pShopMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pShopMesh->ChangeTexture( 0, m_ShopNo - 3 );
	m_pShopMesh->SetMeshPointLight( m_PointLight );
	m_pShopMesh->Render( &m_Transform );
	m_pShopMesh->SetDither( false );
	CCollisionRender::PushMesh( m_pCollisionMesh, &m_Transform );

	// ���̕`��.
	m_pDoorMesh->SetDither( true );
	m_pDoorMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pDoorMesh->SetTransform( m_DoorTransform );
	m_pDoorMesh->SetMeshPointLight( m_PointLight );
	m_pDoorMesh->Render();
	m_pDoorMesh->SetDither( false );

	// �Ŕ̕`��.
	m_pSignboard->Render();

	// ���̕`��.
	m_pSmokeManager->Render();

	// �X���̕`��.
	if ( m_EmployeeTransform.Position == m_StartPos ) return;
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetRotation( m_EmployeeTransform.Rotation + m_EmployeeAddRotation );
	m_pEmployee->SetMeshPointLight( m_EmployeeLight );
	m_pEmployee->Render();
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//---------------------------.
void CIslandShop::Sprite3DRender_A()
{
	if ( m_IsDisp == false ) return;

	m_pEmployee->Sprite3DRender_A();

	if ( m_pEmployee->GetIsLink() == false ) return;
	m_pIcon->Sprite3DRender_A();
}

//---------------------------.
// �Ŕ̍��W�̐ݒ�.
//---------------------------.
void CIslandShop::SetSignboardPosition( const D3DXPOSITION3& Pos )
{
	m_pSignboard->SetPosition( Pos );
}

//---------------------------.
// �����蔻���ǉ�����.
//---------------------------.
void CIslandShop::PushCollisionManager()
{
	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );

	m_pSignboard->PushCollisionManager();
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CIslandShop::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "shop_c" );
	m_pCollisions	 = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CIslandShop::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// �J�����ɂ��A���t�@�l�̍X�V.
//---------------------------.
void CIslandShop::CameraAlphaUpdate()
{
	if ( m_IsDisp == false ) return;

	// �J�������B���Ă��邩.
	bool IsHit = Coll::IsRayToMesh(
		CCameraManager::GetRay(),
		m_pCollisions->GetCollision<CMesh>() );

	// �����ɂ��Ă���.
	if ( IsHit ) {
		if ( m_Alpha <= CONSTANT.CAMERA_ALPHA_MIN ) return;
		m_Alpha -= CONSTANT.CAMERA_ALPHA_SPEED;
		if ( m_Alpha <= CONSTANT.CAMERA_ALPHA_MIN )
			m_Alpha = CONSTANT.CAMERA_ALPHA_MIN;
	}
	// �s�����ɂ��Ă���.
	else {
		if ( m_Alpha >= CONSTANT.CAMERA_ALPHA_MAX ) return;
		m_Alpha += CONSTANT.CAMERA_ALPHA_SPEED;
		if ( m_Alpha >= CONSTANT.CAMERA_ALPHA_MAX )
			m_Alpha = CONSTANT.CAMERA_ALPHA_MAX;
	}
}

//---------------------------.
// �A�C�R���̕\���̍X�V.
//---------------------------.
void CIslandShop::IconUpdate()
{
	// �A�C�R���̍X�V.
	if ( m_pEmployee->GetIsLink() == false ) return;
	m_pIcon->IconUpdate( m_pEmployee->GetHandle() );
}

//---------------------------.
// ���̍Đ�.
//---------------------------.
void CIslandShop::PlaySmoke()
{
	if ( m_pEmployee->GetIsLink()	) return;
	if ( m_IsSmoke == false			) return;
 
	// ���̍Đ�.
	m_pSmokeManager->PlaySmoke( m_SmokePlayPosition, Math::_X_VECTOR, CWind::GetWindVector( m_SmokePlayPosition.y ) * 1.5f );
}

//---------------------------.
// �X����\�����邽�߂̈ړ�.
//---------------------------.
void CIslandShop::EmployeeDispMove()
{
	if ( m_EmployeeTransform.Position	== m_EndPos	) return;
	if ( m_pEmployee->GetIsLink()		== false	) return;
	m_IsSmoke = false;

	// �����J����.
	DoorOpen();

	// �X���̈ړ�.
	if ( m_IsOpenEnd == false ) return;
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_StartPos + ( m_MoveVector * m_MoveCnt );

	// �ړ��I��.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeTransform.Position	= m_EndPos;
		m_IsRotEnd						= false;
		m_IsCloseEnd					= false;

		// ���U��A�j���[�V�����ɕύX.
		m_pEmployee->ChangeWaveHands();
	}
}

//---------------------------.
// �X�����v���C���[�̕�������悤�ɉ�].
//---------------------------.
void CIslandShop::EmployeePlayerLook()
{
	if ( m_pEmployee->GetIsLink()		== false	) return;
	if ( m_EmployeeTransform.Position	!= m_EndPos ) return;

	const float InitRot			= m_EmployeeTransform.Rotation.y;		// �����p�x.
	const float InversionRot	= Math::ToRadian( INVERSION_DROT );		// ���]������p�x.

	// �v���C���[�̕����̊p�x�����߂�.
	float EndRotY = m_pEmployee->GetRotation().y - InitRot;
	if ( EndRotY >  InversionRot ) EndRotY -= Math::RADIAN_MAX;
	if ( EndRotY < -InversionRot ) EndRotY += Math::RADIAN_MAX;

	// ��]���������߂�.
	const float OldRotY		= m_EmployeeAddRotation.y;
	float		DireRotY	= m_pEmployee->GetRotation().y - InitRot - OldRotY;
	if ( DireRotY >  InversionRot ) DireRotY -= Math::RADIAN_MAX;
	if ( DireRotY < -InversionRot ) DireRotY += Math::RADIAN_MAX;

	// �v���C���[�̕����ɉ�]������.
	float RotSpeed = Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;
	if ( DireRotY < 0.0f ) RotSpeed = -RotSpeed;
	m_EmployeeAddRotation.y += RotSpeed;
	if (		DireRotY >= 0.0f && m_EmployeeAddRotation.y - OldRotY >= DireRotY ) m_EmployeeAddRotation.y = EndRotY;
	else if (	DireRotY <  0.0f && m_EmployeeAddRotation.y - OldRotY <= DireRotY ) m_EmployeeAddRotation.y = EndRotY;
	if ( m_EmployeeAddRotation.y >  InversionRot ) m_EmployeeAddRotation.y -= Math::RADIAN_MAX;
	if ( m_EmployeeAddRotation.y < -InversionRot ) m_EmployeeAddRotation.y += Math::RADIAN_MAX;
}

//---------------------------.
// �X�����\���ɂ��邽�߂̈ړ�.
//---------------------------.
void CIslandShop::EmployeeHideMove()
{
	if ( m_pEmployee->GetIsLink()								) return;
	if ( m_pEmployee->GetIsPizzaPassedAnimEnd() == false		) return;
	if ( m_EmployeeTransform.Position			== m_StartPos	) {
		if ( m_IsCloseEnd ) return;
		// �������.
		DoorClose();
		return;
	}

	// �X������]������.
	EmployeeRotation();

	// �X���̈ړ�.
	if ( m_IsRotEnd == false ) return;
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_EndPos - ( m_MoveVector * m_MoveCnt );

	// �ړ��I��.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeAddRotation.y			= 0.0f;
		m_EmployeeTransform.Position	= m_StartPos;
		m_IsOpenEnd						= false;
		m_IsSmoke						= true;
	}
}

//---------------------------.
// �X���̉�].
//---------------------------.
void CIslandShop::EmployeeRotation()
{
	if ( m_IsRotEnd												) return;
	if ( m_EmployeeAddRotation.y >= Math::ToRadian( DROT_MAX )	) return;

	// 180�x��]������.
	float RotSpeed = Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;
	if ( m_EmployeeAddRotation.y < 0 ) RotSpeed = -RotSpeed;
	m_EmployeeAddRotation.y += RotSpeed;

	// ��]�I��.
	const float& RadRotMax = Math::ToRadian( DROT_MAX );
	if ( m_EmployeeAddRotation.y >= RadRotMax || m_EmployeeAddRotation.y <= -RadRotMax ) {
		m_EmployeeAddRotation.y = RadRotMax;
		m_IsRotEnd				= true;
	}
}

//---------------------------.
// �����J����.
//---------------------------.
void CIslandShop::DoorOpen()
{
	if ( m_IsOpenEnd ) return;

	// SE�̍Đ�.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( SHOP_OPEN_SE, m_DoorTransform.Position );
	}

	m_DoorMoveCnt += m_DeltaTime * CONSTANT.DOOR_MOVE_SPEED;
	m_DoorTransform.Position = m_DoorStartPos + ( m_DoorMoveVector * m_DoorMoveCnt );

	// �ړ��I��.
	if ( m_DoorMoveCnt >= DOOR_MOVE_MAX ) {
		m_DoorMoveCnt				= DOOR_MOVE_MIN;
		m_DoorTransform.Position	= m_DoorEndPos;
		m_IsOpenEnd					= true;
		m_IsOpenSE					= false;
	}
}

//---------------------------.
// ���������.
//---------------------------.
void CIslandShop::DoorClose()
{
	if ( m_IsCloseEnd ) return;

	// SE�̍Đ�.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( SHOP_OPEN_SE, m_DoorTransform.Position );
	}

	m_DoorMoveCnt += m_DeltaTime * CONSTANT.DOOR_MOVE_SPEED;
	m_DoorTransform.Position = m_DoorEndPos - ( m_DoorMoveVector * m_DoorMoveCnt );

	// �ړ��I��.
	if ( m_DoorMoveCnt >= DOOR_MOVE_MAX ) {
		m_DoorMoveCnt				= DOOR_MOVE_MIN;
		m_DoorTransform.Position	= m_DoorStartPos;
		m_IsCloseEnd				= true;
		m_IsOpenSE					= false;
	}
}