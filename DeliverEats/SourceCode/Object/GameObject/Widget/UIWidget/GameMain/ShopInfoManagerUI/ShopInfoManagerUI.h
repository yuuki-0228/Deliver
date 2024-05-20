#pragma once
#include "..\..\UIWidget.h"

class CPlayer;
class CShopInfoUI;
class CEmployee;

/************************************************
*	���X�̏ڍ׏��}�l�[�W���[�N���X.
**/
class CShopInfoManagerUI final
	: public CUIWidget
{
public:
	CShopInfoManagerUI();
	~CShopInfoManagerUI();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �`��.
	virtual void Render() override;

	// �v���C���[�̐ݒ�.
	void SetPlayer( CPlayer* pPlayer );

private:
	// �\���̍X�V.
	void OpenUpdate();
	// ��\���̍X�V.
	void CloseUpdate();
	// �ڍ׏��̕ύX�̍X�V.
	void ChangeUpdate();
	
	// �N�[���^�C���̍X�V.
	void CoolTimeUpdate();

private:
	CPlayer*									m_pPlayer;				// �v���C���[.
	std::vector<std::unique_ptr<CShopInfoUI>>	m_pShopInfoList;		// ���X�̏ڍ׏�񃊃X�g.
	std::vector<CEmployee*>						m_pDispEmployeeList;	// �\�����Ă���X�����X�g.
	int											m_DispNum;				// �\�����Ă��鐔.
	float										m_CoolTime;				// �N�[���^�C��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SShopInfo& CONSTANT = CConstant::GetShopInfo();
};
