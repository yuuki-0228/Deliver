#pragma once
#include "..\..\..\Actor.h"

class CLensFlare;
class CSkyBoxMesh;

/************************************************
*	��N���X.
*		���c�F�P.
**/
class CSky final
	: public CActor
{
public:
	CSky();
	virtual ~CSky();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;
	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;

private:
	CSkyBoxMesh*				m_pColorMesh;			// ��̐F�p�̃��b�V��.
	CSkyBoxMesh*				m_pSunAndMonn;			// ���z�ƌ����b�V��.
	CSkyBoxMesh*				m_pCloudMesh;			// �_���b�V��.
	STransform					m_CloudTransform;		// �_�̃g�����X�t�H�[��.
	STransform					m_SunAndMonnTransform;	// ���z�ƌ��̃g�����X�t�H�[��.
	D3DXCOLOR3					m_CloudColor;			// �_�̐F.
	D3DXCOLOR3					m_SkyColor;				// ��̐F.
	std::vector<D3DXVECTOR3>	m_SkyColorVector;		// ��̐F�̕ω��p.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};