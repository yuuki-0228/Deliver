#pragma once
#include "..\..\..\Global.h"

/************************************************
*	ドラック＆ドロップクラス.
*		﨑田友輝.
**/
class CDragAndDrop final
{
public:
	CDragAndDrop();
	~CDragAndDrop();

	// インスタンスの取得.
	static CDragAndDrop* GetInstance();

	// ドラック&ドロップを使用する.
	static void Open();
	// ドラック&ドロップを使用しない.
	static void Close();
	// ドラック&ドロップ終了.
	static void End() { GetInstance()->m_FilePathList.clear(); }
	
	// ファイルがドロップされたか.
	static bool GetIsDrop() { return !GetInstance()->m_FilePathList.empty(); }
	// ファイルパスを取得.
	static std::vector<std::string> GetFilePath() { return GetInstance()->m_FilePathList; }

	// ファイルパスを追加.
	static void AddFilePath( const std::string& FilePath ) { GetInstance()->m_FilePathList.emplace_back( FilePath ); }

	// ウィンドウハンドルを設定.
	static void SethWnd( const HWND& hWnd ) { GetInstance()->m_hWnd = hWnd; }

private:
	HWND						m_hWnd;			// ウィンドウハンドル.
	std::vector<std::string>	m_FilePathList;	// ファイルパスリスト.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDragAndDrop( const CDragAndDrop& )					= delete;
	CDragAndDrop( CDragAndDrop&& )						= delete;
	CDragAndDrop& operator = ( const CDragAndDrop& )	= delete;
	CDragAndDrop& operator = ( CDragAndDrop&& )			= delete;
};