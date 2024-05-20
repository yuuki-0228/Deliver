#include "DirectX11.h"
#include "..\..\Utility\Log\Log.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include <mutex>

// �����E�B���h�E�ɂ��邩.
//#define CLEAR_WINDOW;

namespace {
	constexpr char WINDOW_SETTING_FILE_PATH[] = "Data\\Parameter\\Config\\WindowSetting.json";	// �E�B���h�E�̐ݒ�̃t�@�C���p�X.
	std::mutex D10CFFmtx;	// D3DX10CompileFromFile�֐��pmutex.
	std::mutex D11CFFmtx;	// D3DX11CompileFromFile�֐��pmutex.
	std::mutex D11CSFmtx;	// D3DX11CreateShaderResourceViewFromFile�֐��pmutex.
	std::mutex D9LMFmtx;	// D3DXLoadMeshFromX�֐��pmutex.
	std::mutex D11CVSmtx;	// CreateVertexShader�֐��pmutex.
	std::mutex D11CPSmtx;	// CreatePixelShader�֐��pmutex.
	std::mutex D11CIPmtx;	// CreateInputShader�֐��pmutex.
}

//---------------------------.
// HBITMAP���o�͂���.
//---------------------------.
#ifdef CLEAR_WINDOW
void SaveImageFile(HBITMAP hBmp, WCHAR *filename)
{
	LONG imageSize;       // �摜�T�C�Y
	BITMAPFILEHEADER fh;  // �r�b�g�}�b�v�t�@�C���w�b�_
	BITMAPINFO *pbi;      // �r�b�g�}�b�v���
	BITMAP bmp = { 0 };    // �r�b�g�}�b�v�\����
	LONG bpp;             // ��f��
	LPBYTE bits;          // �摜�r�b�g
	HDC hdc;              // �f�o�C�X�R���e�L�X�g
	HDC hdc_mem;          // �f�o�C�X�R���e�L�X�g�E������
	HANDLE hFile;         // �t�@�C���n���h��
	DWORD writeSize;      // �������񂾃T�C�Y

	// BITMAP�����擾����
	GetObject(hBmp, sizeof(bmp), &bmp);
	hdc = GetDC(0);
	hdc_mem = CreateCompatibleDC(hdc);
	ReleaseDC(0, hdc);
	SelectObject(hdc_mem, hBmp);

	// �t�@�C���T�C�Y�v�Z
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

	// BITMAPFILEHEADER�w�b�_�[�o��
	ZeroMemory(&fh, sizeof(fh));
	memcpy(&fh.bfType, "BM", 2);
	fh.bfSize = imageSize;
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
	  + sizeof(RGBQUAD) * bpp;

	// BITMAPINFOHEADER�w�b�_�[�o��
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

	// �摜�f�[�^�𓾂�
	bits = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];
	GetDIBits(hdc_mem, hBmp, 0, bmp.bmHeight, bits, pbi, DIB_RGB_COLORS);

	// �t�@�C���ɏ�������
	hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
	  FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &fh, sizeof(fh), &writeSize, NULL);
	WriteFile(hFile, pbi,
	  sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp, &writeSize, NULL);
	WriteFile(hFile, bits, bmp.bmWidthBytes * bmp.bmHeight, &writeSize, NULL);
	CloseHandle(hFile);

	// �J��
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
// �C���X�^���X�̎擾.
//---------------------------.
CDirectX11* CDirectX11::GetInstance()
{
	static std::unique_ptr<CDirectX11> pInstance = std::make_unique<CDirectX11>();
	return pInstance.get();
}

//---------------------------.
// DirectX�\�z�֐�.
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

	CLog::PushLog( "DirectX11 �f�o�C�X�쐬 : ����" );

	return S_OK;
}

//---------------------------.
// �������.
//	�������Ƃ��́A��������Ƌt�̏��ŊJ������.
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
// �o�b�N�o�b�t�@�N���A�֐�.
//	���̊֐����Ăяo������Ƀ����_�����O�����邱��.
//---------------------------.
void CDirectX11::ClearBackBuffer()
{
	CDirectX11* pI = GetInstance();

	// �J���[�o�b�N�o�b�t�@.
	pI->m_BackColor.w = 0.0f;
	pI->m_pContext11->ClearRenderTargetView(
		pI->m_pBackBuffer_TexRTV, pI->m_BackColor );

	// �����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���p�C�v���C���ɃZ�b�g.
	pI->m_pContext11->OMSetRenderTargets(
		1,
		&pI->m_pBackBuffer_TexRTV,
		pI->m_pBackBuffer_DSTexDSV );

	// �f�v�X�X�e���V���o�b�N�o�b�t�@.
	pI->m_pContext11->ClearDepthStencilView(
		pI->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}

//---------------------------.
// �\��.
//---------------------------.
void CDirectX11::Present()
{
	CDirectX11* pI = GetInstance();

	pI->m_pSwapChain->Present( 0, 0 );

#ifdef CLEAR_WINDOW
	//(2)�o�b�N�o�b�t�@�̃t�H�[�}�b�g���擾//
	ID3D11Texture2D* pBackBuffer = NULL;
	HRESULT hr = pI->m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if ( FAILED( hr ) ) return;

	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc( &descBackBuffer );
	pBackBuffer->Release();


	//(3)CPU�ǂݏo���\�ȃo�b�t�@��GPU��ɍ쐬//
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


	//(4)�쐬����CPU�ǂݍ��݉\�o�b�t�@��GPU��Ńf�[�^���R�s�[//
	ID3D11Resource* hResource;
	pI->m_pBackBuffer_TexRTV->GetResource( &hResource );
	pI->m_pContext11->CopyResource( hCaptureTexture, hResource );
	hResource->Release();

	//(5)GPU��̓ǂݍ��݉\�o�b�t�@�̃������A�h���X�̃}�b�v���J��//
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pI->m_pContext11->Map( hCaptureTexture, 0, D3D11_MAP_READ, 0, &mappedResource );

	//(6)CPU��̃������Ƀo�b�t�@���m��//
	double width = descBackBuffer.Width;
	double height = descBackBuffer.Height;
	double src_stride = mappedResource.RowPitch;    //(��)descBackBuffer.Width * 4�Ƃ͕K��������v���Ȃ�
	size_t buffer_size = src_stride * height;
	BYTE* bmp_buffer;// = new BYTE[buffer_size];

	//(7)CPU��̃o�b�t�@�̉摜���t�@�C�������o��//
	HBITMAP hBmp;
	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( BITMAPINFO ) );
	bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	hBmp = CreateDIBSection( 0, &bmi, DIB_RGB_COLORS, ( void** )&bmp_buffer, 0, 0 );

	//(8)GPU��̓ǂݍ��݉\�o�b�t�@����CPU��̃o�b�t�@�֓]��//
	CopyMemory( bmp_buffer, mappedResource.pData, buffer_size );
	pI->m_pContext11->Unmap( hCaptureTexture, 0 );
	hCaptureTexture->Release();

	// �X�N�V���e�X�g.
	if ( GetAsyncKeyState( VK_SPACE ) & 0x0001 ) {
		WCHAR filename[] = L"Data\\test.bmp";
		SaveImageFile( hBmp, filename );
	}

	// �E�B���h�E�𓧖��ɂ���.
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
	
	// ���.
	ReleaseDC( NULL, hdcScreen );
	SelectObject( hdcBmp, hBmpPrev );
	DeleteDC( hdcBmp );
	DeleteObject( hBmp );
#endif
}

//---------------------------.
// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬.
//---------------------------.
HRESULT CDirectX11::CreateDeviceAndSwapChain()
{
	CDirectX11* pI = GetInstance();

	// �X���b�v�`�F�[���\����.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );									// 0�ŏ�����.
	sd.BufferCount			= 1;										// �o�b�N�o�b�t�@�̐�.
	sd.BufferDesc.Width		= pI->m_WndWidth;							// �o�b�N�o�b�t�@�̕�.
	sd.BufferDesc.Height	= pI->m_WndHeight;							// �o�b�N�o�b�t�@�̍���.
	sd.BufferDesc.Format	= DXGI_FORMAT_B8G8R8A8_UNORM;				// �t�H�[�}�b�g(32�r�b�g�J���[).
	sd.BufferDesc.RefreshRate.Numerator		= static_cast<UINT>( FPS );	// ���t���b�V�����[�g(����) ��FPS:60.
	sd.BufferDesc.RefreshRate.Denominator	= 1;						// ���t���b�V�����[�g(���q).
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;			// �g����(�\����).
	sd.OutputWindow			= pI->m_hWnd;								// �E�B���h�E�n���h��.
	sd.SampleDesc.Count		= 1;										// �}���`�T���v���̐�.
	sd.SampleDesc.Quality	= 0;										// �}���`�T���v���̃N�I���e�B.
	sd.Windowed				= TRUE;										// �E�B���h�E���[�h(�t���X�N���[������FALSE).

	// �쐬�����݂�@�\���x���̗D����w��.
	//	(GPU���T�|�[�g����@�\�Z�b�g�̒�`).
	//	D3D_FEATURE_LEVEL�񋓌^�̔z��.
	//	D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 �� GPU���x��.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;//�z��̗v�f��.

	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬.
	//	�n�[�h�E�F�A(GPU)�f�o�C�X�ł̍쐬.
	if( FAILED(
		D3D11CreateDeviceAndSwapChain(
			nullptr,					// �r�f�I�A�_�v�^�ւ̃|�C���^.
			D3D_DRIVER_TYPE_HARDWARE,	// �쐬����f�o�C�X�̎��.
			nullptr,					// �\�t�g�E�F�A ���X�^���C�U����������DLL�̃n���h��.
			0,							// �L���ɂ��郉���^�C�����C���[.
			&pFeatureLevels,			// �쐬�����݂�@�\���x���̏������w�肷��z��ւ̃|�C���^.
			1,							// ���̗v�f��.
			D3D11_SDK_VERSION,			// SDK�̃o�[�W����.
			&sd,						// �X���b�v�`�F�[���̏������p�����[�^�̃|�C���^.
			&m_pSwapChain,				// (out)�����_�����O�Ɏg�p����X���b�v�`�F�[��.
			&m_pDevice11,				// (out)�쐬���ꂽ�f�o�C�X.
			pFeatureLevel,				// �@�\���x���̔z��ɂ���ŏ��̗v�f��\���|�C���^.
			&m_pContext11 ) ) )			// (out)�f�o�C�X �R���e�L�X�g.
	{
		// WARP�f�o�C�X�̍쐬.
		//	D3D_FEATURE_LEVEL_9_1�`D3D_FEATURE_LEVEL_10_1.
		if( FAILED(
			D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
				0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
				&sd, &m_pSwapChain, &m_pDevice11,
				pFeatureLevel, &m_pContext11 ) ) )
		{
			// ���t�@�����X�f�o�C�X�̍쐬.
			//	DirectX SDK���C���X�g�[������Ă��Ȃ��Ǝg���Ȃ�.
			if( FAILED(
				D3D11CreateDeviceAndSwapChain(
					nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr,
					0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
					&sd, &m_pSwapChain, &m_pDevice11,
					pFeatureLevel, &m_pContext11 ) ) )
			{
				ErrorMessage( "�f�o�C�X�ƃX���b�v�`�F�[�� �쐬 : ���s" );
				return E_FAIL;
			}
		}
	}

	// �E�B���h�E�̐ݒ�̎擾.
	json WndSetting = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );

	// �t���X�N���[����ꕔ�̋@�\�𖳌�������.
	if ( WndSetting["IsFullScreenLock"] ) {
		// ALT + Enter�Ńt���X�N���[���𖳌�������.
		IDXGIFactory* pFactory = nullptr;
		// ��ō����IDXGISwapChain���g��.
		m_pSwapChain->GetParent( __uuidof( IDXGIFactory ), (void**) &pFactory );
		// �]�v�ȋ@�\�𖳌��ɂ���ݒ������.
		pFactory->MakeWindowAssociation( m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER );
		SAFE_RELEASE( pFactory );
	}

	// �w�i�̐F�̐ݒ�.
	m_InitBackColor = Color4::RGBA(
		static_cast<float>( WndSetting["BackColor"]["R"] ),
		static_cast<float>( WndSetting["BackColor"]["G"] ),
		static_cast<float>( WndSetting["BackColor"]["B"] )
	);
	m_BackColor = m_InitBackColor;
	return S_OK;
}

//---------------------------.
// ���X�^���C�U�X�e�[�g�ݒ�.
//---------------------------.
void CDirectX11::SetRasterizerState( const ERS_STATE& RsState )
{
	CDirectX11* pI = GetInstance();

	switch ( RsState ) {
	case ERS_STATE::None:
		// ���w�ʕ`��.
		pI->m_pContext11->RSSetState( pI->m_pCullNone );
		break;
	case ERS_STATE::Back:
		// �w�ʂ�`�悵�Ȃ�.
		pI->m_pContext11->RSSetState( pI->m_pCullBack );
		break;
	case ERS_STATE::Front:
		// ���ʂ�`�悵�Ȃ�.
		pI->m_pContext11->RSSetState( pI->m_pCullFront );
		break;
	case ERS_STATE::Wire:
		// ���C���[�t���[���`��.
		pI->m_pContext11->RSSetState( pI->m_pWireFrame );
		break;
	default:
		break;
	}
}

//---------------------------.
// �t���X�N���[���̐ݒ�.
//---------------------------.
void CDirectX11::SetFullScreen( const bool Flag )
{
	CDirectX11* pI = GetInstance();

	// ���݂̃X�N���[�������擾.
	BOOL IsFullScreen = FALSE;
	pI->m_pSwapChain->GetFullscreenState( &IsFullScreen, nullptr );

	// �t���X�N���[���ɕύX���邩.
	if ( Flag ) {
		// ���݃t���X�N���[���Ȃ�s��Ȃ�.
		if ( IsFullScreen == TRUE ) return;

		// �t���X�N���[���ɕύX.
		pI->m_pSwapChain->SetFullscreenState( TRUE, nullptr );

		// �}�E�X���\���ɂ���.
		if ( pI->m_IsDispMouseCursor == false ) return;
		ShowCursor( FALSE );
	}
	// ���݃E�B���h�E��ԂɕύX���邩.
	else {
		// ���݃E�B���h�E��ԂȂ�s��Ȃ�.
		if ( IsFullScreen == FALSE ) return;

		// �E�B���h�E�ɕύX.
		pI->m_pSwapChain->SetFullscreenState( FALSE, nullptr );

		// �}�E�X��\������.
		if ( pI->m_IsDispMouseCursor == false ) return;
		ShowCursor( TRUE );
	}
}

//----------------------------.
// �t���X�N���[����Ԃ��擾.
//----------------------------.
bool CDirectX11::IsFullScreen()
{
	CDirectX11* pI = GetInstance();

	if ( GetInstance()->m_pSwapChain == nullptr ) return false;

	// ���݂̃X�N���[�������擾.
	BOOL isState = FALSE;
	GetInstance()->m_pSwapChain->GetFullscreenState( &isState, nullptr );
	return static_cast<bool>( isState );
}

//----------------------------.
// �E�B���h�E�T�C�Y���ύX���ꂽ���ɌĂ�.
//----------------------------.
void CDirectX11::Resize()
{
	CDirectX11* pI = GetInstance();

	if( pI->m_pContext11 == nullptr ) return;

	// ���݂̃X�N���[�������擾.
	BOOL IsFullScreen = FALSE;
	pI->m_pSwapChain->GetFullscreenState( &IsFullScreen, nullptr );

	// �Z�b�g���Ă��郌���_�[�^�[�Q�b�g���O��.
	pI->m_pContext11->OMSetRenderTargets( 0, nullptr, nullptr );

	// �g�p���Ă����o�b�N�o�b�t�@���������.
	SAFE_RELEASE( pI->m_pBackBuffer_TexRTV );
	SAFE_RELEASE( pI->m_pBackBuffer_DSTex );
	SAFE_RELEASE( pI->m_pBackBuffer_DSTexDSV );

	// �X���b�v�`�F�[�������T�C�Y����.
	// width, height ���w�肵�Ȃ��ꍇ�AhWnd���Q�Ƃ��A�����Ōv�Z���Ă����.
	if( FAILED( pI->m_pSwapChain->ResizeBuffers( 0, 0, 0, DXGI_FORMAT_UNKNOWN, 0 ) )){
		ErrorMessage( "�f�v�X�X�e���V���r���[�쐬���s" );
		return;
	}

	// �X���b�v�`�F�[���̃o�b�t�@�̎擾.
	ID3D11Texture2D* pBuufer = nullptr;
	if( FAILED( pI->m_pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBuufer ) )){
		ErrorMessage( "�f�v�X�X�e���V���r���[�쐬���s" );
		return;
	}

	// �e�N�X�`�����̎擾.
	D3D11_TEXTURE2D_DESC texDesc = {0};
	pBuufer->GetDesc( &texDesc );
	pI->m_WndWidth	= texDesc.Width;
	pI->m_WndHeight	= texDesc.Height;
	SAFE_RELEASE( pBuufer );

	if( FAILED( pI->CreateColorBackBufferRTV() ) ) {
		ErrorMessage( "�f�v�X�X�e���V���r���[�쐬���s" );
		return;
	}
	if( FAILED( pI->CreateDepthStencilBackBufferRTV() )){
		ErrorMessage( "�f�v�X�X�e���V���r���[�쐬���s" );
		return;
	}

	// �����_�[�^�[�Q�b�g�̐ݒ�.
	pI->m_pContext11->OMSetRenderTargets( 
		1, 
		&pI->m_pBackBuffer_TexRTV,
		pI->m_pBackBuffer_DSTexDSV );
	// �f�v�X�X�e���V���o�b�t�@.
	pI->m_pContext11->ClearDepthStencilView(
		pI->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	// �r���[�|�[�g�̐ݒ�.
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
// �A�N�e�B�u�E�B���h�E���m�F.
//----------------------------.
void CDirectX11::CheckActiveWindow()
{
	CDirectX11* pI = GetInstance();

	// �����̃E�B���h�E�n���h����PC�̍őO�ʂ̃E�B���h�E�n���h�����r.
	pI->m_IsWindowActive = pI->m_hWnd == GetForegroundWindow();
}

//----------------------------.
// D3DX10CompileFromFile�}�N�����Ăяo��.
//----------------------------.
HRESULT CDirectX11::MutexD3DX10CompileFromFile( const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
	LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX10ThreadPump* pPump,
	ID3D10Blob** ppOutShader, ID3D10Blob** ppOutErrorMsgs, HRESULT* pOutHResult )
{
	std::unique_lock<std::mutex> lock( D10CFFmtx );

	HRESULT hr;
	hr = D3DX10CompileFromFile(
		filepath,		// �V�F�[�_�[�t�@�C���p�X.
		pDefines,		// �}�N����`�̃|�C���^�[ : �I�v�V����.
		pInclude,		// �C���N���[�h�t�@�C�����������邽�߂̃|�C���^ : �I�v�V����.
		entryPoint,		// �V�F�[�_�[�̃G���g���[�|�C���g�֐���.
		shaderModel,	// �V�F�[�_�[���f���̖��O.
		compileFlag,	// �V�F�[�_�[�R���p�C���t���O.
		Flags2,			// �G�t�F�N�g�R���p�C���t���O (0����).
		pPump,			// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[.
		ppOutShader,	// �R���p�C�����ꂽ�V�F�[�_�[�̃f�[�^ (out).
		ppOutErrorMsgs,	// �R���p�C�����̃G���[�o�� (out).
		pOutHResult );	// �߂�l�̃|�C���^.
	return hr;
}

//----------------------------.
// D3DX11CompileFromFile�}�N�����Ăяo��.
//----------------------------.
HRESULT CDirectX11::MutexD3DX11CompileFromFile( const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
	LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX11ThreadPump* pPump,
	ID3D10Blob** ppOutShader, ID3D10Blob** ppOutErrorMsgs, HRESULT* pOutHResult )
{
	std::unique_lock<std::mutex> lock( D11CFFmtx );

	HRESULT hr;
	hr = D3DX11CompileFromFile(
		filepath,		// �V�F�[�_�[�t�@�C���p�X.
		pDefines,		// �}�N����`�̃|�C���^�[ : �I�v�V����.
		pInclude,		// �C���N���[�h�t�@�C�����������邽�߂̃|�C���^ : �I�v�V����.
		entryPoint,		// �V�F�[�_�[�̃G���g���[�|�C���g�֐���.
		shaderModel,	// �V�F�[�_�[���f���̖��O.
		compileFlag,	// �V�F�[�_�[�R���p�C���t���O.
		Flags2,			// �G�t�F�N�g�R���p�C���t���O (0����).
		pPump,			// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[.
		ppOutShader,	// �R���p�C�����ꂽ�V�F�[�_�[�̃f�[�^ (out).
		ppOutErrorMsgs,	// �R���p�C�����̃G���[�o�� (out).
		pOutHResult );	// �߂�l�̃|�C���^.
	return hr;
}

//----------------------------.
// D3DX11CreateShaderResourceViewFromFile�}�N�����Ăяo��.
//----------------------------.
HRESULT CDirectX11::MutexD3DX11CreateShaderResourceViewFromFile(
	const LPCTSTR pSrcFile, D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
	ID3D11ShaderResourceView** ppShaderResourceView, HRESULT* pHResult )
{
	std::unique_lock<std::mutex> lock( D11CSFmtx );

	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile(
		GetInstance()->m_pDevice11,		// ���\�[�X���g�p����f�o�C�X�̃|�C���^.
		pSrcFile,						// �t�@�C����.
		pLoadInfo,						// �V�F�[�_�[���\�[�X�r���[���܂ރt�@�C���̖��O.
		pPump,							// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[
		ppShaderResourceView,			// (out)�e�N�X�`��.
		pHResult );
	return hr;
}

//----------------------------.
// D3DXLoadMeshFromX�}�N�����Ăяo��.
//----------------------------.
HRESULT CDirectX11::MutexD3DXLoadMeshFromX(
	const LPCTSTR pFilename, DWORD Options, LPDIRECT3DDEVICE9 pD3DDevice, LPD3DXBUFFER* ppAdjacency, 
	LPD3DXBUFFER* ppMaterials, LPD3DXBUFFER* ppEffectInstances, DWORD* pNumMaterials, LPD3DXMESH* ppMesh )
{
	std::unique_lock<std::mutex> lock( D9LMFmtx );

	HRESULT hr;

	hr = D3DXLoadMeshFromX(
		pFilename,			// �t�@�C����.
		Options,			// ���b�V���̍쐬�I�v�V����.
		pD3DDevice,			// ���b�V���Ɋ֘A�t�����Ă���f�o�C�X�I�u�W�F�N�g.
		ppAdjacency,		// �אڊ֌W�f�[�^���܂ރo�b�t�@�[�ւ̃|�C���^�[.
		ppMaterials,		// (out)�}�e���A���f�[�^���܂ރo�b�t�@�[�ւ̃|�C���^�[.
		ppEffectInstances,	// (out)���ʃC���X�^���X�̔z����܂ރo�b�t�@�[�ւ̃|�C���^�[.
		pNumMaterials,		// (out)�}�e���A����.
		ppMesh );			// (out)�ǂݍ��܂ꂽ���b�V��.
	return hr;
}

//----------------------------.
// D11CreateVertexShader���Ăяo��.
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
// D11CreatePixelShader���Ăяo��.
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
// D11CreateInputLayout���Ăяo��.
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
// ���X�^���C�U�쐬.
//----------------------------.
HRESULT CDirectX11::CreateRasterizer()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.FillMode = D3D11_FILL_SOLID;//�h��Ԃ�(�\���b�h).

	// �J�����O�̐ݒ�.
	//	D3D11_CULL_BACK:�w�ʂ�`�悵�Ȃ�.
	//	D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
	//	D3D11_CULL_NONE:�J�����O��؂�(���w�ʂ�`�悷��).
	rdc.CullMode = D3D11_CULL_NONE;

	if ( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &m_pCullNone ) ) ){
		ErrorMessage( "���X�^���C�U�[ �쐬 : ���s" );
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_SOLID;// �h��Ԃ�(�\���b�h).
	rdc.CullMode = D3D11_CULL_BACK;	// BACK:�w�ʂ�`�悵�Ȃ�,
	if ( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &m_pCullBack ) ) ){
		ErrorMessage( "���X�^���C�U�[ �쐬 : ���s" );
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_SOLID;// �h��Ԃ�(�\���b�h).
	rdc.CullMode = D3D11_CULL_FRONT;// FRONT:���ʂ�`�悵�Ȃ�.
	if ( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &m_pCullFront ) ) ){
		ErrorMessage( "���X�^���C�U�[ �쐬 : ���s" );
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_WIREFRAME;// ���C���[�t���[��.
	rdc.CullMode = D3D11_CULL_NONE;		// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	if ( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &m_pWireFrame ) ) ){
		ErrorMessage( "���X�^���C�U�[ �쐬 : ���s" );
		return E_FAIL;
	}

	// �|���S���̕\�������肷��t���O.
	//	TRUE:�����Ȃ�O�����B�E���Ȃ�������B
	//	FALSE:�t�ɂȂ�.
	rdc.FrontCounterClockwise = FALSE;

	// �����ɂ��ẴN���b�s���O�L��.
	rdc.DepthClipEnable = FALSE;

	m_pContext11->RSSetState( m_pCullNone );

	return S_OK;
}

//----------------------------.
// �f�v�X�X�e���V���ݒ�.
//	���̊֐��P��ON/OFF��2��ނ��쐬����.
//----------------------------.
HRESULT CDirectX11::CreateDepthStencilState()
{
	// �[�x�e�X�g(Z�e�X�g)�̐ݒ�.
	//	��on/off�̋��ʕ����̂ݐݒ�.
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable	= FALSE;
	dsDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// �[�x�e�X�g(Z�e�X�g)��L���ɂ���.
	dsDesc.DepthEnable = TRUE;//�L��.
	// �[�x�ݒ�쐬.
	if( FAILED(
		m_pDevice11->CreateDepthStencilState(
			&dsDesc, &m_pDepthStencilStateOn ) ) )
	{
		ErrorMessage( "�[�xON�ݒ� �쐬 : ���s" );
		return E_FAIL;
	}

	// �[�x�e�X�g(Z)�e�X�g�𖳌��ɂ���.
	dsDesc.DepthEnable = FALSE;//����.
	// �[�x�ݒ�쐬.
	if( FAILED(
		m_pDevice11->CreateDepthStencilState(
			&dsDesc, &m_pDepthStencilStateOff ) ) )
	{
		ErrorMessage( "�[�xOFF�ݒ� �쐬 : ���s" );
		return E_FAIL;
	}

	return S_OK;
}


//----------------------------.
// �u�����h�X�e�[�g�쐬.
//	�A���t�@�u�����h��ON/OFF�̂Q��ނ��쐬.
//----------------------------.
HRESULT CDirectX11::CreateAlphaBlendState()
{
	// �A���t�@�u�����h�p�u�����h�X�e�[�g�\����.
	//	png�t�@�C�����ɃA���t�@��񂪂���̂ŁA���߂���悤�Ƀu�����h�X�e�[�g�Őݒ肷��.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory( &BlendDesc, sizeof( BlendDesc ) );	// ������.

	BlendDesc.IndependentBlendEnable = false;		// false:RenderTarget[0]�̃����o�[�̂ݎg�p����.
													//	true:RenderTarget[0�`7]���g�p�ł���.
													//	(�����_�[�^�[�Q�b�g���ɓƗ������u�����h����).
	// �A���t�@�g�D�J�o���[�W���g�p����.
	BlendDesc.AlphaToCoverageEnable = false;
	// ���f�ނɑ΂���ݒ�.
	BlendDesc.RenderTarget[0].SrcBlend			= D3D11_BLEND_SRC_ALPHA;		// �A���t�@�u�����h���w��.
	// �d�˂�f�ނɑ΂���ݒ�.
	BlendDesc.RenderTarget[0].DestBlend			= D3D11_BLEND_INV_SRC_ALPHA;	// �A���t�@�u�����h�̔��]���w��.
	// �u�����h�I�v�V����.
	BlendDesc.RenderTarget[0].BlendOp			= D3D11_BLEND_OP_ADD;			// ADD:���Z����.
	// ���f�ނ̃A���t�@�ɑ΂���w��.
	BlendDesc.RenderTarget[0].SrcBlendAlpha		= D3D11_BLEND_ONE;				// ���̂܂܎g�p.
	// �d�˂�f�ނ̃A���t�@�ɑ΂���ݒ�.
	BlendDesc.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND_ZERO;				// �������Ȃ��B
	// �A���t�@�̃u�����h�I�v�V����.
	BlendDesc.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP_ADD;			// ADD:���Z����.
	// �s�N�Z�����̏������݃}�X�N.
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;	// �S�Ă̐���(RGBA)�ւ̃f�[�^�̊i�[��������.

	// �A���t�@�u�����h���g�p����.
	BlendDesc.RenderTarget[0].BlendEnable = true;	// �L��.
	// �u�����h�X�e�[�g�쐬.
	if( FAILED(
		m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaBlendOn ) ) )
	{
		ErrorMessage( "�A���t�@�u�����h�X�e�[�gON�ݒ� �쐬 : ���s" );
		return E_FAIL;
	}

	// �A���t�@�g�D�J�o���[�W���g�p����.
	BlendDesc.AlphaToCoverageEnable = true;	// �L��.
	// �u�����h�X�e�[�g�쐬.
	if ( FAILED(
		m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaToCoverageOn ) ) )
	{
		ErrorMessage( "�A���t�@�g�D�J�o���[�W�X�e�[�gON�ݒ� �쐬 : ���s" );
		return E_FAIL;
	}

	// �A���t�@�g�D�J�o���[�W���g�p���Ȃ�.
	BlendDesc.AlphaToCoverageEnable			= false;	//����.
	// �A���t�@�u�����h���g�p���Ȃ�.
	BlendDesc.RenderTarget[0].BlendEnable	= false;	//����.
	// �u�����h�X�e�[�g�쐬.
	if( FAILED(
		m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaBlendOff ) ) )
	{
		ErrorMessage( "�A���t�@�u�����h�X�e�[�gOFF�ݒ� �쐬 : ���s" );
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------.
// �r���[�|�[�g�쐬.
//----------------------------.
HRESULT CDirectX11::CreateViewports()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT) m_WndWidth;	// ��.
	vp.Height	= (FLOAT) m_WndHeight;	// ����.
	vp.MinDepth = 0.0f;					// �ŏ��[�x(��O).
	vp.MaxDepth = 1.0f;					// �ő�[�x(��).
	vp.TopLeftX = 0.0f;					// ����ʒux.
	vp.TopLeftY = 0.0f;					// ����ʒuy.

	m_pContext11->RSSetViewports( 1, &vp );

	return S_OK;
}

//----------------------------.
// ���ߐݒ�̐؂�ւ�.
//----------------------------.
void CDirectX11::SetAlphaBlend( bool flag )
{
	CDirectX11* pI = GetInstance();
	if ( pI->m_IsAlphaToCoverage ) return;

	UINT mask = 0xffffffff;	// �}�X�N�l.
	ID3D11BlendState* pTmp
		= ( flag == true ) ? pI->m_pAlphaBlendOn : pI->m_pAlphaBlendOff;

	// �A���t�@�u�����h�ݒ���Z�b�g.
	pI->m_pContext11->OMSetBlendState( pTmp, nullptr, mask );
	pI->m_IsAlphaBlend = flag;
}

//----------------------------.
// �A���t�@�g�D�J�o���[�W��L��:�����ɐݒ肷��.
//----------------------------.
void CDirectX11::SetAlphaToCoverage( bool flag )
{
	CDirectX11* pI = GetInstance();

	// �u�����h�X�e�[�g�̐ݒ�.
	UINT mask = 0xffffffff;	// �}�X�N�l.
	ID3D11BlendState* blend 
		= ( flag == true ) ? pI->m_pAlphaToCoverageOn : pI->m_pAlphaBlendOff;
	pI->m_pContext11->OMSetBlendState( blend, nullptr, mask );
	pI->m_IsAlphaToCoverage = flag;
}

//----------------------------.
// �[�x(Z)�e�X�gON/OFF�؂�ւ�.
//----------------------------.
void CDirectX11::SetDepth(bool flag)
{
	CDirectX11* pI = GetInstance();

	ID3D11DepthStencilState* pTmp
		= (flag == true) ? pI->m_pDepthStencilStateOn : pI->m_pDepthStencilStateOff;

	// �[�x�ݒ���Z�b�g.
	pI->m_pContext11->OMSetDepthStencilState( pTmp, 1 );
	pI->m_IsDepth = flag;
}

//----------------------------.
// �o�b�N�o�b�t�@�쐬:�J���[�p�����_�[�^�[�Q�b�g�r���[�쐬.
//----------------------------.
HRESULT CDirectX11::CreateColorBackBufferRTV()
{
	// �o�b�N�o�b�t�@�e�N�X�`�����擾(���ɂ���̂ō쐬�ł͂Ȃ�).
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	if( FAILED(
		m_pSwapChain->GetBuffer(
			0,
			__uuidof( ID3D11Texture2D ),	// __uuidof:���Ɋ֘A�t�����ꂽGUID���擾
											//	Texture2D�̗B��̕��Ƃ��Ĉ���
			(LPVOID*)&pBackBuffer_Tex ) ) )	// (out)�o�b�N�o�b�t�@�e�N�X�`��.
	{
		ErrorMessage( "�X���b�v�`�F�C������o�b�N�o�b�t�@ �擾 : ���s" );
		return E_FAIL;
	}

	// ���̃e�N�X�`���ɑ΂��ă����_�[�^�[�Q�b�g�r���[(RTV)���쐬.
	if( FAILED(
		m_pDevice11->CreateRenderTargetView(
			pBackBuffer_Tex,
			nullptr,
			&m_pBackBuffer_TexRTV ) ) )	// (out)RTV.
	{
		// �o�b�N�o�b�t�@�e�N�X�`�������.
		SAFE_RELEASE( pBackBuffer_Tex );
		ErrorMessage( "�����_�[�^�C�Q�b�g�r���[ �쐬 : ���s" );
		return E_FAIL;
	}
	// �o�b�N�o�b�t�@�e�N�X�`�������.
	SAFE_RELEASE( pBackBuffer_Tex );

	return S_OK;
}

//----------------------------.
// �o�b�N�o�b�t�@�쐬:�f�v�X�X�e���V���p�����_�[�^�[�Q�b�g�r���[�쐬.
//----------------------------.
HRESULT CDirectX11::CreateDepthStencilBackBufferRTV()
{
	CDirectX11* pI = GetInstance();

	// �f�v�X(�[��or�[�x)�X�e���V���r���[�p�̃e�N�X�`�����쐬.
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= m_WndWidth;				// ��.
	descDepth.Height				= m_WndHeight;				// ����.
	descDepth.MipLevels				= 1;						// �~�b�v�}�b�v���x��:1.
	descDepth.ArraySize				= 1;						// �z��:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32�r�b�g�t�H�[�}�b�g.
	descDepth.SampleDesc.Count		= 1;						// �}���`�T���v���̐�.
	descDepth.SampleDesc.Quality	= 0;						// �}���`�T���v���̃N�I���e�B.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// �g�p���@:�f�t�H���g.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// �[�x(�X�e���V���Ƃ��Ďg�p).
	descDepth.CPUAccessFlags		= 0;						// CPU����̓A�N�Z�X���Ȃ�.
	descDepth.MiscFlags				= 0;						// ���̑��̐ݒ�Ȃ�.

	// ���̃e�N�X�`���ɑ΂��ăf�v�X�X�e���V��(DSTex)���쐬.
	if( FAILED(
		m_pDevice11->CreateTexture2D(
			&descDepth,
			nullptr,
			&m_pBackBuffer_DSTex ) ) )	// (out)�f�v�X�X�e���V���p�e�N�X�`��.
	{
		ErrorMessage( "�f�v�X�X�e���V���쐬 : ���s" );
		return E_FAIL;
	}

	// ���̃e�N�X�`���ɑ΂��ăf�v�X�X�e���V���r���[(DSV)���쐬.
	if( FAILED( m_pDevice11->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		nullptr,
		&m_pBackBuffer_DSTexDSV ) ) )	// (out)DSV.
	{
		ErrorMessage( "�f�v�X�X�e���V���r���[ �쐬 : ���s" );
		return E_FAIL;
	}

	// �����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���p�C�v���C���ɃZ�b�g.
	pI->m_pContext11->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV );

	return S_OK;
}
