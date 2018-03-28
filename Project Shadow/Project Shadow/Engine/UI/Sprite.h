#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "InterfaceElement.h"
#include "..\Defs.h"
#include "..\Log.h"
#include "..\App.h"
#include "..\ModuleRender.h"
#include "..\ModuleTextures.h"


class Sprite :
	public InterfaceElement
{
public:
	Sprite(uint _x = 0, uint _y = 0, SDL_Texture* _tex = nullptr, bool _enabled = false, SDL_Rect* _anim = nullptr);
	~Sprite();
	bool Update(float dt) override;

	SDL_Rect idle_anim;
};

#endif