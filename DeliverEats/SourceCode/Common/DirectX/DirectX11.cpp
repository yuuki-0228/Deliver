#include "DirectX11.h"
#include "..\..\Utility\Log\Log.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include <mutex>

// 透明ウィンドウにするか.
//#define CLEAR_WINDOW;

namespace {
	constexpr char WINDOW_SETTING_FILE_PATH[] = "Data\\Parameter\\Config\\WindowSetting.json";	// ウィンドウの設定のファイルパス.
	std::mutex D10CFFmtx;	// D3DX10CompileFromFile関数用mutex.
	std::mutex D11CFFmtx;	// D3DX11CompileFromFile関数用mutex.
	std::mutex D11CSFmtx;	// D3DX11CreateShaderResourceViewFromFile関数用mutex.
	std::mutex D9LMFmtx;	// D3DXLoadMeshFromX関数用mutex.
	std::mutex D11CVSmtx;	// CreateVertexShader関数用mutex.
	std::mutex D11CPSmtx;	// CreatePixelShader関数用mutex.
	std::mutex D11CIPmtx;	// CreateInputShader関数用mutex.
}

//---------------------------.
// HBITMAPを出力する.
//---------------------------.
#ifdef CLEAR_WINDOW
void SaveImageFile(HBITMAP hBmp, WCHAR *filename)
{
	LONG imageSize;       // 画像サイズ
	BITMAPFILEHEADER fh;  // ビットマップファイルヘッダ
	BITMAPINFO *pbi;      // ビットマップ情報
	BITMAP bmp = { 0 };    // ビットマップ構造体
	LONG bpp;             // 画素数
	LPBYTE bits;          // 画像ビット
	HDC hdc;              // デバイスコンテキスト
	HDC hdc_mem;          // デバイスコンテキスト・メモリ
	HANDLE hFile;         // ファイラハンドル
	DWORD writeSize;      // 書き込んだサイズ

	// BITMAP情報を取得する
	GetObject(hBmp, sizeof(bmp), &bmp);
	hdc = GetDC(0);
	hdc_mem = CreateCompatibleDC(hdc);
	ReleaseDC(0, hdc);
	SelectObject(hdc_mem, hBmp);

	// ファイルサイズ計算
	imageSize = bmp.bmWidthBytes * bmp.bmHeight
	  + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	switch (bmp.bmBitsPixel)
	{
	case 2:
	  bpp = 2;
	  break;
	case 4:
	  bpp = 16;
	  break;
	case 8:
	  bpp = 256;
	  break;
	default:
	  bpp = 0;
	}
	imageSize += (sizeof(RGBQUAD) * bpp);

	// BITMAPFILEHEADERヘッダー出力
	ZeroMemory(&fh, sizeof(fh));
	memcpy(&fh.bfType, "BM", 2);
	fh.bfSize = imageSize;
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
	  + sizeof(RGBQUAD) * bpp;

	// BITMAPINFOHEADERヘッダー出力
	pbi = new BITMAPINFO[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp];
	ZeroMemory(pbi, sizeof(BITMAPINFOHEADER));
	pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbi->bmiHeader.biWidth = bmp.bmWidth;
	pbi->bmiHeader.biHeight = bmp.bmHeight;
	pbi->bmiHeader.biPlanes = 1;
	pbi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	pbi->bmiHeader.biCompression = BI_RGB;
	if (bpp != 0)
	{
	  GetDIBColorTable(hdc_mem, 0, bpp, pbi->bmiColors);
	}

	// 画像データを得る
	bits = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];
	GetDIBits(hdc_mem, hBmp, 0, bmp.bmHeight, bits, pbi, DIB_RGB_COLORS);

	// ファイルに書き込む
	hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
	  FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &fh, sizeof(fh), &writeSize, NULL);
	WriteFile(hFile, pbi,
	  sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp, &writeSize, NULL);
	WriteFile(hFile, bits, bmp.bmWidthBytes * bmp.bmHeight, &writeSize, NULL);
	CloseHandle(hFile);

	// 開放
	delete[] pbi;
	delete[] bits;
	DeleteDC(hdc_mem);
}
#endif

CDirectX11::CDirectX11()
	: m_hWnd					( nullptr )
	, m_pDevice11				( nullptr )	
	, m_pContext11				( nullptr )
	, m_pSwapChain				( nullptr )
	, m_pBackBuffer_TexRTV		( nullptr )
	, m_pBackBuffer_DSTex		( nullptr )
	, m_pBackBuffer_DSTexDSV	( nullptr )
	, m_pDepthStencilStateOn	( nullptr )
	, m_pDepthStencilStateOff	( nullptr )
	, m_pAlphaBlendOn			( nullptr )
	, m_pAlphaBlendOff			( nullptr )
	, m_pAlphaToCoverageOn		( nullptr ) 
	, m_pCullNone				( nullptr )
	, m_pCullBack				( nullptr )
	, m_pCullFront				( nullptr )
	, m_pWireFrame				( nullptr )
	, m_WndWidth				( 0 )
	, m_WndHeight				( 0 )
	, m_BackColor				( Color4::White )
	, m_InitBackColor			( Color4::White )
	, m_IsWindowActive			( false )
	, m_IsDepth					( true )
	, m_IsAlphaBlend			( false )
	, m_IsAlphaToCoverage		( false )
	, m_IsDispMouseCursor		( true )
{
}

CDirectX11::~CDirectX11()
{
	Release();
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CDirectX11* CDirectX11::GetInstance()
{
	static std::unique_ptr<CDirectX11> pInstance = std::make_unique<CDirectX11>();
	return pInstance.get();
}

//---------------------------.
// DirectX構築関数.
//---------------------------.
HRESULT CDirectX11::Create( HWND hWnd )
{
	CDirectX11* pI = GetInstance();

	pI->m_hWnd		= hWnd;
	pI->m_WndWidth	= static_cast<UINT>( WND_W );
	pI->m_WndHeight = static_cast<UINT>( WND_H );

	if ( FAILED( pI->CreateDeviceAndSwapChain()			) ) return E_FAIL;
	if ( FAILED( pI->CreateColorBackBufferRTV()			) ) return E_FAIL;
	if ( FAILED( pI->CreateDepthStencilBackBufferRTV()	) ) return E_FAIL;
	if ( FAILED( pI->CreateDepthStencilState()			) ) return E_FAIL;
	if ( FAILED( pI->CreateAlphaBlendState()			) ) return E_FAIL;
	if ( FAILED( pI->CreateViewports()					) ) return E_FAIL;
	if ( FAILED( pI->CreateRasterizer()					) ) return E_FAIL;

	CLog::PushLog( "DirectX11 デバイス作成 : 成功" );

	return S_OK;
}

//---------------------------.
// 解放処理.
//	解放するときは、作った順と逆の順で開放する.
//---------------------------.
void CDirectX11::Release()
{
	CDirectX11* pI = GetInstance();

	SAFE_RELEASE( pI->m_pAlphaToCoverageOn		);
	SAFE_RELEASE( pI->m_pAlphaBlendOff			);
	SAFE_RELEASE( pI->m_pAlphaBlendOn			);

	SAFE_RELEASE( pI->m_pDepthStencilStateOff	);
	SAFE_RELEASE( pI->m_pDepthStencilStateOn	);

	SAFE_RELEASE( pI->m_pBackBuffer_DSTexDSV	);
	SAFE_RELEASE( pI->m_pBackBuffer_DSTex		);
	SAFE_RELEASE( pI->m_pBackBuffer_TexRTV		);

	SAFE_RELEASE( pI->m_pSwapChain				);
	SAFE_RELEASE( pI->m_pContext11				);
	SAFE_RELEASE( pI->m_pDevice11				);
}

//---------------------------.
// バックバッファクリア関数.
//	この関数を呼び出した後にレンダリングをすること.
//---------------------------.
void CDirectX11::ClearBackBuffer()
{
	CDirectX11* pI = GetInstance();

	// カラーバックバッファ.
	pI->m_BackColor.w = 0.0f;
	pI->m_pContext11->ClearRenderTargetView(
		pI->m_pBackBuffer_TexRTV, pI->m_BackColor );

	// レンダーターゲットビューとデプスステンシルビューをパイプラインにセット.
	pI->m_pContext11->OMSetRenderTargets(
		1,
		&pI->m_pBackBuffer_TexRTV,
		pI->m_pBackBuffer_DSTexDSV );

	// デプスステンシルバックバッファ.
	pI->m_pContext11->ClearDepthStencilView(
		pI->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}

//---------------------------.
// 表示.
//---------------------------.
void CDirectX11::Present()
{
	CDirectX11* pI = GetInstance();

	pI->m_pSwapChain->Present( 0, 0 );

#ifdef CLEAR_WINDOW
	//(2)バックバッファのフォーマットを取得//
	ID3D11Texture2D* pBackBuffer = NULL;
	HRESULT hr = pI->m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if ( FAILED( hr ) ) return;

	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc( &descBackBuffer );
	pBackBuffer->Release();


	//(3)CPU読み出し可能なバッファをGPU上に作成//
	D3D11_TEXTURE2D_DESC Texture2DDesc;
	Texture2DDesc.ArraySize = 1;
	Texture2DDesc.BindFlags = 0;
	Texture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Texture2DDesc.Format = descBackBuffer.Format;
	Texture2DDesc.Height = descBackBuffer.Height;
	Texture2DDesc.Width = descBackBuffer.Width;
	Texture2DDesc.MipLevels = 1;
	Texture2DDesc.MiscFlags = 0;
	Texture2DDesc.SampleDesc.Count = 1;
	Texture2DDesc.SampleDesc.Quality = 0;
	Texture2DDesc.Usage = D3D11_USAGE_STAGING;

	ID3D11Texture2D* hCaptureTexture;
	pI->m_pDevice11->CreateTexture2D( &Texture2DDesc, 0, &hCaptureTexture );


	//(4)作成したCPU読み込み可能バッファにGPU上でデータをコピー//
	ID3D11Resource* hResource;
	pI->m_pBackBuffer_TexRTV->GetResource( &hResource );
	pI->m_pContext11->CopyResource( hCaptureTexture, hResource );
	hResource->Release();

	//(5)GPU上の読み込み可能バッファのメモリアドレスのマップを開く//
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pI->m_pContext11->Map( hCaptureTexture, 0, D3D11_MAP_READ, 0, &mappedResource );

	//(6)CPU上のメモリにバッファを確保//
	double width = descBackBuffer.Width;
	double height = descBackBuffer.Height;
	double src_stride = mappedResource.RowPitch;    //(注)descBackBuffer.Width * 4とは必ずしも一致しない
	size_t buffer_size = src_stride * height;
	BYTE* bmp_buffer;// = new BYTE[buffer_size];

	//(7)CPU上のバッファの画像をファイル書き出し//
	HBITMAP hBmp;
	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( BITMAPINFO ) );
	bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	hBmp = CreateDIBSection( 0, &bmi, DIB_RGB_COLORS, ( void** )&bmp_buffer, 0, 0 );

	//(8)GPU上の読み込み可能バッファからCPU上のバッファへ転送//
	CopyMemory( bmp_buffer, mappedResource.pData, buffer_size );
	pI->m_pContext11->Unmap( hCaptureTexture, 0 );
	hCaptureTexture->Release();

	// スクショテスト.
	if ( GetAsyncKeyState( VK_SPACE ) & 0x0001 ) {
		WCHAR filename[] = L"Data\\test.bmp";
		SaveImageFile( hBmp, filename );
	}

	// ウィンドウを透明にする.
	HDC		hdcScreen, hdcBmp;
	HGDIOBJ	hBmpPrev;
	hdcScreen	= GetDC( NULL );
	hdcBmp		= CreateCompatibleDC( hdcScreen );
	hBmpPrev	= SelectObject( hdcBmp, hBmp );

	RECT  rc;
	POINT ptDst, ptSrc;
	GetWindowRect( pI->m_hWnd, &rc );
	ptDst.x = rc.left;
	ptDst.y = rc.top;
	ptSrc.x = 0;
	ptSrc.y = 0;

	SIZE size;
	size.cx = WND_W;
	size.cy = WND_H;

	BLENDFUNCTION	bf;
	bf.BlendOp				= AC_SRC_OVER;
	bf.BlendFlags			= 0;
	bf.AlphaFormat			= AC_SRC_ALPHA;
	bf.SourceConstantAlpha	= 255;
	UpdateLayeredWindow( pI->m_hWnd, hdcScreen, &ptDst, &size, hdcBmp, &ptSrc, 0, &bf, ULW_ALPHA );
	
	// 解放.
	ReleaseDC( NULL, hdcScreen );
	SelectObject( hdcBmp, hBmpPrev );
	DeleteDC( hdcBmp );
	DeleteObject( hBmp );
#endif
}

//---------------------------.
// デバイスとスワップチェーンの作成.
//---------------------------.
HRESULT CDirectX11::CreateDeviceAndSwapChain()
{
	CDirectX11* pI = GetInstance();

	// スワップチェーン構造体.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );									// 0で初期化.
	sd.BufferCount			= 1;										// バックバッファの数.
	sd.BufferDesc.Width		= pI->m_WndWidth;							// バックバッファの幅.
	sd.BufferDesc.Height	= pI->m_WndHeight;							// バックバッファの高さ.
	sd.BufferDesc.Format	= DXGI_FORMAT_B8G8R8A8_UNORM;				// フォーマット(32ビットカラー).
	sd.BufferDesc.RefreshRate.Numerator		= static_cast<UINT>( FPS );	// リフレッシュレート(分母) ※FPS:60.
	sd.BufferDesc.RefreshRate.Denominator	= 1;						// リフレッシュレート(分子).
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;			// 使い方(表示先).
	sd.OutputWindow			= pI->m_hWnd;								// ウィンドウハンドル.
	sd.SampleDesc.Count		= 1;										// マルチサンプルの数.
	sd.SampleDesc.Quality	= 0;										// マルチサンプルのクオリティ.
	sd.Windowed				= TRUE;										// ウィンドウモード(フルスクリーン時はFALSE).

	// 作成を試みる機能レベルの優先を指定.
	//	(GPUがサポートする機能セットの定義).
	//	D3D_FEATURE_LEVEL列挙型の配列.
	//	D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 の GPUレベル.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;//配列の要素数.

	// デバイスとスワップチェーンの作成.
	//	ハードウェア(GPU)デバイスでの作成.
	if( FAILED(
		D3D11CreateDeviceAndSwapChain(
			nullptr,					// ビデオアダプタへのポインタ.
			D3D_DRIVER_TYPE_HARDWARE,	// 作成するデバイスの種類.
			nullptr,					// ソフトウェア ラスタライザを実装するDLLのハンドル.
			0,							// 有効にするランタイムレイヤー.
			&pFeatureLevels,			// 作成を試みる機能レベルの順序を指定する配列へのポインタ.
			1,							// ↑の要素数.
			D3D11_SDK_VERSION,			// SDKのバージョン.
			&sd,						// スワップチェーンの初期化パラメータのポインタ.
			&m_pSwapChain,				// (out)レンダリングに使用するスワップチェーン.
			&m_pDevice11,				// (out)作成されたデバイス.
			pFeatureLevel,				// 機能レベルの配列にある最初の要素を表すポインタ.
			&m_pContext11 ) ) )			// (out)デバイス コンテキスト.
	{
		// WARPデバイスの作成.
		//	D3D_FEATURE_LEVEL_9_1～D3D_FEATURE_LEVEL_10_1.
		if( FAILED(
			D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
				0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
				&sd, &m_pSwapChain, &m_pDevice11,
				pFeatureLevel, &m_pContext11 ) ) )
		{
			// リファレンスデバイスの作成.
			//	DirectX SDKがインストールされていないと使えない.
			if( FAILED(
				D3D11CreateDeviceAndSwapChain(
					nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr,
					0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
					&sd, &m_pSwapChain, &m_pDevice11,
					pFeatureLevel, &m_pContext11 ) ) )
			{
				ErrorMessage( "デバイスとスワップチェーン 作成 : 失敗" );
				return E_FAIL;
			}
		}
	}

	// ウィンドウの設定の取得.
	json WndSetting = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );

	// フルスクリーンや一部の機能を無効化する.
	if ( WndSetting["IsFullScreenLock"] ) {
		// ALT + Enterでフルスクリーンを無効化する.
		IDXGIFactory* pFactory = nullptr;
		// 上で作ったIDXGISwapChainを使う.
		m_pSwapChain->GetParent( __uuidof( IDXGIFactory ), (void**) &pFactory );
		// 余計な機能を無効にする設定をする.
		pFactory->MakeWindowAssociation( m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER );
		SAFE_RELEASE( pFactory );
	}

	// 背景の色の設定.
	m_InitBackColor = Color4::RGBA(
		static_cast<float>( WndSetting["BackColor"]["R"] ),
		static_cast<float>( WndSetting["BackColor"]["G"] ),
		static_cast<float>( WndSetting["BackColor"]["B"] )
	);
	m_BackColor = m_InitBackColor;
	return S_OK;
}

//---------------------------.
// ラスタライザステート設定.
//---------------------------.
void CDirectX11::SetRasterizerState( const ERS_STATE& RsState )
{
	CDirectX11* pI = GetInstance();

	switch ( RsState ) {
	case ERS_STATE::None:
		// 正背面描画.
		pI->m_pContext11->RSSetState( pI->m_pCullNone );
		break;
	case ERS_STATE::Back:
		// 背面を描画しない.
		pI->m_pContext11->RSSetState( pI->m_pCullBack );
		break;
	case ERS_STATE::Front:
		// 正面を描画しない.
		pI->m_pContext11->RSSetState( pI->m_pCullFront );
		break;
	case ERS_STATE::Wire:
		// ワイヤーフレーム描画.
		pI->m_pContext11->RSSetState( pI->m_pWireFrame );
		break;
	default:
		break;
	}
}

//---------------------------.
// フルスクリーンの設定.
//---------------------------.
void CDirectX11::SetFullScreen( const bool Flag )
{
	CDirectX11* pI = GetInstance();

	// 現在のスクリーン情報を取得.
	BOOL IsFullScreen = FALSE;
	pI->m_pSwapChain->GetFullscreenState( &IsFullScreen, nullptr );

	// フルスクリーンに変更するか.
	if ( Flag ) {
		// 現在フルスクリーンなら行わない.
		if ( IsFullScreen == TRUE ) return;

		// フルスクリーンに変更.
		pI->m_pSwapChain->SetFullscreenState( TRUE, nullptr );

		// マウスを非表示にする.
		if ( pI->m_IsDispMouseCursor == false ) return;
		ShowCursor( FALSE );
	}
	// 現在ウィンドウ状態に変更するか.
	else {
		// 現在ウィンドウ状態なら行わない.
		if ( IsFullScreen == FALSE ) return;

		// ウィンドウに変更.
		pI->m_pSwapChain->SetFullscreenState( FALSE, nullptr );

		// マウスを表示する.
		if ( pI->m_IsDispMouseCursor == false ) return;
		ShowCursor( TRUE );
	}
}

//----------------------------.
// フルスクリーン状態か取得.
//----------------------------.
bool CDirectX11::IsFullScreen()
{
	CDirectX11* pI = GetInstance();

	if ( GetInstance()->m_pSwapChain == nullptr ) return false;

	// 現在のスクリーン情報を取得.
	BOOL isState = FALSE;
	GetInstance()->m_pSwapChain->GetFullscreenState( &isState, nullptr );
	return static_cast<bool>( isState );
}

//----------------------------.
// ウィンドウサイズが変更された時に呼ぶ.
//----------------------------.
void CDirectX11::Resize()
{
	CDirectX11* pI = GetInstance();

	if( pI->m_pContext11 == nullptr ) return;

	// 現在のスクリーン情報を取得.
	BOOL IsFullScreen = FALSE;
	pI->m_pSwapChain->GetFullscreenState( &IsFullScreen, nullptr );

	// セットしてあるレンダーターゲットを外す.
	pI->m_pContext11->OMSetRenderTargets( 0, nullptr, nullptr );

	// 使用していたバックバッファを解放する.
	SAFE_RELEASE( pI->m_pBackBuffer_TexRTV );
	SAFE_RELEASE( pI->m_pBackBuffer_DSTex );
	SAFE_RELEASE( pI->m_pBackBuffer_DSTexDSV );

	// スワップチェーンをリサイズする.
	// width, height を指定しない場合、hWndを参照し、自動で計算してくれる.
	if( FAILED( pI->m_pSwapChain->ResizeBuffers( 0, 0, 0, DXGI_FORMAT_UNKNOWN, 0 ) )){
		ErrorMessage( "デプスステンシルビュー作成失敗" );
		return;
	}

	// スワップチェーンのバッファの取得.
	ID3D11Texture2D* pBuufer = nullptr;
	if( FAILED( pI->m_pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBuufer ) )){
		ErrorMessage( "デプスステンシルビュー作成失敗" );
		return;
	}

	// テクスチャ情報の取得.
	D3D11_TEXTURE2D_DESC texDesc = {0};
	pBuufer->GetDesc( &texDesc );
	pI->m_WndWidth	= texDesc.Width;
	pI->m_WndHeight	= texDesc.Height;
	SAFE_RELEASE( pBuufer );

	if( FAILED( pI->CreateColorBackBufferRTV() ) ) {
		ErrorMessage( "デプスステンシルビュー作成失敗" );
		return;
	}
	if( FAILED( pI->CreateDepthStencilBackBufferRTV() )){
		ErrorMessage( "デプスステンシルビュー作成失敗" );
		return;
	}

	// レンダーターゲットの設定.
	pI->m_pContext11->OMSetRenderTargets( 
		1, 
		&pI->m_pBackBuffer_TexRTV,
		pI->m_pBackBuffer_DSTexDSV );
	// デプスステンシルバッファ.
	pI->m_pContext11->ClearDepthStencilView(
		pI->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	// ビューポートの設定.
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)pI->m_WndWidth;
	vp.Height	= (FLOAT)pI->m_WndHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	pI->m_pContext11->RSSetViewports( 1, &vp );
}

//----------------------------.
// アクティブウィンドウか確認.
//----------------------------.
void CDirectX11::CheckActiveWindow()
{
	CDirectX11* pI = GetInstance();

	// 自分のウィンドウハンドルとPCの最前面のウィンドウハンドルを比較.
	pI->m_IsWindowActive = pI->m_hWnd == GetForegroundWindow();
}

//----------------------------.
// D3DX10CompileFromFileマクロを呼び出す.
//----------------------------.
HRESULT CDirectX11::MutexD3DX10CompileFromFile( const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
	LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX10ThreadPump* pPump,
	ID3D10Blob** ppOutShader, ID3D10Blob** ppOutErrorMsgs, HRESULT* pOutHResult )
{
	std::unique_lock<std::mutex> lock( D10CFFmtx );

	HRESULT hr;
	hr = D3DX10CompileFromFile(
		filepath,		// シェーダーファイルパス.
		pDefines,		// マクロ定義のポインター : オプション.
		pInclude,		// インクルードファイルを処理するためのポインタ : オプション.
		entryPoint,		// シェーダーのエントリーポイント関数名.
		shaderModel,	// シェーダーモデルの名前.
		compileFlag,	// シェーダーコンパイルフラグ.
		Flags2,			// エフェクトコンパイルフラグ (0推奨).
		pPump,			// スレッドポンプインターフェイスへのポインター.
		ppOutShader,	// コンパイルされたシェーダーのデータ (out).
		ppOutErrorMsgs,	// コンパイル中のエラー出力 (out).
		pOutHResult );	// 戻り値のポインタ.
	return hr;
}

//----------------------------.
// D3DX11CompileFromFileマクロを呼び出す.
//----------------------------.
HRESULT CDirectX11::MutexD3DX11CompileFromFile( const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
	LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX11ThreadPump* pPump,
	ID3D10Blob** ppOutShader, ID3D10Blob** ppOutErrorMsgs, HRESULT* pOutHResult )
{
	std::unique_lock<std::mutex> lock( D11CFFmtx );

	HRESULT hr;
	hr = D3DX11CompileFromFile(
		filepath,		// シェーダーファイルパス.
		pDefines,		// マクロ定義のポインター : オプション.
		pInclude,		// インクルードファイルを処理するためのポインタ : オプション.
		entryPoint,		// シェーダーのエントリーポイント関数名.
		shaderModel,	// シェーダーモデルの名前.
		compileFlag,	// シェーダーコンパイルフラグ.
		Flags2,			// エフェクトコンパイルフラグ (0推奨).
		pPump,			// スレッドポンプインターフェイスへのポインター.
		ppOutShader,	// コンパイルされたシェーダーのデータ (out).
		ppOutErrorMsgs,	// コンパイル中のエラー出力 (out).
		pOutHResult );	// 戻り値のポインタ.
	return hr;
}

//----------------------------.
// D3DX11CreateShaderResourceViewFromFileマクロを呼び出す.
//----------------------------.
HRESULT CDirectX11::MutexD3DX11CreateShaderResourceViewFromFile(
	const LPCTSTR pSrcFile, D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
	ID3D11ShaderResourceView** ppShaderResourceView, HRESULT* pHResult )
{
	std::unique_lock<std::mutex> lock( D11CSFmtx );

	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile(
		GetInstance()->m_pDevice11,		// リソースを使用するデバイスのポインタ.
		pSrcFile,						// ファイル名.
		pLoadInfo,						// シェーダーリソースビューを含むファイルの名前.
		pPump,							// スレッドポンプインターフェイスへのポインター
		ppShaderResourceView,			// (out)テクスチャ.
		pHResult );
	return hr;
}

//----------------------------.
// D3DXLoadMeshFromXマクロを呼び出す.
//----------------------------.
HRESULT CDirectX11::MutexD3DXLoadMeshFromX(
	const LPCTSTR pFilename, DWORD Options, LPDIRECT3DDEVICE9 pD3DDevice, LPD3DXBUFFER* ppAdjacency, 
	LPD3DXBUFFER* ppMaterials, LPD3DXBUFFER* ppEffectInstances, DWORD* pNumMaterials, LPD3DXMESH* ppMesh )
{
	std::unique_lock<std::mutex> lock( D9LMFmtx );

	HRESULT hr;

	hr = D3DXLoadMeshFromX(
		pFilename,			// ファイル名.
		Options,			// メッシュの作成オプション.
		pD3DDevice,			// メッシュに関連付けられているデバイスオブジェクト.
		ppAdjacency,		// 隣接関係データを含むバッファーへのポインター.
		ppMaterials,		// (out)マテリアルデータを含むバッファーへのポインター.
		ppEffectInstances,	// (out)効果インスタンスの配列を含むバッファーへのポインター.
		pNumMaterials,		// (out)マテリアル数.
		ppMesh );			// (out)読み込まれたメッシュ.
	return hr;
}

//----------------------------.
// D11CreateVertexShaderを呼び出す.
//----------------------------.
HRESULT CDirectX11::MutexDX11CreateVertexShader( ID3DBlob* pCompiledShader, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** pVertexShader )
{
	std::unique_lock<std::mutex> lock( D11CVSmtx );

	HRESULT hr;

	hr = GetInstance()->m_pDevice11->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		pClassLinkage,
		pVertexShader );
	return hr;
}

//----------------------------.
// D11CreatePixelShaderを呼び出す.
//----------------------------.
HRESULT CDirectX11::MutexDX11CreatePixelShader( ID3DBlob* pCompiledShader, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** pPixelShader )
{
	std::unique_lock<std::mutex> lock( D11CPSmtx );

	HRESULT hr;

	hr = GetInstance()->m_pDevice11->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		pClassLinkage,
		pPixelShader );
	return hr;
}

//----------------------------.
// D11CreateInputLayoutを呼び出す.
//----------------------------.
HRESULT CDirectX11::MutexDX11CreateInputLayout( D3D11_INPUT_ELEMENT_DESC* pLayout, UINT numElements, ID3DBlob* pCompiledShader, ID3D11InputLayout** m_pVertexLayout )
{
	std::unique_lock<std::mutex> lock( D11CIPmtx );

	HRESULT hr;
	hr = GetInstance()->m_pDevice11->CreateInputLayout(
		pLayout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		m_pVertexLayout );
	return hr;
}

//----------------------------.
// ラスタライザ作成.
//----------------------------.
HRESULT CDirectX11::CreateRasterizer()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.FillMode = D3D11_FILL_SOLID;//塗りつぶし(ソリッド).

	// カリングの設定.
	//	D3D11_CULL_BACK:背面を描画しない.
	//	D3D11_CULL_FRONT:正面を描画しない.
	//	D3D11_CULL_NONE:カリングを切る(正背面を描画する).
	rdc.CullMode = D3D11_CULL_NONE;

	if ( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &m_pCullNone ) ) ){
		ErrorMessage( "ラスタライザー 作成 : 失敗" );
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_SOLID;// 塗りつぶし(ソリッド).
	rdc.CullMode = D3D11_CULL_BACK;	// BACK:背面を描画しない,
	if ( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &m_pCullBack ) ) ){
		ErrorMessage( "ラスタライザー 作成 : 失敗" );
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_SOLID;// 塗りつぶし(ソリッド).
	rdc.CullMode = D3D11_CULL_FRONT;// FRONT:正面を描画しない.
	if ( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &m_pCullFront ) ) ){
		ErrorMessage( "ラスタライザー 作成 : 失敗" );
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_WIREFRAME;// ワイヤーフレーム.
	rdc.CullMode = D3D11_CULL_NONE;		// BACK:背面を描画しない, FRONT:正面を描画しない.
	if ( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &m_pWireFrame ) ) ){
		ErrorMessage( "ラスタライザー 作成 : 失敗" );
		return E_FAIL;
	}

	// ポリゴンの表裏を決定するフラグ.
	//	TRUE:左回りなら前向き。右回りなら後ろ向き。
	//	FALSE:逆になる.
	rdc.FrontCounterClockwise = FALSE;

	// 距離についてのクリッピング有効.
	rdc.DepthClipEnable = FALSE;

	m_pContext11->RSSetState( m_pCullNone );

	return S_OK;
}

//----------------------------.
// デプスステンシル設定.
//	この関数１つでON/OFFの2種類を作成する.
//----------------------------.
HRESULT CDirectX11::CreateDepthStencilState()
{
	// 深度テスト(Zテスト)の設定.
	//	※on/offの共通部分のみ設定.
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable	= FALSE;
	dsDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// 深度テスト(Zテスト)を有効にする.
	dsDesc.DepthEnable = TRUE;//有効.
	// 深度設定作成.
	if( FAILED(
		m_pDevice11->CreateDepthStencilState(
			&dsDesc, &m_pDepthStencilStateOn ) ) )
	{
		ErrorMessage( "深度ON設定 作成 : 失敗" );
		return E_FAIL;
	}

	// 深度テスト(Z)テストを無効にする.
	dsDesc.DepthEnable = FALSE;//無効.
	// 深度設定作成.
	if( FAILED(
		m_pDevice11->CreateDepthStencilState(
			&dsDesc, &m_pDepthStencilStateOff ) ) )
	{
		ErrorMessage( "深度OFF設定 作成 : 失敗" );
		return E_FAIL;
	}

	return S_OK;
}


//----------------------------.
// ブレンドステート作成.
//	アルファブレンドのON/OFFの２種類を作成.
//----------------------------.
HRESULT CDirectX11::CreateAlphaBlendState()
{
	// アルファブレンド用ブレンドステート構造体.
	//	pngファイル内にアルファ情報があるので、透過するようにブレンドステートで設定する.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory( &BlendDesc, sizeof( BlendDesc ) );	// 初期化.

	BlendDesc.IndependentBlendEnable = false;		// false:RenderTarget[0]のメンバーのみ使用する.
													//	true:RenderTarget[0～7]が使用できる.
													//	(レンダーターゲット毎に独立したブレンド処理).
	// アルファトゥカバレージを使用する.
	BlendDesc.AlphaToCoverageEnable = false;
	// 元素材に対する設定.
	BlendDesc.RenderTarget[0].SrcBlend			= D3D11_BLEND_SRC_ALPHA;		// アルファブレンドを指定.
	// 重ねる素材に対する設定.
	BlendDesc.RenderTarget[0].DestBlend			= D3D11_BLEND_INV_SRC_ALPHA;	// アルファブレンドの反転を指定.
	// ブレンドオプション.
	BlendDesc.RenderTarget[0].BlendOp			= D3D11_BLEND_OP_ADD;			// ADD:加算合成.
	// 元素材のアルファに対する指定.
	BlendDesc.RenderTarget[0].SrcBlendAlpha		= D3D11_BLEND_ONE;				// そのまま使用.
	// 重ねる素材のアルファに対する設定.
	BlendDesc.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND_ZERO;				// 何もしない。
	// アルファのブレンドオプション.
	BlendDesc.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP_ADD;			// ADD:加算合成.
	// ピクセル毎の書き込みマスク.
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;	// 全ての成分(RGBA)へのデータの格納を許可する.

	// アルファブレンドを使用する.
	BlendDesc.RenderTarget[0].BlendEnable = true;	// 有効.
	// ブレンドステート作成.
	if( FAILED(
		m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaBlendOn ) ) )
	{
		ErrorMessage( "アルファブレンドステートON設定 作成 : 失敗" );
		return E_FAIL;
	}

	// アルファトゥカバレージを使用する.
	BlendDesc.AlphaToCoverageEnable = true;	// 有効.
	// ブレンドステート作成.
	if ( FAILED(
		m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaToCoverageOn ) ) )
	{
		ErrorMessage( "アルファトゥカバレージステートON設定 作成 : 失敗" );
		return E_FAIL;
	}

	// アルファトゥカバレージを使用しない.
	BlendDesc.AlphaToCoverageEnable			= false;	//無効.
	// アルファブレンドを使用しない.
	BlendDesc.RenderTarget[0].BlendEnable	= false;	//無効.
	// ブレンドステート作成.
	if( FAILED(
		m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaBlendOff ) ) )
	{
		ErrorMessage( "アルファブレンドステートOFF設定 作成 : 失敗" );
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------.
// ビューポート作成.
//----------------------------.
HRESULT CDirectX11::CreateViewports()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT) m_WndWidth;	// 幅.
	vp.Height	= (FLOAT) m_WndHeight;	// 高さ.
	vp.MinDepth = 0.0f;					// 最小深度(手前).
	vp.MaxDepth = 1.0f;					// 最大深度(奥).
	vp.TopLeftX = 0.0f;					// 左上位置x.
	vp.TopLeftY = 0.0f;					// 左上位置y.

	m_pContext11->RSSetViewports( 1, &vp );

	return S_OK;
}

//----------------------------.
// 透過設定の切り替え.
//----------------------------.
void CDirectX11::SetAlphaBlend( bool flag )
{
	CDirectX11* pI = GetInstance();
	if ( pI->m_IsAlphaToCoverage ) return;

	UINT mask = 0xffffffff;	// マスク値.
	ID3D11BlendState* pTmp
		= ( flag == true ) ? pI->m_pAlphaBlendOn : pI->m_pAlphaBlendOff;

	// アルファブレンド設定をセット.
	pI->m_pContext11->OMSetBlendState( pTmp, nullptr, mask );
	pI->m_IsAlphaBlend = flag;
}

//----------------------------.
// アルファトゥカバレージを有効:無効に設定する.
//----------------------------.
void CDirectX11::SetAlphaToCoverage( bool flag )
{
	CDirectX11* pI = GetInstance();

	// ブレンドステートの設定.
	UINT mask = 0xffffffff;	// マスク値.
	ID3D11BlendState* blend 
		= ( flag == true ) ? pI->m_pAlphaToCoverageOn : pI->m_pAlphaBlendOff;
	pI->m_pContext11->OMSetBlendState( blend, nullptr, mask );
	pI->m_IsAlphaToCoverage = flag;
}

//----------------------------.
// 深度(Z)テストON/OFF切り替え.
//----------------------------.
void CDirectX11::SetDepth(bool flag)
{
	CDirectX11* pI = GetInstance();

	ID3D11DepthStencilState* pTmp
		= (flag == true) ? pI->m_pDepthStencilStateOn : pI->m_pDepthStencilStateOff;

	// 深度設定をセット.
	pI->m_pContext11->OMSetDepthStencilState( pTmp, 1 );
	pI->m_IsDepth = flag;
}

//----------------------------.
// バックバッファ作成:カラー用レンダーターゲットビュー作成.
//----------------------------.
HRESULT CDirectX11::CreateColorBackBufferRTV()
{
	// バックバッファテクスチャを取得(既にあるので作成ではない).
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	if( FAILED(
		m_pSwapChain->GetBuffer(
			0,
			__uuidof( ID3D11Texture2D ),	// __uuidof:式に関連付けされたGUIDを取得
											//	Texture2Dの唯一の物として扱う
			(LPVOID*)&pBackBuffer_Tex ) ) )	// (out)バックバッファテクスチャ.
	{
		ErrorMessage( "スワップチェインからバックバッファ 取得 : 失敗" );
		return E_FAIL;
	}

	// そのテクスチャに対してレンダーターゲットビュー(RTV)を作成.
	if( FAILED(
		m_pDevice11->CreateRenderTargetView(
			pBackBuffer_Tex,
			nullptr,
			&m_pBackBuffer_TexRTV ) ) )	// (out)RTV.
	{
		// バックバッファテクスチャを解放.
		SAFE_RELEASE( pBackBuffer_Tex );
		ErrorMessage( "レンダータイゲットビュー 作成 : 失敗" );
		return E_FAIL;
	}
	// バックバッファテクスチャを解放.
	SAFE_RELEASE( pBackBuffer_Tex );

	return S_OK;
}

//----------------------------.
// バックバッファ作成:デプスステンシル用レンダーターゲットビュー作成.
//----------------------------.
HRESULT CDirectX11::CreateDepthStencilBackBufferRTV()
{
	CDirectX11* pI = GetInstance();

	// デプス(深さor深度)ステンシルビュー用のテクスチャを作成.
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= m_WndWidth;				// 幅.
	descDepth.Height				= m_WndHeight;				// 高さ.
	descDepth.MipLevels				= 1;						// ミップマップレベル:1.
	descDepth.ArraySize				= 1;						// 配列数:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32ビットフォーマット.
	descDepth.SampleDesc.Count		= 1;						// マルチサンプルの数.
	descDepth.SampleDesc.Quality	= 0;						// マルチサンプルのクオリティ.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// 使用方法:デフォルト.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// 深度(ステンシルとして使用).
	descDepth.CPUAccessFlags		= 0;						// CPUからはアクセスしない.
	descDepth.MiscFlags				= 0;						// その他の設定なし.

	// そのテクスチャに対してデプスステンシル(DSTex)を作成.
	if( FAILED(
		m_pDevice11->CreateTexture2D(
			&descDepth,
			nullptr,
			&m_pBackBuffer_DSTex ) ) )	// (out)デプスステンシル用テクスチャ.
	{
		ErrorMessage( "デプスステンシル作成 : 失敗" );
		return E_FAIL;
	}

	// そのテクスチャに対してデプスステンシルビュー(DSV)を作成.
	if( FAILED( m_pDevice11->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		nullptr,
		&m_pBackBuffer_DSTexDSV ) ) )	// (out)DSV.
	{
		ErrorMessage( "デプスステンシルビュー 作成 : 失敗" );
		return E_FAIL;
	}

	// レンダーターゲットビューとデプスステンシルビューをパイプラインにセット.
	pI->m_pContext11->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV );

	return S_OK;
}
