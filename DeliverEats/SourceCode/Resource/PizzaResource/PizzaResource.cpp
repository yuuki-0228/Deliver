#include "PizzaResource.h"

namespace {
	constexpr char FILE_PATH[] = "Data\\Parameter\\Pizza"; // ピザデータファイルパス.
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
// インスタンスの取得.
//---------------------------.
CPizzaResource* CPizzaResource::GetInstance()
{
	static std::unique_ptr<CPizzaResource> pInstance = std::make_unique<CPizzaResource>();
	return pInstance.get();
}

//---------------------------.
// ピザの読み込み.
//---------------------------.
HRESULT CPizzaResource::PizzaLoad()
{
	CPizzaResource* pI = GetInstance();
	std::unique_lock<std::mutex> Lock( pI->m_Mutex );

	auto EffectLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// 拡張子.
		const std::string FilePath	= Entry.path().string();				// ファイルパス.
		const std::string FileName	= Entry.path().stem().string();			// ファイル名.

		// 拡張子が ".json" ではない場合読み込まない.
		if ( Extension != ".json" && Extension != ".JSON" ) return;

		// ピザを読み込みリストに格納する.
		pI->m_PizzaList[FileName] = FileManager::JsonLoad( FilePath );
		pI->m_PizzaNames.emplace_back( FileName );

		CLog::PushLog( FilePath + " 読み込み : 成功" );
	};

	CLog::PushLog( "------ ピザ読み込み開始 ------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, EffectLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// エラーメッセージを表示.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ ピザ読み込み終了 ------" );

	// 読み込み終了.
	pI->m_IsLoadEnd = true;
	return S_OK;
}

//---------------------------.
// エフェクト取得関数.
//---------------------------.
json CPizzaResource::GetPizza( const std::string& FileName )
{
	// 読み込みが終わっていなかったら null を返す.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// 指定したピザを取得.
	for ( auto& m : GetInstance()->m_PizzaList ) {
		if ( m.first == FileName ) return m.second;
	}

	ErrorMessage( FileName + " effects not found" );
	return nullptr;
}

//---------------------------.
// ピザの追加.
//---------------------------.
void CPizzaResource::PushPizzaData( const std::string& FileName, json j )
{
	CPizzaResource* pI = GetInstance();

	// リストに追加.
	pI->m_PizzaList[FileName] = j;
	pI->m_PizzaNames.emplace_back( FileName );
}

//---------------------------.
// ピザの削除.
//---------------------------.
void CPizzaResource::PizzaDelete( const std::string& FileName )
{
	CPizzaResource* pI = GetInstance();

	// リストから削除.
	pI->m_PizzaList.erase( FileName );
	const auto& itEnd = std::remove( pI->m_PizzaNames.begin(), pI->m_PizzaNames.end(), FileName );
	pI->m_PizzaNames.erase( itEnd, pI->m_PizzaNames.end() );

	// ファイルを削除.
	std::filesystem::remove( std::string( FILE_PATH ) + "\\" + FileName + ".json" );
}
