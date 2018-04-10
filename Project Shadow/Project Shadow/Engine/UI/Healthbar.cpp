#include "Healthbar.h"

Healthbar::Healthbar(Hero * character, bool leftSide, uint _x, uint _y, SDL_Texture * _tex, bool _enabled, SDL_Rect * _anim) : Sprite(_x, _y, _tex, _enabled, _anim)
{
	assignedCharacter = character;
	this->leftSide = leftSide;


}

Healthbar::~Healthbar()
{
}

bool Healthbar::Update(float dt)
{
	bool ret = false;

	if (assignedCharacter != nullptr) {

		int curr, max;
		assignedCharacter->GetHP(curr, max);

		percent = (float)curr / max;

		int margin = bar->rect.w - (bar->rect.w * percent);

		if (leftSide)
			SetContentRect(0, 0, margin, 0);

		else
			SetContentRect(margin, 0, 0, 0);

		ret = true;
	}

	ret = Sprite::Update(dt);

	return ret;
}
