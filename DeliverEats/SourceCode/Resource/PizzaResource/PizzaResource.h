#pragma once
#include "..\..\Global.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <filesystem>

/************************************************
*	ピザリソースクラス.
*		﨑田友輝.
**/
class CPizzaResource final
{
public:
	using Pizza_map		= std::unordered_map<std::string, json>;
	using Pizza_List	= std::vector<std::string>;

public:
	CPizzaResource();
	~CPizzaResource();

	// インスタンスの取得.
	static CPizzaResource* GetInstance();

	// ピザの読み込み.
	static HRESULT PizzaLoad();

	// ピザ取得関数.
	static json GetPizza( const std::string& FileName );

	// 保存しているピザリストの取得.
	static Pizza_List GetPizzaList() { return GetInstance()->m_PizzaNames; }

	// ピザデータの取得.
	static Pizza_map  GetPizzaData() { return GetInstance()->m_PizzaList; }

	// ピザの追加.
	static void PushPizzaData( const std::string& FileName, json j );
	// ピザの削除.
	static void PizzaDelete( const std::string& FileName );

private:
	Pizza_map	m_PizzaList;	// ピザリスト.
	Pizza_List	m_PizzaNames;	// ピザの名前リスト.
	std::mutex	m_Mutex;
	bool		m_IsLoadEnd;	// 読み込みが終わったか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CPizzaResource( const CPizzaResource& )					= delete;
	CPizzaResource& operator = ( const CPizzaResource& )	= delete;
	CPizzaResource( CPizzaResource&& )						= delete;
	CPizzaResource& operator = ( CPizzaResource&& )			= delete;
};