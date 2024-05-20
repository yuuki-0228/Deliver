#include "Linking.h"
#include "..\Customer\Customer.h"
#include "..\Employee\Employee.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"
#include "..\..\..\..\..\..\Utility\ImGuiManager\MessageWindow\MessageWindow.h"
#include <functional>
#include <algorithm>
#include <memory>

// �R�Â��̃��b�Z�[�W��R�Â��̐F�Ɠ����ɂ��邩.
//#define LINK_MESSAGE_COLOR;

CLinking::CLinking()
	: m_pEmployeeList		()
	, m_pCustomerList		()
	, m_LinkList			()
	, m_DeleteHandleList	()
	, m_LinkTimeCnt			( INIT_FLOAT )
	, m_DeltaTime			( INIT_FLOAT )
	, m_IsStartLinkEnd		( false )
{
}

CLinking::~CLinking()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CLinking* CLinking::GetInstance()
{
	static std::unique_ptr<CLinking> pInstance = std::make_unique<CLinking>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
bool CLinking::Init()
{
	CLinking* pI = GetInstance();

	pI->m_pEmployeeList.clear();
	pI->m_pCustomerList.clear();
	pI->m_LinkList.clear();
	pI->m_DeleteHandleList.clear();
	pI->m_LinkTimeCnt		= 0.0f;
	pI->m_DeltaTime			= 0.0f;
	pI->m_IsStartLinkEnd	= false;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CLinking::Update( const float& DeltaTime )
{
	CLinking* pI = GetInstance();

	pI->m_DeltaTime = DeltaTime;

	// ���X�g�̍X�V.
	CDeliveryManager::WaitAndLinkListUpdate();
	pI->m_pEmployeeList = CDeliveryManager::GetWaitEmployeeList();
	pI->m_pCustomerList = CDeliveryManager::GetWaitCustomerList();

	StartLink();		// �J�n���̕R�Â�.
	LinkTimeUpdate();	// �R�Â����s�����Ԃ̍X�V.
	NowLinkUpdate();	// �R�Â����̍X�V.
	LinkListUpdate();	// �R�Â����X�g�̍X�V.
}

//---------------------------.
// �R�Â�������.
//---------------------------.
void CLinking::Link()
{
	CLinking* pI = GetInstance();

	// ���X�g�̍X�V.
	CDeliveryManager::WaitAndLinkListUpdate();
	pI->m_pEmployeeList = CDeliveryManager::GetWaitEmployeeList();
	pI->m_pCustomerList = CDeliveryManager::GetWaitCustomerList();

	// �X��/���q����̐��̊m�F.
	const int E_Max = static_cast<int>( pI->m_pEmployeeList[EObjectTag::None].size() );
	const int C_Max = static_cast<int>( pI->m_pCustomerList[EObjectTag::None].size() );
	if ( E_Max <= 0 || C_Max <= 0 ) return;

	// �X��/���q����̃^�C�v���Ƃ̐��̎擾.
	const int E_A_Size = static_cast<int>( pI->m_pEmployeeList[EObjectTag::Apartment].size()	);
	const int E_I_Size = static_cast<int>( pI->m_pEmployeeList[EObjectTag::Island].size()		);
	const int C_A_Size = static_cast<int>( pI->m_pCustomerList[EObjectTag::Apartment].size()	);
	const int C_I_Size = static_cast<int>( pI->m_pCustomerList[EObjectTag::Island].size()		);

	// �X���̕R�Â����ނ������_���őI��.
	EObjectTag E_Tag = EObjectTag::None;
	if ( E_A_Size > 0 && E_I_Size > 0 ) {
		if ( CRandom::Probability( 1, 2 ) )	E_Tag = EObjectTag::Apartment;
		else								E_Tag = EObjectTag::Island;
	}
	else {
		if ( E_A_Size > 0 )	E_Tag = EObjectTag::Apartment;
		else				E_Tag = EObjectTag::Island;
	}

	// ���q����̕R�Â����ނ������_���őI��.
	EObjectTag C_Tag = EObjectTag::None;
	if ( C_A_Size > 0 && C_I_Size > 0 ) {
		if ( CRandom::Probability( 1, 2 ) )	C_Tag = EObjectTag::Apartment;
		else								C_Tag = EObjectTag::Island;
	}
	else {
		if ( C_A_Size > 0 )	C_Tag = EObjectTag::Apartment;
		else				C_Tag = EObjectTag::Island;
	}

	// �R�Â�����X��/���q����������_���őI��.
	const int E_Size	= static_cast<int>( pI->m_pEmployeeList[E_Tag].size() );
	const int C_Size	= static_cast<int>( pI->m_pCustomerList[C_Tag].size() );
	const int E_No		= CRandom::GetRand( 0, E_Size - 1 );
	const int C_No		= CRandom::GetRand( 0, C_Size - 1 );

	// �폜�����n���h�����ė��p���邩.
	Handle handle = static_cast<Handle>( pI->m_LinkList.size() );
	if ( pI->m_DeleteHandleList.size() != 0 ) {
		handle = pI->m_DeleteHandleList.back();
		pI->m_DeleteHandleList.pop_back();
	}
	else pI->m_LinkList.emplace_back();

	// �R�Â����s��.
	pI->m_LinkList[handle] = SLink(
		pI->m_pEmployeeList[E_Tag][E_No],
		pI->m_pCustomerList[C_Tag][C_No],
		pI->CONSTANT.COLOR_LIST[handle],
		pI->CONSTANT.LINK_END_TIME );

	// �n���h����ݒ�.
	pI->m_pCustomerList[C_Tag][C_No]->AttachLink( handle );
	pI->m_pEmployeeList[E_Tag][E_No]->AttachLink( handle );

	CMessageWindow::PushMessage( u8"���q����Ƃ��X��R�Â��܂����B"
#ifdef LINK_MESSAGE_COLOR
		, pI->CONSTANT.COLOR_LIST[handle]
#endif	// #ifdef LINK_MESSAGE_COLOR.
	);
}

//---------------------------.
// �R�Â��̉���.
//---------------------------.
void CLinking::LinkEnd( const Handle& handle )
{
	CLinking* pI = GetInstance();

	// ���������n���h����ۑ�.
	pI->m_DeleteHandleList.emplace_back( handle );

	// �R�Â��̉���.
	pI->m_LinkList[handle].LinkEnd();
}

//---------------------------.
// �S�Ă̕R�Â�������.
//---------------------------.
void CLinking::AllLinkEnd()
{
	CLinking* pI = GetInstance();

	for ( auto& l : pI->m_LinkList ) {
		if ( l.IsGoToPass ) continue;

		// �R�Â��̉���.
		l.LinkEnd();
	}
}

//---------------------------.
// �g�p���Ă���n���h�����擾.
//---------------------------.
bool CLinking::GetIsUseHandle( const Handle& handle )
{
	CLinking* pI = GetInstance();

	if ( handle >= pI->m_LinkList.size() ) return false;
	for ( auto& d : pI->m_DeleteHandleList ) {
		if ( handle == d ) return false;
	}
	return true;
}

//---------------------------.
// �J�n���̕R�Â�.
//---------------------------.
void CLinking::StartLink()
{
	CLinking* pI = GetInstance();

	if ( pI->m_IsStartLinkEnd ) return;

	// �J�n���̕R�Â����s���邩�m�F.
	const int E_Size = static_cast<int>( pI->m_pEmployeeList[EObjectTag::None].size() );
	const int C_Size = static_cast<int>( pI->m_pCustomerList[EObjectTag::None].size() );
	if ( E_Size < pI->CONSTANT.START_LINK_NUM || C_Size < pI->CONSTANT.START_LINK_NUM ) return;

	// �R�Â����s��.
	for ( int i = 0; i < pI->CONSTANT.START_LINK_NUM; ++i ) Link();
	pI->m_IsStartLinkEnd = true;
}

//---------------------------.
// �R�Â����s�����Ԃ̍X�V.
//---------------------------.
void CLinking::LinkTimeUpdate()
{
	CLinking* pI = GetInstance();

	if ( CTime::GetIsGameEnd()			) return;
	if ( pI->m_pEmployeeList.empty()	) return;
	if ( pI->m_pCustomerList.empty()	) return;

	// �R�Â����s�����Ԃ̃J�E���g�̉��Z.
	pI->m_LinkTimeCnt += pI->m_DeltaTime;

	// �R�Â����s�����Ԃ̐ݒ�.
	const float EndTime = CTime::GetIsGoldenTime() ?
		pI->CONSTANT.GOLDEN_TIME_LINK_TIME :
		pI->CONSTANT.NORMAL_LINK_TIME;

	// �R�Â����s��.
	if ( pI->m_LinkTimeCnt >= EndTime ) {
		pI->m_LinkTimeCnt = 0.0f;
		Link();
	}
}

//---------------------------.
// �R�Â����̍X�V.
//---------------------------.
void CLinking::NowLinkUpdate()
{
	CLinking* pI = GetInstance();

	const int Size = static_cast<int>( pI->m_LinkList.size() );
	for ( int i = 0; i < Size; ++i ) {
		if ( pI->m_LinkList[i].IsDeleteHand ) continue;

		// �R�Â��̉����̃J�E���g�̍X�V.
		if ( pI->m_LinkList[i].EndTimeUpdate( pI->m_DeltaTime, i ) ) {

			// �R�Â����������ꂽ���߃n���h����ۑ�����.
			pI->m_DeleteHandleList.emplace_back( i );
		}
	}
}

//---------------------------.
// �R�Â����X�g�̍X�V.
//---------------------------.
void CLinking::LinkListUpdate()
{
	CLinking* pI = GetInstance();

	// �������Ďg�p���Ă��Ȃ��n���h�����X�g���~���ɕ��ѕς���.
	std::sort( pI->m_DeleteHandleList.begin(), pI->m_DeleteHandleList.end(), std::greater<int>() );

	// �g�p���Ă��Ȃ��n���h���̍폜.
	const int Size = static_cast<int>( pI->m_LinkList.size() );
	for ( int i = Size - 1; i >= 0; --i ) {
		if ( pI->m_LinkList[i].IsDeleteHand == false ) break;
		pI->m_LinkList.pop_back();

		// �������Ďg�p���Ă��Ȃ��n���h���̍폜.
		if ( pI->m_DeleteHandleList.empty() ) continue;
		if ( pI->m_DeleteHandleList[0] != i ) continue;
		pI->m_DeleteHandleList.erase( pI->m_DeleteHandleList.begin() );
	}
}

//---------------------------.
// �R�Â��̉����̍X�V.
//---------------------------.
bool CLinking::stLink::EndTimeUpdate( const float& DeltaTime, CLinking::Handle Handle )
{
	if ( IsStop ) return false;

	// �J�E���g�̒ǉ�.
	EndTimeCnt += DeltaTime;

	// �R�Â�����������.
	if ( EndTimeCnt >= EndTime ) {
		LinkEnd();
		CMessageWindow::PushMessage( u8"�R�Â����������܂����B"
#ifdef LINK_MESSAGE_COLOR
			, CConstant::GetLinkingr().COLOR_LIST[Handle]
#endif	// #ifdef LINK_MESSAGE_COLOR.
		);
		return true;
	}
	return false;
}

//---------------------------.
// �R�Â��̉���.
//---------------------------.
void CLinking::stLink::LinkEnd()
{
	// �A�j���[�V���������Ƃɖ߂�.
	pEmployee->ChangeNormal();
	pCustomer->ChangeNormal();

	// �R�Â�������.
	pEmployee->DetachLink();
	pCustomer->DetachLink();
	IsDeleteHand = true;
}