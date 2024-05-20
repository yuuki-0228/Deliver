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
	constexpr float DROT_MAX		= 180.0f;	// ��]�̍ő�l.
	constexpr float MOVE_MIN		= 0.0f;		// �ړ��̍ŏ��l.
	constexpr float MOVE_MAX		= 1.0f;		// �ړ��̍ő�l.
	constexpr float WINDOW_MOVE_MIN	= 0.0f;		// ���̈ړ��̍ŏ��l.
	constexpr float WINDOW_MOVE_MAX = 1.0f;		// ���̈ړ��̍ő�l.
	constexpr float INVERSION_DROT	= 180.0f;	// �p�x�𔽓]������p�x(�x).

	constexpr char CUSTOMER_START_POS_BONE_NAME[]	= "costomer_position1";		// ���q����̊J�n�̈ʒu�̃{�[����.
	constexpr char CUSTOMER_END_POS_BONE_NAME[]		= "costomer_position2";		// ���q����̏I���̈ʒu�̃{�[����.
	constexpr char ICON_POS_BONE_NAME[]				= "speech_bubble_position";	// �A�C�R���̈ʒu�̃{�[����.
	constexpr char WINDOW_START_POS_BONE_NAME[]		= "window_position1";		// ���̊J�n���̃{�[����.
	constexpr char WINDOW_END_POS_BONE_NAME[]		= "window_position2";		// ���̏I�����̃{�[����.
	constexpr char ROOM_OPEN_SE[]					= "RoomOpen";				// �J�[�e�����J��������SE.
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
// ������.
//---------------------------.
bool CApartmentRoom::Init()
{
	// ���b�V���̎擾.
	m_pRoomMesh = CMeshResource::GetSkin( "room_s" );
	m_pRoomMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pRoomMesh->SetTransform( m_Transform );

	// �A�C�R���̎擾.
	m_pIcon = std::make_unique<CStageIcon>();
	m_pIcon->SetPosition( m_pRoomMesh->GetPosFromBone( ICON_POS_BONE_NAME ) );
	m_pIcon->InitIcon( EObjectTag::Customer );

	// ���W�̎擾.
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

	// ���̃��b�V���̎擾.
	m_pWindowMesh = CMeshResource::GetSkin( "room_window_s" );
	m_pWindowMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pWindowMesh->SetTransform( m_WindowTransform );

	// ���q����̏�����.
	m_pCustomer = std::make_unique<CCustomer>( EObjectTag::Apartment );
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetAddMoney( CONSTANT.APARTMENT_ADD_MONEY );
	m_pCustomer->SetDirection( GetRoomDirection() );
	m_pCustomer->Init();

	// ���C�g�̐ݒ�.
	m_LightOnHour		= CRandom::GetRand( 16, 17 );
	m_LightOnMinute		= CRandom::GetRand( 0,  50 );
	m_LightOffHour		= CRandom::GetRand( 22, 24 );
	m_LightOffMinute	= CRandom::GetRand( 0,  50 );
	m_PointLight.Brightness = INIT_FLOAT;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CApartmentRoom::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	
	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.6f;

	CustomerDispMove();		// ���q�����\�����邽�߂̈ړ�.
	CustomerHideMove();		// ���q������\���ɂ��邽�߂̈ړ�.
	CustomerUpdate();		// ���q����̍X�V.
	IconUpdate();			// �A�C�R���̍X�V.
	LightUpdate();			// ���C�g�̍X�V.
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CApartmentRoom::LateUpdate( const float& DeltaTime )
{
	m_pCustomer->LateUpdate( DeltaTime );

	// ���q������v���C���[�̕�������悤�ɉ�]������.
	CustomerPlayerLook();
}

//---------------------------.
// �`��.
//---------------------------.
void CApartmentRoom::Render()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	// �Ƃ̕`��.
	m_pRoomMesh->SetDither( true );
	m_pRoomMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pRoomMesh->SetMeshPointLight( m_PointLight );
	m_pRoomMesh->Render( &m_Transform );
	m_pRoomMesh->SetDither( false );

	// ���̕`��.
	m_pWindowMesh->SetDither( true );
	m_pWindowMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pWindowMesh->SetMeshPointLight( m_PointLight );
	m_pWindowMesh->Render( &m_WindowTransform );
	m_pWindowMesh->SetDither( false );

	// ���q����̕`��.
	if ( m_CustomerTransform.Position == m_StartPos ) return;
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetRotation( m_CustomerTransform.Rotation + m_CustomerAddRotation );
	m_pCustomer->SetMeshPointLight( m_PointLight );
	m_pCustomer->Render();
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
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
// �A���t�@�̐ݒ�.
//---------------------------.
void CApartmentRoom::SetAlpha( const float Alpha )
{
	m_Alpha = Alpha;
	m_pCustomer->SetAlpha( m_Alpha );
}

//---------------------------.
// �����̌������擾.
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
// �A�C�R���̍X�V.
//---------------------------.
void CApartmentRoom::IconUpdate()
{
	// �A�C�R���̍X�V.
	if ( m_pCustomer->GetIsLink() == false ) return;
	m_pIcon->IconUpdate( m_pCustomer->GetHandle() );

	// �A�C�R���̃^�O�̍X�V.
	m_pIcon->SetTag( EObjectTag::Customer );
	if ( m_pCustomer->GetIsGoHandingCustomer() == false ) return;
	m_pIcon->SetTag(  EObjectTag::GoHandingCustomer );
}

//---------------------------.
// ���C�g�̍X�V.
//---------------------------.
void CApartmentRoom::LightUpdate()
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
void CApartmentRoom::LightOn( const std::pair<int, int>& Time )
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
void CApartmentRoom::LightOff( const std::pair<int, int>& Time )
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
// ���q����̍X�V.
//---------------------------.
void CApartmentRoom::CustomerUpdate()
{
	m_pCustomer->SetTransform( m_CustomerTransform );
	m_pCustomer->SetRotation( m_CustomerTransform.Rotation + m_CustomerAddRotation );
	m_pCustomer->Update( m_DeltaTime );
}

//---------------------------.
// ���q����̕\�����邽�߂̈ړ�.
//---------------------------.
void CApartmentRoom::CustomerDispMove()
{
	if ( m_CustomerTransform.Position	== m_EndPos	) return;
	if ( m_pCustomer->GetIsLink()		== false	) return;

	// �����J����.
	WindowOpen();

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
void CApartmentRoom::CustomerPlayerLook()
{
	if ( m_CustomerTransform.Position			!= m_EndPos ) return;
	if ( m_pCustomer->GetIsLink()				== false	) return;
	if ( m_pCustomer->GetIsGoHandingCustomer()	== false	) return;

	const float InitRot			= m_CustomerTransform.Rotation.y;	// �����p�x.
	const float InversionRot	= Math::ToRadian( INVERSION_DROT );	// ���]������p�x.

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
void CApartmentRoom::CustomerHideMove()
{
	if ( m_pCustomer->GetIsLink()							) return;
	if ( m_pCustomer->GetIsGetPizzaAnimEnd() == false		) return;
	if ( m_CustomerTransform.Position		 == m_StartPos	) {
		if ( m_IsCloseEnd ) return;
		// �������.
		WindowClose();
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
void CApartmentRoom::CustomerRotation()
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
void CApartmentRoom::WindowOpen()
{
	if ( m_IsOpenEnd ) return;

	// SE�̍Đ�.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( ROOM_OPEN_SE, m_WindowTransform.Position );
	}

	m_WindowMoveCnt += m_DeltaTime * CONSTANT.WINDOW_MOVE_SPEED;
	m_WindowTransform.Position = m_WindowStartPos + ( m_WindowMoveVector * m_WindowMoveCnt );

	// �ړ��I��.
	if ( m_WindowMoveCnt >= WINDOW_MOVE_MAX ) {
		m_WindowMoveCnt				= WINDOW_MOVE_MIN;
		m_WindowTransform.Position	= m_WindowEndPos;
		m_IsOpenEnd					= true;
		m_IsOpenSE					= false;
	}
}

//---------------------------.
// ���������.
//---------------------------.
void CApartmentRoom::WindowClose()
{
	if ( m_IsCloseEnd ) return;

	// SE�̍Đ�.
	if ( m_IsOpenSE == false ) {
		m_IsOpenSE = true;
		CSoundManager::PlaySE3D( ROOM_OPEN_SE, m_WindowTransform.Position );
	}

	m_WindowMoveCnt += m_DeltaTime * CONSTANT.WINDOW_MOVE_SPEED;
	m_WindowTransform.Position = m_WindowEndPos - ( m_WindowMoveVector * m_WindowMoveCnt );

	// �ړ��I��.
	if ( m_WindowMoveCnt >= WINDOW_MOVE_MAX ) {
		m_WindowMoveCnt				= WINDOW_MOVE_MIN;
		m_WindowTransform.Position	= m_WindowStartPos;
		m_IsCloseEnd				= true;
		m_IsOpenSE					= false;
	}
}
