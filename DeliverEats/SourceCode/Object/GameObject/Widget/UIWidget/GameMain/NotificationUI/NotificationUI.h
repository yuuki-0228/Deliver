#pragma once
#include "..\..\..\..\..\..\Global.h"
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Common\Font\Font.h"
#include "..\..\..\..\..\..\Utility\Constant\Constant.h"

// メッセージの種類.
enum class enMessageType : unsigned char {
	None,

	// 下に行くほど重要度が高い.
	Evaluation,			// お客さんからの評価.
	RankingUpdate,		// ランキング更新.
	WindIsGenerated,	// 風が発生.
	GoldenTime,			// ゴールデンタイム関係.
	TimeLeft,			// 残り時間関係.
	Warning,			// エリア外などの通知.

	Max
} typedef EMessageType;

/************************************
*	通知クラス.
**/
class CNotificationUI final
{
private:
	// メッセージ構造体.
	struct stMessage {
		EMessageType	Type;			// メッセージタイプ.
		std::string		Text;			// テキスト.
		float			ElapsedTime;	// 通知をもらったゲーム内の分.

		stMessage()
			: stMessage( EMessageType::None, "", 0.0f )
		{
		}
		stMessage( const EMessageType type, const std::string& text, const float time )
			: Type			( type )
			, Text			( text )
			, ElapsedTime	( time )
		{
		}
	} typedef SMessage;

public:
	using MessageList		= std::vector<SMessage>;
	using FontList			= std::vector<SFontRenderStateList>;
	using IconNoMap			= std::unordered_map<EMessageType, int>;
	using IconNameMap		= std::unordered_map<int, std::string>;
	using IconSEMap			= std::unordered_map<int, std::string>;

public:
	CNotificationUI();
	~CNotificationUI();

	// インスタンスの取得.
	static CNotificationUI* GetInstance();

	// 初期化.
	static bool Init();
	// 更新.
	static void Update( const float& deltaTime );
	// 描画.
	static void Render();

	// 文字の追加.
	static void PushMessage( const EMessageType& Type, const std::string& Text );

private:
	// 表示処理.
	static void OpenUpdate( const int Type );
	// 閉じる処理.
	static void CloseUpdate();
	// 変更による閉じる処理.
	static void ChangeCloseUpdate();
	// 通知の変更終了処理.
	static void ChangeUpdate();
	// 自動で閉じる処理.
	static void AutoCloseUpdate( const float& deltaTime );

	// 表示設定.
	static void OpenSetting( const int Type );
	// 変更設定.
	static void ChangeSetting();

	// メッセージを並び替える.
	static void MessageSort();

private:
	CSprite*				m_pBackSprite;		// 背景画像.
	CSprite*				m_pIconSprite;		// アイコン画像.
	CFont*					m_pFont;			// フォント.
	SSpriteRenderStateList	m_BackState;		// 背景の情報.
	SSpriteRenderStateList	m_IconState;		// アイコンの情報.
	FontList				m_FontState;		// フォントの情報.
	IconNoMap				m_IconNoList;		// アイコンの番号リスト( EMessageType -> AnimNumber ).
	IconNameMap				m_IconNameList;		// アイコンの名前リスト( AnimNumber -> アイコン名 ).
	IconSEMap				m_IconSEList;		// アイコンのSEリスト( AnimNumber -> SE名 ).
	MessageList				m_MessageList;		// メッセージリスト.
	MessageList				m_Message;			// 現在のメッセージ.
	std::vector<bool>		m_IsOpen;			// 通知を表示させるか.
	std::vector<D3DXSCALE3>	m_MessageScale;		// メッセージの拡縮.
	std::vector<float>		m_MessageAlpha;		// メッセージのアルファ値.
	bool					m_IsClose;			// 通知を閉じるか.
	bool					m_IsChangeClose;	// 通知の変更で閉じるか.
	bool					m_IsChange;			// 通知を変更するか.
	bool					m_IsDisp;			// 通知を表示しているか.
	float					m_DispTimeCnt;		// 表示時間用のカウント.

private:
	// 外部データの定数.
	const ConstantStruct::SNotification& CONSTANT = CConstant::GetNotification();
};