#include "NowTimeUI.h"
#include "..\..\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\..\Time\Time.h"

CNowTimeUI::CNowTimeUI()
	: m_pFont		( nullptr )
	, m_FontState	()
{
}

CNowTimeUI::~CNowTimeUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CNowTimeUI::Init()
{
	// フォントの取得.
	m_pFont		= CFontResource::GetFont( "コーポレート・ロゴ ver2 Medium", &m_FontState );

	// スマホの画面の位置を取得.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize = CMiniMapUI::GetScreenSize();
	
	// 表示位置の設定.
	m_FontState.Transform.Position.x	= ScreenPos.x - ScreenSize.w / 2.0f + m_pFont->GetFontState().Disp.w / 2.0f + 10.0f;
	m_FontState.Transform.Position.y	= ScreenPos.y - ScreenSize.h / 2.0f - m_pFont->GetFontState().Disp.h / 2.0f - 10.0f;
	m_FontState.Transform.Scale			= { 0.5f, 0.5f, 0.5f };

	// 文字の初期化.
	m_FontState.Text = "12:00";
	
	// 色の設定.
	m_FontState.Color = Color4::Black;

	// 文字の表示方法を中央揃えに変更.
	m_FontState.TextAlign = ETextAlign::Left;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CNowTimeUI::Update( const float& DeltaTime )
{
	// 時間の文字列の取得.
	const CTime::Time_String NowTime = CTime::GetTimeString();

	// フォントに文字列を設定.
	m_FontState.Text = NowTime.first + ":" + NowTime.second;
}

//---------------------------.
// 描画.
//---------------------------.
void CNowTimeUI::Render()
{
	m_pFont->RenderUI( &m_FontState );
}
