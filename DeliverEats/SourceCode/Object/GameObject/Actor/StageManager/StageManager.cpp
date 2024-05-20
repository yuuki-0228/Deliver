#include "StageManager.h"
#include "World\World.h"
#include "Apartment\ATypeApartment\ATypeApartment.h"
#include "Apartment\BTypeApartment\BTypeApartment.h"
#include "Island\Island.h"
#include "..\..\..\..\Common\DirectX\DirectX11.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"

namespace {
	constexpr char STAGE_FILE_PATH[] = "Data\\Parameter\\Main\\Stage.json";
}

CStageManager::CStageManager()
	: m_pWorld				( nullptr )
	, m_pATypeApartmentList	()
	, m_pBTypeApartmentList	()
	, m_pIslandList			()
{
}

CStageManager::~CStageManager()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CStageManager::Init()
{
	// ワールドの初期化.
	m_pWorld = std::make_unique<CWorld>();
	m_pWorld->Init();

	// jsonファイルの読み込み.
	json j = FileManager::JsonLoad( STAGE_FILE_PATH );
	if ( j.empty() ) return false;

	// 建物の作成.
	const int ATypeSize		= static_cast<int>( j["ATypeApartment"].size() );
	const int BTypeSize		= static_cast<int>( j["BTypeApartment"].size() );
	const int IslandSize	= static_cast<int>( j["Island"].size() );
	m_pATypeApartmentList.resize( ATypeSize );
	m_pBTypeApartmentList.resize( BTypeSize );
	m_pIslandList.resize( IslandSize );

	// Aタイプのマンションの設定.
	for ( int i = 0; i < ATypeSize; ++i ) {
		m_pATypeApartmentList[i] = std::make_unique<CATypeApartment>();
		m_pATypeApartmentList[i]->Create( j["ATypeApartment"][i] );
		m_pATypeApartmentList[i]->Init();
	}
	// Bタイプのマンションの設定.
	for ( int i = 0; i < BTypeSize; ++i ) {
		m_pBTypeApartmentList[i] = std::make_unique<CBTypeApartment>();
		m_pBTypeApartmentList[i]->Create( j["BTypeApartment"][i] );
		m_pBTypeApartmentList[i]->Init();
	}
	// 浮島の設定.
	for ( int i = 0; i < IslandSize; ++i ) {
		m_pIslandList[i] = std::make_unique<CIsland>();
		m_pIslandList[i]->Create( j["Island"][i] );
		m_pIslandList[i]->Init();
	}
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CStageManager::Update( const float& DeltaTime )
{
	m_pWorld->Update( DeltaTime );
	for ( auto& a : m_pATypeApartmentList	) a->Update( DeltaTime );
	for ( auto& b : m_pBTypeApartmentList	) b->Update( DeltaTime );
	for ( auto& i : m_pIslandList			) i->Update( DeltaTime );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CStageManager::LateUpdate( const float& DeltaTime )
{
	m_pWorld->LateUpdate( DeltaTime );
	for ( auto& a : m_pATypeApartmentList	) a->LateUpdate( DeltaTime );
	for ( auto& b : m_pBTypeApartmentList	) b->LateUpdate( DeltaTime );
	for ( auto& i : m_pIslandList			) i->LateUpdate( DeltaTime );
}

//---------------------------.
// デバックの更新.
//---------------------------.
void CStageManager::DebugUpdate( const float& DeltaTime )
{
	m_pWorld->DebugUpdate( DeltaTime );
	for ( auto& a : m_pATypeApartmentList	) a->DebugUpdate( DeltaTime );
	for ( auto& b : m_pBTypeApartmentList	) b->DebugUpdate( DeltaTime );
	for ( auto& i : m_pIslandList			) i->DebugUpdate( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CStageManager::Render()
{
	// 背面の描画をしないようにする.
	CDirectX11::SetRasterizerState( ERS_STATE::Back );

	m_pWorld->Render();
	for ( auto& a : m_pATypeApartmentList	) a->Render();
	for ( auto& b : m_pBTypeApartmentList	) b->Render();
	for ( auto& i : m_pIslandList			) i->Render();

	CDirectX11::SetRasterizerState( ERS_STATE::None );
}

//---------------------------.
// スプライト(3D)の描画.
//---------------------------.
void CStageManager::Sprite3DRender_A()
{
	m_pWorld->Sprite3DRender_A();
	for ( auto& a : m_pATypeApartmentList	) a->Sprite3DRender_A();
	for ( auto& b : m_pBTypeApartmentList	) b->Sprite3DRender_A();
	for ( auto& i : m_pIslandList			) i->Sprite3DRender_A();
}
void CStageManager::Sprite3DRender_B()
{
	m_pWorld->Sprite3DRender_B();
	for ( auto& a : m_pATypeApartmentList	) a->Sprite3DRender_B();
	for ( auto& b : m_pBTypeApartmentList	) b->Sprite3DRender_B();
	for ( auto& i : m_pIslandList			) i->Sprite3DRender_B();
}

//---------------------------.
// ステージエディタのImGuiの描画.
//---------------------------.
void CStageManager::StageEditorImGuiRender()
{
	// Aタイプマンションの設定.
	ImGui::Separator();
	ImGui::SetNextTreeNodeOpen( true, ImGuiCond_Once );
	if ( ImGui::TreeNode( "ATypeApartment" ) ) {
		ImGui::Separator();
		const int ATypeSize = static_cast<int>( m_pATypeApartmentList.size() );
		for ( int i = 0; i < ATypeSize; ++i ) {
			if ( CImGuiManager::Button( std::string( "x##AType_" + std::to_string( i ) ).c_str(), Color4::Red ) ) {
				m_pATypeApartmentList.erase( m_pATypeApartmentList.begin() + i );
				break;
			}
			ImGui::SameLine();
			if ( ImGui::TreeNode( std::string( "ATypeApartment_" + std::to_string( i ) ).c_str() ) ) {
				ImGui::Separator();
				m_pATypeApartmentList[i]->StageEditorImGuiRender();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::TreePop();
	}

	// Bタイプマンションの設定.
	ImGui::Separator();
	ImGui::SetNextTreeNodeOpen( true, ImGuiCond_Once );
	if ( ImGui::TreeNode( "BTypeApartment" ) ) {
		ImGui::Separator();
		const int BTypeSize = static_cast<int>( m_pBTypeApartmentList.size() );
		for ( int i = 0; i < BTypeSize; ++i ) {
			if ( CImGuiManager::Button( std::string( "x##BType_" + std::to_string( i ) ).c_str(), Color4::Red ) ) {
				m_pBTypeApartmentList.erase( m_pBTypeApartmentList.begin() + i );
				break;
			}
			ImGui::SameLine();
			if ( ImGui::TreeNode( std::string( "BTypeApartment_" + std::to_string( i ) ).c_str() ) ) {
				ImGui::Separator();
				m_pBTypeApartmentList[i]->StageEditorImGuiRender();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::TreePop();
	}

	// 浮島の設定.
	ImGui::Separator();
	ImGui::SetNextTreeNodeOpen( true, ImGuiCond_Once );
	if ( ImGui::TreeNode( "Island" ) ) {
		ImGui::Separator();
		const int IslandSize = static_cast<int>( m_pIslandList.size() );
		for ( int i = 0; i < IslandSize; ++i ) {
			if ( CImGuiManager::Button( std::string( "x##Island_" + std::to_string( i ) ).c_str(), Color4::Red ) ) {
				m_pIslandList.erase( m_pIslandList.begin() + i );
				break;
			}
			ImGui::SameLine();
			if ( ImGui::TreeNode( std::string( "Island_" + std::to_string( i ) ).c_str() ) ) {
				ImGui::Separator();
				m_pIslandList[i]->StageEditorImGuiRender();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
}

//---------------------------.
// ステージの情報をImGuiで描画.
//---------------------------.
void CStageManager::StageStateImGuiRender()
{
	ImGui::Separator();
	ImGui::Text( u8"ATypeApartment ： %d個", m_pATypeApartmentList.size() );
	ImGui::Separator();
	for ( auto& a : m_pATypeApartmentList ) a->FloorStateImGuiRender();
	ImGui::Separator();
	ImGui::Text( u8"BTypeApartment ： %d個", m_pBTypeApartmentList.size() );
	ImGui::Separator();
	for ( auto& b : m_pBTypeApartmentList ) b->FloorStateImGuiRender();
	ImGui::Separator();
	ImGui::Text( u8"IslandList     ： %d個", m_pIslandList.size() );
	ImGui::Separator();
	for ( auto& i : m_pIslandList ) i->IslandStateImGuiRender();
	ImGui::Separator();
}

//---------------------------.
// オブジェクトの追加.
//---------------------------.
void CStageManager::AddObject( const std::string& ObjectName )
{
	// Aタイプマンションの追加.
	if ( ObjectName == "ATypeApartment" ) {
		// 仮データの作成.
		json j;
		j["Position"]["x"]	= 0.0f;
		j["Position"]["z"]	= 0.0f;
		j["Floar"]			= { nullptr };

		// Aタイプのマンションの作成.
		m_pATypeApartmentList.emplace_back( std::make_unique<CATypeApartment>() );
		m_pATypeApartmentList.back()->Create( j );
		m_pATypeApartmentList.back()->Init();
	}
	// Bタイプマンションの追加.
	else if ( ObjectName == "BTypeApartment" ) {
		// 仮データの作成.
		json j;
		j["Position"]["x"]	= 0.0f;
		j["Position"]["z"]	= 0.0f;
		j["Floar"]			= { nullptr };

		// Bタイプのマンションの作成.
		m_pBTypeApartmentList.emplace_back( std::make_unique<CBTypeApartment>() );
		m_pBTypeApartmentList.back()->Create( j );
		m_pBTypeApartmentList.back()->Init();
	}
	// 浮島の追加.
	else {
		// 仮データの作成.
		json j;
		j["Position"]["x"]	= 0.0f;
		j["Position"]["y"]	= 50.0f;
		j["Position"]["z"]	= 0.0f;
		j["What"]			= "Shop";

		// 浮島の作成.
		m_pIslandList.emplace_back( std::make_unique<CIsland>() );
		m_pIslandList.back()->Create( j );
		m_pIslandList.back()->Init();
	}
}

//---------------------------.
// 浮島のデータの取得.
//---------------------------.
json CStageManager::GetStageData()
{
	json Out;
	for ( auto& a : m_pATypeApartmentList	) Out["ATypeApartment"].emplace_back( a->GetApartmentData() );
	for ( auto& b : m_pBTypeApartmentList	) Out["BTypeApartment"].emplace_back( b->GetApartmentData() );
	for ( auto& i : m_pIslandList			) Out["Island"].emplace_back( i->GetIslandData() );
	return Out;
}

//---------------------------.
// プレイヤーの開始位置の取得.
//---------------------------.
void CStageManager::SetPlayerStartPos( CPlayer* pPlayer )
{
	for ( auto& i : m_pIslandList ) {
		if ( i->SetPlayerStartPos( pPlayer ) ) return;
	}
}
