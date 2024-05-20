#pragma once
#include "..\..\..\Actor.h"

class CSignboard;
class CEmployee;
class CSmokeManager;
class CStageIcon;

/************************************************
*	浮島の店クラス.
*		﨑田友輝.
**/
class CIslandShop final
	: public CActor
{
public:
	CIslandShop();
	virtual ~CIslandShop();

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

	// お店のタイプの取得.
	int GetShopNo() const { return m_ShopNo; }
	// お店のタイプの指定.
	void SetShopNo( const int No ) { m_ShopNo = No; }

	// 看板の座標の設定.
	void SetSignboardPosition( const D3DXPOSITION3& Pos );

	// 当たり判定を追加する.
	void PushCollisionManager();

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

	// アイコンの更新.
	void IconUpdate();

	// 煙の再生.
	void PlaySmoke();

	// 店員を表示するための移動.
	void EmployeeDispMove();
	// 店員がプレイヤーの方を見るように回転.
	void EmployeePlayerLook();
	// 店員を非表示にするための移動.
	void EmployeeHideMove();
	// 店員の回転.
	void EmployeeRotation();

	// 扉を開ける.
	void DoorOpen();
	// 扉を閉じたか.
	void DoorClose();

private:
	CSkinMesh*						m_pShopMesh;			// 部屋メッシュ.
	CSkinMesh*						m_pDoorMesh;			// 扉メッシュ.
	CCollisionMesh*					m_pCollisionMesh;		// 当たり判定用メッシュ.
	std::unique_ptr<CStageIcon>		m_pIcon;				// アイコン.
	std::unique_ptr<CSignboard>		m_pSignboard;			// 看板.
	std::unique_ptr<CEmployee>		m_pEmployee;			// 店員.
	std::unique_ptr<CSmokeManager>	m_pSmokeManager;		// 煙マネージャー.
	SMeshPointLight					m_PointLight;			// ポイントライト.
	SMeshPointLight					m_EmployeeLight;		// 店員のポイントライトを初期化する用.
	STransform						m_EmployeeTransform;	// 店員のトランスフォーム.
	STransform						m_DoorTransform;		// 扉のトランスフォーム.
	D3DXROTATION3					m_EmployeeAddRotation;	// 店員の追加の角度
	D3DXPOSITION3					m_SmokePlayPosition;	// 煙の再生位置.
	D3DXPOSITION3					m_StartPos;				// 開始座標.
	D3DXPOSITION3					m_EndPos;				// 終了座標.
	D3DXVECTOR3						m_MoveVector;			// 移動ベクトル.
	D3DXPOSITION3					m_DoorStartPos;			// 扉の開始座標.
	D3DXPOSITION3					m_DoorEndPos;			// 扉の終了座標.
	D3DXVECTOR3						m_DoorMoveVector;		// 扉の移動ベクトル.
	float							m_MoveCnt;				// 移動用のカウント.
	float							m_DoorMoveCnt;			// 扉移動用のカウント.
	float							m_Alpha;				// アルファ値.
	int								m_ShopNo;				// お店のタイプ.
	bool							m_IsRotEnd;				// 回転終了したか.
	bool							m_IsOpenEnd;			// 窓を開けたか.
	bool							m_IsCloseEnd;			// 窓を閉じたか.
	bool							m_IsSmoke;				// 煙を出すか.
	bool							m_IsOpenSE;				// SEを再生したか.

private:
	// 外部データの定数.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};