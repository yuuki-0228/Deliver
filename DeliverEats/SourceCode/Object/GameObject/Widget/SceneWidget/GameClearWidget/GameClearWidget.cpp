#include "GameClearWidget.h"

CGameClearWidget::CGameClearWidget()
{
}

CGameClearWidget::~CGameClearWidget()
{
}

//---------------------------.
// ‰Šú‰».
//---------------------------.
bool CGameClearWidget::Init()
{
	SetSceneName( ESceneList::GameClear );
	GetSpriteList();

	return true;
}

//---------------------------.
// XV.
//---------------------------.
void CGameClearWidget::Update( const float & DeltaTime )
{
}
