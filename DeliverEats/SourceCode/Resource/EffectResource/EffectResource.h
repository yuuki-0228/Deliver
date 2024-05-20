#pragma once
#include "..\..\Global.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <filesystem>

class CEffect;

/************************************************
*	エフェクトリソースクラス.
*		﨑田友輝.
**/
class CEffectResource final
{
public:
	using Effect_map	= std::unordered_map<std::string, std::unique_ptr<CEffect>>;
	using Effect_List	= std::vector<std::string>;

public:
	CEffectResource();
	~CEffectResource();

	// インスタンスの取得.
	static CEffectResource* GetInstance();

	// エフェクトの読み込み.
	static HRESULT EffectLoad();

	// エフェクト取得関数.
	static CEffect* GetEffect( const std::string& FileName );

	// 保存しているエフェクトリストの取得.
	static Effect_List GetEffectList() { return GetInstance()->m_EffectNames; }

private:
	Effect_map	m_EffectList;	// エフェクトリスト.
	Effect_List	m_EffectNames;	// エフェクトの名前リスト.
	std::mutex	m_Mutex;
	bool		m_IsLoadEnd;	// 読み込みが終わったか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CEffectResource( const CEffectResource& )				= delete;
	CEffectResource& operator = ( const CEffectResource& )	= delete;
	CEffectResource( CEffectResource&& )					= delete;
	CEffectResource& operator = ( CEffectResource&& )		= delete;
};