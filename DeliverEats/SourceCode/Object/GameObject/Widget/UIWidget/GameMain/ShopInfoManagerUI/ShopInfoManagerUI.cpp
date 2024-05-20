#include "ShopInfoManagerUI.h"
#include "ShopInfoUI\ShopInfoUI.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Employee\Employee.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\..\..\..\Time\Time.h"

namespace {
	constexpr int SHOP_INFO_NUM_MAX = 3;	// ���X�̏ڍ׏��̐��̏��.
	constexpr int SHOP_INFO_LEFT	= 0;	// �����̂��X�̏ڍ׏��.
	constexpr int SHOP_INFO_CENTER	= 1;	// �^�񒆂̂��X�̏ڍ׏��.
	constexpr int SHOP_INFO_RIGHT	= 2;	// �E���̂��X�̏ڍ׏��.
}

CShopInfoManagerUI::CShopInfoManagerUI()
	: m_pPlayer				( nullptr )
	, m_pShopInfoList		( SHOP_INFO_NUM_MAX )
	, m_pDispEmployeeList	( SHOP_INFO_NUM_MAX )
	, m_DispNum				( INIT_INT )
	, m_CoolTime			( INIT_FLOAT )
{
}

CShopInfoManagerUI::~CShopInfoManagerUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CShopInfoManagerUI::Init()
{
	// �ڍ׏��̏�����.
	for ( auto& i : m_pShopInfoList ) {
		i = std::make_unique<CShopInfoUI>();
		i->Init();
	}

	// �X���̏�����.
	for ( auto& e : m_pDispEmployeeList ) e = nullptr;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CShopInfoManagerUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	OpenUpdate();		// �\���̍X�V.
	CloseUpdate();		// ��\���̍X�V.
	ChangeUpdate();		// �ύX�̍X�V.
	CoolTimeUpdate();	// �N�[���^�C���̍X�V.

	for ( auto& i : m_pShopInfoList ) i->Update( DeltaTime );
}

//---------------------------.
// �`��.
//---------------------------.
void CShopInfoManagerUI::Render()
{
	const int Size = static_cast<int>(m_pShopInfoList.size());
	for ( int i = Size - 1; i >= 0; --i ) {
		m_pShopInfoList[i]->Render();
	}
}

//---------------------------.
// �v���C���[�̐ݒ�.
//---------------------------.
void CShopInfoManagerUI::SetPlayer( CPlayer* pPlayer )
{
	m_pPlayer = pPlayer;
	for ( auto& i : m_pShopInfoList ) i->SetPlayer( pPlayer );
}

//---------------------------.
// �\���̍X�V.
//---------------------------.
void CShopInfoManagerUI::OpenUpdate()
{
	if ( CTime::GetIsGameEnd()						) return;
	if ( m_pPlayer->GetIsFoodHave()					) return;
	if ( m_pShopInfoList.size() > SHOP_INFO_NUM_MAX ) return;
	if ( m_CoolTime				> 0.0f				) return;

	// �X���̃��X�g���擾.
	CDeliveryManager::WaitAndLinkListUpdate();
	std::vector<CEmployee*> EmployeeList = CDeliveryManager::GetLinkSortEmployee();

	// �\���ł���X�����Ȃ��ꍇ�\���������炷.
	const int EmployeeSize = static_cast<int>( EmployeeList.size() );
	if ( EmployeeSize < m_DispNum ) {
		// �\�����Ă��鐔�����炷.
		m_DispNum--;

		// ���X�̏ڍ׏����\���ɂ���.
		m_pShopInfoList[m_DispNum]->Close();
		m_CoolTime = CONSTANT.ANIM_COOL_TIME;
		return;
	}
	if ( EmployeeSize == 0 ) return;

	// �\������X���̎擾.
	std::vector<CEmployee*> pDispEmployeeList( m_DispNum );
	for ( int i = 0; i < m_DispNum; ++i ) {
		pDispEmployeeList[i] = EmployeeList[i];
	}

	// �\�����Ă���X����ύX���邩.
	for ( int i = 0; i < m_DispNum; ++i ) {
		// �\�����Ă��邩���ׂ�.
		bool		IsDisp	= false;
		const int	Size	= static_cast<int>( pDispEmployeeList.size() );
		for ( int ENo = 0; ENo < Size; ++ENo ) {
			if ( m_pDispEmployeeList[i] != pDispEmployeeList[ENo] ) continue;
			pDispEmployeeList.erase( pDispEmployeeList.begin() + ENo );
			IsDisp = true;
			break;
		}

		// �\�����Ă��Ȃ��ꍇ�؂�ւ���.
		if ( IsDisp == false ) {
			if ( m_pShopInfoList[i]->GetIsAnimation() ) continue;

			// ���݂̈ʒu�����ŕ\�����Ă��邨�X�̏����폜����.
			for ( int DNo = i + i; DNo < m_DispNum; ++DNo ) {
				const int Size = static_cast<int>( pDispEmployeeList.size() );
				for ( int ENo = 0; ENo < Size; ++ENo ) {
					if ( m_pDispEmployeeList[DNo] != pDispEmployeeList[ENo] ) continue;
					pDispEmployeeList.erase( pDispEmployeeList.begin() + ENo );
					break;
				}
			}

			// �\�����Ă��邨�X�̕ύX.
			m_pDispEmployeeList[i] = pDispEmployeeList[0];

			// ���݂̏ڍ׏������.
			m_pShopInfoList[i]->Close();

			// �V�����J��.
			m_pShopInfoList.emplace_back( std::make_unique<CShopInfoUI>() );
			m_pShopInfoList.back()->SetPlayer( m_pPlayer );
			m_pShopInfoList.back()->Init();
			m_pShopInfoList.back()->Open( i, pDispEmployeeList[0]->GetHandle() );

			// �v�f�̈ʒu��ύX.
			std::swap( m_pShopInfoList[i], m_pShopInfoList.back() );
			m_CoolTime = CONSTANT.ANIM_COOL_TIME;
			return;
		}
	}

	// �V�����ǉ�����.
	if ( m_DispNum == SHOP_INFO_NUM_MAX ) return;
	m_CoolTime = CONSTANT.ANIM_COOL_TIME;

	// �X���̕ۑ�.
	m_pDispEmployeeList[m_DispNum] = EmployeeList[m_DispNum];

	// �ڍ׏��̐ݒ�.
	m_pShopInfoList[m_DispNum]->Open( m_DispNum, EmployeeList[m_DispNum]->GetHandle() );

	// �\������̉��Z.
	m_DispNum++;
}

//---------------------------.
// ��\���̍X�V.
//---------------------------.
void CShopInfoManagerUI::CloseUpdate()
{
	if ( m_pPlayer->GetIsFoodHave() == false && CTime::GetIsGameEnd() == false	) return;
	if ( m_DispNum					<= 0										) return;
	if ( m_pShopInfoList.size()		> SHOP_INFO_NUM_MAX							) return;
	if ( m_CoolTime					> 0.0f										) return;

	// �\�����Ă��鐔�����炷.
	m_DispNum--;

	// ���X�̏ڍ׏����\���ɂ���.
	m_pShopInfoList[m_DispNum]->Close();
	m_CoolTime = CONSTANT.ANIM_COOL_TIME;
}

//---------------------------.
// �ڍ׏��̕ύX�̍X�V.
//---------------------------.
void CShopInfoManagerUI::ChangeUpdate()
{
	const int Size = static_cast<int>( m_pShopInfoList.size() );
	if ( Size <= SHOP_INFO_NUM_MAX ) return;

	// �ύX���I��������.
	for ( int i = Size - 1; i >= SHOP_INFO_NUM_MAX; --i ) {
		if ( m_pShopInfoList[i]->GetIsDisp() ) return;
		m_pShopInfoList.pop_back();
	}
}

//---------------------------.
// �N�[���^�C���̍X�V.
//---------------------------.
void CShopInfoManagerUI::CoolTimeUpdate()
{
	if ( m_CoolTime <= 0.0f ) return;

	// �N�[���^�C�������炷.
	m_CoolTime -= m_DeltaTime;
	if ( m_CoolTime <= 0.0f ) m_CoolTime = 0.0f;
}
