#pragma once
#include <vector>
#include "..\..\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\..\..\Utility\Constant\Constant.h"

class CEmployee;
class CCustomer;

namespace {
	// �R�Â����s���Ă��Ȃ�
	constexpr int NO_LINK_HANDLE = -1;
}

/************************************************
*	���q����ƓX���̕R�Â��N���X.
*		���c�F�P.
**/
class CLinking final
{
public:
	// �R�Â��n���h��.
	using Handle = int;

private:
	// �R�Â��\����.
	struct stLink {
		stLink()
			: stLink( nullptr, nullptr, INIT_FLOAT3, INIT_FLOAT )
		{}
		stLink( CEmployee* pE, CCustomer* pC, const D3DXCOLOR3& Color, const float& End )
			: pEmployee		( pE )
			, pCustomer		( pC )
			, Color			( Color )
			, EndTimeCnt	( INIT_FLOAT )
			, EndTime		( End )
			, IsStop		( false )
			, IsDeleteHand	( false )
			, IsGoToPass	( false )
		{}

		CEmployee*	pEmployee;		// �X��.
		CCustomer*	pCustomer;		// ���q����.
		D3DXCOLOR3	Color;			// �R�Â��̐F.
		float		EndTimeCnt;		// �R�Â������p�̃J�E���g.
		float		EndTime;		// �R�Â���������.
		bool		IsStop;			// �R�Â��̉����̒�~���邩.
		bool		IsDeleteHand;	// �g�p���Ă��Ȃ��n���h����.
		bool		IsGoToPass;		// �n���ɍs���L������.

		// �R�Â��̉����̍X�V.
		bool EndTimeUpdate( const float& DeltaTime, CLinking::Handle Handle );
		// �R�Â��̉���.
		void LinkEnd();
	} typedef SLink;

public:
	CLinking();
	~CLinking();

	// �C���X�^���X�̎擾.
	static CLinking* GetInstance();

	// ������.
	static bool Init();

	// �X�V.
	static void Update( const float& DeltaTime );

	// �R�Â�����Ă���X�����擾.
	static CEmployee* GetEmployee( const Handle& handle ) { return GetInstance()->m_LinkList[handle].pEmployee; }
	// �R�Â�����Ă��邨�q������擾.
	static CCustomer* GetCustomer( const Handle& handle ) { return GetInstance()->m_LinkList[handle].pCustomer; }
	// �R�Â��̉������ԃJ�E���g�̎擾.
	static float GetLinkEndTimeCnt( const Handle& handle ) { return GetInstance()->m_LinkList[handle].EndTimeCnt; }
	// �R�Â��̉������Ԃ̎擾.
	static float GetLinkEndTime( const Handle& handle ) { return GetInstance()->m_LinkList[handle].EndTime; }
	// �R�Â��̐F�̎擾.
	static D3DXCOLOR3 GetLinkColor( const Handle& handle ) { return GetInstance()->m_LinkList[handle].Color; }

	// �R�Â��̉����̒�~.
	static void SetIsLinkEndStop( const Handle& handle, const bool Flag ) { GetInstance()->m_LinkList[handle].IsStop = Flag; }
	// �n���ɍs�����q���񂩐ݒ�.
	static void SetIsGoToPass( const Handle& handle, const bool Flag ) { GetInstance()->m_LinkList[handle].IsGoToPass = Flag; }

	// �R�Â�������.
	static void Link();
	// �R�Â��̉���.
	static void LinkEnd( const Handle& handle );
	// �S�Ă̕R�Â�������.
	static void AllLinkEnd();

	// �g�p���Ă���n���h�����擾.
	static bool GetIsUseHandle( const Handle& handle );

private:
	// �J�n���̕R�Â�.
	static void StartLink();
	// �R�Â����s�����Ԃ̍X�V.
	static void LinkTimeUpdate();
	// �R�Â����̍X�V.
	static void NowLinkUpdate();
	// �R�Â����X�g�̍X�V.
	static void LinkListUpdate();

private:
	CDeliveryManager::EmployeeList	m_pEmployeeList;	// �X�����X�g.
	CDeliveryManager::CustomerList	m_pCustomerList;	// ���q���񃊃X�g.
	std::vector<SLink>				m_LinkList;			// �R�Â����X�g.
	std::vector<Handle>				m_DeleteHandleList;	// �������Ďg�p���Ă��Ȃ��n���h�����X�g.
	float							m_LinkTimeCnt;		// �R�Â����s�����ԗp�̃J�E���g.
	float							m_DeltaTime;		// �f���^�^�C��.
	bool							m_IsStartLinkEnd;	// �J�n���̕R�Â����I��������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SLinking& CONSTANT = CConstant::GetLinkingr();
};
