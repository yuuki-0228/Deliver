#include "HeightGaugeUI.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Customer\Customer.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Employee\Employee.h"

namespace {
	enum enSpriteList :unsigned char {
		Meter,			// ���[�^�[.
		PlayerIcon,		// �v���C���[�̃A�C�R��.
		CharacterIcon,	// �X��,�q�̃A�C�R��.

		Max
	};

	// �߂��̂��X�̕\�����̏��.
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
// ������.
//---------------------------.
bool CHeightGaugeUI::Init()
{
	// �摜�̎擾.
	m_pSpriteList[Meter]			= CSpriteResource::GetSprite( "HeightGauge",				&m_SpriteStateList[Meter] );
	m_pSpriteList[PlayerIcon]		= CSpriteResource::GetSprite( "HeightGaugePlayerIcon",		&m_SpriteStateList[PlayerIcon] );
	m_pSpriteList[CharacterIcon]	= CSpriteResource::GetSprite( "HeightGaugeCharacterIcon",	&m_SpriteStateList[CharacterIcon] );

	// ���W�̐ݒ�.
	const SSize& Size = m_pSpriteList[Meter]->GetSpriteState().Disp;
	m_SpriteStateList[Meter].Transform.Position = { Size.w / 2.0f + 5.0f, FWND_H - 5.0f, 0.0f };
	m_SpriteStateList[PlayerIcon].Transform.Position.x = m_SpriteStateList[Meter].Transform.Position.x + Size.w / 2.0f + 5.0f;
	m_SpriteStateList[CharacterIcon].Transform.Position.x = m_SpriteStateList[Meter].Transform.Position.x;

	// �T�C�Y�̐ݒ�.
	m_SpriteStateList[CharacterIcon].Transform.Scale.y	= 20.0f;

	// ���X�̈ʒu�̏��̐ݒ�.
	for ( auto& e : m_EmployeeIconState ) e = m_SpriteStateList[CharacterIcon];
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CHeightGaugeUI::Update( const float& DeltaTime )
{
	// ���x�v�̏��̎擾.
	const float HeightGaugeY = m_SpriteStateList[Meter].Transform.Position.y;
	const float HeightGaugeH = m_pSpriteList[Meter]->GetSpriteState().Disp.h;

	// �v���C���[�̍��x�̈ʒu�̍X�V.
	m_SpriteStateList[PlayerIcon].Transform.Position.y = HeightGaugeY -
		HeightGaugeH * ( ( m_pPlayer->GetTransform().Position.y - CONSTANT.DEAD_DOWN_POS_Y ) / ( CONSTANT.DEAD_UP_POS_Y - CONSTANT.DEAD_DOWN_POS_Y ) );

	// �߂��̂��X�̍��x�̍X�V.
	if ( m_pPlayer->GetIsFoodHave() == false ) {
		// �߂��̂��X���擾.
		std::vector<CEmployee*> EmployeeList = CDeliveryManager::GetLinkSortEmployee();
		const int ListSize = static_cast< int >( EmployeeList.size() );

		// �\��������̐ݒ�.
		if ( ListSize < EMPLOYEE_MAX )	m_EmployeeNum = ListSize;
		else							m_EmployeeNum = EMPLOYEE_MAX;

		// �߂��̂��X�̍��x�̍X�V.
		for ( int i = 0; i < m_EmployeeNum; ++i ) {
			m_EmployeeIconState[i].Color				= Color4::RGBA( EmployeeList[i]->GetLinkColor(), 0.5f );
			m_EmployeeIconState[i].Transform.Position.y	= HeightGaugeY -
				HeightGaugeH * ( ( EmployeeList[i]->GetPosition().y - CONSTANT.DEAD_DOWN_POS_Y ) / ( CONSTANT.DEAD_UP_POS_Y - CONSTANT.DEAD_DOWN_POS_Y ) );
		}
		return;
	}

	// �n���ɍs�����q����̍��x�̍X�V.
	m_pCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( m_pCustomer == nullptr ) return;
	m_SpriteStateList[CharacterIcon].Color					= Color4::RGBA( m_pCustomer->GetLinkColor(), 0.5f );
	m_SpriteStateList[CharacterIcon].Transform.Position.y	= HeightGaugeY -
		HeightGaugeH * ( ( m_pCustomer->GetPosition().y - CONSTANT.DEAD_DOWN_POS_Y ) / ( CONSTANT.DEAD_UP_POS_Y - CONSTANT.DEAD_DOWN_POS_Y ) );
}

//---------------------------.
// �`��.
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
