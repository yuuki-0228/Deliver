#pragma once
#include "..\SceneBase.h"

class CSkinMesh;
class CTitleWidget;
class CConfilmationWindowUI;

/************************************************
*	�^�C�g���N���X.
*		���c�F�P.
**/
class CTitle final
	: public CSceneBase
{
public:
	CTitle();
	virtual ~CTitle();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// ���f���̕`��.
	virtual void ModelRender() override;
	// �X�v���C�g(UI)�̕`��.
	virtual void SpriteUIRender() override;
	// �X�v���C�g(3D)/Effect�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

private:
	CSkinMesh*								m_pHouse;					// ��.
	CSkinMesh*								m_pDoor;					// �h�A.
	std::unique_ptr<CTitleWidget>			m_pTitleWidget;				// UI.
	std::unique_ptr<CConfilmationWindowUI>	m_pConfilmationWindowUI;	// �m�F���.
	STransform								m_DoorTransform;			// �h�A�̃g�����X�t�H�[��.
};