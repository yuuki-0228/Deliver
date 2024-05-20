#include "Common.h"

CCommon::CCommon()
	: m_pDevice		( nullptr )
	, m_pContext	( nullptr )
	, m_Transform	()
	, m_Color		( Color4::White )
{
	m_pContext	= CDirectX11::GetContext();
	m_pDevice	= CDirectX11::GetDevice();
}

CCommon::~CCommon()
{
	m_pContext	= nullptr;
	m_pDevice	= nullptr;
}

//---------------------------.
// アルファ値の設定.
//---------------------------.
void CCommon::SetAlpha( const float Alpha )
{
	m_Color.w = Alpha;
	if (		m_Color.w < 0.0f ) m_Color.w = 0.0f;
	else if (	m_Color.w > 1.0f ) m_Color.w = 1.0f;
}
