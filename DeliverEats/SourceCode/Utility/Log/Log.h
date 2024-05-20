#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <time.h>

/************************************************
*	ログクラス.
*		﨑田友輝.
**/
class CLog final
{
public:
	CLog();
	~CLog();

	// インスタンスの取得.
	static CLog* GetInstance();

	// ログテキストを作成.
	//	アプリ起動時に作成する.
	static HRESULT OpenLogText();

	// ログテキストを閉じる.
	static HRESULT CloseLogText();

	// ログファイルの破棄.
	static HRESULT DeleteLogText();

	// ログの入力.
	static HRESULT PushLog( const char*			Log );
	static HRESULT PushLog( const std::string&	Log );

private:
	bool m_Stop;	// ログを停止するか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CLog( const CLog& )					= delete;
	CLog& operator = ( const CLog& )	= delete;
	CLog( CLog&& )						= delete;
	CLog& operator = ( CLog&& )			= delete;
};
