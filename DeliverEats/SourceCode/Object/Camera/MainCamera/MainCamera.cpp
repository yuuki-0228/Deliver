#include "MainCamera.h"
#include "..\..\..\Utility\Math\Math.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Scene\SceneManager\SceneManager.h"
#include "..\..\..\Utility\Easing\Easing.h"

namespace {
	constexpr float LIMIT_THETA = Math::PI * 0.4f;	// m_Thetaの可動範囲.
}

CMainCamera::CMainCamera()
	: m_Theta					( INIT_FLOAT )
	, m_RotX					( INIT_FLOAT )
	, m_pCustomer				( nullptr )
	, m_IsSwitching( false )
	, m_Count					( INIT_FLOAT )
	, m_StartPosition			( INIT_FLOAT3 )
	, m_EndPosition				( INIT_FLOAT3 )
	, m_IsReturnDefaultPos		( true )
	, m_ReturnValue				( INIT_FLOAT )
	, m_AddValue				( INIT_FLOAT )
	, m_OldTarPosY				( INIT_FLOAT )
	, m_IsCompleteReturnRStick	( true )
	, m_OldRTrigger				( INIT_FLOAT )
{
	// 値の初期化.
	m_ReturnValue	= CONSTANT.INIT_RETURN_VALUE;
	m_AddValue		= CONSTANT.INIT_ADD_VALUE;
}

CMainCamera::~CMainCamera()
{
	m_pCustomer = nullptr;
}

//---------------------------.
// 更新.
//	ターゲットが設定させていない場合処理は行わない.
//---------------------------.
void CMainCamera::Update( const float& DeltaTime )
{
	// ポーズ中なら処理を行わない.
	if ( CSceneManager::GetIsPause() ) return;
	// ターゲットが存在しないなら処理を行わない.
	if ( m_pTarget == nullptr )return;

	Rotate();

	// ターゲットのトランスフォームを取得.
	STransform TrgTransform = m_pTarget->GetTransform();
	// 過去の座標と現在の座標の差分を取る.(SubPosYは降下時+,上昇時-).
	const float SubPosY = m_OldTarPosY - TrgTransform.Position.y;

	// カメラの座標の更新.
	m_RotX = Math::ToRadian( ( CONSTANT.ADJUST_ANGLE * m_Theta - CONSTANT.ADD_DELAY_ANGLE * SubPosY ) - CONSTANT.INIT_ROTATION );
	// 傾ける角度を設定する.
	if (		m_RotX >= Math::ToRadian( CONSTANT.MOVABLE_MAX ) ) m_RotX = Math::ToRadian( CONSTANT.MOVABLE_MAX );
	else if (	m_RotX <= Math::ToRadian( CONSTANT.MOVABLE_MIN ) ) m_RotX = Math::ToRadian( CONSTANT.MOVABLE_MIN );
	m_RotX -= Math::ToRadian( CONSTANT.ADJUST_ANGLE );

	// 注視点の更新.
	m_LookPosition	= TrgTransform.Position;

	// 1F前のY座標の更新.
	m_OldTarPosY	= TrgTransform.Position.y;

	// 移動できないなら以後の処理は行わない.
	if ( m_IsMoveStop ) return;
	const float Distance = -15.0f;
	// ターゲットの向きを取得.
	const D3DXVECTOR3 TarVec = TrgTransform.GetRotationVector();

	D3DXVECTOR3* pPos = m_IsSwitching ? &m_EndPosition : &m_Transform.Position;
	pPos->x = m_LookPosition.x + Distance * sinf( m_RotX ) * TarVec.x;
	pPos->y = m_LookPosition.y + Distance * cosf( m_RotX );
	pPos->z = m_LookPosition.z + Distance * sinf( m_RotX ) * TarVec.z;

	MoveFromGazingCamera( m_EndPosition, DeltaTime );
}

//---------------------------.
// 注視カメラから切り替わった際に呼び出す.
//---------------------------.
void CMainCamera::StartMove()
{
	m_IsSwitching = true;

	m_Theta = INIT_FLOAT;
	m_OldTarPosY = m_pTarget->GetPosition().y;

	m_StartPosition = m_Transform.Position;
	m_Count = INIT_FLOAT;
}

//---------------------------.
// カメラを元の角度に戻す処理.
//---------------------------.
void CMainCamera::UpdateReturnRotation( float& RotTheta, const float Rot )
{
	// 現在の角度に応じて角度を戻す処理を行う.
	if ( RotTheta >= Rot ) {
		RotTheta -= Math::ToRadian( m_ReturnValue );
		if ( RotTheta <= Rot ) InitRotation( Rot );
	}
	else {
		RotTheta += Math::ToRadian( m_ReturnValue );
		if ( RotTheta >= Rot ) InitRotation( Rot );
	}

	// 毎フレーム戻す角度の大きさを増やす.
	m_ReturnValue += Math::ToRadian( m_AddValue );
	// 加算する値を増やす.
	m_AddValue *= 1.1f;
}

//---------------------------.
// カメラが元の角度に戻った後行う処理.
//---------------------------.
void CMainCamera::InitRotation(const float Rot)
{
	// 初期状態に戻す.
	m_Theta					= Rot;
	m_IsReturnDefaultPos	= false;
	m_ReturnValue			= CONSTANT.INIT_RETURN_VALUE;
	m_AddValue				= CONSTANT.INIT_ADD_VALUE;
}

//---------------------------.
// Rスティックでの操作処理.
//---------------------------.
void CMainCamera::Rotate()
{
	// 元の角度に戻そうとした瞬間を取得する.
	const bool	OldIsReturnRot = m_IsReturnDefaultPos;
	// Rスティックが押されたら,回転角度を初期化する.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_RIGHT_THUMB ) )m_IsReturnDefaultPos = true;
	// Rスティックの入力を受け取る.
	const float RTrigger = CONSTANT.IS_REVERSE ? CXInput::GetRThumbY() : -CXInput::GetRThumbY();

	// カメラが元の角度に戻っていないなら.
	if ( m_IsReturnDefaultPos ) {
		// 元の位置にあるRスティックが傾いた瞬間なら.
		if ( m_OldRTrigger == 0.0f && RTrigger != 0.0f ) m_IsReturnDefaultPos = false;

		// カメラを元の角度に戻す処理.
		UpdateReturnRotation( m_Theta );
	}
	// カメラが元の角度に戻っていて、Rスティックが倒れていないなら.
	else if ( m_IsCompleteReturnRStick ) {
		m_Theta -= RTrigger * 0.03f;

		float AdjustRotation = CONSTANT.INIT_ROTATION / CONSTANT.ADJUST_ANGLE;
		if ( m_Theta >= AdjustRotation + LIMIT_THETA ) m_Theta = AdjustRotation + LIMIT_THETA;
		else if ( m_Theta <= AdjustRotation - LIMIT_THETA ) m_Theta = AdjustRotation - LIMIT_THETA;
	}

	// Rスティックが傾いているなら.
	if ( RTrigger != 0.0f ) {
		// 元の角度に戻ろうとした瞬間なら.
		if ( OldIsReturnRot && m_IsReturnDefaultPos == false ) {
			m_IsCompleteReturnRStick = false;
		}
	}
	// Rスティックを元の位置に戻した瞬間.
	else if ( m_OldRTrigger != 0.0f ) {
		m_IsCompleteReturnRStick = true;
	}
	m_OldRTrigger = RTrigger;
}

//---------------------------.
// 注視カメラから切り替わった時の処理.
//---------------------------.
void CMainCamera::MoveFromGazingCamera( const D3DXVECTOR3 GoalPos, const float& DeltaTime )
{
	if ( m_IsSwitching == false )return;

	// プレイヤーから客に向かうベクトルを計算する.
	D3DXVECTOR3 Vec = m_pTarget->GetPosition() - m_pCustomer->GetPosition() - D3DXVECTOR3( 0.0f, 2.0f, 0.0f );
	// ベクトルを正規化する.
	D3DXVec3Normalize( &Vec, &Vec );
	// カメラの位置.
	m_StartPosition = m_pTarget->GetPosition() + Vec * 15.0f;

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
