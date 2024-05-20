#include "PizzaResource.h"

namespace {
	constexpr char FILE_PATH[] = "Data\\Parameter\\Pizza"; // �s�U�f�[�^�t�@�C���p�X.
}

CPizzaResource::CPizzaResource()
	: m_PizzaList		()
	, m_PizzaNames		()
	, m_Mutex			()
	, m_IsLoadEnd		( false )
{
}

CPizzaResource::~CPizzaResource()
{
	m_PizzaList.clear();
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CPizzaResource* CPizzaResource::GetInstance()
{
	static std::unique_ptr<CPizzaResource> pInstance = std::make_unique<CPizzaResource>();
	return pInstance.get();
}

//---------------------------.
// �s�U�̓ǂݍ���.
//---------------------------.
HRESULT CPizzaResource::PizzaLoad()
{
	CPizzaResource* pI = GetInstance();
	std::unique_lock<std::mutex> Lock( pI->m_Mutex );

	auto EffectLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// �g���q.
		const std::string FilePath	= Entry.path().string();				// �t�@�C���p�X.
		const std::string FileName	= Entry.path().stem().string();			// �t�@�C����.

		// �g���q�� ".json" �ł͂Ȃ��ꍇ�ǂݍ��܂Ȃ�.
		if ( Extension != ".json" && Extension != ".JSON" ) return;

		// �s�U��ǂݍ��݃��X�g�Ɋi�[����.
		pI->m_PizzaList[FileName] = FileManager::JsonLoad( FilePath );
		pI->m_PizzaNames.emplace_back( FileName );

		CLog::PushLog( FilePath + " �ǂݍ��� : ����" );
	};

	CLog::PushLog( "------ �s�U�ǂݍ��݊J�n ------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, EffectLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// �G���[���b�Z�[�W��\��.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ �s�U�ǂݍ��ݏI�� ------" );

	// �ǂݍ��ݏI��.
	pI->m_IsLoadEnd = true;
	return S_OK;
}

//---------------------------.
// �G�t�F�N�g�擾�֐�.
//---------------------------.
json CPizzaResource::GetPizza( const std::string& FileName )
{
	// �ǂݍ��݂��I����Ă��Ȃ������� null ��Ԃ�.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// �w�肵���s�U���擾.
	for ( auto& m : GetInstance()->m_PizzaList ) {
		if ( m.first == FileName ) return m.second;
	}

	ErrorMessage( FileName + " effects not found" );
	return nullptr;
}

//---------------------------.
// �s�U�̒ǉ�.
//---------------------------.
void CPizzaResource::PushPizzaData( const std::string& FileName, json j )
{
	CPizzaResource* pI = GetInstance();

	// ���X�g�ɒǉ�.
	pI->m_PizzaList[FileName] = j;
	pI->m_PizzaNames.emplace_back( FileName );
}

//---------------------------.
// �s�U�̍폜.
//---------------------------.
void CPizzaResource::PizzaDelete( const std::string& FileName )
{
	CPizzaResource* pI = GetInstance();

	// ���X�g����폜.
	pI->m_PizzaList.erase( FileName );
	const auto& itEnd = std::remove( pI->m_PizzaNames.begin(), pI->m_PizzaNames.end(), FileName );
	pI->m_PizzaNames.erase( itEnd, pI->m_PizzaNames.end() );

	// �t�@�C�����폜.
	std::filesystem::remove( std::string( FILE_PATH ) + "\\" + FileName + ".json" );
}
