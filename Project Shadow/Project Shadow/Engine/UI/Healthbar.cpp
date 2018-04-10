#include "Healthbar.h"

Healthbar::Healthbar(Hero * character, int charNum, bool leftSide, uint _x, uint _y, SDL_Texture * _tex, bool _enabled, SDL_Rect * _anim) : Sprite(_x, _y, _tex, _enabled, _anim)
{
	assignedCharacter = character;
	this->leftSide = leftSide;
	characterNum = charNum;

	/*if (charNum == 0)
	{
		this->setPosition(10, 10);
	}*/
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
			SetContentRect(INT_MAX, INT_MAX, margin, INT_MAX);

		else
			SetContentRect(margin, INT_MAX, INT_MAX, INT_MAX);

		ret = true;
	}



	ret = Sprite::Update(dt);

	return ret;
}
