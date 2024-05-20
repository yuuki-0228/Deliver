#pragma once
#include "..\..\..\..\Actor.h"

class CApartmentEmptyRoom;
class CApartmentRoom;
class CApartmentShop;

/************************************************
*	マンションBパターン(8x2)の1フロアクラス.
*		﨑田友輝.
**/
class CBTypeApartmentFloor final
	: public CActor
{
public:
	using EmptyRoomList = std::vector<std::pair<std::unique_ptr<CApartmentEmptyRoom>, int>>;
	using RoomList		= std::vector<std::pair<std::unique_ptr<CApartmentRoom>, int>>;
	using ShopList		= std::vector<std::pair<std::unique_ptr<CApartmentShop>, int>>;

public:
	CBTypeApartmentFloor();
	virtual ~CBTypeApartmentFloor();

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

	// フロアの作成.
	void Create( json j );
	// フロアの移動.
	void FloorMove( const D3DXPOSITION3& NewPos );
	// 部屋の数を取得<店の数, <部屋の数, 空部屋の数>>.
	std::pair<int, std::pair<int, int>> GetRoomNum() const {
		return std::make_pair( static_cast<int>( m_pShopList.size() ), std::make_pair(
			static_cast<int>( m_pRoomList.size() ),
			static_cast<int>( m_pEmptyRoomList.size() ) 
		) );
	}

	// 上に設置する座標を取得する.
	D3DXPOSITION3 GetSettingPosition();

	// フロアのデータの取得.
	json GetFloorData();

	// アルファの設定.
	void SetAlpha( const float& Alpha );
	// 空のフロアに設定.
	void SetEmptyFloor() { m_IsEmptyFloor = true; }

private:
	// ライトの更新.
	void LightUpdate();

private:
	CSkinMesh*		m_pApartmentMesh;	// マンション(1階)のメッシュ.
	SMeshPointLight	m_PointLight;		// ポイントライト.
	EmptyRoomList	m_pEmptyRoomList;	// 空部屋リスト.
	RoomList		m_pRoomList;		// 部屋リスト.
	ShopList		m_pShopList;		// お店リスト.
	float			m_Alpha;			// アルファ値.
	int				m_LightHour;		// ライトをつける時間.
	int				m_LightMinute;		// ライトをつける分.
	bool			m_IsEmptyFloor;		// 空のフロアか.
	bool			m_IsLightOn;		// ライトをつけたか.

private:
	// 外部データの定数.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
