#include "Random.h"
#include <fstream>
#include <sstream>
#include "..\FileManager\FileManager.h"

namespace{
	constexpr char TEXT_PATH[] = "Data\\Parameter\\Config\\Random.json";
}

CRandom::CRandom()
	: m_rnd			()
	, m_mt			()
	, m_RandLock	( false )
{
}

CRandom::~CRandom()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CRandom* CRandom::GetInstance()
{
	static std::unique_ptr<CRandom> pInstance = std::make_unique<CRandom>();
	return pInstance.get();
}

//----------------------------.
// 乱数の初期化.
//----------------------------.
HRESULT CRandom::Init()
{
	// 乱数の初期化数字を取得.
	int InitNum = GetInstance()->m_rnd();

	// 乱数の固定化が有効なら.
	if ( GetInstance()->m_RandLock ){
		// 前回の初期化の値で初期化.
		const json& Text = FileManager::JsonLoad( TEXT_PATH );
		InitNum			 = Text["Init"];
	}
	else{
		// 乱数の初期化の値を保存.
		json j;
		j[".Comment"]	= u8"乱数を初期する値";
		j["Init"]		= InitNum;
		FileManager::JsonSave( TEXT_PATH, j );
	}

	// 乱数の初期化.
	std::mt19937 InitMt( InitNum );
	GetInstance()->m_mt = InitMt;

	CLog::PushLog( "乱数の初期化 : 成功" );
	return S_OK;
}

//----------------------------.
// ランダムな数値( float型 )を取得.
//	Min以上、Max未満を返す.
//----------------------------.
float CRandom::GetRand( float min, float max )
{
	// 下限と上限を指定しランダムで数値を返す.
	std::uniform_real_distribution<float>	Rand_Num( min, max );
	return Rand_Num( GetInstance()->m_mt );
}
float CRandom::GetRand( std::pair<float, float> Rand )
{
	return GetRand( Rand.first, Rand.second );
}

//----------------------------.
// ランダムな数値( int型 )を取得.
//	Min以上、Max以下を返す.
//----------------------------.
int CRandom::GetRand( int min, int max )
{
	// 下限と上限を指定しランダムで数値を返す.
	std::uniform_int_distribution<>	Rand_Num( min, max );
	return Rand_Num( GetInstance()->m_mt );
}
int CRandom::GetRand( std::pair<int, int> Rand )
{
	return GetRand( Rand.first, Rand.second );
}

//----------------------------.
// 確率( 割合 or 〇/〇 ).
//----------------------------.
bool CRandom::Probability( int Molecule, int Denominator )
{
	return Probability( static_cast<float>( Molecule ) / static_cast<float>( Denominator ) );
}
bool CRandom::Probability( float Molecule, float Denominator )
{
	return Probability( Molecule / Denominator );
}
bool CRandom::Probability( float Ratio )
{
	return static_cast<float>( GetRand( 1, 100 ) ) <= Ratio * 100.0f;
}
