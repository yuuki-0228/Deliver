#pragma once
#include "..\..\..\Actor.h"

class CGarage;
class CCustomer;
class CStageIcon;

/************************************************
*	浮島の家クラス.
*		﨑田友輝.
**/
class CIslandHouse final
	: public CActor
{
public:
	CIslandHouse();
	virtual ~CIslandHouse();

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

	// ガレージの座標の設定.
	void SetGaragePosition( const D3DXPOSITION3& Pos );

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
	// ライトの更新.
	void LightUpdate();
	// ライトをつける.
	void LightOn( const std::pair<int, int>& Time );
	// ライトを消す.
	void LightOff( const std::pair<int, int>& Time );

	// お客さんを表示するための移動.
	void CustomerDispMove();
	// お客さんがプレイヤーの方を見るように回転.
	void CustomerPlayerLook();
	// お客さんを非表示にするための移動.
	void CustomerHideMove();
	// お客さんの回転.
	void CustomerRotation();

	// 扉を開ける.
	void DoorOpen();
	// 扉を閉じたか.
	void DoorClose();

private:
	CSkinMesh*					m_pHouseMesh;			// 部屋メッシュ.
	CSkinMesh*					m_pDoorMesh;			// 扉メッシュ.
	CCollisionMesh*				m_pCollisionMesh;		// 当たり判定用メッシュ.
	std::unique_ptr<CStageIcon>	m_pIcon;				// アイコン.
	std::unique_ptr<CGarage>	m_pGarage;				// ガレージ.
	std::unique_ptr<CCustomer>	m_pCustomer;			// お客さん.
	SMeshPointLight				m_PointLight;			// ポイントライト.
	SMeshPointLight				m_CustomerLight;		// お客さんのポイントライトを初期化する用.
	STransform					m_CustomerTransform;	// お客さんのトランスフォーム.
	STransform					m_DoorTransform;		// 扉のトランスフォーム.
	D3DXROTATION3				m_CustomerAddRotation;	// お客さんの追加の角度.
	D3DXPOSITION3				m_StartPos;				// 開始座標.
	D3DXPOSITION3				m_EndPos;				// 終了座標.
	D3DXVECTOR3					m_MoveVector;			// 移動ベクトル.
	float						m_MoveCnt;				// 移動用のカウント.
	float						m_DoorMoveCnt;			// 扉用のカウント.
	float						m_Alpha;				// アルファ値.
	int							m_LightOnHour;			// ライトをつける時間.
	int							m_LightOnMinute;		// ライトをつける分.
	int							m_LightOffHour;			// ライトを消す時間.
	int							m_LightOffMinute;		// ライトを消す分.
	bool						m_IsRotEnd;				// 回転終了したか.
	bool						m_IsOpenEnd;			// 窓を開けたか.
	bool						m_IsCloseEnd;			// 窓を閉じたか.
	bool						m_IsLightOn;			// ライトをつけたか.
	bool						m_IsLightOff;			// ライトを消したか.
	bool						m_IsOpenSE;				// SEを再生したか.

private:
	// 外部データの定数.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};
