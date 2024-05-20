#pragma once
#include "..\SystemBase.h"
#include "..\..\Utility\Input\KeyBind.h"

/************************************************
*	入力設定クラス.
*		﨑田友輝.
**/
class CInputSetting final
	: public CSystemBase
{
public:
	CInputSetting();
	~CInputSetting();

protected:
	// 初期化関数.
	virtual void Init() override;

private:
	KeyBind_map	m_KeyBindList;	// キーバインドのリスト.
	std::string	m_ActionName;	// アクション名.
};
