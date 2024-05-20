#pragma once
#include "..\..\..\..\Actor.h"

/************************************************
*	マンションタイプの空部屋クラス.
*		﨑田友輝.
**/
class CApartmentEmptyRoom final
	: public CActor
{
public:
	CApartmentEmptyRoom();
	virtual ~CApartmentEmptyRoom();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// 表示するか.
	void SetIsDisp( const bool Flag ) { m_FloorIsDisp = Flag; }
	// アルファの設定.
	void SetAlpha( const float Alpha ) { m_Alpha = Alpha; };

private:
	// ライトの更新.
	void LightUpdate();
	// ライトをつける.
	void LightOn( const std::pair<int, int>& Time );
	// ライトを消す.
	void LightOff( const std::pair<int, int>& Time );

private:
	CSkinMesh*					m_pRoomMesh;		// 部屋メッシュ.
	SMeshPointLight				m_PointLight;		// ポイントライト.
	float						m_Alpha;			// アルファ値.
	int							m_LightOnHour;		// ライトをつける時間.
	int							m_LightOnMinute;	// ライトをつける分.
	int							m_LightOffHour;		// ライトを消す時間.
	int							m_LightOffMinute;	// ライトを消す分.
	bool						m_FloorIsDisp;		// 1フロア単位の表示するかどうか.
	bool						m_IsLightOn;		// ライトをつけたか.
	bool						m_IsLightOff;		// ライトを消したか.
	bool						m_IsEmptyRoom;		// 人が住んでいないか.

private:
	// 外部データの定数.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
