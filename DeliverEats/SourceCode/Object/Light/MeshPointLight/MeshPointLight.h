#pragma once
#include "..\..\..\Global.h"

/************************************************
*	���b�V���ɑ΂��Ẵ|�C���g���C�g�\����.
**/
struct stMeshPointLight{
	float	Intensity;	// ����.
	float	Brightness;	// ���邳.

	stMeshPointLight()
		: Intensity		( 2.0f )
		, Brightness	( 1.0f )
	{}
	stMeshPointLight( const float intensity, const float brightness )
		: Intensity		( intensity )
		, Brightness	( brightness )
	{}
} typedef SMeshPointLight;