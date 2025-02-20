#include "MeshResource.h"
#include "..\..\Common\Mesh\StaticMesh\StaticMesh.h"
#include "..\..\Common\Mesh\SkyBoxMesh\SkyBoxMesh.h"
#include "..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"

namespace {
	constexpr char FILE_PATH[] = "Data\\Mesh";	// メッシュファイルパス.
}

CMeshResource::CMeshResource()
	: m_CollisionMeshList	()
	, m_SkyBoxMeshList		()
	, m_StaticMeshList		()
	, m_SkinMeshList		()
	, m_CollisionMeshNames	()
	, m_SkyBoxMeshNames		()
	, m_StaticMeshNames		()
	, m_SkinMeshNames		()
	, m_IsLoadEnd			( false )
	, m_TexturePathList		()
{
}

CMeshResource::~CMeshResource()
{
	m_SkinMeshList.clear();
	m_StaticMeshList.clear();
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CMeshResource* CMeshResource::GetInstance()
{
	static std::unique_ptr<CMeshResource> pInstance = std::make_unique<CMeshResource>();
	return pInstance.get();
}

//---------------------------.
// モデルの読み込み.
//---------------------------.
HRESULT CMeshResource::MeshLoad()
{
	CMeshResource* pI = GetInstance();
	std::unique_lock<std::mutex> lock( pI->m_Mutex );

	auto MeshLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// 拡張子.
		const std::string FilePath	= Entry.path().string();				// ファイルパス.
		const std::string FileName	= Entry.path().stem().string();			// ファイル名.
		const std::string LoadMsg	= FilePath + " 読み込み : 成功";			// あらかじめロード完了メッセージを設定する.
		const std::string ErrorMsg	= FileName + " : X ファイル読み込み失敗";	// あらかじめエラーメッセージを設定する.

		// 拡張子が ".x" ではない場合読み込まない.
		if ( Extension != ".x" && Extension != ".X" ) return;

		// ファイル名の最後に "_s" があるためスキンメッシュとして保存.
		const std::wstring wFilePath = StringConversion::to_wString( FilePath );
		if ( FileName.substr( FileName.length() - 2 ) == "_s" ) {
			// メッシュの読み込み.
			pI->m_SkinMeshList[FileName] = std::make_unique<CSkinMesh>();
			if ( FAILED( pI->m_SkinMeshList[FileName]->Init( wFilePath.c_str() ) ) )
				throw ErrorMsg.c_str();
			pI->m_SkinMeshNames.emplace_back( FileName );
		}
		// ファイル名の最後に "_c" があるためコリジョンメッシュとして保存.
		else if ( FileName.substr( FileName.length() - 2 ) == "_c" ) {
			// メッシュの読み込み.
			pI->m_CollisionMeshList[FileName] = std::make_unique<CCollisionMesh>();
			if ( FAILED( pI->m_CollisionMeshList[FileName]->Init( wFilePath.c_str() ) ) )
				throw ErrorMsg.c_str();
			pI->m_CollisionMeshNames.emplace_back( FileName );
		}
		// ファイル名の最後に "_b" があるためスカイボックスメッシュとして保存.
		else if ( FileName.substr( FileName.length() - 2 ) == "_b" ) {
			// メッシュの読み込み.
			pI->m_SkyBoxMeshList[FileName] = std::make_unique<CSkyBoxMesh>();
			if ( FAILED( pI->m_SkyBoxMeshList[FileName]->Init( wFilePath.c_str() ) ) )
				throw ErrorMsg.c_str();
			pI->m_SkyBoxMeshNames.emplace_back( FileName );
		}
		// それ以外のためスタティックメッシュとして保存.
		else {
			// メッシュの読み込み.
			pI->m_StaticMeshList[FileName] = std::make_unique<CStaticMesh>();
			if ( FAILED( pI->m_StaticMeshList[FileName]->Init( wFilePath.c_str() ) ) )
				throw ErrorMsg.c_str();
			pI->m_StaticMeshNames.emplace_back( FileName );
		}
		CLog::PushLog( LoadMsg.c_str() );
	};

	CLog::PushLog( "------ メッシュ読み込み開始 ------" );
	try {
		std::filesystem::recursive_directory_iterator dir_itr( FILE_PATH ), end_itr;
		std::for_each( dir_itr, end_itr, MeshLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// エラーメッセージを表示.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	catch ( const char* e ) {
		// エラーメッセージを表示.
		ErrorMessage( e );
		return E_FAIL;
	}
	CLog::PushLog( "------ メッシュ読み込み終了 ------" );

	// 読み込み終了.
	pI->m_IsLoadEnd = true;
	return S_OK;
}

//---------------------------.
// コリジョンメッシュの取得.
//---------------------------.
CCollisionMesh* CMeshResource::GetCollision( const std::string& name )
{
	// モデルの読み込みが終了してないので null を返す.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// 指定したモデルを返す.
	for ( auto& m : GetInstance()->m_CollisionMeshList ) {
		if ( m.first == name ) return m.second.get();
	}

	ErrorMessage( name + " mesh not found" );
	return nullptr;
}

//---------------------------.
// スカイボックスメッシュの取得.
//---------------------------.
CSkyBoxMesh* CMeshResource::GetSkyBox( const std::string& name )
{
	// モデルの読み込みが終了してないので null を返す.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// 指定したモデルを返す.
	for ( auto& m : GetInstance()->m_SkyBoxMeshList ) {
		if ( m.first == name ) return m.second.get();
	}

	ErrorMessage( name + " mesh not found" );
	return nullptr;
}

//---------------------------.
// スタティックメッシュの取得.
//---------------------------.
CStaticMesh* CMeshResource::GetStatic( const std::string& name )
{
	// モデルの読み込みが終了してないので null を返す.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// 指定したモデルを返す.
	for ( auto& m : GetInstance()->m_StaticMeshList ) {
		if ( m.first == name ) return m.second.get();
	}

	ErrorMessage( name + " mesh not found" );
	return nullptr;
}

//---------------------------.
// スキンメッシュの取得.
//---------------------------.
CSkinMesh* CMeshResource::GetSkin( const std::string& name, SAnimationController* pOutAC )
{
	// モデルの読み込みが終了してないので null を返す.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// 指定したモデルを返す.
	for ( auto& m : GetInstance()->m_SkinMeshList ) {
		if ( m.first != name ) continue;

		// アニメーションコントローラーのコピーを作成する.
		if ( pOutAC != nullptr ) {
			LPD3DXANIMATIONCONTROLLER pAC = m.second.get()->GetAnimationController();
			if ( FAILED( pOutAC->SetAnimController( pAC ) ) ) return nullptr;
		}
		return m.second.get();
	}

	ErrorMessage( name + " mesh not found" );
	return nullptr;
}
