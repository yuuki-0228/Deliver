#include "NotificationUI.h"
#include "..\..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Resource\FontResource\FontResource.h"

namespace {
	// 通知のNo.
	enum enNotificationNo : unsigned char {
		Now,	// 今の通知のスプライト.
		Next,	// 次の通知のスプライト.

		Notificatione_Max
	} typedef ENotificationNo;

	// フォントNo.
	enum enFontNo : unsigned char {
		Name,		// アイコンの名前.
		Message,	// メッセージ.
		Time,		// 送られてきた時間.

		Font_Max
	} typedef EFontNo;

	// アイコンNo.
	enum enIconNo : unsigned char {
		DeliverEatsIcon,	// お客さんの評価やランキングの更新用アイコン.
		NewsIcon,			// 風が発生した時用のアイコン.
		MessageIcon,		// ゴールデンタイムや残り時間用アイコン.
		EmergencyIcon,		// マップ外の警告用アイコン.

		Icon_Max
	} typedef EIconNo;
}

CNotificationUI::CNotificationUI()
	: m_pBackSprite		( nullptr )
	, m_pIconSprite		( nullptr )
	, m_pFont			( nullptr )
	, m_BackState		( Notificatione_Max )
	, m_IconState		( Notificatione_Max )
	, m_FontState		( Notificatione_Max )
	, m_IconNoList		()
	, m_IconNameList	()
	, m_IconSEList		()
	, m_MessageList		()
	, m_Message			( Notificatione_Max, SMessage() )
	, m_IsOpen			( Notificatione_Max, false )
	, m_MessageScale	( Notificatione_Max, STransform::NORMAL_SCALE_VEC3 )
	, m_MessageAlpha	( Notificatione_Max, Color::ALPHA_MAX )
	, m_IsClose			( false )
	, m_IsChangeClose	( false )
	, m_IsChange		( false )
	, m_IsDisp			( false )
	, m_DispTimeCnt		( INIT_FLOAT )
{
}

CNotificationUI::~CNotificationUI()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CNotificationUI* CNotificationUI::GetInstance()
{
	static std::unique_ptr<CNotificationUI> pInstance = std::make_unique<CNotificationUI>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
bool CNotificationUI::Init()
{
	CNotificationUI* pI = GetInstance();

	// 画像の取得.
	pI->m_pBackSprite	= CSpriteResource::GetSprite( "NotificationBack" );
	pI->m_pIconSprite	= CSpriteResource::GetSprite( "NotificationIcon" );

	// 画像の情報の取得.
	for ( auto& s : pI->m_BackState ) s = pI->m_pBackSprite->GetRenderState();
	for ( int i = 0; i < Notificatione_Max; ++i ) {
		pI->m_IconState[i] = pI->m_pIconSprite->GetRenderState();
		pI->m_IconState[i].Transform.AttachParent( &pI->m_BackState[i].Transform );
		pI->m_IconState[i].AnimState.IsSetAnimNumber = true;
	}

	// フォントの取得.
	pI->m_pFont = CFontResource::GetFont( "コーポレート・ロゴ ver2 Medium" );
	for ( int n = 0; n < Notificatione_Max; ++n ) {
		pI->m_FontState[n].resize( Font_Max );
		for ( int f = 0; f < Font_Max; ++f ) {
			pI->m_FontState[n][f] = pI->m_pFont->GetRenderState();
			pI->m_FontState[n][f].Transform.AttachParent( &pI->m_BackState[n].Transform );
			pI->m_FontState[n][f].Transform.Scale = { 0.8f, 0.8f, 0.8f };
		}
	}

	// 描画エリアの設定.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize	= CMiniMapUI::GetScreenSize();
	for ( auto& s : pI->m_BackState ) s.RenderArea = { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	for ( auto& s : pI->m_IconState ) s.RenderArea = { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	for ( auto& List : pI->m_FontState ) {
		for ( auto& s : List ) s.RenderArea = { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	}

	// アイコンのNo設定.
	pI->m_IconNoList[EMessageType::Evaluation]		= DeliverEatsIcon;
	pI->m_IconNoList[EMessageType::RankingUpdate]	= DeliverEatsIcon;
	pI->m_IconNoList[EMessageType::WindIsGenerated]	= NewsIcon;
	pI->m_IconNoList[EMessageType::GoldenTime]		= MessageIcon;
	pI->m_IconNoList[EMessageType::TimeLeft]		= DeliverEatsIcon;
	pI->m_IconNoList[EMessageType::Warning]			= EmergencyIcon;

	// アイコンの名前の設定.
	pI->m_IconNameList[DeliverEatsIcon]				= "DeliverEats";
	pI->m_IconNameList[NewsIcon]					= "天気";
	pI->m_IconNameList[MessageIcon]					= "Message";
	pI->m_IconNameList[EmergencyIcon]				= "緊急速報";
	
	// アイコンのSE名の設定.
	pI->m_IconSEList[DeliverEatsIcon]				= "DeliverEatsIcon";
	pI->m_IconSEList[NewsIcon]						= "NewsIcon";
	pI->m_IconSEList[MessageIcon]					= "MessageIcon";
	pI->m_IconSEList[EmergencyIcon]					= "EmergencyIcon";

	// 変数の初期化.
	pI->m_MessageList.clear();
	for ( auto&  m : pI->m_Message		) m = SMessage();
	for ( auto&& f : pI->m_IsOpen		) f = false;
	for ( auto&  s : pI->m_MessageScale ) s = STransform::NORMAL_SCALE_VEC3;
	for ( auto&  a : pI->m_MessageAlpha ) a = Color::ALPHA_MAX;
	pI->m_IsClose		= false;
	pI->m_IsChangeClose = false;
	pI->m_IsChange		= false;
	pI->m_IsDisp		= false;
	pI->m_DispTimeCnt	= INIT_FLOAT;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CNotificationUI::Update( const float& deltaTime )
{
	// 開く処理.
	OpenUpdate( Now );
	OpenUpdate( Next );
	// 閉じる処理.
	CloseUpdate();
	// 変更による閉じる処理.
	ChangeCloseUpdate();
	// 変更処理.
	ChangeUpdate();
	// 自動で閉じる処理.
	AutoCloseUpdate( deltaTime );

	// メッセージを優先順に並べ替える.
	MessageSort();
}

//---------------------------.
// 描画.
//---------------------------.
void CNotificationUI::Render()
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_Message[Now].Type == EMessageType::None ) return;

	// 通知の描画.
	const int Max = pI->m_IsChange ? Notificatione_Max : 1;
	for ( int n = 0; n < Max; ++n ) {
		pI->m_BackState[n].Transform.Scale	= pI->m_MessageScale[n];
		pI->m_BackState[n].Color.w			= pI->m_MessageAlpha[n];
		pI->m_IconState[n].Color.w			= pI->m_MessageAlpha[n];
		pI->m_pBackSprite->RenderUI( &pI->m_BackState[n] );
		pI->m_pIconSprite->RenderUI( &pI->m_IconState[n] );

		// テキストの描画.
		for ( int f = 0; f < Font_Max; ++f ) {
			pI->m_FontState[n][f].Color.w	= pI->m_MessageAlpha[n];
			pI->m_pFont->RenderUI( &pI->m_FontState[n][f] );
		}
	}
}

//---------------------------.
// 文字の追加.
//---------------------------.
void CNotificationUI::PushMessage( const EMessageType& Type, const std::string& Text )
{
	CNotificationUI* pI = GetInstance();

	// 現在通知を表示していない場合通知を表示する.
	if ( pI->m_Message[Now].Type == EMessageType::None ) {
		pI->m_Message[Now] = SMessage( Type, Text, CTime::GetElapsedTime() );
		OpenSetting( Now );
		return;
	}

	// 今表示している通知より優先度が低い場合.
	if ( pI->m_Message[Now].Type > Type || pI->m_IsChange ) {
		// リストに追加する.
		pI->m_MessageList.emplace_back( SMessage( Type, Text, CTime::GetElapsedTime() ) );

		// 優先順位の並びにソートをかける.
		MessageSort();
		return;
	}

	// 今表示している通知より優先度が高いため通知を更新する.
	pI->m_Message[Next] = SMessage( Type, Text, CTime::GetElapsedTime() );
	ChangeSetting();
}

//---------------------------.
// 表示処理.
//---------------------------.
void CNotificationUI::OpenUpdate( const int Type )
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsOpen[Type] == false ) return;

	const D3DXPOSITION3&	ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&			ScreenSize	= CMiniMapUI::GetScreenSize();
	D3DXPOSITION3*			BackPos		= &pI->m_BackState[Type].Transform.Position;
	const SSize&			BackSize	= pI->m_pBackSprite->GetSpriteState().Disp;
	const float				EndPosY		= ScreenPos.y - ScreenSize.h / 2 + BackSize.h / 2.0f + pI->CONSTANT.DISP_ICON_POSITION_Y;

	// 不透明にしていく.
	if ( pI->m_MessageAlpha[Type] < Color::ALPHA_MAX ) {
		pI->m_MessageAlpha[Type] += pI->CONSTANT.ALPHA_SPEED;
		if ( pI->m_MessageAlpha[Type] >= Color::ALPHA_MAX ) {
			pI->m_MessageAlpha[Type] = Color::ALPHA_MAX;
		}
	}

	// 通知を下げていく.
	BackPos->y += pI->CONSTANT.MOVE_SPEED;

	// 通知の動きを止める.
	if ( BackPos->y > EndPosY ) {
		BackPos->y			= EndPosY;
		pI->m_IsOpen[Type]	= false;
		pI->m_IsDisp		= true;
		pI->m_DispTimeCnt	= INIT_FLOAT;
	}
}

//---------------------------.
// 閉じる処理.
//---------------------------.
void CNotificationUI::CloseUpdate()
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsClose == false ) return;

	const D3DXPOSITION3&	ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&			ScreenSize	= CMiniMapUI::GetScreenSize();
	D3DXPOSITION3*			BackPos		= &pI->m_BackState[Now].Transform.Position;
	const SSize&			BackSize	= pI->m_pBackSprite->GetSpriteState().Disp;
	const float				EndPosY		= ScreenPos.y - ScreenSize.h / 2 - BackSize.h / 2.0f;

	// 透明にしていく.
	if ( pI->m_MessageAlpha[Now] > Color::ALPHA_MIN ) {
		pI->m_MessageAlpha[Now] -= pI->CONSTANT.ALPHA_SPEED;
		if ( pI->m_MessageAlpha[Now] <= Color::ALPHA_MIN ) {
			pI->m_MessageAlpha[Now] = Color::ALPHA_MIN;
		}
	}

	// 通知を下げていく.
	BackPos->y -= pI->CONSTANT.MOVE_SPEED;

	// 通知の動きを止める.
	if ( BackPos->y >= EndPosY ) return;
	BackPos->y			= EndPosY;
	pI->m_IsClose		= false;
	pI->m_IsDisp		= false;
	pI->m_DispTimeCnt	= INIT_FLOAT;
	pI->m_Message[Now]	= SMessage();
}

//---------------------------.
// 変更による閉じる処理.
//---------------------------.
void CNotificationUI::ChangeCloseUpdate()
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsChangeClose == false ) return;

	// 縮小していく.
	float Scale = pI->m_MessageScale[Now].x;
	Scale -= pI->CONSTANT.SCALE_SPEED;

	// 縮小を停止させる.
	if ( Scale < STransform::SCALE_MIN ) {
		Scale				= STransform::SCALE_MIN;
		pI->m_IsChangeClose = false;
	}
	pI->m_MessageScale[Now] = D3DXSCALE3( Scale, Scale, Scale );
}

//---------------------------.
// 通知の変更終了処理.
//---------------------------.
void CNotificationUI::ChangeUpdate()
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsChange == false	) return;
	if ( pI->m_IsOpen[Next]			) return;

	// 現在の通知に変更.
	pI->m_Message[Now]						= pI->m_Message[Next];
	pI->m_Message[Next]						= SMessage();
	pI->m_BackState[Now]					= pI->m_BackState[Next];
	pI->m_IconState[Now]					= pI->m_IconState[Next];
	pI->m_IconState[Now].Transform.pParent	= &pI->m_BackState[Now].Transform;
	for ( int f = 0; f < Font_Max; ++f ) {
		pI->m_FontState[Now][f]						= pI->m_FontState[Next][f];
		pI->m_FontState[Now][f].Transform.pParent	= &pI->m_BackState[Now].Transform;
	}

	// 拡縮を元に戻す.
	pI->m_MessageScale[Now] = STransform::NORMAL_SCALE_VEC3;

	// 通知の変更終了.
	pI->m_IsChange		= false;
	pI->m_IsChangeClose = false;
	pI->m_IsDisp		= true;
	pI->m_DispTimeCnt	= INIT_FLOAT;
}

//---------------------------.
// 自動で閉じる処理.
//---------------------------.
void CNotificationUI::AutoCloseUpdate( const float& deltaTime )
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsDisp == false ) return;

	// 表示時間を増やしていく.
	pI->m_DispTimeCnt += deltaTime;

	// 一定時間表示している場合通知を閉じる.
	if ( pI->m_DispTimeCnt < pI->CONSTANT.DISP_TIME ) return;
	pI->m_DispTimeCnt	= INIT_FLOAT;
	pI->m_IsDisp		= false;

	// 表示する通知があるか.
	if ( pI->m_MessageList.empty() ) {
		//  通知を閉じる.
		pI->m_IsClose = true;
		return;
	}
	
	// 次の通知に切り替える.
	pI->m_Message[Next] = pI->m_MessageList.back();
	ChangeSetting();
	pI->m_MessageList.pop_back();
}

//---------------------------.
// 表示設定.
//---------------------------.
void CNotificationUI::OpenSetting( const int Type )
{
	CNotificationUI* pI = GetInstance();

	pI->m_IsOpen[Type]	= true;
	pI->m_IsDisp		= false;

	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize = CMiniMapUI::GetScreenSize();

	// 透明にする.
	pI->m_MessageAlpha[Type] = Color::ALPHA_MIN;

	// 通知の背景の変更.
	STransform*		BackTransform	= &pI->m_BackState[Type].Transform;
	const SSize&	BackSize		= pI->m_pBackSprite->GetSpriteState().Disp;
	BackTransform->Position.x		= ScreenPos.x;
	BackTransform->Position.y		= ScreenPos.y - ScreenSize.h / 2.0f - BackSize.h / 2;

	// 通知のアイコンの設定.
	D3DXPOSITION3*	IconPos						= &pI->m_IconState[Type].Transform.Position;
	const SSize&	IconSize					= pI->m_pIconSprite->GetSpriteState().Disp;
	const int		IconNo						= pI->m_IconNoList[pI->m_Message[Type].Type];
	IconPos->x									= -BackSize.w / 2.0f + IconSize.w / 2.0f + pI->CONSTANT.ICON_POSITION.x;
	IconPos->y									= -BackSize.h / 2.0f + IconSize.h / 2.0f + pI->CONSTANT.ICON_POSITION.y;
	pI->m_IconState[Type].AnimState.AnimNumber	= IconNo;

	// フォント(アイコンの名前)の設定.
	D3DXPOSITION3* FontNamePos				= &pI->m_FontState[Type][Name].Transform.Position;
	FontNamePos->x							= IconPos->x + pI->CONSTANT.FONT_POSITION.x;
	FontNamePos->y							= IconPos->y;
	pI->m_FontState[Type][Name].Color		= Color4::Gray;
	pI->m_FontState[Type][Name].Text		= pI->m_IconNameList[IconNo];

	// フォント(メッセージ)の設定.
	D3DXPOSITION3* FontMessagePos			= &pI->m_FontState[Type][Message].Transform.Position;
	FontMessagePos->x						= IconPos->x;
	if ( pI->m_Message[Type].Text.find( "\n" ) != std::string::npos ){
		FontMessagePos->y					= IconPos->y + IconSize.w / 2.0f + pI->CONSTANT.FONT_POSITION.y * 0.5f;
	}
	else {
		FontMessagePos->y					= IconPos->y + IconSize.w / 2.0f + pI->CONSTANT.FONT_POSITION.y;
	}
	pI->m_FontState[Type][Message].Color	= Color4::Black;
	pI->m_FontState[Type][Message].Text		= pI->m_Message[Type].Text;

	// 通知の取得時間テキストの作成.
	std::string	TimeText					= "今";
	const int	Minute						= CTime::GetElapsedMinute( pI->m_Message[Type].ElapsedTime );
	if (		Minute >= 60 ) TimeText		= std::to_string( Minute / 60 ) + "時間前";
	else if (	Minute >   0 ) TimeText		= std::to_string( Minute ) + "分前";

	// フォント(通知の取得時間)の設定.
	D3DXPOSITION3* FontTimePos				= &pI->m_FontState[Type][Time].Transform.Position;
	FontTimePos->x							= BackSize.w / 2.0f - pI->CONSTANT.FONT_POSITION.x / 2.0f;
	FontTimePos->y							= IconPos->y;
	pI->m_FontState[Type][Time].TextAlign	= ETextAlign::Right;
	pI->m_FontState[Type][Time].Color		= Color4::Gray;
	pI->m_FontState[Type][Time].Text		= TimeText;

	// SEの再生.
	CSoundManager::PlaySE( pI->m_IconSEList[IconNo] );
}

//---------------------------.
// 変更設定.
//---------------------------.
void CNotificationUI::ChangeSetting()
{
	CNotificationUI* pI = GetInstance();

	pI->m_IsChange		= true;

	// 次に表示する通知を表示させる.
	OpenSetting( Next );

	// 今表示している通知を縮小で非表示にする.
	pI->m_IsChangeClose = true;
}

//---------------------------.
// メッセージを並び替える.
//---------------------------.
void CNotificationUI::MessageSort()
{
	CNotificationUI* pI = GetInstance();

	// 重要度が低い順に並べ替える.
	std::vector<std::pair<EMessageType, int>> ImpoList;
	const int ListSize = static_cast<int>( pI->m_MessageList.size() );
	for ( int i = 0; i < ListSize; ++i ){
		ImpoList.emplace_back( std::make_pair( pI->m_MessageList[i].Type, i ) );
	}
	std::sort( ImpoList.begin(), ImpoList.end() );

	// 並び替えた順番に変更する.
	MessageList OutList;
	for ( auto& [Type, No] : ImpoList ) OutList.emplace_back( pI->m_MessageList[No] );
	pI->m_MessageList = OutList;
}
