#pragma once
#include "..\..\Global.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <filesystem>

/************************************************
*	�s�U���\�[�X�N���X.
*		���c�F�P.
**/
class CPizzaResource final
{
public:
	using Pizza_map		= std::unordered_map<std::string, json>;
	using Pizza_List	= std::vector<std::string>;

public:
	CPizzaResource();
	~CPizzaResource();

	// �C���X�^���X�̎擾.
	static CPizzaResource* GetInstance();

	// �s�U�̓ǂݍ���.
	static HRESULT PizzaLoad();

	// �s�U�擾�֐�.
	static json GetPizza( const std::string& FileName );

	// �ۑ����Ă���s�U���X�g�̎擾.
	static Pizza_List GetPizzaList() { return GetInstance()->m_PizzaNames; }

	// �s�U�f�[�^�̎擾.
	static Pizza_map  GetPizzaData() { return GetInstance()->m_PizzaList; }

	// �s�U�̒ǉ�.
	static void PushPizzaData( const std::string& FileName, json j );
	// �s�U�̍폜.
	static void PizzaDelete( const std::string& FileName );

private:
	Pizza_map	m_PizzaList;	// �s�U���X�g.
	Pizza_List	m_PizzaNames;	// �s�U�̖��O���X�g.
	std::mutex	m_Mutex;
	bool		m_IsLoadEnd;	// �ǂݍ��݂��I�������.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CPizzaResource( const CPizzaResource& )					= delete;
	CPizzaResource& operator = ( const CPizzaResource& )	= delete;
	CPizzaResource( CPizzaResource&& )						= delete;
	CPizzaResource& operator = ( CPizzaResource&& )			= delete;
};