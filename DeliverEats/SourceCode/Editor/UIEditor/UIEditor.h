#pragma once
#include "..\EditorBase.h"
#include "..\..\Scene\SceneManager\SceneManager.h"

class CSceneWidget;

/************************************************
*	UIGfB^.
*		ú±cFP.
**/
class CUIEditor final
	: public CEditorBase
{
public:
	CUIEditor();
	virtual ~CUIEditor();

	// ú».
	virtual bool Init() override;

	// XV.
	virtual void Update( const float& DeltaTime ) override;

	// ImGui`æ.
	virtual void ImGuiRender() override;
	// fÌ`æ.
	virtual void ModelRender() override {};
	// UIÌ`æ.
	virtual void UIRender() override;
	// GtFNg`æ.
	virtual void EffectRneder() override {};

private:
	// \¦·éV[ÌÏX.
	void ChangeDispScene( const bool IsSaveStop = false );

	// Ot@CÌÇÝÝ.
	HRESULT ZLogLoad();

	// Ot@CÌÇÝÝ.
	HRESULT YLogLoad();

	// Û¶.
	void Save();
	// ³Éß·.
	void Undo();
	// âè¼·.
	void Redu();

private:
	std::unique_ptr<CSceneWidget>	m_pUI;				// UI.
	ESceneList						m_DispScene;		// \¦µÄ¢éV[.
	ESceneList						m_UIScene;			// UIÌ\¦·éV[.

	const D3DXCOLOR4				m_OldBackColor;		// ÌÌwiæ.
	D3DXCOLOR4						m_BackColor;		// wiJ[.
	D3DXCOLOR4						m_HitBoxColor;		// ½è»èJ[.

	int								m_GridSpace;		// ObhüÌÔu.
	CSprite*						m_pGrid;			// Obgü.
	SSpriteRenderState				m_GridState;		// Obhîñ.

	std::string						m_SelectUI;			// IðµÄ¢éUI.
	std::vector<std::string>		m_UndoLogList;		// ³Éß·OXg.
	std::vector<std::string>		m_ReduLogList;		// âè¼·OXg.
	int								m_LogMax;			// OÌãÀ.

	bool							m_IsAutoSave;		// ©®ÅÛ¶·é©.
	bool							m_IsCreaterLog;		// Oðì¬·é©.
	bool							m_IsDrag;			// }EXÅÂ©ßé.
	bool							m_IsDispHitBox;		// ½è»èð`æ·é©.
	bool							m_IsWidgetUpdate;	// UIÌXVðs¤©.
	bool							m_IsFileDelete;		// t@Cðí·é©.
};