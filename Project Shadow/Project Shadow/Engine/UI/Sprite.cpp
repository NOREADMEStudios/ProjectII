#include "Sprite.h"
#include "..\ModuleRender.h"

Sprite::Sprite(uint _x, uint _y, SDL_Texture* _tex, bool _enabled, SDL_Rect* _anim) : InterfaceElement()
{
	type = SPRITE;
	tex = _tex;
	rel_pos.x = _x;
	rel_pos.y = _y;
	initial_pos = rel_pos;
	enabled = _enabled;

	if (_anim != NULL)
	{
		idle_anim = *_anim;
	}
	else
	{
		idle_anim.x = 0;
		idle_anim.y = 0;
		SDL_QueryTexture(tex, NULL, NULL, &idle_anim.w, &idle_anim.h);
	}

	rect.w = idle_anim.w;
	rect.h = idle_anim.h;

	content_rect = rect;

	currentAnim = &idle_anim;
}


Sprite::~Sprite()
{
}

bool Sprite::Update(float dt)
{
	bool ret = InterfaceElement::Update(dt);
	return ret;
}

bool Sprite::PostUpdate()
{
	if (currentAnim == nullptr)
		return false;

	if (culled && parent != nullptr) {
		SDL_Rect actual_anim_rect = { 0, 0, 0, 0 };
		actual_anim_rect = *currentAnim;
		actual_anim_rect.x += result_rect.x - rect.x;
		actual_anim_rect.y += result_rect.y - rect.y;
		actual_anim_rect.w += result_rect.w - rect.w;
		actual_anim_rect.h += result_rect.h - rect.h;

		App->render->BlitGui(tex, result_rect.x, result_rect.y, &actual_anim_rect, false, 1.0f, 0.0, false, { 255,255,255,255 }, INT_MAX, INT_MAX, scale);
	}
	else App->render->BlitGui(tex, rect.x, rect.y, currentAnim, false, 1.0f, 0.0, false, { 255,255,255,255 }, INT_MAX, INT_MAX, scale);

	return InterfaceElement::PostUpdate();
}

void Sprite::ChangeAnimation(SDL_Rect anim){
	secondary_anim = anim;
	currentAnim = &secondary_anim;
}