#pragma once
#include "..\..\Global.h"
#include <nlohmnn\json.hpp>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// json型を作成.
using json = nlohmann::json;

/************************************************
*	ファイル関係一覧.
*		�ｱ田友輝.
**/
namespace FileManager {
	// テキストファイルの読み込み.
	//	"//" : コメントアウト.
	//	"#"  : 区切り文字(',')までコメントアウト.
	std::vector<std::string> TextLoad(
		const std::string&	FilePath,
		const bool			IsCommentOut = true,
		const char			Delim = ',' );

	// テキストファイルに書き込む.
	HRESULT TextSave( const std::string& FilePath, const std::string& Data );

	// バイナリデータでの読み込み.
	template<class T>
	HRESULT BinaryLoad( const char* FilePath, T& Out, const int& SeekPoint = 0 );
	template<class T>
	HRESULT BinaryLoad( const char* FilePath, std::vector<T>& OutList );

	// バイナリデータでの書き込む.
	template<class T>
	HRESULT BinarySave( const char* FilePath, const T& Data );
	template<class T>
	HRESULT BinarySave( const char* FilePath, const std::vector<T>& DataList );

	// json形式でjsonファイルを開く.
	json JsonLoad( const std::string& FilePath );

	// json形式をjsonファイルで書き込む.
	HRESULT JsonSave( const std::string& FilePath, const json& Data );

	// json を std::unordered_map に変換.
	std::unordered_map<std::string, std::string> JsonToMap( const json& Json );

	// std::unordered_map を json に変換.
	json MapToJson( const std::unordered_map<std::string, std::string> Map );
	json MapToJson( const std::unordered_map<std::string, std::vector<std::string>> Map );

	// ファイルディレクトリを作成する.
	HRESULT CreateFileDirectory( const std::string& FilePath );
}

//---------------------------.
// バイナリデータでの読み込み.
//---------------------------.
template<class T>
HRESULT FileManager::BinaryLoad( const char* FilePath, T& Out, const int& SeekPoint )
{
	// ファイルを開く.
	std::fstream o( FilePath, std::ios::in | std::ios::binary );
	if ( !o ) return E_FAIL;

	// 読み込み.
	o.seekg( SeekPoint * sizeof( T ) );
	o.read(  reinterpret_cast<char*>( std::addressof( Out ) ), sizeof( T ) );

	//ファイルを閉じる
	o.close();
	return S_OK;
}

//---------------------------.
// バイナリデータをvectorで読み込み.
//---------------------------.
template<class T>
HRESULT FileManager::BinaryLoad( const char* FilePath, std::vector<T>& OutList )
{
	// ファイルを開く.
	std::ifstream o( FilePath, std::ios::in | std::ios::binary );
	if ( !o ) return E_FAIL;

	// 初期化.
	OutList.clear();
	int Size = 0;

	// 読み込み.
	o.read( reinterpret_cast<char*>( &Size ), sizeof( Size ) );
	OutList.resize( Size );
	o.read( reinterpret_cast<char*>( &OutList[0] ), Size * sizeof( T ) );

	// ファイルを閉じる.
	o.close();
	return S_OK;
}

//---------------------------.
// バイナリデータでの書き込み.
//---------------------------.
template<class T>
HRESULT FileManager::BinarySave( const char* FilePath, const T& Data )
{
	// ファイルを開く.
	std::fstream i( FilePath, std::ios::out | std::ios::binary | std::ios::trunc );
	if ( !i ) {
		// 開けないためファイルディレクトリを作成する.
		FileManager::CreateFileDirectory( FilePath );

		// 書き込みなおす.
		FileManager::BinarySave<T>( FilePath, Data );
		return S_OK;
	};

	// 書き込む.
	i.write( reinterpret_cast<const char*>( std::addressof( Data ) ), sizeof( T ) );

	//ファイルを閉じる
	i.close();
	return S_OK;
}

//---------------------------.
// バイナリデータをvectorで書き込み.
//---------------------------.
template<class T>
HRESULT FileManager::BinarySave( const char* FilePath, const std::vector<T>& DataList )
{
	// ファイルを開く.
	std::ofstream i( FilePath, std::ios::out | std::ios::binary );
	if ( !i ) {
		// 開けないためファイルディレクトリを作成する.
		FileManager::CreateFileDirectory( FilePath );

		// 書き込みなおす.
		FileManager::BinarySave<T>( FilePath, DataList );
	};

	// 書き込み.
	const int& Size = DataList.size();
	i.write( reinterpret_cast<const char*>( &Size ), sizeof( Size ) );
	i.write( reinterpret_cast<const char*>( &DataList[0] ), Size * sizeof( T ) );

	// ファイルを閉じる.
	i.close();
	return S_OK;
}