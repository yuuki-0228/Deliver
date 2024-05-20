#include "ApartmentEmptyRoom.h"
#include "..\..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\..\Utility\Random\Random.h"

CApartmentEmptyRoom::CApartmentEmptyRoom()
	: m_pRoomMesh		( nullptr )
	, m_PointLight		()
	, m_Alpha			( Color::ALPHA_MAX )
	, m_LightOnHour		( INIT_INT )
	, m_LightOnMinute	( INIT_INT )
	, m_LightOffHour	( INIT_INT )
	, m_LightOffMinute	( INIT_INT )
	, m_FloorIsDisp		( true )
	, m_IsLightOn		( false )
	, m_IsLightOff		( false )
	, m_IsEmptyRoom		( false )
{
}

CApartmentEmptyRoom::~CApartmentEmptyRoom()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CApartmentEmptyRoom::Init()
{
	// ���b�V���̎擾.
	m_pRoomMesh = CMeshResource::GetSkin( "EmptyRoom_s" );
	m_pRoomMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pRoomMesh->SetTransform( m_Transform );

	// �l���Z��ł��邩�ݒ�.
	m_IsEmptyRoom	= CRandom::Probability( 1, 4 );

	// ���C�g�̐ݒ�.
	m_PointLight.Brightness = INIT_FLOAT;
	if ( m_IsEmptyRoom ) return true;
	const int Max = CRandom::Probability( 1, 5 ) ? CRandom::Probability( 1, 5 ) ? 22 : 20 : 18;
	m_LightOnHour		= CRandom::GetRand( 16, Max );
	m_LightOnMinute		= CRandom::GetRand( 0,  50 );
	m_LightOffHour		= CRandom::GetRand( 22, 24 );
	m_LightOffMinute	= CRandom::GetRand( 0,  50 );
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CApartmentEmptyRoom::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	if ( m_IsEmptyRoom == false ) {
		LightUpdate();	// ���C�g�̍X�V.
	}

	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.6f;
}

//---------------------------.
// �`��.
//---------------------------.
void CApartmentEmptyRoom::Render()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	// �Ƃ̕`��.
	m_pRoomMesh->SetDither( true );
	m_pRoomMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pRoomMesh->SetMeshPointLight( m_PointLight );
	m_pRoomMesh->Render( &m_Transform );
	m_pRoomMesh->SetDither( false );
}

//---------------------------.
// ���C�g�̍X�V.
//---------------------------.
void CApartmentEmptyRoom::LightUpdate()
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
void CApartmentEmptyRoom::LightOn( const std::pair<int, int>& Time )
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
void CApartmentEmptyRoom::LightOff( const std::pair<int, int>& Time )
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
