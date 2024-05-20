#pragma once
#include "..\SceneBase.h"

class CStageEditor;

/************************************************
*	�X�e�[�W�G�f�B�^�V�[��.
**/
class CStageEdit
	: public CSceneBase
{
public:
	CStageEdit();
	~CStageEdit();

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
	virtual void Sprite3DRender_A() override {};
	virtual void Sprite3DRender_B() override {};

private:
	std::unique_ptr<CStageEditor>	m_pStageEditor;		// �X�e�[�W�G�f�B�^.

};