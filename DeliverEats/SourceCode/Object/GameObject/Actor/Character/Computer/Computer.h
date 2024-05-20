#pragma once
#include "..\Character.h"

/************************************************
*	�R���s���[�^�N���X.
**/
class CComputer final
	: public CCharacter
{
public:
	CComputer();
	virtual ~CComputer();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �����蔻��I����Ăяo�����X�V.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

private:
	// �g�����X�t�H�[���̍X�V.
	void TransformUpdate();

private:
	std::vector<CSkinMesh*>	m_pSkinMesh;		// �X�L�����b�V��.
	std::vector<STransform>	m_TransformList;	// �g�����X�t�H�[��.

};
