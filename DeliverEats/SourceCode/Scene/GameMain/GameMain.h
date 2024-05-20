#pragma once
#include "..\SceneBase.h"

class CPlayer;
class CStageManager;
class CGameMainWidget;

/************************************************
*	ゲームクラス.
*		﨑田友輝.
**/
class CGameMain final
	: public CSceneBase
{
public:
	CGameMain();
	~CGameMain();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// モデルの描画.
	virtual void ModelRender() override;
	// スプライト(UI)の描画.
	virtual void SpriteUIRender() override;
	// スプライト(3D)/Effectの描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

private:
	// BGMの更新.
	void BGMUpdate();

private:
	std::shared_ptr<CPlayer>			m_pPlayer;			// プレイヤー.
	std::shared_ptr<CStageManager>		m_pStageManager;	// ステージマネージャー.
	std::unique_ptr<CGameMainWidget>	m_GameMainWidget;	// UI.

private:
	// 外部データの定数.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};