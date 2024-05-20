#pragma once
#include "..\..\UIWidget.h"

class CPlayer;
class CShopInfoUI;
class CEmployee;

/************************************************
*	お店の詳細情報マネージャークラス.
**/
class CShopInfoManagerUI final
	: public CUIWidget
{
public:
	CShopInfoManagerUI();
	~CShopInfoManagerUI();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーの設定.
	void SetPlayer( CPlayer* pPlayer );

private:
	// 表示の更新.
	void OpenUpdate();
	// 非表示の更新.
	void CloseUpdate();
	// 詳細情報の変更の更新.
	void ChangeUpdate();
	
	// クールタイムの更新.
	void CoolTimeUpdate();

private:
	CPlayer*									m_pPlayer;				// プレイヤー.
	std::vector<std::unique_ptr<CShopInfoUI>>	m_pShopInfoList;		// お店の詳細情報リスト.
	std::vector<CEmployee*>						m_pDispEmployeeList;	// 表示している店員リスト.
	int											m_DispNum;				// 表示している数.
	float										m_CoolTime;				// クールタイム.

private:
	// 外部データの定数.
	const ConstantStruct::SShopInfo& CONSTANT = CConstant::GetShopInfo();
};
