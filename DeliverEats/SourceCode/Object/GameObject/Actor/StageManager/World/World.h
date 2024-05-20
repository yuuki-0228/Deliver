#pragma once
#include "..\..\Actor.h"
#include "..\..\..\..\..\Common\Sprite\Sprite.h"

class CSky;
class CCloud;

/************************************************
*	���[���h�N���X.
*		���c�F�P.
**/
class CWorld final
	: public CActor
{
public:
	CWorld();
	virtual ~CWorld();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;
	// �X�v���C�g(3D)�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

private:
	std::unique_ptr<CSky>	m_pSky;		// ��.
	std::unique_ptr<CCloud>	m_pCloud;	// �_.
};