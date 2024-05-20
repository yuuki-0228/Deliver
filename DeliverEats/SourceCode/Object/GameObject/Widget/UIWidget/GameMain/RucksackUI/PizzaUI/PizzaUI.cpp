#include "PizzaUI.h"
#include "..\..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\..\..\..\Utility\Random\Random.h"
#include "..\..\..\..\..\..\..\Resource\PizzaResource\PizzaResource.h"

namespace {
	constexpr float PIZZA_SCALE_START_MOVE	= 25.0f;	// ピザの縮小を開始させる移動量.
	constexpr float PIZZA_MOVE				= 34.0f;	// ピザの移動量.
	constexpr float INGREDIENTS_MOVE		= 85.0f;	// ピザの具材の移動できる移動量(ピザの中央からの半径).
	constexpr float PIZZA_SCALE				= 0.1f;		// ピザの縮小する量.
	constexpr float GAUGE_MAX				= 1.0f;		// ゲージの最大.
}

CPizzaUI::CPizzaUI()
	: m_pPlayer					( nullptr )
	, m_pPizzaBase				( nullptr )
	, m_pSauce					( nullptr )
	, m_pIngredients			( nullptr )
	, m_pIngredientSauce		( nullptr )
	, m_PizzaBaseState			()
	, m_IngredientsStateBase	()
	, m_SauceStateBase			()
	, m_IngredientSauceState	()
	, m_SauceStateList			()
	, m_CheeseStateList			()
	, m_IngredientsStateList	()
	, m_pSauceMaskList			( 8 )
	, m_PizzaBasePos			( INIT_FLOAT3 )
{
}

CPizzaUI::~CPizzaUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CPizzaUI::Init()
{
	// 画像の取得.
	m_pPizzaBase		= CSpriteResource::GetSprite( "PizzaBase",				&m_PizzaBaseState );
	m_pIngredients		= CSpriteResource::GetSprite( "PizzaIngredient",		&m_IngredientsStateBase );
	m_pIngredientSauce	= CSpriteResource::GetSprite( "PizzaIngredientSauce",	&m_IngredientSauceState );
	m_pSauce			= CSpriteResource::GetSprite( "PizzaSauce",				&m_SauceStateBase );

	// 情報の設定.
	m_PizzaBaseState.AnimState.IsSetAnimNumber			= true;
	m_IngredientsStateBase.AnimState.IsSetAnimNumber	= true;
	m_IngredientSauceState.AnimState.IsSetAnimNumber	= true;
	m_SauceStateBase.AnimState.IsSetAnimNumber			= true;

	// 親の設定.
	m_IngredientsStateBase.Transform.AddParent( &m_PizzaBaseState.Transform );
	m_IngredientSauceState.Transform.AttachParent( &m_PizzaBaseState.Transform );
	m_SauceStateBase.Transform.AttachParent( &m_PizzaBaseState.Transform );

	// 座標の設定.
	m_PizzaBasePos								= m_Transform.Position;
	m_IngredientsStateBase.Transform.Position	= INIT_FLOAT3;
	m_SauceStateBase.Transform.Position			= INIT_FLOAT3;

	// マスク画像の保存.
	for( int i = 0; i < 8; ++i ){
		m_pSauceMaskList[i].resize( 4 );
		for ( int No = 0; No < 4; ++No ) {
			m_pSauceMaskList[i][No] = CSpriteResource::GetSprite( "PizzaSauceMask_" + std::to_string( i ) + std::to_string( No ) )->GetTexture();
		}
	}
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CPizzaUI::Update( const float& DeltaTime )
{
	SettingPizza();		// ピザの設定.
	DeletePizza();		// ピザの削除.
	PizzaMoveUpdate();	// ピザ移動.
	PizzaCrashUpdate();	// ピザをぐちゃぐちゃにしていく.
}

//---------------------------.
// 描画.
//---------------------------.
void CPizzaUI::Render()
{
	if ( m_PizzaBaseState.IsDisp == false ) return;

	m_pPizzaBase->RenderUI( &m_PizzaBaseState );
	for ( auto& s : m_SauceStateList		) m_pSauce->RenderUI( &s );
	for ( auto& c : m_CheeseStateList		) m_pSauce->RenderUI( &c );
	for ( auto& i : m_IngredientsStateList	) {
		if ( i.AnimState.AnimNumber < 0 ) {
			m_IngredientSauceState.Transform			= i.Transform;
			m_IngredientSauceState.AnimState.AnimNumber = std::abs( i.AnimState.AnimNumber ) - 1;
			m_pIngredientSauce->RenderUI( &m_IngredientSauceState );
			continue;
		}
		m_pIngredients->RenderUI( &i );
	}
}

//---------------------------.
// ピザの設定.
//---------------------------.
void CPizzaUI::SettingPizza()
{
	if ( m_PizzaBaseState.IsDisp			 ) return;
	if ( m_pPlayer->GetIsFoodHave() == false ) return;

	// ランダムでピザを選ぶ.
	const CPizzaResource::Pizza_List&	PizzaList	= CPizzaResource::GetPizzaList();
	const std::string&					PizzaName	= CRandom::GetRand( PizzaList );
	json								Data		= CPizzaResource::GetPizza( PizzaName );

	// ピザの生地の設定.
	m_PizzaBaseState.IsDisp					= true;
	m_PizzaBaseState.AnimState.AnimNumber	= 1;
	m_PizzaBaseState.Transform.Position		= m_PizzaBasePos;
	m_PizzaBaseState.Transform.Scale		= STransform::NORMAL_SCALE_VEC3;

	// ソースの設定.
	const int SSize = static_cast<int>( Data["Sauce"].size() );
	m_SauceStateList.resize( SSize, m_SauceStateBase );
	for ( int SauceNo = 0; SauceNo < SSize; ++SauceNo ) {
		json SauceData = Data["Sauce"][SauceNo];
		m_SauceStateList[SauceNo].AnimState.AnimNumber				= SauceData["Type"];
		m_SauceStateList[SauceNo].pMaskTexture						= m_pSauceMaskList[SauceData["MaskType"]][SauceData["MaskNo"]];
		m_SauceStateList[SauceNo].Transform.pParent					= &m_PizzaBaseState.Transform;
	}

	// チーズの設定.
	const int CSize = static_cast<int>( Data["Cheese"].size() );
	m_CheeseStateList.resize( CSize, m_SauceStateBase );
	for ( int CheeseNo = 0; CheeseNo < CSize; ++CheeseNo ) {
		json CheeseData = Data["Cheese"][CheeseNo];
		m_CheeseStateList[CheeseNo].AnimState.AnimNumber			= CheeseData["Type"];
		m_CheeseStateList[CheeseNo].pMaskTexture					= m_pSauceMaskList[CheeseData["MaskType"]][CheeseData["MaskNo"]];
		m_CheeseStateList[CheeseNo].Transform.pParent				= &m_PizzaBaseState.Transform;
	}

	// 具材の設定.
	const int ISize = static_cast<int>( Data["Ingredients"].size() );
	m_IngredientsStateList.resize( ISize, m_IngredientsStateBase );
	for ( int IngredientNo = 0; IngredientNo < ISize; ++IngredientNo ) {
		json IngredientData = Data["Ingredients"][IngredientNo];
		m_IngredientsStateList[IngredientNo].AnimState.AnimNumber	= IngredientData["Type"];
		m_IngredientsStateList[IngredientNo].Transform.Position.x	= IngredientData["Position"]["x"];
		m_IngredientsStateList[IngredientNo].Transform.Position.y	= IngredientData["Position"]["y"];
		m_IngredientsStateList[IngredientNo].Transform.Rotation.z	= IngredientData["Rotation"];
		m_IngredientsStateList[IngredientNo].Transform.pParent		= &m_PizzaBaseState.Transform;
		
		float Speed  = CRandom::GetRand( 0.08f, 0.10f + ( 0.10f / ISize ) * IngredientNo );
		Speed		+= CRandom::GetRand( 0.00f, ( 0.30f / ISize ) * IngredientNo );
		m_IngredientsStateList[IngredientNo].UIAnimState.Speed_x	= Speed;
		m_IngredientsStateList[IngredientNo].UIAnimState.Speed_y	= Speed;
	}
}

//---------------------------.
// ピザの削除.
//---------------------------.
void CPizzaUI::DeletePizza()
{
	if ( m_PizzaBaseState.IsDisp == false	) return;
	if ( m_pPlayer->GetIsFoodHave()			) return;

	// 持っているピザを削除する.
	m_PizzaBaseState.IsDisp = false;
	m_IngredientsStateList.clear();
}

//---------------------------.
// ピザの移動の更新.
//---------------------------.
void CPizzaUI::PizzaMoveUpdate()
{
	if ( m_PizzaBaseState.IsDisp == false ) return;
	
	// 傾きの取得.
	const float LRGauge = m_pPlayer->GetRuckSackLRGauge();
	const float UDGauge = m_pPlayer->GetRuckSackUDGauge();

	// ピザの移動量の取得.
	const float XMove	= PIZZA_MOVE * LRGauge;
	const float YMove	= PIZZA_MOVE * UDGauge;

	// ピザの拡縮の取得.
	float XScale	= PIZZA_SCALE * ( ( std::abs( XMove ) - PIZZA_SCALE_START_MOVE ) / ( PIZZA_MOVE - PIZZA_SCALE_START_MOVE ) );
	float YScale	= PIZZA_SCALE * ( ( std::abs( YMove ) - PIZZA_SCALE_START_MOVE ) / ( PIZZA_MOVE - PIZZA_SCALE_START_MOVE ) );
	if ( XScale < 0.0f ) XScale = 0.0f;
	if ( YScale < 0.0f ) YScale = 0.0f;

	// ピザを傾きに応じて移動させる.
	m_PizzaBaseState.Transform.Position.x	= m_PizzaBasePos.x + XMove;
	m_PizzaBaseState.Transform.Position.y	= m_PizzaBasePos.y + YMove;

	// ピザを傾きに応じて縮小させる.
	m_PizzaBaseState.Transform.Scale.x		= STransform::NORMAL_SCALE - XScale;
	m_PizzaBaseState.Transform.Scale.y		= STransform::NORMAL_SCALE - YScale;
}

//---------------------------.
// ピザをぐちゃぐちゃにしていく更新.
//---------------------------.
void CPizzaUI::PizzaCrashUpdate()
{
	if ( m_PizzaBaseState.IsDisp == false ) return;

	// 現在のゲージの位置の取得.
	const float LRGauge = m_pPlayer->GetRuckSackLRGauge();
	const float UDGauge = m_pPlayer->GetRuckSackUDGauge();

	// ゲージが端っこにあるか.
	if ( -GAUGE_MAX < LRGauge && LRGauge < GAUGE_MAX &&
		 -GAUGE_MAX < UDGauge && UDGauge < GAUGE_MAX ) return;
	
	// 具材を動かす.
	for ( auto& i : m_IngredientsStateList ) {
		if ( i.AnimState.AnimNumber < 0 ) continue;

		// 移動速度の設定.
		D3DXVECTOR2 MoveSpeed = INIT_FLOAT2;
		if ( LRGauge <= -GAUGE_MAX ) MoveSpeed.x = -i.UIAnimState.Speed_x;
		if ( LRGauge >=  GAUGE_MAX ) MoveSpeed.x =  i.UIAnimState.Speed_x;
		if ( UDGauge <= -GAUGE_MAX ) MoveSpeed.y = -i.UIAnimState.Speed_y;
		if ( UDGauge >=  GAUGE_MAX ) MoveSpeed.y =  i.UIAnimState.Speed_y;

		// 動かした後の座標を求める.
		STransform NewPos	= i.Transform;
		NewPos.Position.x	+= MoveSpeed.x;
		NewPos.Position.y	+= MoveSpeed.y;

		// ピザの外に移動していないか確認.
		const float Radius = D3DXVec3Length( &NewPos.Position );
		if ( Radius > INGREDIENTS_MOVE ) continue;

		// 具材を動かす.
		i.Transform = NewPos;
	}
}
