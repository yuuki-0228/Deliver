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
// 初期化.
//---------------------------.
bool CApartmentEmptyRoom::Init()
{
	// メッシュの取得.
	m_pRoomMesh = CMeshResource::GetSkin( "EmptyRoom_s" );
	m_pRoomMesh->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_pRoomMesh->SetTransform( m_Transform );

	// 人が住んでいるか設定.
	m_IsEmptyRoom	= CRandom::Probability( 1, 4 );

	// ライトの設定.
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
// 更新.
//---------------------------.
void CApartmentEmptyRoom::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// 距離によるアルファフェードの更新.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	if ( m_IsEmptyRoom == false ) {
		LightUpdate();	// ライトの更新.
	}

	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 2.6f;
}

//---------------------------.
// 描画.
//---------------------------.
void CApartmentEmptyRoom::Render()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	// 家の描画.
	m_pRoomMesh->SetDither( true );
	m_pRoomMesh->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pRoomMesh->SetMeshPointLight( m_PointLight );
	m_pRoomMesh->Render( &m_Transform );
	m_pRoomMesh->SetDither( false );
}

//---------------------------.
// ライトの更新.
//---------------------------.
void CApartmentEmptyRoom::LightUpdate()
{
	if ( m_IsLightOff ) return;

	// 時間の取得.
	const std::pair<int, int>& Time = CTime::GetTime();

	// ライトの更新.
	if ( m_IsLightOn )	LightOff( Time );
	else				LightOn(  Time );
}

//---------------------------.
// ライトをつける.
//---------------------------.
void CApartmentEmptyRoom::LightOn( const std::pair<int, int>& Time )
{
	// 電気をつける時間かどうか.
	if ( m_LightOnHour		> Time.first	) return;
	if ( m_LightOnMinute	> Time.second	) return;

	// 電気をつける.
	m_IsLightOn				= true;
	m_PointLight.Brightness = 1.0f;
}

//---------------------------.
// ライトの消す.
//---------------------------.
void CApartmentEmptyRoom::LightOff( const std::pair<int, int>& Time )
{
	// 現在の時間を24時以降も2〇時の表示に変更する.
	std::pair<int, int> cTime = Time;
	if ( cTime.first < 12 ) cTime.first += 24;

	// 電気をつける時間かどうか.
	if ( m_LightOffHour		> cTime.first	) return;
	if ( m_LightOffMinute	> cTime.second	) return;

	// 電気を消す.
	m_IsLightOff			= true;
	m_PointLight.Brightness = 0.0f;
}
