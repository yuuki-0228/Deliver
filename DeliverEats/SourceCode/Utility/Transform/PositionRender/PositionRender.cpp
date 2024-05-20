#include "PositionRender.h"
#include "..\..\..\Common\DirectX\DirectX11.h"
#include "..\..\..\Common\Mesh\StaticMesh\StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\Input\Input.h"

CPositionRender::CPositionRender()
	: m_pMesh		( nullptr )
	, m_Transform	()
	, m_IsRender	( false, u8"���W�̕\��/��\��", u8"System" )
{
}

CPositionRender::~CPositionRender()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CPositionRender* CPositionRender::GetInstance()
{
	static std::unique_ptr<CPositionRender> pInstance = std::make_unique<CPositionRender>();
	return pInstance.get();
}

//---------------------------.
// �X�V.
//---------------------------.
void CPositionRender::Update( const float& DeltaTime )
{
	CPositionRender* pI = GetInstance();

	// �\���̐؂�ւ�.
	if ( CDebugKeyInput::IsANDKeyDown( VK_CONTROL, VK_F11 ) ) {
		pI->m_IsRender = !pI->m_IsRender;
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CPositionRender::Render( const STransform& Transform )
{
#ifdef _DEBUG
	if ( Transform.IsPostionRender == false ) return;

	CPositionRender* pI = GetInstance();

	// �\�����邩.
	if ( pI->m_IsRender == false ) return;

	// ���f���̎擾.
	if ( pI->m_pMesh == nullptr ) pI->m_pMesh = CMeshResource::GetStatic( "XYZModel" );

	// ��ԑ傫���T�C�Y���擾.
	float Scale = Transform.Scale.x;
	if ( Scale < Transform.Scale.y ) Scale = Transform.Scale.y;
	if ( Scale < Transform.Scale.z ) Scale = Transform.Scale.z;

	// �g�����X�t�H�[���̍X�V.
	pI->m_Transform.Position		= Transform.Position;
	pI->m_Transform.Scale			= { Scale, Scale, Scale };
	pI->m_Transform.IsPostionRender	= false;

	// �g�����X�t�H�[���̕`��.
	pI->m_pMesh->Render( &pI->m_Transform );
#endif // #ifdef _DEBUG.
}
