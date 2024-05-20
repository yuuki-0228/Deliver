#pragma once
#include "SceneManager\SceneManager.h"
#include "..\Common\SoundManeger\SoundManeger.h"
#include "..\Object\Camera\Cameras\Cameras.h"
#include "..\Object\Camera\CameraManager\CameraManager.h"
#include "..\Utility\Input\Input.h"

class CCameras;

/************************************************
*	シーンベースクラス.
*		﨑田友輝.
**/
class CSceneBase
{
public:
	CSceneBase();
	virtual ~CSceneBase();

	// 初期化.
	virtual bool Init() = 0;

	// 更新.
	virtual void Update( const float& DeltaTime ) = 0;

	// モデルの描画.
	virtual void ModelRender() = 0;
	// スプライト(UI)の描画.
	virtual void SpriteUIRender() = 0;
	// スプライト(3D)/Effectの描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() = 0;
	virtual void Sprite3DRender_B() = 0;

protected:
	std::unique_ptr<CCameras>	m_pCameras;		// 複数のカメラを所持するクラス.
	std::string					m_BGMName;		// BGM名.

};