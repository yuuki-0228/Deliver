#include "LoadRender.h"
#include "..\..\..\Common\DirectX\DirectX11.h"
#include "..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\Utility\FrameRate\FrameRate.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Utility\Random\Random.h"

namespace{
	constexpr int	TEXT_NUM					= 13;							// テキストの表示数.
	constexpr int	TEXT_MAX					= 10;							// テキストの上限.
	constexpr float	TEXT_GAP					= 20.0f;						// テキストの隙間.
	constexpr float	TEXT_JUMP_Y					= 20.0f;						// テキストのジャンプさせる高さ.
	constexpr float	TEXT_ANIM_SPEED				= 5.0f;							// テキストのアニメーションの速度.
	constexpr int	CLOUD_SPOWN_TIME			= 50;							// 雲を召喚していく時間.
	constexpr float	CLOUD_SPOWN_POS_X			= 20.0f;						// 雲を召喚するX座標.
	constexpr float	CLOUD_DELETE_POS_X			= FWND_W - CLOUD_SPOWN_POS_X;	// 雲を削除するX座標.
	constexpr float	CLOUD_SPOWN_POS_Y_MIN		= 50.0f;						// 雲を召喚するY座標の最小値.
	constexpr float	CLOUD_SPOWN_POS_Y_MAX		= 620.0f;						// 雲を召喚するY座標の最大値.
	constexpr float	CLOUD_START_MOVE_SPPED		= 5.0f;							// 雲の開始時の移動速度.
	constexpr float	CLOUD_END_MOVE_SPPED		= 14.0f;						// 雲の最終の移動速度.
	constexpr float	CLOUD_RADIUS				= 8.0f;							// 雲を当たり判定の半径.
	constexpr float	CLOUD_AVERT_RADIUS			= 23.0f;						// 雲のギリギリを避けたか調べる当たり判定の半径.
	constexpr float GRAVITY_POWER				= 0.4f;							// 重力の強さ.
	constexpr float	PLAYER_POS_X				= 1000.0f;						// プレイヤーのX座標.
	constexpr float	PLAYER_FIRE_POS_X			= 37.0f;						// プレイヤーの炎のX座標.
	constexpr float	PLAYER_FIRE_POS_Y			= 11.0f;						// プレイヤーの炎のY座標.
	constexpr float	PLAYER_RADIUS				= 23.0f;						// プレイヤーの当たり判定の半径.
	constexpr float	PLAYER_AVERT_RADIUS			= 38.0f;						// プレイヤーのギリギリを避けたか調べる当たり判定の半径.
	constexpr float	PLAYER_ROT_SPEED			= 6.0f;							// プレイヤーの雲に当たった時のアニメーションの回転速度.
	constexpr int	COOL_TIME					= 10;							// 当たり判定のクールタイム.
	constexpr float	CEILING_POS_Y				= 100.0f;						// 天井のY座標.
	constexpr float	GROUND_POS_Y				= 550.0f;						// 地面のY座標.
	constexpr float	FLIGHT_ANIM_SIZE			= 20.0f;						// 飛行アニメーション用のサイズ.
	constexpr char	HIT_SE[]					= "MiniGameHit";				// 雲に当たった時のSE.
	constexpr char	AVERT_SE[]					= "MiniGameAvert";				// ギリギリを避けた時のSE.
}

CLoadRender::CLoadRender()
	: m_pText				( nullptr )
	, m_pPlayer				( nullptr )
	, m_pPlayerFire			( nullptr )
	, m_pPlayerIcon			( nullptr )
	, m_pCloud				( nullptr )
	, m_pLRTrigger			( nullptr )
	, m_TextStateList		( TEXT_NUM )
	, m_CloudStateList		()
	, m_CloudStateBase		()
	, m_PlayerState			()
	, m_PlayerFireState		()
	, m_PlayerIconState		()
	, m_LRTriggerState		()
	, m_AnimTimeCnt			( INIT_FLOAT )
	, m_Power				( INIT_FLOAT )
	, m_PlayerFireCnt		( INIT_FLOAT )
	, m_CloudMoveSpeed		( CLOUD_START_MOVE_SPPED )
	, m_CloudTimeCnt		( INIT_INT )
	, m_TextAnimNo			( INIT_INT )
	, m_CoolTime			( INIT_INT )
	, m_IsCloudHit			( false )
	, m_LeftRotation		( false )
{
	m_pText			= std::make_unique<CSprite>();
	m_pPlayer		= std::make_unique<CSprite>();
	m_pPlayerFire	= std::make_unique<CSprite>();
	m_pPlayerIcon	= std::make_unique<CSprite>();
	m_pCloud		= std::make_unique<CSprite>();
	m_pLRTrigger	= std::make_unique<CSprite>();
}

CLoadRender::~CLoadRender()
{
}

//---------------------------.
// 読み込み.
//---------------------------.
HRESULT CLoadRender::Load()
{
	if ( m_pText->Init(			"Data\\Sprite\\Scene\\Loading\\LoadText.png"		) ) return E_FAIL;
	if ( m_pPlayer->Init(		"Data\\Sprite\\Scene\\Loading\\LoadPlayer.png"		) ) return E_FAIL;
	if ( m_pPlayerFire->Init(	"Data\\Sprite\\Scene\\Loading\\LoadPlayerFire.png"	) ) return E_FAIL;
	if ( m_pPlayerIcon->Init(	"Data\\Sprite\\Scene\\Loading\\LoadPlayerIcon.png"	) ) return E_FAIL;
	if ( m_pCloud->Init(		"Data\\Sprite\\Scene\\Loading\\LoadCloud.png"		) ) return E_FAIL;
	if ( m_pLRTrigger->Init(	"Data\\Sprite\\Scene\\Loading\\LoadLRTrigger.png"	) ) return E_FAIL;

	// 文字の設定.
	const SSize&				TextSize	= m_pText->GetSpriteState().Disp;
	const SSpriteRenderState&	TextState	= m_pText->GetRenderState();
	for ( int i = 0; i < TEXT_NUM; ++i ) {
		const int No = i > TEXT_MAX ? TEXT_MAX : i;
		m_TextStateList[i] = TextState;
		m_TextStateList[i].AnimState.IsSetAnimNumber	= true;
		m_TextStateList[i].AnimState.AnimNumber			= No;
		m_TextStateList[i].Transform.Position.x			= FWND_W - TextSize.w * ( TEXT_NUM - i ) - TEXT_GAP;
		m_TextStateList[i].Transform.Position.y			= FWND_H - TextSize.h - TEXT_GAP;
	}

	// 操作説明のテキストの位置の設定.
	m_LRTriggerState = m_pLRTrigger->GetRenderState();
	m_LRTriggerState.Transform.Position.x = TEXT_GAP;
	m_LRTriggerState.Transform.Position.y = FWND_H - TEXT_GAP;

	// 雲の設定.
	const SSize& CloudSize = m_pCloud->GetSpriteState().Disp;
	m_CloudStateBase								= m_pCloud->GetRenderState();
	m_CloudStateBase.AnimState.IsSetAnimNumber		= true;
	m_CloudStateBase.RenderArea = {
		CLOUD_SPOWN_POS_X + CloudSize.w / 2.0f,
		0.0f,
		CLOUD_DELETE_POS_X - CLOUD_SPOWN_POS_X - CloudSize.w,
		FWND_H
	};

	// プレイヤーの設定.
	m_PlayerState = m_pPlayer->GetRenderState();
	m_PlayerState.Transform.Position.x		= PLAYER_POS_X;
	m_PlayerState.Transform.Position.y		= GROUND_POS_Y;
	m_PlayerFireState.Transform.AddParent( &m_PlayerState.Transform );
	m_PlayerFireState.Transform.Position.x	= PLAYER_FIRE_POS_X;
	m_PlayerFireState.Transform.Position.y	= PLAYER_FIRE_POS_Y;

	// プレイヤーのアイコンの設定.
	m_PlayerIconState.Transform.Position.x = TEXT_GAP;
	m_PlayerIconState.Transform.Position.y = m_PlayerState.Transform.Position.y;

	// 背景の色の設定.
	CDirectX11::SetBackColor( { 0.0f, 0.0f, 0.0f, 0.0f } );
	return S_OK;
}

//---------------------------.
// 更新.
//---------------------------.
void CLoadRender::Update( const float& DeltaTime )
{
	CloudSpown();				// 雲の召喚.
	CloudMove();				// 雲の移動.
	CloudDelete();				// 使用していない雲の削除.
	TextAnimation();			// テキストのアニメーション.
	PlayerJump();				// プレイヤーのジャンプ.
	PlayerFall();				// プレイヤーの落下.
	GroundCollision();			// 地面の当たり判定.
	CeilingCollision();			// 天井の当たり判定.
	CloudCollision();			// 雲の当たり判定.
	PlayerCloudHitAnimation();	// プレイヤーの雲に当たった時のアニメーション.
	PlayerPositionUpdate();		// プレイヤーの座標の更新.
	PlayerFireUpdate();			// プレイヤーの炎の更新.
	CoolTimeUpdate();			// クールタイムの更新.

	// BGMの再生.
	CSoundManager::PlayBGM( "MiniGame" );
}

//---------------------------.
// 描画.
//---------------------------.
void CLoadRender::Render()
{
	CDirectX11::SetDepth( false );
	for( auto& t : m_TextStateList	) m_pText->RenderUI( &t );
	m_pLRTrigger->RenderUI( &m_LRTriggerState );
	m_pPlayer->RenderUI( &m_PlayerState );
	m_pPlayerFire->RenderUI( &m_PlayerFireState );
	m_pPlayerIcon->RenderUI( &m_PlayerIconState );
	for( auto& c : m_CloudStateList ) m_pCloud->RenderUI( &c );
	CDirectX11::SetDepth( true );
}

//---------------------------.
// 雲の召喚.
//---------------------------.
void CLoadRender::CloudSpown()
{
	// 雲を召喚するか.
	m_CloudTimeCnt++;
	if ( m_CloudTimeCnt <= CLOUD_SPOWN_TIME / ( m_CloudMoveSpeed / CLOUD_START_MOVE_SPPED ) ) return;
	m_CloudTimeCnt = 0;

	// 雲を召喚する.
	const int NoMax		= m_CloudStateBase.AnimState.AnimNumberMax - 1;
	const int SpownMax	= CRandom::Probability( 1, 2 ) ? 2 : CRandom::GetRand( 3, 4 );
	const int SpownNum	= CRandom::GetRand( 2, SpownMax );
	for ( int i = 0; i < SpownNum; ++i ) {
		// 使用していないのがあれば使用する.
		for ( auto& c : m_CloudStateList ) {
			if ( c.IsDisp ) continue;

			// 雲を召喚する.
			c.IsDisp				= true;
			c.Transform.Position.x	= CLOUD_SPOWN_POS_X;
			c.Transform.Position.y	= CRandom::GetRand( CLOUD_SPOWN_POS_Y_MIN, CLOUD_SPOWN_POS_Y_MAX );
			c.AnimState.AnimNumber	= CRandom::GetRand( 0, NoMax );
			return;
		}

		// 新しく作成.
		m_CloudStateList.emplace_back( m_CloudStateBase );
		m_CloudStateList.back().Transform.Position.x	= CLOUD_SPOWN_POS_X;
		m_CloudStateList.back().Transform.Position.y	= CRandom::GetRand( CLOUD_SPOWN_POS_Y_MIN, CLOUD_SPOWN_POS_Y_MAX );
		m_CloudStateList.back().AnimState.AnimNumber	= CRandom::GetRand( 0, NoMax );
	}
}

//---------------------------.
// 雲の移動.
//---------------------------.
void CLoadRender::CloudMove()
{
	// 雲の移動速度を上げる.
	if ( m_CloudMoveSpeed < CLOUD_END_MOVE_SPPED ) {
		m_CloudMoveSpeed += 0.01f;
		if ( m_CloudMoveSpeed >= CLOUD_END_MOVE_SPPED ) m_CloudMoveSpeed = CLOUD_END_MOVE_SPPED;
	}

	for ( auto& c : m_CloudStateList ) {
		if ( c.IsDisp == false ) continue;

		// 雲の移動.
		c.Transform.Position.x += m_CloudMoveSpeed;

		// 移動が終了したか.
		if ( c.Transform.Position.x >= CLOUD_DELETE_POS_X ) {
			c.IsDisp = false;
		}
	}
}

//---------------------------.
// 使用していない雲の削除.
//---------------------------.
void CLoadRender::CloudDelete()
{
	const int Size = static_cast<int>( m_CloudStateList.size() );
	for ( int i = Size - 1; i >= 0; i-- ) {
		if ( m_CloudStateList[i].IsDisp ) break;
		m_CloudStateList.pop_back();
	}
}

//---------------------------.
// テキストのアニメーション.
//---------------------------.
void CLoadRender::TextAnimation()
{
	// 文字のアニメーションの更新.
	m_AnimTimeCnt += TEXT_ANIM_SPEED * ( m_CloudMoveSpeed / CLOUD_START_MOVE_SPPED );

	// 位置の更新.
	const SSize Size = m_pText->GetSpriteState().Disp;
	m_TextStateList[m_TextAnimNo].Transform.Position.y = 
		FWND_H - Size.h - TEXT_GAP - TEXT_JUMP_Y * sinf( Math::ToRadian( m_AnimTimeCnt ) );
	 
	// 次の文字に移動.
	if ( m_AnimTimeCnt >= Math::DEGREE_MID ) {
		m_AnimTimeCnt = 0.0f;
		m_TextAnimNo++;
		if ( m_TextAnimNo >= TEXT_NUM ) m_TextAnimNo = 0;
	}
}

//---------------------------.
// プレイヤーのジャンプ.
//---------------------------.
void CLoadRender::PlayerJump()
{
	if ( m_IsCloudHit ) return;

	// プレイヤーのジャンプ.
	if ( m_PlayerState.Transform.Position.y <= CEILING_POS_Y + FLIGHT_ANIM_SIZE ) return;

	// トリガーの押し具合の取得.
	const float LTrigger = CXInput::GetLTrigger();
	const float RTrigger = CXInput::GetRTrigger();

	// プレイヤーのパワーの更新.
	m_Power -= ( LTrigger + RTrigger + 1.1f ) / 5.0f;
}

//---------------------------.
// プレイヤーの落下.
//---------------------------.
void CLoadRender::PlayerFall()
{
	// プレイヤーの重力.
	if ( m_PlayerState.Transform.Position.y >= GROUND_POS_Y - FLIGHT_ANIM_SIZE ) return;
	
	// プレイヤーを落下させていく.
	m_Power += GRAVITY_POWER;
}

//---------------------------.
// プレイヤーの雲に当たった時のアニメーション.
//---------------------------.
void CLoadRender::PlayerCloudHitAnimation()
{
	if ( m_IsCloudHit == false ) return;

	// プレイヤーを回転させる.
	const float RotSpeed = m_LeftRotation ? -PLAYER_ROT_SPEED : PLAYER_ROT_SPEED;
	m_PlayerState.Transform.Rotation.z += Math::ToRadian( RotSpeed );

	// アニメーション終了.
	if ( ( m_PlayerState.Transform.Rotation.z <= -Math::RADIAN_MAX ) || 
		 ( m_PlayerState.Transform.Rotation.z >=  Math::RADIAN_MAX ) )
	{
		m_PlayerState.Transform.Rotation.z	= 0.0f;
		m_IsCloudHit						= false;
		m_LeftRotation						= false;
		m_CoolTime							= COOL_TIME;
	}
}

//---------------------------.
// プレイヤーの座標の更新.
//---------------------------.
void CLoadRender::PlayerPositionUpdate()
{
	m_PlayerState.Transform.Position.y		+= m_Power;
	m_PlayerIconState.Transform.Position.y	 = m_PlayerState.Transform.Position.y;
}

//---------------------------.
// プレイヤーの炎の更新.
//---------------------------.
void CLoadRender::PlayerFireUpdate()
{
	// アニメーション用のカウントの加算.
	m_PlayerFireCnt += Math::ToRadian( 3.0f );
	if ( m_PlayerFireCnt >= Math::RADIAN_MID ) m_PlayerFireCnt = 0.0f;

	// 炎の拡縮.
	const float Scale = STransform::NORMAL_SCALE - 0.3f * sinf( m_PlayerFireCnt );
	m_PlayerFireState.Transform.Scale = { Scale, Scale, Scale };
}

//---------------------------.
// 地面の当たり判定.
//---------------------------.
void CLoadRender::GroundCollision()
{
	if ( m_PlayerState.Transform.Position.y < GROUND_POS_Y ) return;
	
	// 地面の位置に止める.
	m_PlayerState.Transform.Position.y	= GROUND_POS_Y;
	if ( m_Power > 0.0f ) m_Power = 0.0f;
}

//---------------------------.
// 天井の当たり判定.
//---------------------------.
void CLoadRender::CeilingCollision()
{
	if ( m_PlayerState.Transform.Position.y > CEILING_POS_Y ) return;

	// 天井の位置で止める.
	m_PlayerState.Transform.Position.y = CEILING_POS_Y;
	if ( m_Power < 0.0f ) m_Power = 0.0f;
}

//---------------------------.
// 雲の当たり判定.
//---------------------------.
void CLoadRender::CloudCollision()
{
	if ( m_IsCloudHit	) return;
	if ( m_CoolTime > 0 ) return;

	// プレイヤーの座標の取得.
	const D3DXPOSITION3 PPos = m_PlayerState.Transform.Position;

	// 雲と当たっているか調べる.
	for ( auto& c : m_CloudStateList ) {
		if ( c.IsDisp == false ) continue;

		// 雲の座標の取得.
		const D3DXPOSITION3& CPos = c.Transform.Position;

		// 円通しが当たっているか調べる.
		const float dx	= PPos.x - CPos.x;
		const float dy	= PPos.y - CPos.y;
		const float dr	= dx * dx + dy * dy;
		const float ar	= PLAYER_RADIUS + CLOUD_RADIUS;
		const float br	= PLAYER_AVERT_RADIUS + CLOUD_AVERT_RADIUS;
		const float dla = ar * ar;
		const float dlb = br * br;

		// 当たっているか.
		if ( dr < dla ) {
			m_IsCloudHit = true;
			if ( CPos.y >= PPos.y ) m_LeftRotation = true;

			// 雲の速度を遅くする.
			if ( m_CloudMoveSpeed > CLOUD_START_MOVE_SPPED ) {
				m_CloudMoveSpeed /= 2.0f;
				if ( m_CloudMoveSpeed <= CLOUD_START_MOVE_SPPED ) m_CloudMoveSpeed = CLOUD_START_MOVE_SPPED;
			}

			// SEの再生.
			CSoundManager::PlaySE( HIT_SE );
			return;
		}

		// ギリギリを避けたか.
		if ( dr < dlb ) {
			// SEの再生.
			CSoundManager::PlaySE( AVERT_SE, 0.05f );
		}
	}
}

//---------------------------.
// クールタイムの更新.
//---------------------------.
void CLoadRender::CoolTimeUpdate()
{
	if ( m_CoolTime <= 0 ) return;

	// クールタイムを減らす.
	m_CoolTime--;
	if ( m_CoolTime <= 0 ) m_CoolTime = 0;
}
