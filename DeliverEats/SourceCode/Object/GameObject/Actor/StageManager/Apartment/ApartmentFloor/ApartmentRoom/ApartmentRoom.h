#pragma once
#include "..\..\..\..\Actor.h"

class CCustomer;
class CStageIcon;

/************************************************
*	マンションタイプの部屋クラス.
*		﨑田友輝.
**/
class CApartmentRoom final
	: public CActor
{
public:
	CApartmentRoom();
	virtual ~CApartmentRoom();

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

	// 表示するか.
	void SetIsDisp( const bool Flag ) { m_FloorIsDisp = Flag; }
	// アルファの設定.
	void SetAlpha( const float Alpha );

private:
	// 部屋の向きを取得.
	EDirection GetRoomDirection();

	// アイコンの更新.
	void IconUpdate();
	// ライトの更新.
	void LightUpdate();
	// ライトをつける.
	void LightOn( const std::pair<int, int>& Time );
	// ライトを消す.
	void LightOff( const std::pair<int, int>& Time );

	// お客さんの更新.
	void CustomerUpdate();
	// お客さんの表示するための移動.
	void CustomerDispMove();
	// お客さんがプレイヤーの方を見るように回転.
	void CustomerPlayerLook();
	// お客さんを非表示にするための移動.
	void CustomerHideMove();
	// お客さんの回転.
	void CustomerRotation();

	// 窓を開ける.
	void WindowOpen();
	// 窓を閉じたか.
	void WindowClose();

private:
	CSkinMesh*					m_pRoomMesh;			// 部屋メッシュ.
	CSkinMesh*					m_pWindowMesh;			// 窓メッシュ.
	std::unique_ptr<CStageIcon>	m_pIcon;				// アイコン.
	std::unique_ptr<CCustomer>	m_pCustomer;			// お客さん.
	SMeshPointLight				m_PointLight;			// ポイントライト.
	STransform					m_CustomerTransform;	// お客さんのトランスフォーム.
	STransform					m_WindowTransform;		// 窓のトランスフォーム.
	D3DXROTATION3				m_CustomerAddRotation;	// お客さんの追加の角度.
	D3DXPOSITION3				m_StartPos;				// 開始座標.
	D3DXPOSITION3				m_EndPos;				// 終了座標.
	D3DXVECTOR3					m_MoveVector;			// 移動ベクトル.
	D3DXPOSITION3				m_WindowStartPos;		// 窓の開始座標.
	D3DXPOSITION3				m_WindowEndPos;			// 窓の終了座標.
	D3DXVECTOR3					m_WindowMoveVector;		// 窓の移動ベクトル.
	float						m_MoveCnt;				// 移動用のカウント.
	float						m_WindowMoveCnt;		// 窓移動用のカウント.
	float						m_Alpha;				// アルファ値.
	int							m_LightOnHour;			// ライトをつける時間.
	int							m_LightOnMinute;		// ライトをつける分.
	int							m_LightOffHour;			// ライトを消す時間.
	int							m_LightOffMinute;		// ライトを消す分.
	bool						m_FloorIsDisp;			// 1フロア単位の表示するかどうか.
	bool						m_IsRotEnd;				// 回転終了したか.
	bool						m_IsOpenEnd;			// 窓を開けたか.
	bool						m_IsCloseEnd;			// 窓を閉じたか.
	bool						m_IsLightOn;			// ライトをつけたか.
	bool						m_IsLightOff;			// ライトを消したか.
	bool						m_IsOpenSE;				// SEを再生したか.

private:
	// 外部データの定数.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
