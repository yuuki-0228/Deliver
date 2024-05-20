#include "GetMoneyUI.h"
#include "..\..\..\..\..\Actor\Character\Player\Player.h"

CGetMoneyUI::CGetMoneyUI()
	: m_pPlayer		( nullptr )
	, m_pBack		( nullptr )
	, m_pFont		( nullptr )
	, m_BackState	()
	, m_FontState	()
{
}

CGetMoneyUI::~CGetMoneyUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CGetMoneyUI::Init()
{
	m_pBack = CSpriteResource::GetSprite( "RucksackMoneyBack", &m_BackState );
	m_pFont = CFontResource::GetFont( "NasuM", &m_FontState );

	// ���W�̐ݒ�.
	m_BackState.Transform.Position.x	= m_Transform.Position.x + 55.0f;
	m_BackState.Transform.Position.y	= m_Transform.Position.y - 85.0f;

	// �t�H���g�̐ݒ�.
	const SSize& FontSize = m_pFont->GetFontState().Disp;
	m_FontState.TextAlign				= ETextAlign::Center;
	m_FontState.Color					= Color4::Black;
	m_FontState.Transform.Position		= m_BackState.Transform.Position;
	m_FontState.Transform.Position.y	-= FontSize.h / 2.0f;
	m_FontState.Transform.Scale			= { 0.70f, 0.85f, 0.85f };
	m_BackState.Color.w					= 0.8f;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGetMoneyUI::Update( const float& DeltaTime )
{
	// �\�����邩�ݒ�.
	m_BackState.IsDisp = m_pPlayer->GetIsFoodHave();
	m_FontState.IsDisp = m_BackState.IsDisp;
	if ( m_BackState.IsDisp == false ) return;

	// ���i�̏��̎擾.
	const SFoodState&	FoodState		= m_pPlayer->GetFoodState();
	const int			MinimumPrice	= FoodState.Money - FoodState.DiscountMoney;

	// �Œ�����炦��l�i��3�����ƂɃJ���}������.
	m_FontState.Text = std::to_string( MinimumPrice );
	for ( int i = static_cast<int>( m_FontState.Text.length() ) - 3; i > 0; i -= 3 ) {
		m_FontState.Text.insert( i, "," );
	}

	// �Œ�����炦��l�i�̍ŏ���"��"������.
	m_FontState.Text = "��" + m_FontState.Text;

	// �Œ�����炦��l�i�̐F�̕ύX����.
	m_FontState.Color = Color4::Black;
	if ( MinimumPrice < FoodState.Money ) {
		m_FontState.Color = Color4::Red;
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CGetMoneyUI::Render()
{
	m_pBack->RenderUI( &m_BackState );
	m_pFont->RenderUI( &m_FontState );
}
