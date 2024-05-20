#pragma once
#include "..\..\..\..\..\Global.h"
#include "..\..\..\GameObjectList.h"

class CEmployee;
class CCustomer;
class CPlayer;

/************************************************
*	���q����ƓX���̃}�l�[�W���[�N���X.
**/
class CDeliveryManager
{
public:
	using EmployeeList = std::unordered_map<EObjectTag, std::vector<CEmployee*>>;
	using CustomerList = std::unordered_map<EObjectTag, std::vector<CCustomer*>>;

public:
	CDeliveryManager();
	~CDeliveryManager();

	// �C���X�^���X�̎擾.
	static CDeliveryManager* GetInstance();

	// ������.
	static void Init();
	// �S�Ă̏��������I����Ă���s��������.
	static void EndInit();

	// �����蔻��I����Ăяo�����X�V.
	static void LateUpdate( const float& DeltaTime );

	// �ҋ@��/�R�Â�����Ă��郊�X�g�̍X�V.
	static void WaitAndLinkListUpdate();

	// �S�ēX���̐�.
	static int GetAllEmployeeSize() { return static_cast<int>( GetInstance()->m_pAllEmployeeList.size() ); }
	// �S�Ă̂��q����̐�.
	static int GetAllCustomerSize() { return static_cast<int>( GetInstance()->m_pAllCustomerList.size() ); }

	// �S�ēX�����X�g�̎擾.
	//	[None] : �S��, [Apartment] : �}���V�����̂�, [Island] : �����̂�.
	static EmployeeList GetAllEmployeeList() { return GetInstance()->m_pAllEmployeeList; }
	// �S�Ă��q���񃊃X�g�̎擾.
	//	[None] : �S��, [Apartment] : �}���V�����̂�, [Island] : �����̂�.
	static CustomerList GetAllCustomerList() { return GetInstance()->m_pAllCustomerList; }

	// �ҋ@���̓X�����X�g�̎擾.
	//	[None] : �S��, [Apartment] : �}���V�����̂�, [Island] : �����̂�.
	static EmployeeList GetWaitEmployeeList() { return GetInstance()->m_pWaitEmployeeList; }
	// �ҋ@���̂��q���񃊃X�g�̎擾.
	//	[None] : �S��, [Apartment] : �}���V�����̂�, [Island] : �����̂�.
	static CustomerList GetWaitCustomerList() { return GetInstance()->m_pWaitCustomerList; }

	// �R�Â�����Ă���X�����X�g�̎擾.
	//	[None] : �S��, [Apartment] : �}���V�����̂�, [Island] : �����̂�.
	static EmployeeList GetLinkEmployeeList() { return GetInstance()->m_pLinkEmployeeList; }
	// �R�Â�����Ă��邨�q���񃊃X�g�̎擾.
	//	[None] : �S��, [Apartment] : �}���V�����̂�, [Island] : �����̂�.
	static CustomerList GetLinkCustomerList() { return GetInstance()->m_pLinkCustomerList; }

	// �߂��̕R�Â�����Ă���X�����X�g�̎擾.
	//	[None] : �S��, [Apartment] : �}���V�����̂�, [Island] : �����̂�.
	static EmployeeList GetShortlyEmployeeList() { return GetInstance()->m_pShortlyEmployeeList; }
	// �߂��̕R�Â�����Ă��邨�q���񃊃X�g�̎擾.
	//	[None] : �S��, [Apartment] : �}���V�����̂�, [Island] : �����̂�.
	static CustomerList GetShortlyCustomerList() { return GetInstance()->m_pShortlyCustomerList; }

	// ���i�����炤�X���̎擾.
	static CEmployee* GetGetFoodEmployee() { return GetInstance()->m_pGetFoodEmployee; }
	// �n���ɍs�����q����̎擾.
	static CCustomer* GetGoHandingCustomer() { return GetInstance()->m_pGoHandingCustomer; }

	// ��ԋ߂��R�Â�����Ă���X���̎擾.
	static CEmployee* GetClosestEmployee() { return GetInstance()->m_pClosestEmployee; }
	// ��ԋ߂��R�Â�����Ă��邨�q����̎擾.
	static CCustomer* GetClosestCustomer() { return GetInstance()->m_pClosestCustomer; }

	// �߂����̓X�����X�g�̎擾.
	static std::vector<CEmployee*> GetLinkSortEmployee() { return GetInstance()->m_pLinkSortEmployeeList; }

	// �v���C���[�̐ݒ�.
	static void SetPlayer( CPlayer* pPlayer ) { GetInstance()->m_pPlayer = pPlayer; }

	// �X���̒ǉ�.
	static void AddEmployee( CEmployee* pEmployee, const EObjectTag Tag );
	// �s�U�q��SE���Đ�����X���̒ǉ�.
	static void AddKilnEmployee( CEmployee* pEmployee );
	// ���q����̒ǉ�.
	static void AddEmployee( CCustomer* pCustomer, const EObjectTag Tag );

private:
	EmployeeList			m_pAllEmployeeList;			// �X�����X�g.
	CustomerList			m_pAllCustomerList;			// ���q���񃊃X�g.
	EmployeeList			m_pWaitEmployeeList;		// �ҋ@���̓X�����X�g.
	CustomerList			m_pWaitCustomerList;		// �ҋ@���̂��q���񃊃X�g.
	EmployeeList			m_pLinkEmployeeList;		// �R�Â�����Ă���X�����X�g.
	CustomerList			m_pLinkCustomerList;		// �R�Â�����Ă��邨�q���񃊃X�g.
	EmployeeList			m_pShortlyEmployeeList;		// �߂��̕R�Â�����Ă���X�����X�g.
	CustomerList			m_pShortlyCustomerList;		// �߂��̕R�Â�����Ă��邨�q���񃊃X�g.
	std::vector<CEmployee*>	m_pLinkSortEmployeeList;	// �R�Â�����Ă���߂����̓X�����X�g.
	std::vector<CEmployee*>	m_pKilnEmployeeList;		// �s�U�q��SE���Đ�����X�����X�g.
	CEmployee*				m_pGetFoodEmployee;			// ���i�����炤�X��.
	CCustomer*				m_pGoHandingCustomer;		// �n���ɍs�����q����.
	CEmployee*				m_pClosestEmployee;			// ��ԋ߂��R�Â�����Ă���X��.
	CCustomer*				m_pClosestCustomer;			// ��ԋ߂��R�Â�����Ă��邨�q����.
	CPlayer*				m_pPlayer;					// �v���C���[.
};