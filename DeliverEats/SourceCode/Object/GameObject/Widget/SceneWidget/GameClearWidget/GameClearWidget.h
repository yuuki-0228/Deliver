#pragma once
#include "..\SceneWidget.h"

/************************************************
*	ゲームクリアUIクラス.
*		﨑田友輝.
**/
class CGameClearWidget final
	: public CSceneWidget
{
public:
	CGameClearWidget();
	virtual ~CGameClearWidget();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

private:
};
