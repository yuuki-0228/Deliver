#include "SmokeManager.h"
#include "Smoke\Smoke.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"

CSmokeManager::CSmokeManager()
	: m_pSmokeList	()
	, m_TimeCnt		( 0.0f )
{
	Init();
}

CSmokeManager::~CSmokeManager()
{
}

//------------------------------------.
// 初期化.
//------------------------------------.
bool CSmokeManager::Init()
{
	m_TimeCnt = 0.0f;
	return true;
}

//------------------------------------.
// 更新.
//------------------------------------.
void CSmokeManager::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	for ( auto& s : m_pSmokeList ) s->Update( deltaTime );

	RenderOrder();		// 描画順番の並び変え.
	SmokeListDelete();	// 煙リストの削除.
	CoolTimeUpdate();	// クールタイムの更新.
}

//------------------------------------.
// 更新.
//------------------------------------.
void CSmokeManager::Render()
{
	for ( auto& s : m_pSmokeList ) s->Render();
}

//------------------------------------.
// アニメーションの開始.
//	待ち時間がある場合処理を行わない.
//------------------------------------.
void CSmokeManager::PlaySmoke( const D3DXPOSITION3& Pos, const D3DXVECTOR3& MoveVec, const D3DXVECTOR3& pAddMoveVec )
{
	if ( m_TimeCnt > 0.0f	) return;

	m_TimeCnt = CONSTANT.COOL_TIME;

	// 使用していないのがあれば使用する.
	for ( auto& s : m_pSmokeList ) {
		if ( s->IsAnimEnd() == false ) continue;

		// アニメーションを開始させる.
		s->AnimPlay( Pos, MoveVec, pAddMoveVec );
		return;
	}

	// 新しく作成.
	m_pSmokeList.emplace_back( std::make_unique<CSmoke>() );

	// アニメーションを開始させる.
	m_pSmokeList.back()->AnimPlay( Pos, MoveVec, pAddMoveVec );
}

//------------------------------------.
// 描画順番を並び変える.
//------------------------------------.
void CSmokeManager::RenderOrder()
{
	// カメラの座標を取得.
	const D3DXVECTOR3& CamPos = CCameraManager::GetPosition();

	// エフェクトとカメラとの距離を調べる.
	std::vector<std::pair<float, std::unique_ptr<CSmoke>>> LengthList;
	const int Size = static_cast<int>( m_pSmokeList.size() );
	for ( int i = 0; i < Size; i++ ){
		const D3DXVECTOR3 vLength = CamPos - m_pSmokeList[i]->GetPosition();
		LengthList.emplace_back( std::make_pair( D3DXVec3Length( &vLength ), std::move( m_pSmokeList[i] ) ) );
	}
	// カメラに遠い順に並べ替える.
	std::sort( LengthList.begin(), LengthList.end(), std::greater<>() );

	// エフェクトを並び変える.
	for ( int i = 0; i < Size; i++ )
		m_pSmokeList[i] = std::move( LengthList[i].second );
}

//------------------------------------.
// 煙リストの削除.
//------------------------------------.
void CSmokeManager::SmokeListDelete()
{
	for ( int i = static_cast<int>( m_pSmokeList.size() ) - 1; i >= 0; i-- ) {
		if ( m_pSmokeList[i]->IsAnimEnd() == false ) break;
		m_pSmokeList.pop_back();
	}
}

//------------------------------------.
// クールタイムの更新.
//	待ち時間がない場合処理を行わない.
//------------------------------------.
void CSmokeManager::CoolTimeUpdate()
{
	if ( m_TimeCnt == 0.0f ) return;
	m_TimeCnt -= m_DeltaTime;
	if ( m_TimeCnt <= 0.0f )
		m_TimeCnt = 0.0f;
}