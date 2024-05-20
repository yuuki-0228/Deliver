#pragma once
#include "..\Global.h"
#include "..\Common\Sprite\Sprite.h"

class CFrameRate;
class CLoadManager;
class CLoadRender;
class CMaskFade;

/************************************************
*	メインクラス.
**/
class CMain final
{
public:
	CMain();
	~CMain();

	void	Update(		const float& DeltaTime );	// 更新処理.
	void	LoadUpdate( const float& DeltaTime );	// ロード中の更新処理.
	HRESULT Create();								// 構築処理.
	void	Loop();									// メインループ.

	// ウィンドウ初期化関数.
	HRESULT InitWindow( HINSTANCE hInstance );

private:
	// ウィンドウ関数(メッセージ毎の処理).
	static LRESULT CALLBACK MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam );
	
	// FPSの描画.
	void FPSRender();

	// フェードイン.
	bool FadeIn();
	// フェードアウト.
	bool FadeOut();

private:
	HWND							m_hWnd;			// ウィンドウハンドル.
	std::unique_ptr<CFrameRate>		m_pFrameRate;	// フレームレート.
	std::unique_ptr<CLoadManager>	m_pLoadManager;	// ロードマネージャー.
	std::unique_ptr<CLoadRender>	m_pLoadRender;	// ロード中の描画.
	std::unique_ptr<CSprite>		m_pFade;		// フェード.
	SSpriteRenderState				m_FadeState;	// フェードの情報.
	int								m_FadeNo;		// フェードNo.
	bool							m_IsGameLoad;	// 読み込みが終了したか.
	bool							m_IsFPSRender;	// FPSを表示させるか.
	bool							m_IsEndFadeIn;	// フェードインが終わったか.
	bool							m_IsEndFadeOut;	// フェードアウトが終わったか.
};