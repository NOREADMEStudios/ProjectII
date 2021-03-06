#include "Healthbar.h"

Healthbar::Healthbar(Character * character, int charNum, bool leftSide, uint _x, uint _y, SDL_Texture * _tex, bool _enabled, SDL_Rect * _anim) : Sprite(_x, _y, _tex, _enabled, _anim)
{
	assignedCharacter = character;
	this->leftSide = leftSide;
	characterNum = charNum;
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
/*
		uint _lives = assignedCharacter->GetCurrentLives();
		for (size_t i = 0; i < lives.size(); i++) {
			if (i < _lives) continue;
			lives[i]->Enable(false);
		}
*/
		for (int i = 0; i < abilities.size(); i++) {
			if (assignedCharacter->IsAbCooldown(i)) {
				grayAbilities[i]->Enable(true);
			}
			else if (!(assignedCharacter->IsAbCooldown(i)) && (abilities[i]->isEnabled())) {
			
				grayAbilities[i]->Enable(false);
			}		
		}
		ret = true;
	}

	ret = Sprite::Update(dt);

	return ret;
}
