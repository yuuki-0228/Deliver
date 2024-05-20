#include "WarningUI.h"
#include "..\NotificationUI\NotificationUI.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr int	WARNING_NUM	= 4;										// �x��UI�̐�.
	constexpr int	LEFT		= static_cast<int>( EDirection::Left	);	// ����.
	constexpr int	RIGHT		= static_cast<int>( EDirection::Right	);	// �E��.
	constexpr int	UP			= static_cast<int>( EDirection::Up		);	// �㑤.
	constexpr int	DOWN		= static_cast<int>( EDirection::Down	);	// ����.
	constexpr char	WIND_SE[]	= "StageOutWind";							// �X�e�[�W�O��SE
}

CWarningUI::CWarningUI()
	: m_pPlayer				( nullptr )
	, m_IsUpPushMessage		( false )
	, m_IsDownPushMessage	( false )
	, m_IsRadiusPushMessage	( false )
	, m_SEVolume			( INIT_FLOAT )
{
}

CWarningUI::~CWarningUI()
{
	CSoundManager::StopSE( WIND_SE, true );
}

//---------------------------.
// ������.
//---------------------------.
bool CWarningUI::Init()
{
	for ( int i = 0; i < WARNING_NUM; ++i ) {
		m_pSpriteState["WarningFrame"][i]->IsDisp	= false;
		m_pSpriteState["Warning"][i]->IsDisp		= false;
	}
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CWarningUI::Update( const float& DeltaTime )
{
	if ( m_pPlayer == nullptr ) return;

	// �x�����\���ɂ���.
	bool IsDisp = false;
	for ( int i = 0; i < WARNING_NUM; ++i ) {
		m_pSpriteState["WarningFrame"][i]->IsDisp	= false;
		m_pSpriteState["Warning"][i]->IsDisp		= false;
	}

	// �v���C���[�������Y���W�ȉ��ɂ���ꍇ.
	const D3DXPOSITION3& PlayerPos = m_pPlayer->GetPosition();
	if ( PlayerPos.y <= CONSTANT.DEAD_DOWN_POS_Y + CONSTANT.WARNING_SIZE ) {
		IsDisp											= true;
		m_pSpriteState["Warning"][DOWN]->IsDisp			= true;
		m_pSpriteState["WarningFrame"][DOWN]->IsDisp	= true;
		m_pSpriteState["WarningFrame"][DOWN]->Color.w	= ( CONSTANT.DEAD_DOWN_POS_Y + CONSTANT.WARNING_SIZE - PlayerPos.y ) / CONSTANT.WARNING_SIZE;

		// �v���C���[�ɒ�R����͂�������.
		m_pPlayer->PushVector( D3DXVECTOR3( Math::Y_VECTOR ) * CONSTANT.POWER_TO_RESIST );

		// �ʒm�𑗂�.
		if ( m_IsDownPushMessage == false ) {
			CNotificationUI::PushMessage( EMessageType::Warning, CONSTANT.DOWN_WARNING_MESSAGE );
		}
		m_IsDownPushMessage = true;
	}
	else m_IsDownPushMessage = false;

	// �v���C���[�������Y���W�ȏ�ɂ���ꍇ.
	if ( PlayerPos.y >= CONSTANT.DEAD_UP_POS_Y - CONSTANT.WARNING_SIZE * 2.0f ) {
		IsDisp											= true;
		m_pSpriteState["Warning"][UP]->IsDisp			= true;
		m_pSpriteState["WarningFrame"][UP]->IsDisp		= true;
		m_pSpriteState["WarningFrame"][UP]->Color.w		= -( CONSTANT.DEAD_UP_POS_Y - CONSTANT.WARNING_SIZE * 1.5f - PlayerPos.y ) / CONSTANT.WARNING_SIZE * 1.5f;

		// �v���C���[�ɒ�R����͂�������.
		m_pPlayer->PushVector( D3DXVECTOR3( Math::_Y_VECTOR ) * CONSTANT.POWER_TO_RESIST );

		// �ʒm�𑗂�.
		if ( m_IsUpPushMessage == false ) {
			CNotificationUI::PushMessage( EMessageType::Warning, CONSTANT.UP_WARNING_MESSAGE );
		}
		m_IsUpPushMessage = true;
	}
	else m_IsUpPushMessage = false;

	// �v���C���[������̔��a�ȏ�ɂ���ꍇ.
	const float Distance = sqrtf( PlayerPos.x * PlayerPos.x + PlayerPos.z * PlayerPos.z );
	if ( Distance >= CONSTANT.DEAD_RADIUS - CONSTANT.WARNING_SIZE * 2.0f ) {
		IsDisp = true;
		for ( int i = 0; i < WARNING_NUM; ++i ) {
			m_pSpriteState["Warning"][i]->IsDisp		= true;
			m_pSpriteState["WarningFrame"][i]->IsDisp	= true;
			m_pSpriteState["WarningFrame"][i]->Color.w	= -( CONSTANT.DEAD_RADIUS - CONSTANT.WARNING_SIZE * 2.0f - Distance ) / CONSTANT.DEAD_RADIUS * 2.0f;
		}

		// �v���C���[�ɒ�R����͂�������.
		D3DXVECTOR3 Vector = -PlayerPos;
		D3DXVec3Normalize( &Vector, &Vector );
		m_pPlayer->PushVector( Vector * CONSTANT.POWER_TO_RESIST );

		// �ʒm�𑗂�.
		if ( m_IsRadiusPushMessage == false ) {
			CNotificationUI::PushMessage( EMessageType::Warning, CONSTANT.RADIUS_WARNING_MESSAGE );
		}
		m_IsRadiusPushMessage = true;
	}
	else m_IsRadiusPushMessage = false;

	// SE�̍Đ�.
	if ( IsDisp ) {
		// �Đ�.
		if ( m_SEVolume < 1.0f ) {
			m_SEVolume += CONSTANT.ADD_SE_VOLUME * DeltaTime;
			if ( m_SEVolume >= 1.0f ) m_SEVolume = 1.0f;
			CSoundManager::SetSEVolume( WIND_SE, m_SEVolume );
		}
		CSoundManager::PlaySELoop( WIND_SE );
	}
	else {
		// ��~.
		if ( m_SEVolume > 0.0f ) {
			m_SEVolume -= CONSTANT.ADD_SE_VOLUME * DeltaTime;
			if ( m_SEVolume <= 0.0f ) {
				m_SEVolume = 0.0f;
				CSoundManager::StopSE( WIND_SE );
			}
			CSoundManager::SetSEVolume( WIND_SE, m_SEVolume );
		}
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CWarningUI::Render()
{
}
