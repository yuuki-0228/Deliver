#include "MainCamera.h"
#include "..\..\..\Utility\Math\Math.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Scene\SceneManager\SceneManager.h"
#include "..\..\..\Utility\Easing\Easing.h"

namespace {
	constexpr float LIMIT_THETA = Math::PI * 0.4f;	// m_Theta�̉��͈�.
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
	// �l�̏�����.
	m_ReturnValue	= CONSTANT.INIT_RETURN_VALUE;
	m_AddValue		= CONSTANT.INIT_ADD_VALUE;
}

CMainCamera::~CMainCamera()
{
	m_pCustomer = nullptr;
}

//---------------------------.
// �X�V.
//	�^�[�Q�b�g���ݒ肳���Ă��Ȃ��ꍇ�����͍s��Ȃ�.
//---------------------------.
void CMainCamera::Update( const float& DeltaTime )
{
	// �|�[�Y���Ȃ珈�����s��Ȃ�.
	if ( CSceneManager::GetIsPause() ) return;
	// �^�[�Q�b�g�����݂��Ȃ��Ȃ珈�����s��Ȃ�.
	if ( m_pTarget == nullptr )return;

	Rotate();

	// �^�[�Q�b�g�̃g�����X�t�H�[�����擾.
	STransform TrgTransform = m_pTarget->GetTransform();
	// �ߋ��̍��W�ƌ��݂̍��W�̍��������.(SubPosY�͍~����+,�㏸��-).
	const float SubPosY = m_OldTarPosY - TrgTransform.Position.y;

	// �J�����̍��W�̍X�V.
	m_RotX = Math::ToRadian( ( CONSTANT.ADJUST_ANGLE * m_Theta - CONSTANT.ADD_DELAY_ANGLE * SubPosY ) - CONSTANT.INIT_ROTATION );
	// �X����p�x��ݒ肷��.
	if (		m_RotX >= Math::ToRadian( CONSTANT.MOVABLE_MAX ) ) m_RotX = Math::ToRadian( CONSTANT.MOVABLE_MAX );
	else if (	m_RotX <= Math::ToRadian( CONSTANT.MOVABLE_MIN ) ) m_RotX = Math::ToRadian( CONSTANT.MOVABLE_MIN );
	m_RotX -= Math::ToRadian( CONSTANT.ADJUST_ANGLE );

	// �����_�̍X�V.
	m_LookPosition	= TrgTransform.Position;

	// 1F�O��Y���W�̍X�V.
	m_OldTarPosY	= TrgTransform.Position.y;

	// �ړ��ł��Ȃ��Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( m_IsMoveStop ) return;
	const float Distance = -15.0f;
	// �^�[�Q�b�g�̌������擾.
	const D3DXVECTOR3 TarVec = TrgTransform.GetRotationVector();

	D3DXVECTOR3* pPos = m_IsSwitching ? &m_EndPosition : &m_Transform.Position;
	pPos->x = m_LookPosition.x + Distance * sinf( m_RotX ) * TarVec.x;
	pPos->y = m_LookPosition.y + Distance * cosf( m_RotX );
	pPos->z = m_LookPosition.z + Distance * sinf( m_RotX ) * TarVec.z;

	MoveFromGazingCamera( m_EndPosition, DeltaTime );
}

//---------------------------.
// �����J��������؂�ւ�����ۂɌĂяo��.
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
// �J���������̊p�x�ɖ߂�����.
//---------------------------.
void CMainCamera::UpdateReturnRotation( float& RotTheta, const float Rot )
{
	// ���݂̊p�x�ɉ����Ċp�x��߂��������s��.
	if ( RotTheta >= Rot ) {
		RotTheta -= Math::ToRadian( m_ReturnValue );
		if ( RotTheta <= Rot ) InitRotation( Rot );
	}
	else {
		RotTheta += Math::ToRadian( m_ReturnValue );
		if ( RotTheta >= Rot ) InitRotation( Rot );
	}

	// ���t���[���߂��p�x�̑傫���𑝂₷.
	m_ReturnValue += Math::ToRadian( m_AddValue );
	// ���Z����l�𑝂₷.
	m_AddValue *= 1.1f;
}

//---------------------------.
// �J���������̊p�x�ɖ߂�����s������.
//---------------------------.
void CMainCamera::InitRotation(const float Rot)
{
	// ������Ԃɖ߂�.
	m_Theta					= Rot;
	m_IsReturnDefaultPos	= false;
	m_ReturnValue			= CONSTANT.INIT_RETURN_VALUE;
	m_AddValue				= CONSTANT.INIT_ADD_VALUE;
}

//---------------------------.
// R�X�e�B�b�N�ł̑��쏈��.
//---------------------------.
void CMainCamera::Rotate()
{
	// ���̊p�x�ɖ߂����Ƃ����u�Ԃ��擾����.
	const bool	OldIsReturnRot = m_IsReturnDefaultPos;
	// R�X�e�B�b�N�������ꂽ��,��]�p�x������������.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_RIGHT_THUMB ) )m_IsReturnDefaultPos = true;
	// R�X�e�B�b�N�̓��͂��󂯎��.
	const float RTrigger = CONSTANT.IS_REVERSE ? CXInput::GetRThumbY() : -CXInput::GetRThumbY();

	// �J���������̊p�x�ɖ߂��Ă��Ȃ��Ȃ�.
	if ( m_IsReturnDefaultPos ) {
		// ���̈ʒu�ɂ���R�X�e�B�b�N���X�����u�ԂȂ�.
		if ( m_OldRTrigger == 0.0f && RTrigger != 0.0f ) m_IsReturnDefaultPos = false;

		// �J���������̊p�x�ɖ߂�����.
		UpdateReturnRotation( m_Theta );
	}
	// �J���������̊p�x�ɖ߂��Ă��āAR�X�e�B�b�N���|��Ă��Ȃ��Ȃ�.
	else if ( m_IsCompleteReturnRStick ) {
		m_Theta -= RTrigger * 0.03f;

		float AdjustRotation = CONSTANT.INIT_ROTATION / CONSTANT.ADJUST_ANGLE;
		if ( m_Theta >= AdjustRotation + LIMIT_THETA ) m_Theta = AdjustRotation + LIMIT_THETA;
		else if ( m_Theta <= AdjustRotation - LIMIT_THETA ) m_Theta = AdjustRotation - LIMIT_THETA;
	}

	// R�X�e�B�b�N���X���Ă���Ȃ�.
	if ( RTrigger != 0.0f ) {
		// ���̊p�x�ɖ߂낤�Ƃ����u�ԂȂ�.
		if ( OldIsReturnRot && m_IsReturnDefaultPos == false ) {
			m_IsCompleteReturnRStick = false;
		}
	}
	// R�X�e�B�b�N�����̈ʒu�ɖ߂����u��.
	else if ( m_OldRTrigger != 0.0f ) {
		m_IsCompleteReturnRStick = true;
	}
	m_OldRTrigger = RTrigger;
}

//---------------------------.
// �����J��������؂�ւ�������̏���.
//---------------------------.
void CMainCamera::MoveFromGazingCamera( const D3DXVECTOR3 GoalPos, const float& DeltaTime )
{
	if ( m_IsSwitching == false )return;

	// �v���C���[����q�Ɍ������x�N�g�����v�Z����.
	D3DXVECTOR3 Vec = m_pTarget->GetPosition() - m_pCustomer->GetPosition() - D3DXVECTOR3( 0.0f, 2.0f, 0.0f );
	// �x�N�g���𐳋K������.
	D3DXVec3Normalize( &Vec, &Vec );
	// �J�����̈ʒu.
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
