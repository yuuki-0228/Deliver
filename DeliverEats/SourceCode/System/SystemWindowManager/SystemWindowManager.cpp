#include "SystemWindowManager.h"
#include "..\InputSetting\InputSetting.h"
#include "..\WindowSetting\WindowSetting.h"
#include "..\BugReport\BugReport.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\Message\Message.h"

CSystemWindowManager::CSystemWindowManager()
	: m_pInputSetting	( nullptr )
	, m_pWindowSetting	( nullptr )
	, m_pBugReport		( nullptr )
	, m_pButton			( nullptr )
{
	m_pInputSetting		= std::make_unique<CInputSetting>();
	m_pWindowSetting	= std::make_unique<CWindowSetting>();
	m_pBugReport		= std::make_unique<CBugReport>();
	m_pButton			= CSpriteResource::GetSprite( "BugButton" );
	m_pButton->SetIsAllDisp( false );
}

CSystemWindowManager::~CSystemWindowManager()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CSystemWindowManager* CSystemWindowManager::GetInstance()
{
	static std::unique_ptr<CSystemWindowManager> pInstance = std::make_unique<CSystemWindowManager>();
	return pInstance.get();
}

//---------------------------.
// 更新.
//---------------------------.
void CSystemWindowManager::Update( const float& DeltaTime )
{
	CSystemWindowManager* pI = GetInstance();
#if _DEBUG

	// バグボタンの表示.
	pI->m_pButton->SetIsAllDisp( !pI->m_pBugReport->GetBugListEmpty() );

	// バグボタンが押された場合.
	if ( CMessage::Check( "BugButtonDown" ) ) pI->m_pBugReport->Open();
#endif
}

//---------------------------.
// 描画.
//---------------------------.
void CSystemWindowManager::Render()
{
#if _DEBUG
	CSystemWindowManager* pI = GetInstance();

	pI->m_pInputSetting->Render();
	pI->m_pWindowSetting->Render();
	pI->m_pBugReport->Render();
#endif
}
