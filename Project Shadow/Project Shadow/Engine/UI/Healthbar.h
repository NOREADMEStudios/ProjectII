#ifndef __HEALTHBAR_H__
#define __HEALTHBAR_H__

#include "Sprite.h"
#include "../Warrior.h"
#include "../Defs.h"

class Label;

class Healthbar :
	public Sprite
{
public:

	Healthbar(Character* character, int charNum, bool leftSide = true, uint _x = 0, uint _y = 0, SDL_Texture* _tex = nullptr, bool _enabled = false, SDL_Rect* _anim = nullptr);
	~Healthbar();

	bool Update(float dt) override;

	Character* assignedCharacter = nullptr;
	Sprite* bar = nullptr;
	Sprite* characterMugShot = nullptr;
	VECTOR(Sprite*) abilities {};
	VECTOR(Sprite*) grayAbilities {};
	Label* characterText = nullptr;
	int characterNum = 0; //0 = player 1 (up left), 1 = player 2 (up right), 2 = player 3 (down left), 3 = player 4 (down right)
	bool leftSide = false;
	float percent = 1;
};

#endif