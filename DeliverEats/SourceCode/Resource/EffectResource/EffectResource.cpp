#include "EffectResource.h"
#include "..\..\Common\EffectManager\Effect\Effect.h"

namespace {
	constexpr char FILE_PATH[] = "Data\\Effect"; // エフェクトファイルパス.
}

CEffectResource::CEffectResource()
	: m_EffectList		()
	, m_EffectNames		()
	, m_Mutex			()
	, m_IsLoadEnd		( false )
{
}

CEffectResource::~CEffectResource()
{
	m_EffectList.clear();
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CEffectResource* CEffectResource::GetInstance()
{
	static std::unique_ptr<CEffectResource> pInstance = std::make_unique<CEffectResource>();
	return pInstance.get();
}

//---------------------------.
// エフェクトの読み込み.
//---------------------------.
HRESULT CEffectResource::EffectLoad()
{
	CEffectResource* pI = GetInstance();
	std::unique_lock<std::mutex> Lock( pI->m_Mutex );

	auto EffectLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// 拡張子.
		const std::string FilePath	= Entry.path().string();				// ファイルパス.
		const std::string FileName	= Entry.path().stem().string();			// ファイル名.

		// 拡張子が ".efk" ではない場合読み込まない.
		if ( Extension != ".efk" && Extension != ".EFK" ) return;

		// エフェクトを読み込みリストに格納する.
		pI->m_EffectList[FileName] = std::make_unique<CEffect>( FilePath );
		pI->m_EffectNames.emplace_back( FileName );
	};

	CLog::PushLog( "------ エフェクト読み込み開始 ------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, EffectLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// エラーメッセージを表示.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ エフェクト読み込み終了 ------" );

	// 読み込み終了.
	pI->m_IsLoadEnd = true;
	return S_OK;
}

//---------------------------.
// エフェクト取得関数.
//---------------------------.
CEffect* CEffectResource::GetEffect( const std::string& FileName )
{
	// 読み込みが終わっていなかったら null を返す.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// 指定したエフェクトを取得.
	for ( auto& m : GetInstance()->m_EffectList ) {
		if ( m.first == FileName ) return m.second.get();
	}

	ErrorMessage( FileName + " effects not found" );
	return nullptr;
}
