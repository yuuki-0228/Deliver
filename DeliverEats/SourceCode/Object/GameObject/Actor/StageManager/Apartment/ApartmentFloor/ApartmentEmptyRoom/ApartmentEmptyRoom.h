#pragma once
#include "..\..\..\..\Actor.h"

/************************************************
*	�}���V�����^�C�v�̋󕔉��N���X.
*		���c�F�P.
**/
class CApartmentEmptyRoom final
	: public CActor
{
public:
	CApartmentEmptyRoom();
	virtual ~CApartmentEmptyRoom();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �\�����邩.
	void SetIsDisp( const bool Flag ) { m_FloorIsDisp = Flag; }
	// �A���t�@�̐ݒ�.
	void SetAlpha( const float Alpha ) { m_Alpha = Alpha; };

private:
	// ���C�g�̍X�V.
	void LightUpdate();
	// ���C�g������.
	void LightOn( const std::pair<int, int>& Time );
	// ���C�g������.
	void LightOff( const std::pair<int, int>& Time );

private:
	CSkinMesh*					m_pRoomMesh;		// �������b�V��.
	SMeshPointLight				m_PointLight;		// �|�C���g���C�g.
	float						m_Alpha;			// �A���t�@�l.
	int							m_LightOnHour;		// ���C�g�����鎞��.
	int							m_LightOnMinute;	// ���C�g�����镪.
	int							m_LightOffHour;		// ���C�g����������.
	int							m_LightOffMinute;	// ���C�g��������.
	bool						m_FloorIsDisp;		// 1�t���A�P�ʂ̕\�����邩�ǂ���.
	bool						m_IsLightOn;		// ���C�g��������.
	bool						m_IsLightOff;		// ���C�g����������.
	bool						m_IsEmptyRoom;		// �l���Z��ł��Ȃ���.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
