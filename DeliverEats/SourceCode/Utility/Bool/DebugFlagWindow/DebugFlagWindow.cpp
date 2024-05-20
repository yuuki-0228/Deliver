#include "DebugFlagWindow.h"
#include "..\FlagManager\FlagManager.h"
#include "..\..\ImGuiManager\ImGuiManager.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

CDebugFlagWindow::CDebugFlagWindow()
	: m_FindString			( "" )
	, m_IsDispNameNoneFlag	( false, u8"無名のフラグを表示/非表示", "" )
{
}

CDebugFlagWindow::~CDebugFlagWindow()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CDebugFlagWindow* CDebugFlagWindow::GetInstance()
{
	static std::unique_ptr<CDebugFlagWindow> pInstance = std::make_unique<CDebugFlagWindow>();
	return pInstance.get();
}

//----------------------------.
// 描画.
//----------------------------.
void CDebugFlagWindow::Render()
{
#ifdef _DEBUG
	CDebugFlagWindow* pI = GetInstance();

	ImGui::Begin( "DebugFlagWindow" );

	// メッセージ検索の表示.
	FlagFind();

	// 各フラグのグループごとの処理リスト.
	std::unordered_map<std::string, std::vector<CFlagManager::Handle>> GroupFuncList;

	// フラグを表示するか調べる.
	ImGui::BeginChild( ImGui::GetID( ( void* )0 ), ImVec2( 0.0f, 0.0f ), ImGuiWindowFlags_NoTitleBar );
	const CFlagManager::Handle Size = CFlagManager::GetFlagNum();
	for ( CFlagManager::Handle i = 0; i < Size; i++ ) {
		std::string Name	= CFlagManager::GetName( i );
		std::string Group	= CFlagManager::GetGroup( i );

		// 表示するフラグか調べる.
		if ( pI->m_IsDispNameNoneFlag == false && Name == "" ) continue;
		if ( Name == "" ) {
			// 無名のフラグに仮の名前をつける.
			Name	= "NameNoneFlag_" + std::to_string( static_cast<int>( i ) );
			CFlagManager::SetGroup( i, "NameNone" );
		}
		if ( Name.find( pI->m_FindString ) == std::string::npos ) continue;

		// 処理の追加.
		GroupFuncList[Group.c_str()].emplace_back( i );
	}

	// フラグの表示.
	for ( auto& [Group, Handle] : GroupFuncList ) {
		// グループに指定がない場合そのまま表示する.
		if ( Group == "" ) {
			for ( auto& h : Handle ) {
				bool				Flag = CFlagManager::IsBitFlag( h );
				const std::string	Name = CFlagManager::GetName( h );
				CFlagManager::SetBitFlag( h, CImGuiManager::CheckBox( Name.c_str(), &Flag ) );
			}
			continue;
		}

		// グループに指定があるグループを作成し表示する.
		if ( ImGui::TreeNodeEx( Group.c_str(), ImGuiTreeNodeFlags_Framed ) ) {
			for ( auto& h : Handle ) {
				bool		Flag = CFlagManager::IsBitFlag( h );
				std::string	Name = CFlagManager::GetName( h );
				if ( Group == "NameNone" ) Name = "NameNoneFlag_" + std::to_string( static_cast<int>( h ) );
				CFlagManager::SetBitFlag( h, CImGuiManager::CheckBox( Name.c_str(), &Flag ) );
			}
			ImGui::TreePop();
		}
	}

	ImGui::EndChild();
	ImGui::End();
#endif // #ifdef _DEBUG.
}

//---------------------.
//	フラグ検索処理.
//---------------------.
void CDebugFlagWindow::FlagFind()
{
#ifdef _DEBUG
	ImGui::Text( u8"検索 :" );
	ImGui::SameLine();
	ImGui::InputText( " ", &GetInstance()->m_FindString );
	ImGui::Separator();
#endif // #ifdef _DEBUG.
}
