#include "DeliverCustomerIconUI.h"
#include "..\..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\..\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\..\Actor\Character\DeliveryManager\Customer\Customer.h"

namespace {
	// アイコンのローカル座標.
	constexpr float ICON_POS[3] = { 85.0f, 85.0f, 0 };
}

CDeliverCustomerIconUI::CDeliverCustomerIconUI()
	: m_pPlayer				( nullptr )
	, m_pIcon				( nullptr )
	, m_pBack				( nullptr )
	, m_pGazingIcon			( nullptr )
	, m_IconState			()
	, m_BackState			()
	, m_GazingIconState		()
	, m_Color				( INIT_FLOAT3 )
{
}

CDeliverCustomerIconUI::~CDeliverCustomerIconUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CDeliverCustomerIconUI::Init()
{
	// 画像の取得.
	m_pIcon			= CSpriteResource::GetSprite( "CustomerIcon",		&m_IconState );
	m_pBack			= CSpriteResource::GetSprite( "CustomerIconBack",	&m_BackState );
	m_pGazingIcon	= CSpriteResource::GetSprite( "GazingIcon" );
	m_pGazingIcon->InitSceneSpriteState( "Icon", 0 );
	m_GazingIconState = m_pGazingIcon->GetRenderState();

	// 座標の設定.
	m_BackState.IsDisp						= false;
	m_IconState.IsDisp						= false;
	m_BackState.Transform.Position			= m_Transform.Position - D3DXPOSITION3( ICON_POS );
	m_IconState.Transform.Position			= m_BackState.Transform.Position;
	m_GazingIconState.Transform.Position	= m_BackState.Transform.Position;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CDeliverCustomerIconUI::Update( const float& DeltaTime )
{
	// 注視中なら注視中アイコンの表示/非表示.
	if ( m_pPlayer->GetIsActiveGazingCam() )	m_GazingIconState.IsDisp = true;
	else										m_GazingIconState.IsDisp = false;

	// お客さんのアイコンの表示/非表示.
	const CCustomer* pCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( pCustomer == nullptr ) {
		m_BackState.IsDisp		= false;
		m_IconState.IsDisp		= false;
		return;
	}
	m_BackState.IsDisp			= true;
	m_IconState.IsDisp			= true;

	// お客さんのアイコンの色の設定.
	m_Color = pCustomer->GetLinkColor();
}

//---------------------------.
// 描画.
//---------------------------.
void CDeliverCustomerIconUI::Render()
{
	m_BackState.Color	= Color4::RGBA( m_Color, 0.5f );
	m_pBack->RenderUI( &m_BackState );
	m_BackState.Color	= Color4::RGBA( Color::Wheat, 0.5f );
	m_pBack->RenderUI( &m_BackState );
	m_IconState.Color	= Color4::RGBA( m_Color );
	m_pIcon->RenderUI( &m_IconState );
	m_IconState.Color	= Color4::RGBA( Color::Black, 0.1f );
	m_pIcon->RenderUI( &m_IconState );
	m_pGazingIcon->RenderUI( &m_GazingIconState );
}
