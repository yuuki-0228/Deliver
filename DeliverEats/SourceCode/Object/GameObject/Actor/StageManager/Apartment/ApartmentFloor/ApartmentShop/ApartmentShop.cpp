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
	constexpr float MOVE_MIN			= 0.0f;		// �ړ��̍ŏ��l.
	constexpr float MOVE_MAX			= 1.0f;		// �ړ��̍ő�l.
	constexpr float GETGAUGE_MAX		= 1.0f;		// �󂯎��Q�[�W�̍ő�l.
	constexpr float GETGAUGE_MIN		= 0.0f;		// �󂯎��Q�[�W�̍ŏ��l.
	constexpr float INVERSION_DROT		= 180.0f;	// �p�x�𔽓]������p�x(�x).

	constexpr char EMPLOYEE_RESISTOR_POS_BONE_NAME[]		= "employee_position1";		// �X���̃��W�̈ʒu�̃{�[����.
	constexpr char EMPLOYEE_KILN_POS_BONE_NAME[]			= "employee_position2";		// �X���̃s�U�q�̈ʒu�̃{�[����.
	constexpr char EMPLOYEE_REFRIGERATOR_POS_BONE_NAME[]	= "employee_position3";		// �X���̗①�ɂ̈ʒu�̃{�[����.
	constexpr char SMOKE_POS_BONE_NAME[]					= "kemuri_position";		// ���̈ʒu�̃{�[����.
	constexpr char FIRE_POS_BONE_NAME[]						= "fire1";					// �s�U�q�̉��̈ʒu�̃{�[����.
	constexpr char ICON_POS_BONE_NAME[]						= "hukidasi_bone";			// �A�C�R���̈ʒu�̃{�[����.

	// �A�j���[�V����No.
	enum enAnimationNo : unsigned char {
		MoveResistorAnim,		// ���W�Ɉړ�.
		ResistorAnim,			// ���W.
		MoveKilnAnim,			// �q�Ɉړ�.
		KilnAnim,				// �q.
		MoveRefrigeratorAnim,	// �①�ɂɈړ�.
		RefrigeratorAnim,		// �①��.

		AnimationNo_Max
	} typedef EAnimationNo;

	// �X���̈ړ��ꏊNo.
	enum enEmployeePositionNo : unsigned char {
		Resistor,		// ���W.
		Kiln,			// �q.
		Refrigerator,	// �①��.

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
// ������.
//---------------------------.
bool CApartmentShop::Init()
{
	// ���b�V���̎擾.
	m_pShopMesh = CMeshResource::GetSkin( "shop_apart_s" );
	m_pShopMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pShopMesh->SetTransform( m_Transform );

	// �s�U�q�̉��̏�����.
	m_pFire = std::make_unique<CPizzaKilnFire>();
	m_pFire->SetTransform( m_Transform );
	m_pFire->SetPosition( m_pShopMesh->GetPosFromBone( FIRE_POS_BONE_NAME ) );
	m_pFire->Init();

	// �A�C�R���̎擾.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pShopMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::Employee );
	 
	// ���W�̕ۑ�.
	m_EmployeeSetPosition[Resistor		] = m_pShopMesh->GetPosFromBone( EMPLOYEE_RESISTOR_POS_BONE_NAME );
	m_EmployeeSetPosition[Kiln			] = m_pShopMesh->GetPosFromBone( EMPLOYEE_KILN_POS_BONE_NAME );
	m_EmployeeSetPosition[Refrigerator	] = m_pShopMesh->GetPosFromBone( EMPLOYEE_REFRIGERATOR_POS_BONE_NAME );
	m_EmployeeMoveVectir[Resistor		] = m_EmployeeSetPosition[Resistor]		- m_EmployeeSetPosition[Kiln];
	m_EmployeeMoveVectir[Kiln			] = m_EmployeeSetPosition[Kiln]			- m_EmployeeSetPosition[Refrigerator];
	m_EmployeeMoveVectir[Refrigerator	] = m_EmployeeSetPosition[Refrigerator]	- m_EmployeeSetPosition[Resistor];

	// �X���̏�����.
	m_pEmployee						= std::make_unique<CEmployee>( EObjectTag::Apartment, m_ShopNo );
	m_EmployeeTransform				= m_Transform;
	m_EmployeeTransform.Position	= m_EmployeeSetPosition[Kiln];
	m_EmployeeTransform.Rotation.y	+= Math::ToRadian( 90.0f );
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetAddMoney( CONSTANT.APARTMENT_ADD_MONEY );
	m_pEmployee->SetDirection( GetRoomDirection() );
	m_pEmployee->Init();

	// ���̏�����.
	m_pSmokeManager		= std::make_unique<CSmokeManager>();
	m_pSmokeManager->Init();
	m_SmokePlayPosition = m_pShopMesh->GetPosFromBone( SMOKE_POS_BONE_NAME );
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CApartmentShop::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.6f;

	GoResistor();							// ���W�Ɉړ�.
	GoRefrigerator();						// �①�ɂɈړ�.
	GoKiln();								// �q�Ɉړ�.
	PlaySmoke();							// ���̍Đ�.
	EmployeeUpdate();						// �X���̍X�V.
	m_pSmokeManager->Update( DeltaTime );	// ���̍X�V.
	m_pFire->Update( DeltaTime );			// �s�U�q�̉��̍X�V.

	// �s�U�qSE���Đ����邩.
	if ( m_AnimationNo == KilnAnim && m_pEmployee->GetIsShortly() ) {
		CDeliveryManager::AddKilnEmployee( m_pEmployee.get() );
	}

	IconUpdate();							// �A�C�R���̕\���̍X�V.
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CApartmentShop::LateUpdate( const float& DeltaTime )
{
	m_pEmployee->LateUpdate( DeltaTime );

	// �X�����v���C���[�̕�������悤�ɉ�]������.
	EmployeePlayerLook();
}

//---------------------------.
// �`��.
//---------------------------.
void CApartmentShop::Render()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	// ���X�̕`��.
	m_pShopMesh->SetDither( true );
	m_pShopMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pShopMesh->ChangeTexture( 0, m_ShopNo - 1 );
	m_pShopMesh->SetMeshPointLight( m_PointLight );
	m_pShopMesh->Render( &m_Transform );
	m_pShopMesh->SetDither( false );

	// �s�U�q�̕`��.
	m_pFire->SetPosition( m_pShopMesh->GetPosFromBone( FIRE_POS_BONE_NAME ) );
	m_pFire->Render();

	// �X���̕`��.
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetRotation( m_EmployeeTransform.Rotation + m_EmployeeAddRotation );
	m_pEmployee->SetMeshPointLight( m_PointLight );
	m_pEmployee->Render();

	// ���̕`��.
	m_pSmokeManager->Render();
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
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
// �A���t�@�̐ݒ�.
//---------------------------.
void CApartmentShop::SetAlpha( const float Alpha )
{
	m_Alpha = Alpha;
	m_pEmployee->SetAlpha( m_Alpha );
}

//---------------------------.
// �����̌������擾.
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
// �A�C�R���̍X�V.
//---------------------------.
void CApartmentShop::IconUpdate()
{
	// �A�C�R���̍X�V.
	if ( m_pEmployee->GetIsLink() == false ) return;
	m_pIcon->IconUpdate( m_pEmployee->GetHandle() );
}

//---------------------------.
// �X���̍X�V.
//---------------------------.
void CApartmentShop::EmployeeUpdate()
{
	m_pEmployee->SetTransform( m_EmployeeTransform );
	m_pEmployee->SetRotation( m_EmployeeTransform.Rotation + m_EmployeeAddRotation );
	m_pEmployee->Update( m_DeltaTime );
}

//---------------------------.
// �X���̉�].
//---------------------------.
void CApartmentShop::EmployeeRotation( const float EndRot )
{
	m_EmployeeAddRotation.y -= Math::ToRadian( CONSTANT.ROT_SPPED ) * m_DeltaTime;

	// ��]���I��������.
	if ( m_EmployeeAddRotation.y <= Math::ToRadian( EndRot ) ) {
		m_EmployeeAddRotation.y = Math::ToRadian( EndRot );
		if ( m_EmployeeAddRotation.y < Math::RADIAN_MIN ) m_EmployeeAddRotation.y += Math::RADIAN_MAX;
		m_RotationEnd = true;
	}
}

//---------------------------.
// ���W�Ɉړ�.
//---------------------------.
void CApartmentShop::GoResistor()
{
	if ( m_EmployeeTransform.Position	== m_EmployeeSetPosition[Resistor]	) return;
	if ( m_pEmployee->GetIsLink()		== false							) return;
	m_pEmployee->SetIsLookPlayer( false );
	m_IsSmoke = false;

	// �X���̉�].
	if ( m_RotationEnd == false ) {
		EmployeeRotation( -180.0f );
		return;
	}
	if ( m_RotationEnd == false ) return;

	// ���W�̑O�܂ňړ�������.
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_EmployeeSetPosition[Kiln] + ( m_EmployeeMoveVectir[Resistor] * m_MoveCnt );

	// �ړ��I��.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeTransform.Position	= m_EmployeeSetPosition[Resistor];
		m_RotationEnd					= false;
		m_AnimationNo					= ResistorAnim;
		m_pEmployee->SetIsLookPlayer( true );
	}
}

//---------------------------.
// �①�ɂɈړ�.
//---------------------------.
void CApartmentShop::GoRefrigerator()
{
	if ( m_AnimationNo == MoveKilnAnim || m_AnimationNo == KilnAnim ) return;
	if ( m_pEmployee->GetIsLink()									) return;
	m_pEmployee->SetIsLookPlayer( false );

	// �X���̉�].
	if ( m_RotationEnd == false ) {
		EmployeeRotation( -180.0f );
		return;
	}
	if ( m_RotationEnd == false ) return;

	// �①�ɂ̑O�܂ňړ�������.
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_EmployeeSetPosition[Resistor] + ( m_EmployeeMoveVectir[Refrigerator] * m_MoveCnt );

	// �ړ��I��.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeTransform.Position	= m_EmployeeSetPosition[Refrigerator];
		m_RotationEnd					= false;
		m_AnimationNo					= MoveKilnAnim;
	}
}

//---------------------------.
// �q�Ɉړ�.
//---------------------------.
void CApartmentShop::GoKiln()
{
	if ( m_AnimationNo != MoveKilnAnim	) return;
	if ( m_pEmployee->GetIsLink()		) return;

	// �X���̉�].
	if ( m_RotationEnd == false ) {
		EmployeeRotation( 0.0f );
		return;
	}
	if ( m_RotationEnd == false ) return;

	// �q�̑O�܂ňړ�������.
	m_MoveCnt += m_DeltaTime * CONSTANT.MOVE_SPEED;
	m_EmployeeTransform.Position = m_EmployeeSetPosition[Refrigerator] + ( m_EmployeeMoveVectir[Kiln] * m_MoveCnt );

	// �ړ��I��.
	if ( m_MoveCnt >= MOVE_MAX ) {
		m_MoveCnt						= MOVE_MIN;
		m_EmployeeTransform.Position	= m_EmployeeSetPosition[Kiln];
		m_RotationEnd					= false;
		m_AnimationNo					= KilnAnim;
		m_IsSmoke						= true;
	}
}

//---------------------------.
// ���̍Đ�.
//---------------------------.
void CApartmentShop::PlaySmoke()
{
	if ( m_pEmployee->GetIsLink()	) return;
	if ( m_IsSmoke == false			) return;

	// ���̍Đ�.
	m_pSmokeManager->PlaySmoke( m_SmokePlayPosition, -m_Transform.GetRotationVector(), CWind::GetWindVector( m_SmokePlayPosition.y ) * 1.5f );
}

//---------------------------.
// �X�����v���C���[�̕�������悤�ɉ�].
//---------------------------.
void CApartmentShop::EmployeePlayerLook()
{
	if ( m_AnimationNo				!= ResistorAnim ) return;
	if ( m_pEmployee->GetIsLink()	== false		) return;

	const float InitRot			= m_EmployeeTransform.Rotation.y;	// �����p�x.
	const float InversionRot	= Math::ToRadian( INVERSION_DROT );	// ���]������p�x.

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
