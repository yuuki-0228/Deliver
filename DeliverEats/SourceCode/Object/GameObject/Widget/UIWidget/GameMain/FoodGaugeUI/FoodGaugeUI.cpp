#include "FoodGaugeUI.h"
#include "..\..\..\..\..\GameObject\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\..\GameObject\Actor\Character\DeliveryManager\Employee\Employee.h"
#include "..\..\..\..\..\GameObject\Actor\Character\DeliveryManager\Customer\Customer.h"

CFoodGaugeUI::CFoodGaugeUI()
	: m_ReceiveGauge			()
	, m_SendGauge				()
{
}

CFoodGaugeUI::~CFoodGaugeUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CFoodGaugeUI::Init()
{
	m_ReceiveGauge.Value	= INIT_FLOAT;
	m_ReceiveGauge.Alpha	= Color::ALPHA_MIN;
	m_SendGauge.Value		= INIT_FLOAT;
	m_SendGauge.Alpha		= Color::ALPHA_MIN;

	m_pSpriteState["FoodGauge"][0]->RenderArea.z = INIT_FLOAT;
	SetReceiveDisp( false );
	SetSendDisp( false );
	return true;
}
//---------------------------.
// 更新.
//---------------------------.
void CFoodGaugeUI::Update( const float& DeltaTime )
{
	CEmployee* pEmployee = CDeliveryManager::GetGetFoodEmployee();
	CCustomer* pCustomer = CDeliveryManager::GetGoHandingCustomer();

	if ( pEmployee == nullptr && pCustomer == nullptr ) {
		m_pSpriteState["FoodGauge"][0]->IsDisp			= false;
		m_pSpriteState["FoodGaugeBack"][0]->IsDisp		= false;
		m_pSpriteState["FoodGaugeFrame"][0]->IsDisp		= false;
	}
	else {
		m_pSpriteState["FoodGauge"][0]->Color.w			= Color::ALPHA_MIN;
		m_pSpriteState["FoodGaugeBack"][0]->Color.w		= Color::ALPHA_MIN;
		m_pSpriteState["FoodGaugeFrame"][0]->Color.w	= Color::ALPHA_MIN;
		m_pSpriteState["FoodGauge"][0]->IsDisp			= true;
		m_pSpriteState["FoodGaugeBack"][0]->IsDisp		= true;
		m_pSpriteState["FoodGaugeFrame"][0]->IsDisp		= true;
	}

	// 受け渡しゲージの更新.
	SendGauge( DeltaTime, pCustomer );
	// 受け取りゲージの更新.
	ReceiveGauge( DeltaTime, pEmployee );
}

//---------------------------.
// 描画.
//---------------------------.
void CFoodGaugeUI::Render()
{
}

//---------------------------.
// 受け取りゲージの処理.
//---------------------------.
void CFoodGaugeUI::ReceiveGauge( const float& DeltaTime, CEmployee* pEmployee )
{
	// 対象がいないなら各種設定を初期化して,以下の処理は行わない.
	if ( pEmployee == nullptr ) {
		m_ReceiveGauge.Value							= 0.0f;
		m_ReceiveGauge.Alpha							= Color::ALPHA_MIN;
		m_pSpriteState["TakeFoodString"][0]->Color.w	= Color::ALPHA_MIN;
		m_pSpriteState["TakeFoodString"][0]->IsDisp		= false;
		return;
	}
	// 対象がいるなら受け取りゲージを表示する.
	else {
		m_pSpriteState["TakeFoodString"][0]->IsDisp		= true;;
	}

	// 画像のサイズを取得する.
	const SSize& Size = m_pSpriteList["FoodGauge"][0]->GetSpriteState().Base;
	// 画像のサイズに合わせてゲージを増やす.
	m_ReceiveGauge.Value = pEmployee->GetGauge() * Size.w;

	// アルファ値の設定.
	m_ReceiveGauge.Alpha = pEmployee->GetDeleteTimeCnt() / CConstant::GetDeliveryCharacter().GAUGE_DELETE_TIME;
	if ( m_ReceiveGauge.Alpha == Color::ALPHA_MIN )	SetReceiveDisp( false );
	else											SetReceiveAlpha( m_ReceiveGauge.Alpha );

	// 画像の座標を取得.
	const D3DXVECTOR3& Position		= m_pSpriteState["FoodGauge"][0]->Transform.Position;
	// 描画範囲を設定.
	const D3DXVECTOR4& RenderArea	= { Position.x, Position.y, m_ReceiveGauge.Value, Size.h };
	m_pSpriteState["FoodGauge"][0]->RenderArea = RenderArea;
}

//---------------------------.
// 受け渡しゲージの処理.
//---------------------------.
void CFoodGaugeUI::SendGauge( const float& DeltaTime, CCustomer* pCustomer )
{
	// 対象がいないなら各種設定を初期化して,以下の処理は行わない.
	if ( pCustomer == nullptr ) {
		m_SendGauge.Value								= 0.0f;
		m_SendGauge.Alpha								= Color::ALPHA_MIN;
		m_pSpriteState["SendFoodString"][0]->Color.w	= Color::ALPHA_MIN;
		m_pSpriteState["SendFoodString"][0]->IsDisp		= false;
		return;
	}
	// 対象がいるなら受け渡しゲージを表示する.
	else {
		m_pSpriteState["SendFoodString"][0]->IsDisp		= true;
	}

	// 画像のサイズを取得する.
	const SSize& Size = m_pSpriteList["FoodGauge"][0]->GetSpriteState().Base;
	// 画像のサイズに合わせてゲージを増やす.
	m_SendGauge.Value = pCustomer->GetGauge() * Size.w;
	 
	// アルファ値の設定.
	m_SendGauge.Alpha = pCustomer->GetDeleteTimeCnt() / CConstant::GetDeliveryCharacter().GAUGE_DELETE_TIME;
	if ( m_SendGauge.Alpha == Color::ALPHA_MIN )	SetSendDisp( false );
	else											SetSendAlpha( m_SendGauge.Alpha );

	// 画像の座標を取得.
	const D3DXVECTOR3& Position		= m_pSpriteState["FoodGauge"][0]->Transform.Position;
	// 描画範囲を設定.
	const D3DXVECTOR4& RenderArea	= { Position.x, Position.y, m_SendGauge.Value, Size.h };
	m_pSpriteState["FoodGauge"][0]->RenderArea = RenderArea;
}

//---------------------------.
// 表示/非表示切り替え.
//---------------------------.
void CFoodGaugeUI::SetReceiveDisp( bool Flag )
{
	m_pSpriteState["FoodGauge"][0]->IsDisp			= Flag;
	m_pSpriteState["FoodGaugeBack"][0]->IsDisp		= Flag;
	m_pSpriteState["FoodGaugeFrame"][0]->IsDisp		= Flag;
	m_pSpriteState["TakeFoodString"][0]->IsDisp		= Flag;
}

//---------------------------.
// Alphaの一括管理.
//---------------------------.
void CFoodGaugeUI::SetReceiveAlpha( float Alpha )
{
	m_pSpriteState["FoodGauge"][0]->Color.w			= Alpha;
	m_pSpriteState["FoodGaugeBack"][0]->Color.w		= Alpha;
	m_pSpriteState["FoodGaugeFrame"][0]->Color.w	= Alpha;
	m_pSpriteState["TakeFoodString"][0]->Color.w	= Alpha;
}

//---------------------------.
// 表示/非表示切り替え.
//---------------------------.
void CFoodGaugeUI::SetSendDisp( bool Flag )
{
	m_pSpriteState["FoodGauge"][0]->IsDisp			= Flag;
	m_pSpriteState["FoodGaugeBack"][0]->IsDisp		= Flag;
	m_pSpriteState["FoodGaugeFrame"][0]->IsDisp		= Flag;
	m_pSpriteState["SendFoodString"][0]->IsDisp		= Flag;
}

//---------------------------.
// Alphaの一括管理.
//---------------------------.
void CFoodGaugeUI::SetSendAlpha( float Alpha )
{
	m_pSpriteState["FoodGauge"][0]->Color.w			= Alpha;
	m_pSpriteState["FoodGaugeBack"][0]->Color.w		= Alpha;
	m_pSpriteState["FoodGaugeFrame"][0]->Color.w	= Alpha;
	m_pSpriteState["SendFoodString"][0]->Color.w	= Alpha;
}
