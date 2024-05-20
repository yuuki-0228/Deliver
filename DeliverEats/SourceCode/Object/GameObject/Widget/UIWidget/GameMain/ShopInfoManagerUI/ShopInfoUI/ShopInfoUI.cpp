#include "ShopInfoUI.h"
#include "..\..\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\..\Actor\Character\DeliveryManager\Employee\Employee.h"
#include "..\..\..\..\..\..\..\Utility\Easing\Easing.h"

namespace {
	constexpr float ICON_BACK_START_UV_Y	= 0.5f; // �A�C�R���̊J�n����UV��Y���W.
	constexpr float EASING_MAX				= 1.0f; // �C�[�W���O�̏I���̏��.

	// �t�H���gNo.
	enum enFontNo : unsigned char {
		Money,		// �l�i.
		Distance,	// ����.

		Font_Max
	} typedef EFontNo;
}

CShopInfoUI::CShopInfoUI()
	: m_pPlayer			( nullptr )
	, m_pEmployee		( nullptr )
	, m_pGauge			( nullptr )
	, m_pIcon			( nullptr )
	, m_pFrame			( nullptr )
	, m_pFont			( nullptr )
	, m_GaugeState		()
	, m_IconState		()
	, m_FrameState		()
	, m_FontStateBase	()
	, m_FontState		( Font_Max )
	, m_Handle			( NO_LINK_HANDLE )
	, m_Color			( INIT_FLOAT4 )
	, m_AnimTimeCnt		( INIT_FLOAT )
	, m_IsDisp			( false )
	, m_IsOpen			( false )
	, m_IsClose			( false )
{
}

CShopInfoUI::~CShopInfoUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CShopInfoUI::Init()
{
	m_pGauge	= CSpriteResource::GetSprite( "ShopInfoGauge",	&m_GaugeState );
	m_pIcon		= CSpriteResource::GetSprite( "ShopInfoIcon",	&m_IconState );
	m_pFrame	= CSpriteResource::GetSprite( "ShopInfoFrame",	&m_FrameState );
	m_pFont		= CFontResource::GetFont( "�R�[�|���[�g�E���S ver2 Medium", &m_FontStateBase );
	m_FontStateBase.TextAlign	= ETextAlign::Center;
	m_FontStateBase.Color		= Color4::Black;

	// �X�}�z�̏����擾.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize = CMiniMapUI::GetScreenSize();

	// �`��͈͂̐ݒ�.
	m_GaugeState.RenderArea		= { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	m_IconState.RenderArea		= { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	m_FrameState.RenderArea		= { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	m_FontStateBase.RenderArea	= { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };

	// �}�X�N�̐ݒ�.
	m_GaugeState.pMaskTexture = CSpriteResource::GetSprite( "ShopInfoMask" )->GetTexture();

	// �e�̐ݒ�.
	m_GaugeState.Transform.AttachParent(	&m_FrameState.Transform );
	m_IconState.Transform.AttachParent(		&m_FrameState.Transform );
	m_FontStateBase.Transform.AttachParent( &m_FrameState.Transform );

	// ���W�̐ݒ�.
	const SSize& Size = m_pFrame->GetSpriteState().Disp;
	m_FontStateBase.Transform.Position.x	= Size.w / 2.0f;
	m_GaugeState.Transform.Position			= CONSTANT.ICON_POSITION;
	m_IconState.Transform.Position			= CONSTANT.ICON_POSITION;

	// �t�H���g�̈ʒu�̐ݒ�.
	for ( auto& f : m_FontState ) f = m_FontStateBase;
	m_FontState[Money].Transform.Position.y		= CONSTANT.MONEY_FONT_POSITION_Y;
	m_FontState[Money].Transform.Scale			= CONSTANT.FONT_SCALE;
	m_FontState[Distance].Transform.Position.y	= CONSTANT.DISTANCE_FONT_POSITION_Y;
	m_FontState[Distance].Transform.Scale		= CONSTANT.FONT_SCALE;

	// ��\���ɂ���.
	m_IsDisp = false;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CShopInfoUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	if ( m_IsDisp == false ) return;

	OpenUpdate();	// �J������̍X�V.
	CloseUpdate();	// ���铮��̍X�V.
	InfoUpdate();	// �ڍ׏��̍X�V.
}

//---------------------------.
// �`��.
//---------------------------.
void CShopInfoUI::Render()
{
	if ( m_IsDisp == false ) return;

	m_GaugeState.Color	= m_Color;
	m_pGauge->RenderUI(	&m_GaugeState );
	m_GaugeState.Color	= Color4::RGBA( Color::White, 0.5f );
	m_pGauge->RenderUI(	&m_GaugeState );
	m_IconState.Color	= m_Color;
	m_pIcon->RenderUI(	&m_IconState );
	m_IconState.Color	= Color4::RGBA( Color::Black, 0.1f );
	m_pIcon->RenderUI(	&m_IconState );
	m_pFrame->RenderUI(	&m_FrameState );
	for( auto& f : m_FontState ) m_pFont->RenderUI( &f );
}

//---------------------------.
// �\��.
//---------------------------.
void CShopInfoUI::Open( int No, CLinking::Handle Handle )
{
	// �n���h���̕ۑ�.
	m_Handle = Handle;

	// �X���̎擾.
	m_pEmployee = CLinking::GetEmployee( m_Handle );

	// �F�̐ݒ�.
	m_Color = Color4::RGBA( CLinking::GetLinkColor( m_Handle ) );

	// �Œ�����炦��l�i�̐ݒ�.
	m_FontState[Money].Text = std::to_string( m_pEmployee->GetPizzaMoney() );
	for ( int i = static_cast<int>( m_FontState[Money].Text.length() ) - 3; i > 0; i -= 3 ) {
		m_FontState[Money].Text.insert( i, "," );
	}
	m_FontState[Money].Text = "��" + m_FontState[Money].Text;

	// �X�}�z�̏����擾.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize = CMiniMapUI::GetScreenSize();

	// �ʒu���擾.
	const SSize& FrameSize = m_pFrame->GetSpriteState().Disp;
	m_FrameState.Transform.Position.y = ScreenPos.y + ScreenSize.h / 2.0f;
	switch ( No ) {
	case 0: m_FrameState.Transform.Position.x = ScreenPos.x - ScreenSize.w / 3.0f - FrameSize.w / 2.0f;	break;
	case 1:	m_FrameState.Transform.Position.x = ScreenPos.x - FrameSize.w  / 2.0f;						break;
	case 2: m_FrameState.Transform.Position.x = ScreenPos.x + ScreenSize.w / 3.0f - FrameSize.w / 2.0f;	break;
	default: break;
	}

	// �\��.
	m_IsDisp	= true;
	m_IsOpen	= true;
	m_IsClose	= false;
}

//---------------------------.
// ��\��.
//---------------------------.
void CShopInfoUI::Close()
{
	m_IsClose	= true;
	m_IsOpen	= false;
}

//---------------------------.
// �ڍ׏��̍X�V.
//---------------------------.
void CShopInfoUI::InfoUpdate()
{
	if ( m_IsDisp == false								) return;
	if ( m_IsClose										) return;
	if ( CLinking::GetIsUseHandle( m_Handle ) == false	) return;

	// �A�C�R���̃Q�[�W�̍X�V.
	const float Ratio = CLinking::GetLinkEndTimeCnt( m_Handle ) / CLinking::GetLinkEndTime( m_Handle );
	m_GaugeState.AnimState.UV.y = ICON_BACK_START_UV_Y - ICON_BACK_START_UV_Y * Ratio;
	
	// �����̍X�V.
	const D3DXPOSITION3& EPos			= m_pEmployee->GetPosition();
	const D3DXPOSITION3& PPos			= m_pPlayer->GetPosition();
	const D3DXVECTOR3&	 Vector			= EPos - PPos;
	int 				 ShopDistance	= static_cast<int>( D3DXVec3Length( &Vector ) ) - CONSTANT.SHOP_DISTANCE_ADJUS;
	if ( ShopDistance < 0 ) ShopDistance = 0;
	m_FontState[Distance].Text = std::to_string( ShopDistance ) + "m";
}

//---------------------------.
// �J������̍X�V.
//---------------------------.
void CShopInfoUI::OpenUpdate()
{
	if ( m_IsDisp	== false	) return;
	if ( m_IsOpen	== false	) return;
	if ( m_IsClose				) return;

	// �X�}�z�̏����擾.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize = CMiniMapUI::GetScreenSize();
	const SSize&		 FrameSize	= m_pFrame->GetSpriteState().Disp;

	// �ʒu���X�V.
	m_AnimTimeCnt += m_DeltaTime;
	const float NowPoint = CEasing::OutQuint( m_AnimTimeCnt, EASING_MAX );
	m_FrameState.Transform.Position.y = ScreenPos.y + ScreenSize.h / 2.0f - ( FrameSize.h + CONSTANT.DISP_POSITION_Y ) * NowPoint;

	if ( m_AnimTimeCnt > EASING_MAX ) {
		m_AnimTimeCnt	= 0.0f;
		m_IsOpen		= false;
	}
}

//---------------------------.
// ���铮��̍X�V.
//---------------------------.
void CShopInfoUI::CloseUpdate()
{
	if ( m_IsDisp	== false	) return;
	if ( m_IsClose	== false	) return;
	if ( m_IsOpen				) return;

	// �X�}�z�̏����擾.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize = CMiniMapUI::GetScreenSize();
	const SSize&		 FrameSize	= m_pFrame->GetSpriteState().Disp;

	// �ʒu���X�V.
	m_AnimTimeCnt += m_DeltaTime;
	const float NowPoint = 1.0f - CEasing::InBack( m_AnimTimeCnt, EASING_MAX, CONSTANT.CLOSE_OVER_MOVE_Y );
	m_FrameState.Transform.Position.y = ScreenPos.y + ScreenSize.h / 2.0f - ( FrameSize.h + CONSTANT.DISP_POSITION_Y ) * NowPoint;

	if ( m_AnimTimeCnt > EASING_MAX ) {
		m_AnimTimeCnt	= 0.0f;
		m_IsClose		= false;
		m_IsDisp		= false;
	}
}
