#include "DebugWindow.h"
#include "..\..\..\Common\SoundManeger\SoundManeger.h"

CDebugWindow::CDebugWindow()
	: m_WatchQueue		()
	, m_MenuQueue		()
	, m_IsRender		( false )
{
}

CDebugWindow::~CDebugWindow()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CDebugWindow* CDebugWindow::GetInstance()
{
	static std::unique_ptr<CDebugWindow> pInstance = std::make_unique<CDebugWindow>();
	return pInstance.get();
}

//----------------------------.
// 描画.
//----------------------------.
void CDebugWindow::Render()
{
	ImGui::Begin( "DebugWindow" );
	
	// デバックの表示.
	const int QueueSize = static_cast<int>( GetInstance()->m_WatchQueue.size() );
	if ( ImGui::BeginTabBar( "TabBar", ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_TabListPopupButton ) ) {
		for ( int i = 0; i < QueueSize; i++ ) {
			std::function<void()> proc = GetInstance()->m_WatchQueue.front();
			proc();
			GetInstance()->m_WatchQueue.pop();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

//----------------------------.
// ウォッチに関数を追加する.
//----------------------------.
void CDebugWindow::PushProc( const char* Name, const std::function<void()>& Proc )
{
	CDebugWindow* pI = GetInstance();

	if ( pI->m_IsRender == false ) return;
	pI->m_WatchQueue.push(
	[=]() {
		if ( ImGui::BeginTabItem( Name ) ) {
			Proc();
			ImGui::EndTabItem();
		}
	} );
}
