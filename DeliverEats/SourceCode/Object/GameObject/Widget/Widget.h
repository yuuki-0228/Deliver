#pragma once
#include "..\GameObject.h"
#include "..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\Common\Font\Font.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Resource\FontResource\FontResource.h"

namespace {
	// IðñÌ.
	enum class enChoice : unsigned char {
		StartChoice,	// ÅÌIð.
		EndChoice,		// ÅãÌIð.

		Max,			// Ååvf.
	} typedef EChoice;

	// ^CgÌIðñÌ.
	enum class enTitleChoice : unsigned char
	{
		GameStart,			// Q[Jn.
		MiniGame,			// ~jQ[.
		PizzaEdit,			// sUGfB^.
		RankingDelete,		// LOÌí.
		GameEnd,			// Q[I¹.

		Max,				// Ååvf.
		Start	= 0,		// êÔÅÌIð.
		End		= Max - 1,	// êÔÅãÌIð.
	} typedef ETitleChoice;

	// gkÇñÌ.
	enum class enScaling : unsigned char
	{
		None,			// Ò@.
		Expand,			// gå.
		Shrink,			// k¬.
	} typedef EScaling;
}

/************************************************
*	Q[àÌUIx[XNX.
*		ú±cFP.
**/
class CWidget
	: public CGameObject
{
public:
	using Sprite_map	= std::unordered_map<std::string, std::vector<CSprite*>>;
	using State_map		= std::unordered_map<std::string, SSpriteRenderStateList>;

public:
	CWidget();
	virtual ~CWidget();

	// `æ.
	virtual void Sprite3DRender_A() override final {};
	virtual void Sprite3DRender_B() override final {};

protected:
	const float m_Wnd_W;		// EBhEÌ.
	const float m_Wnd_H;		// EBhEÌ³.
};
