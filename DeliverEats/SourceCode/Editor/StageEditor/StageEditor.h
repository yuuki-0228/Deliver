#pragma once
#include "..\EditorBase.h"

class CStageManager;

/************************************************
*	ステージエディタ.
*		﨑田友輝.
**/
class CStageEditor
	: public CEditorBase
{
public:
	CStageEditor();
	~CStageEditor();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// ImGui描画.
	virtual void ImGuiRender() override;
	// モデルの描画.
	virtual void ModelRender() override;
	// UIの描画.
	virtual void UIRender() override;
	// エフェクト描画.
	virtual void EffectRneder() override {};

private:
	std::unique_ptr<CStageManager>	m_pStage;		// ステージ.
	std::string						m_SelectObject;	// 選択しているオブジェクト.
};