#pragma once
#include "..\CameraBase.h"
#include "..\..\..\Utility\Constant\Constant.h"

/************************************************
*	注視カメラクラス.
*		井谷凌.
**/
class CGazingCamera final
	: public CCameraBase
{
public:
	CGazingCamera();
	virtual ~CGazingCamera();

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// プレイヤーを設定.
	void SetPlayer( CObject* obj ) { m_pPlayer = obj; }

	// メインカメラから切り替わった際に呼び出す.
	void StartMove();

	// メインカメラの傾きを取得する.
	void SetRotX( const float RotX ) { m_RotX = RotX; }

	// 切り替え中か取得する.
	bool GetIsSwitching() { return m_IsSwitching; }
private:
	// メインカメラから切り替わった時の処理.
	void MoveFromMainCamera( const D3DXVECTOR3 GoalPos, const float& DeltaTime );

private:
	CObject*		m_pPlayer;			// プレイヤー.

	// メインカメラから切り替わった際に使用.
	bool			m_IsSwitching;		// メインカメラから切り替え中か？
	D3DXPOSITION3	m_StartPosition;	// 開始位置.
	float			m_Count;			// 移動中に使用するカウント.
	float			m_RotX;				// 疑似的なメインカメラの再現に使用.

private:
	// 外部データの定数.
	const ConstantStruct::SCamera& CONSTANT = CConstant::GetCamera();
};