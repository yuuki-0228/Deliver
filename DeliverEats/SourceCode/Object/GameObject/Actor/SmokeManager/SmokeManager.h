#pragma once
#include "..\Actor.h"

class CSmoke;

/************************************************
*	煙エフェクトマネージャークラス.
**/
class CSmokeManager final
	: public CActor
{
public:
	CSmokeManager();
	virtual ~CSmokeManager();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& deltaTime ) override;
	// 描画.
	virtual void Render() override;

	// 煙の発生.
	void PlaySmoke( const D3DXPOSITION3& Pos, const D3DXVECTOR3& MoveVec, const D3DXVECTOR3& pAddMoveVec = { 0.0f, 0.0f, 0.0f } );
	// 煙エフェクトが終了しているか.
	bool IsEffectEnd() { return m_pSmokeList.size() == 0; }

private:
	// 描画順番を並び変える.
	void RenderOrder();
	// 煙リストの削除.
	void SmokeListDelete();
	// クールタイムの更新.
	void CoolTimeUpdate();

private:
	std::vector<std::unique_ptr<CSmoke>>	m_pSmokeList;	// 煙リスト.
	float									m_TimeCnt;		// 次の煙が出るまでのクールタイム.

private:
	// 外部データの定数.
	const ConstantStruct::SSmoke& CONSTANT = CConstant::GetSmoke();
};