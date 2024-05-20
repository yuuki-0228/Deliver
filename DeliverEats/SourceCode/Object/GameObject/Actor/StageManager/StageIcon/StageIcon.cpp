#include "StageIcon.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace {
	// �A�C�R���̔w�i�̊J�n����y���W��UV�̈ʒu.
	constexpr float ICON_BACK_START_UV_Y = 0.5f;
}

CStageIcon::CStageIcon()
	: m_Transform		()
	, m_pIcon			( nullptr )
	, m_pIconBack		( nullptr )
	, m_IconState		()
	, m_IconFrameState	()
	, m_IconBackState	()
{
}

CStageIcon::~CStageIcon()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CStageIcon::InitIcon( const EObjectTag Tag )
{
	// �摜�̐ݒ�.
	m_pIcon		= CSpriteResource::GetSprite( "StageIcon",		&m_IconState );
	m_pIconBack	= CSpriteResource::GetSprite( "StageIconGauge",	&m_IconBackState );
	m_IconState.AnimState.IsSetAnimNumber	= true;
	m_IconFrameState						= m_IconState;
	m_IconFrameState.AnimState.AnimNumber	= 4;
	m_IconBackState.AnimState.UV.y			= ICON_BACK_START_UV_Y;

	// �}�X�N�摜�̐ݒ�.
	m_IconBackState.pMaskTexture = CSpriteResource::GetSprite( "MapIconMaskDown" )->GetTexture();

	// �^�O�ɉ����Č����ڂ�ύX.
	SetTag( Tag );

	// �f�B�U�������g�p����.
	m_pIcon->SetDither( true );
	m_pIconBack->SetDither( true );

	// �F�̐ݒ�.
	if ( Tag == EObjectTag::PlayerIsland ) {
		m_IconState.Color		= Color4::Dodgerblue;
		m_IconBackState.Color	= Color4::Dodgerblue;
	}
	return true;
}

//---------------------------.
// �A�C�R���̍X�V.
//---------------------------.
void CStageIcon::IconUpdate( const CLinking::Handle Handle )
{
	// �A�C�R���̐F�̍X�V.
	m_IconState.Color			= Color4::RGBA( CLinking::GetLinkColor( Handle ) );
	m_IconBackState.Color		= Color4::RGBA( CLinking::GetLinkColor( Handle ) );

	// �c��̕R�Â��̎��Ԃ̃Q�[�W�̍X�V
	const float Ratio = CLinking::GetLinkEndTimeCnt( Handle ) / CLinking::GetLinkEndTime( Handle );
	m_IconBackState.AnimState.UV.y = ICON_BACK_START_UV_Y - ICON_BACK_START_UV_Y * Ratio;
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//---------------------------.
void CStageIcon::Sprite3DRender_A()
{
	// �g�����X�t�H�[���̍X�V.
	m_IconBackState.Transform	= m_Transform;
	m_IconState.Transform		= m_Transform;
	m_IconFrameState.Transform	= m_Transform;

	// �A�C�R���̕`��.
	m_pIcon->Render3D( &m_IconFrameState,		true );
	m_pIcon->Render3D( &m_IconState,			true );
	m_pIconBack->Render3D( &m_IconBackState,	true );
	m_pIconBack->Render3D( &m_IconBackState,	true );
}

//---------------------------.
// �^�O�̐ݒ�.
//---------------------------.
void CStageIcon::SetTag( const EObjectTag Tag )
{
	switch ( Tag ) {
	case EObjectTag::Customer:			m_IconState.AnimState.AnimNumber = 0;	break;
	case EObjectTag::Employee:			m_IconState.AnimState.AnimNumber = 1;	break;
	case EObjectTag::GoHandingCustomer: m_IconState.AnimState.AnimNumber = 2;	break;
	case EObjectTag::PlayerIsland:		m_IconState.AnimState.AnimNumber = 3;	break;
	default:																	break;
	}
}
