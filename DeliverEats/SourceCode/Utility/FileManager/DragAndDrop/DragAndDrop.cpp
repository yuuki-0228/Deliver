#include "DragAndDrop.h"

CDragAndDrop::CDragAndDrop()
	: m_hWnd			()
	, m_FilePathList	()
{
}

CDragAndDrop::~CDragAndDrop()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CDragAndDrop* CDragAndDrop::GetInstance()
{
	static std::unique_ptr<CDragAndDrop> pInstance = std::make_unique<CDragAndDrop>();
	return pInstance.get();
}

//---------------------------.
// ドラック&ドロップを使用する.
//---------------------------.
void CDragAndDrop::Open()
{
	// ドラック&ドロップを使用する.
	DragAcceptFiles( GetInstance()->m_hWnd, TRUE );
}

//---------------------------.
// ドラック&ドロップを使用しない.
//---------------------------.
void CDragAndDrop::Close()
{
	CDragAndDrop* pI = GetInstance();

	// ドラック&ドロップを使用しない.
	DragAcceptFiles( pI->m_hWnd, FALSE );

	// 保存しているファイルパスを削除する.
	pI->m_FilePathList.clear();
}
