#include "GameClearWidget.h"

CGameClearWidget::CGameClearWidget()
{
}

CGameClearWidget::~CGameClearWidget()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CGameClearWidget::Init()
{
	SetSceneName( ESceneList::GameClear );
	GetSpriteList();

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGameClearWidget::Update( const float & DeltaTime )
{
}
