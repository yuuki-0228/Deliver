#pragma once
#include "..\Actor.h"

class CPlayer;
class CWorld;
class CATypeApartment;
class CBTypeApartment;
class CIsland;

/************************************************
*	ステージマネージャー.
*		﨑田友輝.
**/
class CStageManager
	: public CActor
{
public:
	CStageManager();
	virtual ~CStageManager();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 当たり判定終了後呼び出される更新.
	virtual void LateUpdate( const float& DeltaTime ) override;
	// デバックの更新.
	virtual void DebugUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;
	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

	// ステージエディタのImGuiの描画.
	void StageEditorImGuiRender();
	// ステージの情報をImGuiで描画.
	void StageStateImGuiRender();

	// オブジェクトの追加.
	//	ATypeApartment / BTypeApartment / Island.
	void AddObject( const std::string& ObjectName );

	// 浮島のデータの取得.
	json GetStageData();

	// プレイヤーの開始位置の取得.
	void SetPlayerStartPos( CPlayer* pPlayer );

private:
	std::unique_ptr<CWorld>							m_pWorld;				// ワールド.
	std::vector<std::unique_ptr<CATypeApartment>>	m_pATypeApartmentList;	// Aタイプのマンションリスト.
	std::vector<std::unique_ptr<CBTypeApartment>>	m_pBTypeApartmentList;	// Bタイプのマンションリスト.
	std::vector<std::unique_ptr<CIsland>>			m_pIslandList;			// 浮島リスト.
	D3DXPOSITION3									m_PlayerStartPos;		// プレイヤーの開始位置.
};