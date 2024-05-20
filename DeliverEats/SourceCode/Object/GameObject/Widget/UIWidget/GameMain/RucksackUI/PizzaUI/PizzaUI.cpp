#include "PizzaUI.h"
#include "..\..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\..\..\..\Utility\Random\Random.h"
#include "..\..\..\..\..\..\..\Resource\PizzaResource\PizzaResource.h"

namespace {
	constexpr float PIZZA_SCALE_START_MOVE	= 25.0f;	// �s�U�̏k�����J�n������ړ���.
	constexpr float PIZZA_MOVE				= 34.0f;	// �s�U�̈ړ���.
	constexpr float INGREDIENTS_MOVE		= 85.0f;	// �s�U�̋�ނ̈ړ��ł���ړ���(�s�U�̒�������̔��a).
	constexpr float PIZZA_SCALE				= 0.1f;		// �s�U�̏k�������.
	constexpr float GAUGE_MAX				= 1.0f;		// �Q�[�W�̍ő�.
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
// ������.
//---------------------------.
bool CPizzaUI::Init()
{
	// �摜�̎擾.
	m_pPizzaBase		= CSpriteResource::GetSprite( "PizzaBase",				&m_PizzaBaseState );
	m_pIngredients		= CSpriteResource::GetSprite( "PizzaIngredient",		&m_IngredientsStateBase );
	m_pIngredientSauce	= CSpriteResource::GetSprite( "PizzaIngredientSauce",	&m_IngredientSauceState );
	m_pSauce			= CSpriteResource::GetSprite( "PizzaSauce",				&m_SauceStateBase );

	// ���̐ݒ�.
	m_PizzaBaseState.AnimState.IsSetAnimNumber			= true;
	m_IngredientsStateBase.AnimState.IsSetAnimNumber	= true;
	m_IngredientSauceState.AnimState.IsSetAnimNumber	= true;
	m_SauceStateBase.AnimState.IsSetAnimNumber			= true;

	// �e�̐ݒ�.
	m_IngredientsStateBase.Transform.AddParent( &m_PizzaBaseState.Transform );
	m_IngredientSauceState.Transform.AttachParent( &m_PizzaBaseState.Transform );
	m_SauceStateBase.Transform.AttachParent( &m_PizzaBaseState.Transform );

	// ���W�̐ݒ�.
	m_PizzaBasePos								= m_Transform.Position;
	m_IngredientsStateBase.Transform.Position	= INIT_FLOAT3;
	m_SauceStateBase.Transform.Position			= INIT_FLOAT3;

	// �}�X�N�摜�̕ۑ�.
	for( int i = 0; i < 8; ++i ){
		m_pSauceMaskList[i].resize( 4 );
		for ( int No = 0; No < 4; ++No ) {
			m_pSauceMaskList[i][No] = CSpriteResource::GetSprite( "PizzaSauceMask_" + std::to_string( i ) + std::to_string( No ) )->GetTexture();
		}
	}
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CPizzaUI::Update( const float& DeltaTime )
{
	SettingPizza();		// �s�U�̐ݒ�.
	DeletePizza();		// �s�U�̍폜.
	PizzaMoveUpdate();	// �s�U�ړ�.
	PizzaCrashUpdate();	// �s�U�������Ⴎ����ɂ��Ă���.
}

//---------------------------.
// �`��.
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
// �s�U�̐ݒ�.
//---------------------------.
void CPizzaUI::SettingPizza()
{
	if ( m_PizzaBaseState.IsDisp			 ) return;
	if ( m_pPlayer->GetIsFoodHave() == false ) return;

	// �����_���Ńs�U��I��.
	const CPizzaResource::Pizza_List&	PizzaList	= CPizzaResource::GetPizzaList();
	const std::string&					PizzaName	= CRandom::GetRand( PizzaList );
	json								Data		= CPizzaResource::GetPizza( PizzaName );

	// �s�U�̐��n�̐ݒ�.
	m_PizzaBaseState.IsDisp					= true;
	m_PizzaBaseState.AnimState.AnimNumber	= 1;
	m_PizzaBaseState.Transform.Position		= m_PizzaBasePos;
	m_PizzaBaseState.Transform.Scale		= STransform::NORMAL_SCALE_VEC3;

	// �\�[�X�̐ݒ�.
	const int SSize = static_cast<int>( Data["Sauce"].size() );
	m_SauceStateList.resize( SSize, m_SauceStateBase );
	for ( int SauceNo = 0; SauceNo < SSize; ++SauceNo ) {
		json SauceData = Data["Sauce"][SauceNo];
		m_SauceStateList[SauceNo].AnimState.AnimNumber				= SauceData["Type"];
		m_SauceStateList[SauceNo].pMaskTexture						= m_pSauceMaskList[SauceData["MaskType"]][SauceData["MaskNo"]];
		m_SauceStateList[SauceNo].Transform.pParent					= &m_PizzaBaseState.Transform;
	}

	// �`�[�Y�̐ݒ�.
	const int CSize = static_cast<int>( Data["Cheese"].size() );
	m_CheeseStateList.resize( CSize, m_SauceStateBase );
	for ( int CheeseNo = 0; CheeseNo < CSize; ++CheeseNo ) {
		json CheeseData = Data["Cheese"][CheeseNo];
		m_CheeseStateList[CheeseNo].AnimState.AnimNumber			= CheeseData["Type"];
		m_CheeseStateList[CheeseNo].pMaskTexture					= m_pSauceMaskList[CheeseData["MaskType"]][CheeseData["MaskNo"]];
		m_CheeseStateList[CheeseNo].Transform.pParent				= &m_PizzaBaseState.Transform;
	}

	// ��ނ̐ݒ�.
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
// �s�U�̍폜.
//---------------------------.
void CPizzaUI::DeletePizza()
{
	if ( m_PizzaBaseState.IsDisp == false	) return;
	if ( m_pPlayer->GetIsFoodHave()			) return;

	// �����Ă���s�U���폜����.
	m_PizzaBaseState.IsDisp = false;
	m_IngredientsStateList.clear();
}

//---------------------------.
// �s�U�̈ړ��̍X�V.
//---------------------------.
void CPizzaUI::PizzaMoveUpdate()
{
	if ( m_PizzaBaseState.IsDisp == false ) return;
	
	// �X���̎擾.
	const float LRGauge = m_pPlayer->GetRuckSackLRGauge();
	const float UDGauge = m_pPlayer->GetRuckSackUDGauge();

	// �s�U�̈ړ��ʂ̎擾.
	const float XMove	= PIZZA_MOVE * LRGauge;
	const float YMove	= PIZZA_MOVE * UDGauge;

	// �s�U�̊g�k�̎擾.
	float XScale	= PIZZA_SCALE * ( ( std::abs( XMove ) - PIZZA_SCALE_START_MOVE ) / ( PIZZA_MOVE - PIZZA_SCALE_START_MOVE ) );
	float YScale	= PIZZA_SCALE * ( ( std::abs( YMove ) - PIZZA_SCALE_START_MOVE ) / ( PIZZA_MOVE - PIZZA_SCALE_START_MOVE ) );
	if ( XScale < 0.0f ) XScale = 0.0f;
	if ( YScale < 0.0f ) YScale = 0.0f;

	// �s�U���X���ɉ����Ĉړ�������.
	m_PizzaBaseState.Transform.Position.x	= m_PizzaBasePos.x + XMove;
	m_PizzaBaseState.Transform.Position.y	= m_PizzaBasePos.y + YMove;

	// �s�U���X���ɉ����ďk��������.
	m_PizzaBaseState.Transform.Scale.x		= STransform::NORMAL_SCALE - XScale;
	m_PizzaBaseState.Transform.Scale.y		= STransform::NORMAL_SCALE - YScale;
}

//---------------------------.
// �s�U�������Ⴎ����ɂ��Ă����X�V.
//---------------------------.
void CPizzaUI::PizzaCrashUpdate()
{
	if ( m_PizzaBaseState.IsDisp == false ) return;

	// ���݂̃Q�[�W�̈ʒu�̎擾.
	const float LRGauge = m_pPlayer->GetRuckSackLRGauge();
	const float UDGauge = m_pPlayer->GetRuckSackUDGauge();

	// �Q�[�W���[�����ɂ��邩.
	if ( -GAUGE_MAX < LRGauge && LRGauge < GAUGE_MAX &&
		 -GAUGE_MAX < UDGauge && UDGauge < GAUGE_MAX ) return;
	
	// ��ނ𓮂���.
	for ( auto& i : m_IngredientsStateList ) {
		if ( i.AnimState.AnimNumber < 0 ) continue;

		// �ړ����x�̐ݒ�.
		D3DXVECTOR2 MoveSpeed = INIT_FLOAT2;
		if ( LRGauge <= -GAUGE_MAX ) MoveSpeed.x = -i.UIAnimState.Speed_x;
		if ( LRGauge >=  GAUGE_MAX ) MoveSpeed.x =  i.UIAnimState.Speed_x;
		if ( UDGauge <= -GAUGE_MAX ) MoveSpeed.y = -i.UIAnimState.Speed_y;
		if ( UDGauge >=  GAUGE_MAX ) MoveSpeed.y =  i.UIAnimState.Speed_y;

		// ����������̍��W�����߂�.
		STransform NewPos	= i.Transform;
		NewPos.Position.x	+= MoveSpeed.x;
		NewPos.Position.y	+= MoveSpeed.y;

		// �s�U�̊O�Ɉړ����Ă��Ȃ����m�F.
		const float Radius = D3DXVec3Length( &NewPos.Position );
		if ( Radius > INGREDIENTS_MOVE ) continue;

		// ��ނ𓮂���.
		i.Transform = NewPos;
	}
}
