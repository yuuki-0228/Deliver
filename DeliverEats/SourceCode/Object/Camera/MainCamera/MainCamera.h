#pragma once
#include "..\CameraBase.h"
#include "..\..\..\Utility\Constant\Constant.h"

/************************************************
*	メインのカメラクラス.
*		井谷凌.
**/
class CMainCamera final
	: public CCameraBase
{
public:
	CMainCamera();
	~CMainCamera();

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 最終的な角度を返す.
	float GetRotX() { return m_RotX; }

	// 渡しに行くべき客を設定する.
	void SetCustomer(CObject* pCustomer ) { m_pCustomer = pCustomer; }

	// 注視カメラから切り替わった際に呼び出す.
	void StartMove();

	// 切り替え中か取得する.
	bool GetIsSwitching() { return m_IsSwitching; }
private:
	// カメラを元の角度に戻す処理.
	void UpdateReturnRotation( float& RotTheta, const float Rot = 0.0f );
	// カメラが元の角度に戻った後行う処理.
	void InitRotation( const float Rot = 0.0f );

	// Rスティックでの操作処理.
	void Rotate();

	// 注視カメラから切り替わった時の処理.
	void MoveFromGazingCamera( const D3DXVECTOR3 GoalPos, const float& DeltaTime );

private:
	float			m_Theta;					// Rスティックの入力で加わる値.
	float			m_RotX;						// カメラの最終的な角度.

	// 注視カメラから切り替わった際に使用.
	CObject*		m_pCustomer;				// 渡しに行くべき客.(注視から切り替わった際に使用).
	bool			m_IsSwitching;				// 注視カメラから切り替え中か？
	float			m_Count;					// 移動中に使用するカウント.
	D3DXPOSITION3	m_StartPosition;			// 移動開始位置.
	D3DXPOSITION3	m_EndPosition;				// 移動終了位置.

	bool			m_IsReturnDefaultPos;		// 元の角度に戻ろうとしているか?
	float			m_ReturnValue;				// 元の角度に戻すために加減算する値.
	float			m_AddValue;					// m_ReturnValueに加算する値.

	float			m_OldTarPosY;				// 1F前のターゲットのY座標.

	bool			m_IsCompleteReturnRStick;	// Rスティックが元の位置にいるか?
	float			m_OldRTrigger;				// 1F前のRスティックの入力値.

private:
	// 外部データの定数.
	const ConstantStruct::SCamera& CONSTANT = CConstant::GetCamera();
};
