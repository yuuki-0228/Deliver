#include "GazingCamera.h"
#include "..\..\..\Utility\Easing\Easing.h"

namespace {
	constexpr float ADJUST_POSITION = 15.0f;	// カメラがプレイヤーの後ろにくるように調整する値.
}

CGazingCamera::CGazingCamera()
	: m_pPlayer			( nullptr )
	, m_IsSwitching		( false )
	, m_StartPosition	( INIT_FLOAT3 )
	, m_Count			( INIT_FLOAT )
	, m_RotX			( INIT_FLOAT )
{
}

CGazingCamera::~CGazingCamera()
{
}

//---------------------------.
// 更新.
//	ターゲットが設定されていない場合処理は行わない.
//---------------------------.
void CGazingCamera::Update( const float& DeltaTime )
{
	// 注視点をプレイヤーに合わせる.
	m_LookPosition = m_pPlayer->GetPosition();

	// カメラが停止している(場外に出たり壁や地面に激突した場合)なら以後の処理は行わない.
	if ( m_IsMoveStop )return;

	// プレイヤーからターゲットに向かうベクトルを計算する.
	D3DXVECTOR3 Vec = m_pPlayer->GetPosition() - m_pTarget->GetPosition() - D3DXVECTOR3( 0.0f, 2.0f, 0.0f );
	// ベクトルを正規化する.
	D3DXVec3Normalize( &Vec, &Vec );
	// カメラの位置.
	const D3DXVECTOR3 GazingPos = m_pPlayer->GetPosition() + Vec * ADJUST_POSITION;

	// メインカメラから切り替わり、移動が完了していないなら.
	if ( m_IsSwitching ) {
		MoveFromMainCamera( GazingPos, DeltaTime );
		return;
	}

	// 移動が完了したなら.
	// カメラの位置をプレイヤーに合わせる.
	m_Transform.Position = GazingPos;
}

//---------------------------.
// メインカメラから切り替わった際に呼び出す.
//---------------------------.
void CGazingCamera::StartMove()
{
	m_IsSwitching = true;

	m_StartPosition = m_Transform.Position;
	m_Count = INIT_FLOAT;
}

//---------------------------.
// メインカメラから切り替わった時の処理.
//---------------------------.
void CGazingCamera::MoveFromMainCamera( const D3DXVECTOR3 GoalPos, const float& DeltaTime )
{
	// 切り替え中はメインカメラの動きを再現する.
	const float Distance = -15.0f;
	// ターゲットの向きを取得.
	const D3DXVECTOR3 TarVec = m_pPlayer->GetTransform().GetRotationVector();
	m_StartPosition.x = m_pPlayer->GetPosition().x + Distance * sinf( m_RotX ) * TarVec.x;
	m_StartPosition.y = m_pPlayer->GetPosition().y + Distance * cosf( m_RotX );
	m_StartPosition.z = m_pPlayer->GetPosition().z + Distance * sinf( m_RotX ) * TarVec.z;

	// 切り替え処理.

	m_Count += DeltaTime;
	if ( m_Count >= 1.0f )m_Count = 1.0f;

	D3DXVECTOR3 ToStartVector = m_StartPosition - GoalPos;
	float EaseCount = CEasing::InOutSine( m_Count, 1.0f );
	m_Transform.Position = m_StartPosition - ToStartVector * EaseCount;

	if ( m_Count >= 1.0f ) {
		m_IsSwitching = false;
		m_Transform.Position = GoalPos;
	}
}
