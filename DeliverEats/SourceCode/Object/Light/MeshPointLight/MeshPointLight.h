#pragma once
#include "..\..\..\Global.h"

/************************************************
*	メッシュに対してのポイントライト構造体.
**/
struct stMeshPointLight{
	float	Intensity;	// 強さ.
	float	Brightness;	// 明るさ.

	stMeshPointLight()
		: Intensity		( 2.0f )
		, Brightness	( 1.0f )
	{}
	stMeshPointLight( const float intensity, const float brightness )
		: Intensity		( intensity )
		, Brightness	( brightness )
	{}
} typedef SMeshPointLight;