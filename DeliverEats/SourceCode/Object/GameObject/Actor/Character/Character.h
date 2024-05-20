#pragma once
#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\Common\Mesh\StaticMesh\StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"

/************************************************
*	キャラクタークラス.
*		﨑田友輝.
**/
class CCharacter
	: public CActor
{
public:
	CCharacter();
	virtual ~CCharacter();

};
