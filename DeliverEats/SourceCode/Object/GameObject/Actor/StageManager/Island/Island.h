#pragma once
#include "..\..\Actor.h"

class CPlayer;
class CIslandPlayerHouse;
class CIslandHouse;
class CIslandShop;

/************************************************
*	浮島クラス.
*		﨑田友輝.
**/
class CIsland final
	: public CActor
{
public:
	CIsland();
	virtual ~CIsland();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 当たり判定終了後呼び出される更新.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;
	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;

	// ステージエディタのImGuiの描画.
	void StageEditorImGuiRender();
	// 浮島の情報をImGuiで描画.
	void IslandStateImGuiRender();

	// 浮島のデータの取得.
	json GetIslandData();

	// 浮島の作成.
	void Create( json j );

	// プレイヤーを開始位置に設定.
	bool SetPlayerStartPos( CPlayer* pPlayer );

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

private:
	// カメラによるアルファ値の更新.
	void CameraAlphaUpdate();

private:
	std::unique_ptr<CIslandPlayerHouse>	m_pPlayerHouse;		// プレイヤーの家.
	std::unique_ptr<CIslandHouse>		m_pHouse;			// 家.
	std::unique_ptr<CIslandShop>		m_pShop;			// お店.
	CSkinMesh*							m_pIslandMash;		// 浮島モデル.
	CCollisionMesh*						m_pCollisionMesh;	// 当たり判定用メッシュ.
	float								m_Alpha;			// アルファ値.

private:
	// 外部データの定数.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};
