#pragma once
#include "..\SceneWidget.h"

class CConfilmationWindowUI;
class CStatusBarUI;

/************************************************
*	タイトルUIクラス.
*		井谷凌.
**/
class CTitleWidget final
	: public CSceneWidget
{
public:
	CTitleWidget();
	virtual ~CTitleWidget();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	void SetConfilmationWindowUI( CConfilmationWindowUI& CnfWndUI ) { m_pConfilmationWindowUI = &CnfWndUI; }

protected:
	// 追加で行う描画.
	virtual void LateAddRender() override;

private:
	float	m_DeltaTime;

	int		m_Choice;	// 選択肢.

	CSprite*						m_pText;					// テキスト.
	CConfilmationWindowUI*			m_pConfilmationWindowUI;	// 確認画面.
	std::unique_ptr<CStatusBarUI>	m_pStatusBarUI;				// ステータスバーUI.
	SSpriteRenderState				m_TextState;				// テキストの情報.
};
