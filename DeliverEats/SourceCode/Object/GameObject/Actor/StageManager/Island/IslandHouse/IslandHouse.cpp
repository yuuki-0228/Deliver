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
	constexpr float DROT_MAX			= 180.0f;	// ��]�̍ő�l(�x).
	constexpr float MOVE_MIN			= 0.0f;		// �ړ��̍ŏ��l.
	constexpr float MOVE_MAX			= 1.0f;		// �ړ��̍ő�l.
	constexpr float DOOR_MIN			= 0.0f;		// ���̉�]�̍ŏ��l.
	constexpr float DOOR_MAX			= 1.0f;		// ���̉�]�̍ő�l.
	constexpr float DOOR_DROT_MIN		= 0.0f;		// ���̉�]�̍ŏ��l(�x).
	constexpr float DOOR_DROT_MAX		= -90.0f;	// ���̉�]�̍ő�l(�x).
	constexpr float INVERSION_DROT		= 180.0f;	// �p�x�𔽓]������p�x(�x).

	constexpr char ICON_POS_BONE_NAME[]				= "i_h_hukidasi";			// �A�C�R���̈ʒu�̃{�[����.
	constexpr char CUSTOMER_START_POS_BONE_NAME[]	= "i_customer_position1";	// ���q����̊J�n�̈ʒu�̃{�[����.
	constexpr char CUSTOMER_END_POS_BONE_NAME[]		= "i_customer_position2";	// ���q����̏I���̈ʒu�̃{�[����.
	constexpr char DOOR_POS_BONE_NAME[]				= "door_position";			// ���̈ʒu�̃{�[����.
	constexpr char HOUSE_OPEN_SE[]					= "HouseOpen";				// �����J��������SE.
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
// ������.
//---------------------------.
bool CIslandHouse::Init()
{
	// �Ƃ̃��b�V���̎擾.
	m_pHouseMesh = CMeshResource::GetSkin( "island_house_s" );
	m_pHouseMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pHouseMesh->SetTransform( m_Transform );

	// �A�C�R���̎擾.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pHouseMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::Customer );

	// ���W�̎擾.
	m_StartPos						= m_pHouseMesh->GetPosFromBone( CUSTOMER_START_POS_BONE_NAME );
	m_EndPos						= m_pHouseMesh->GetPosFromBone( CUSTOMER_END_POS_BONE_NAME );
	m_MoveVector					= m_EndPos - m_StartPos;
	m_CustomerTransform				= m_Transform;
	m_CustomerTransform.Position	= m_StartPos;
	m_DoorTransform					= m_Transform;
	m_DoorTransform.Position		= m_pHouseMesh->GetPosFromBone( DOOR_POS_BONE_NAME );

	// ���̃��b�V���̎擾.
	m_pDoorMesh = CMeshResource::GetSkin( "house_door_s" );
	m_pDoorMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pDoorMesh->SetTransform( m_DoorTransform );

	// �K���[�W�̏�����.
	m_pGarage						= std::make_unique<CGarage>();
	m_pGarage->SetTransform( m_Transform );
	m_pGarage->Init();

	// ���q����̏�����.
	m_pCustomer						= std::make_unique<CCustomer>( EObjectTag::Island );
	m_CustomerTransform.Rotation	= { 0.0f, Math::ToRadian( -90.0f ), 0.0f };
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetDirection( EDirection::Down );
	m_pCustomer->Init();

	// �|�C���g���C�g�̐ݒ�.
	m_LightOnHour		= CRandom::GetRand( 16, 17 );
	m_LightOnMinute		= CRandom::GetRand( 0,  50 );
	m_LightOffHour		= CRandom::GetRand( 22, 24 );
	m_LightOffMinute	= CRandom::GetRand( 0,  50 );
	m_PointLight.Brightness		= INIT_FLOAT;
	m_CustomerLight.Brightness	= INIT_FLOAT;

	// �^�O�̐ݒ�.
	m_ObjectTag = EObjectTag::IslandObject;

	InitCollision();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CIslandHouse::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector = m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance = D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.0f;

	CustomerDispMove();					// ���q�����\�����邽�߂̈ړ�.
	CustomerHideMove();					// ���q������\���ɂ��邽�߂̈ړ�.
	m_pGarage->Update( DeltaTime );		// �K���[�W�̍X�V.
	m_pCustomer->Update( DeltaTime );	// ���q����̍X�V.

	IconUpdate();						// �A�C�R���̍X�V.
	LightUpdate();						// ���C�g�̍X�V.
	CameraAlphaUpdate();				// �J�����ɂ��A���t�@����.
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CIslandHouse::LateUpdate( const float& DeltaTime )
{
	m_pCustomer->LateUpdate( DeltaTime );

	// ���q������v���C���[�̕�������悤�ɉ�]������.
	CustomerPlayerLook();
}

//---------------------------.
// �`��.
//---------------------------.
void CIslandHouse::Render()
{
	if ( m_IsDisp == false ) return;

	// �Ƃ̕`��.
	m_pHouseMesh->SetDither( true );
	m_pHouseMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pHouseMesh->SetMeshPointLight( m_PointLight );
	m_pHouseMesh->Render( &m_Transform );
	m_pHouseMesh->SetDither( false );
	CCollisionRender::PushMesh( m_pCollisionMesh, &m_Transform );

	// ���̕`��.
	CDirectX11::SetRasterizerState( ERS_STATE::None );
	m_pDoorMesh->SetDither( true );
	m_pDoorMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pDoorMesh->SetTransform( m_DoorTransform );
	m_pDoorMesh->SetMeshPointLight( m_PointLight );
	m_pDoorMesh->Render();
	m_pDoorMesh->SetDither( false );
	CDirectX11::SetRasterizerState( ERS_STATE::Back );

	// �K���[�W�̕`��.
	m_pGarage->Render();

	// ���q����̕`��.
	if ( m_CustomerTransform.Position == m_StartPos ) return;
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetRotation( m_CustomerTransform.Rotation + m_CustomerAddRotation );
	m_pCustomer->SetMeshPointLight( m_CustomerLight );
	m_pCustomer->Render();
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//---------------------------.
void CIslandHouse::Sprite3DRender_A()
{
	if ( m_IsDisp == false ) return;

	m_pCustomer->Sprite3DRender_A();

	if ( m_pCustomer->GetIsLink() == false ) return;
	m_pIcon->Sprite3DRender_A();
}

//---------------------------.
// �K���[�W�̍��W�̐ݒ�.
//---------------------------.
void CIslandHouse::SetGaragePosition( const D3DXPOSITION3& Pos )
{
	m_pGarage->SetPosition( Pos );
}

//---------------------------.
// �����蔻���ǉ�����.
//---------------------------.
void CIslandHouse::PushCollisionManager()
{
	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Hit );

	m_pGarage->PushCollisionManager();
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CIslandHouse::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "house_c" );
	m_pCollisions	 = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CIslandHouse::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_Transform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_Transform );
}

//---------------------------.
// �J�����ɂ��A���t�@�l�̍X�V.
//---------------------------.
void CIslandHouse::CameraAlphaUpdate()
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
// �A�C�R���̍X�V.
//---------------------------.
void CIslandHouse::IconUpdate()
{
	// �A�C�R���̍X�V.
	if ( m_pCustomer->GetIsLink() == false ) return;
	m_pIcon->IconUpdate( m_pCustomer->GetHandle() );

	// �A�C�R���̃^�O�̍X�V.
	m_pIcon->SetTag( EObjectTag::Customer );
	if ( m_pCustomer->GetIsGoHandingCustomer() == false ) return;
	m_pIcon->SetTag( EObjectTag::GoHandingCustomer );
}

//---------------------------.
// ���C�g�̍X�V.
//---------------------------.
void CIslandHouse::LightUpdate()
{
	if ( m_IsLightOff ) return;

	// ���Ԃ̎擾.
	const std::pair<int, int>& Time = CTime::GetTime();

	// ���C�g�̍X�V.
	if ( m_IsLightOn )	LightOff( Time );
	else				LightOn(  Time );
}

//---------------------------.
// ���C�g������.
//---------------------------.
void CIslandHouse::LightOn( const std::pair<int, int>& Time )
{
	// �d�C�����鎞�Ԃ��ǂ���.
	if ( m_LightOnHour		> Time.first	) return;
	if ( m_LightOnMinute	> Time.second	) return;

	// �d�C������.
	m_IsLightOn				= true;
	m_PointLight.Brightness = 1.0f;
}

//---------------------------.
// ���C�g�̏���.
//---------------------------.
void CIslandHouse::LightOff( const std::pair<int, int>& Time )
{
	// ���݂̎��Ԃ�24���ȍ~��2�Z���̕\���ɕύX����.
	std::pair<int, int> cTime = Time;
	if ( cTime.first < 12 ) cTime.first += 24;

	// �d�C�����鎞�Ԃ��ǂ���.
	if ( m_LightOffHour		> cTime.first	) return;
	if ( m_LightOffMinute	> cTime.second	) return;

	// �d�C������.
	m_IsLightOff			= true;
	m_PointLight.Brightness = 0.0f;
}

//---------------------------.
// ���q�����\�����邽�߂̈ړ�.
//---------------------------.
void CIslandHouse::CustomerDispMove()
{
	if ( m_CustomerTransform.Position	== m_EndPos	) return;
	if ( m_pCustomer->GetIsLink()		== false	) return;

	// �����J����.
	DoorOpen();

	// ���q����̈ړ�.
	if ( m_IsOpenEnd == false ) return;
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_CustomerTransform.Position = m_StartPos + ( m_MoveVector * m_MoveCnt );

	// �ړ��I��.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_CustomerTransform.Position	= m_EndPos;
		m_IsRotEnd						= false;
		m_IsCloseEnd					= false;

		// ���U��A�j���[�V�����ɕύX.
		m_pCustomer->ChangeWaveHands();
	}
}

//---------------------------.
// ���q���񂪃v���C���[�̕�������悤�ɉ�].
//---------------------------.
void CIslandHouse::CustomerPlayerLook()
{
	if ( m_pCustomer->GetIsLink()		== false	) return;
	if ( m_CustomerTransform.Position	!= m_EndPos ) return;

	const float InitRot			= m_CustomerTransform.Rotation.y;		// �����p�x.
	const float InversionRot	= Math::ToRadian( INVERSION_DROT );		// ���]������p�x.

	// �v���C���[�̕����̊p�x�����߂�.
	float EndRotY = m_pCustomer->GetRotation().y - InitRot;
	if ( EndRotY >  InversionRot ) EndRotY -= Math::RADIAN_MAX;
	if ( EndRotY < -InversionRot ) EndRotY += Math::RADIAN_MAX;

	// ��]���������߂�.
	const float OldRotY		= m_CustomerAddRotation.y;
	float		DireRotY	= m_pCustomer->GetRotation().y - InitRot - OldRotY;
	if ( DireRotY >  InversionRot ) DireRotY -= Math::RADIAN_MAX;
	if ( DireRotY < -InversionRot ) DireRotY += Math::RADIAN_MAX;


	// �v���C���[�̕����ɉ�]������.
	float RotSpeed = Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;
	if ( DireRotY < 0.0f ) RotSpeed = -RotSpeed;
	m_CustomerAddRotation.y += RotSpeed;
	if (		DireRotY >= 0.0f && m_CustomerAddRotation.y - OldRotY >= DireRotY ) m_CustomerAddRotation.y = EndRotY;
	else if (	DireRotY <  0.0f && m_CustomerAddRotation.y - OldRotY <= DireRotY ) m_CustomerAddRotation.y = EndRotY;
	if ( m_CustomerAddRotation.y >  InversionRot ) m_CustomerAddRotation.y -= Math::RADIAN_MAX;
	if ( m_CustomerAddRotation.y < -InversionRot ) m_CustomerAddRotation.y += Math::RADIAN_MAX;
}

//---------------------------.
// ���q������\���ɂ��邽�߂̈ړ�.
//---------------------------.
void CIslandHouse::CustomerHideMove()
{
	if ( m_pCustomer->GetIsLink()							) return;
	if ( m_pCustomer->GetIsGetPizzaAnimEnd() == false		) return;
	if ( m_CustomerTransform.Position		 == m_StartPos	) {
		if ( m_IsCloseEnd ) return;
		// �������.
		DoorClose();
		return;
	}

	// ���q�������]������.
	CustomerRotation();

	// ���q����̈ړ�.
	if ( m_IsRotEnd == false ) return;
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_CustomerTransform.Position = m_EndPos - ( m_MoveVector * m_MoveCnt );

	// �ړ��I��.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_CustomerAddRotation.y			= 0.0f;
		m_CustomerTransform.Position	= m_StartPos;
		m_IsOpenEnd						= false;
	}
}

//---------------------------.
// ���q����̉�].
//---------------------------.
void CIslandHouse::CustomerRotation()
{
	if ( m_IsRotEnd												) return;
	if ( m_CustomerAddRotation.y >= Math::ToRadian( DROT_MAX )	) return;

	// 180�x��]������.
	float RotSpeed = Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;
	if ( m_CustomerAddRotation.y < 0 ) RotSpeed = -RotSpeed;
	m_CustomerAddRotation.y += RotSpeed;

	// ��]�I��.
	const float& RadRotMax = Math::ToRadian( DROT_MAX );
	if ( m_CustomerAddRotation.y >= RadRotMax || m_CustomerAddRotation.y <= -RadRotMax ) {
		m_CustomerAddRotation.y = RadRotMax;
		m_IsRotEnd				= true;
	}
}

//---------------------------.
// �����J����.
//---------------------------.
void CIslandHouse::DoorOpen()
{
	if ( m_IsOpenEnd ) return;

	// SE�̍Đ�.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( HOUSE_OPEN_SE, m_DoorTransform.Position );
	}

	m_DoorMoveCnt				+= m_DeltaTime * CONSTANT.DOOR_MOVE_SPEED;
	m_DoorTransform.Rotation.y = Math::ToRadian( DOOR_DROT_MAX * m_DoorMoveCnt );

	// �ړ��I��.
	if ( m_DoorMoveCnt >= DOOR_MAX ) {
		m_DoorMoveCnt				= DOOR_MIN;
		m_DoorTransform.Rotation.y	= Math::ToRadian( DOOR_DROT_MAX );
		m_IsOpenEnd					= true;
		m_IsOpenSE					= false;
	}
}

//---------------------------.
// ���������.
//---------------------------.
void CIslandHouse::DoorClose()
{
	if ( m_IsCloseEnd ) return;

	// SE�̍Đ�.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( HOUSE_OPEN_SE, m_DoorTransform.Position );
	}

	m_DoorMoveCnt				+= m_DeltaTime * CONSTANT.DOOR_MOVE_SPEED;
	m_DoorTransform.Rotation.y	 = Math::ToRadian( DOOR_DROT_MAX - ( DOOR_DROT_MAX * m_DoorMoveCnt ) );

	// �ړ��I��.
	if ( m_DoorMoveCnt >= DOOR_MAX ) {
		m_DoorMoveCnt				= DOOR_MIN;
		m_DoorTransform.Rotation.y	= Math::ToRadian( DOOR_DROT_MIN );
		m_IsCloseEnd				= true;
		m_IsOpenSE					= false;
	}
}