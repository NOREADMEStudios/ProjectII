#ifndef __HEALTHBAR_H__
#define __HEALTHBAR_H__

#include "Sprite.h"
#include "../Hero.h"


class Healthbar :
	public Sprite
{
public:

	Healthbar(Hero* character, bool leftSide = true, uint _x = 0, uint _y = 0, SDL_Texture* _tex = nullptr, bool _enabled = false, SDL_Rect* _anim = nullptr);
	~Healthbar();

	bool Update(float dt) override;

	Hero* assignedCharacter = nullptr;
	Sprite* bar;
	bool leftSide = false;
	float percent = 1;
};

#endif