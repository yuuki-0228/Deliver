#include "HeightGaugeUI.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Customer\Customer.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Employee\Employee.h"

namespace {
	enum enSpriteList :unsigned char {
		Meter,			// メーター.
		PlayerIcon,		// プレイヤーのアイコン.
		CharacterIcon,	// 店員,客のアイコン.

		Max
	};

	// 近くのお店の表示数の上限.
	constexpr int EMPLOYEE_MAX = 3;
}

CHeightGaugeUI::CHeightGaugeUI()
	: m_pPlayer					( nullptr )
	, m_pCustomer				( nullptr )
	, m_pSpriteList				( Max )
	, m_SpriteStateList			( Max )
	, m_EmployeeIconState		( EMPLOYEE_MAX )
	, m_EmployeeNum				( INIT_INT )
{
}

CHeightGaugeUI::~CHeightGaugeUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CHeightGaugeUI::Init()
{
	// 画像の取得.
	m_pSpriteList[Meter]			= CSpriteResource::GetSprite( "HeightGauge",				&m_SpriteStateList[Meter] );
	m_pSpriteList[PlayerIcon]		= CSpriteResource::GetSprite( "HeightGaugePlayerIcon",		&m_SpriteStateList[PlayerIcon] );
	m_pSpriteList[CharacterIcon]	= CSpriteResource::GetSprite( "HeightGaugeCharacterIcon",	&m_SpriteStateList[CharacterIcon] );

	// 座標の設定.
	const SSize& Size = m_pSpriteList[Meter]->GetSpriteState().Disp;
	m_SpriteStateList[Meter].Transform.Position = { Size.w / 2.0f + 5.0f, FWND_H - 5.0f, 0.0f };
	m_SpriteStateList[PlayerIcon].Transform.Position.x = m_SpriteStateList[Meter].Transform.Position.x + Size.w / 2.0f + 5.0f;
	m_SpriteStateList[CharacterIcon].Transform.Position.x = m_SpriteStateList[Meter].Transform.Position.x;

	// サイズの設定.
	m_SpriteStateList[CharacterIcon].Transform.Scale.y	= 20.0f;

	// お店の位置の情報の設定.
	for ( auto& e : m_EmployeeIconState ) e = m_SpriteStateList[CharacterIcon];
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CHeightGaugeUI::Update( const float& DeltaTime )
{
	// 高度計の情報の取得.
	const float HeightGaugeY = m_SpriteStateList[Meter].Transform.Position.y;
	const float HeightGaugeH = m_pSpriteList[Meter]->GetSpriteState().Disp.h;

	// プレイヤーの高度の位置の更新.
	m_SpriteStateList[PlayerIcon].Transform.Position.y = HeightGaugeY -
		HeightGaugeH * ( ( m_pPlayer->GetTransform().Position.y - CONSTANT.DEAD_DOWN_POS_Y ) / ( CONSTANT.DEAD_UP_POS_Y - CONSTANT.DEAD_DOWN_POS_Y ) );

	// 近くのお店の高度の更新.
	if ( m_pPlayer->GetIsFoodHave() == false ) {
		// 近くのお店を取得.
		std::vector<CEmployee*> EmployeeList = CDeliveryManager::GetLinkSortEmployee();
		const int ListSize = static_cast< int >( EmployeeList.size() );

		// 表示する個数の設定.
		if ( ListSize < EMPLOYEE_MAX )	m_EmployeeNum = ListSize;
		else							m_EmployeeNum = EMPLOYEE_MAX;

		// 近くのお店の高度の更新.
		for ( int i = 0; i < m_EmployeeNum; ++i ) {
			m_EmployeeIconState[i].Color				= Color4::RGBA( EmployeeList[i]->GetLinkColor(), 0.5f );
			m_EmployeeIconState[i].Transform.Position.y	= HeightGaugeY -
				HeightGaugeH * ( ( EmployeeList[i]->GetPosition().y - CONSTANT.DEAD_DOWN_POS_Y ) / ( CONSTANT.DEAD_UP_POS_Y - CONSTANT.DEAD_DOWN_POS_Y ) );
		}
		return;
	}

	// 渡しに行くお客さんの高度の更新.
	m_pCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( m_pCustomer == nullptr ) return;
	m_SpriteStateList[CharacterIcon].Color					= Color4::RGBA( m_pCustomer->GetLinkColor(), 0.5f );
	m_SpriteStateList[CharacterIcon].Transform.Position.y	= HeightGaugeY -
		HeightGaugeH * ( ( m_pCustomer->GetPosition().y - CONSTANT.DEAD_DOWN_POS_Y ) / ( CONSTANT.DEAD_UP_POS_Y - CONSTANT.DEAD_DOWN_POS_Y ) );
}

//---------------------------.
// 描画.
//---------------------------.
void CHeightGaugeUI::Render()
{
	if ( m_pPlayer == nullptr ) return;
	m_pSpriteList[Meter]->RenderUI( &m_SpriteStateList[Meter] );
	m_pSpriteList[PlayerIcon]->RenderUI( &m_SpriteStateList[PlayerIcon] );

	if ( m_pPlayer->GetIsFoodHave() == false ) {
		for ( int i = 0; i < m_EmployeeNum; ++i ) {
			m_pSpriteList[CharacterIcon]->RenderUI( &m_EmployeeIconState[i] );
		}
		return;
	}

	if ( m_pCustomer == nullptr ) return;
	m_pSpriteList[CharacterIcon]->RenderUI( &m_SpriteStateList[CharacterIcon] );
}
