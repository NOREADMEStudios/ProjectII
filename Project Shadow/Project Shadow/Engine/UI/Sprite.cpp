#include "Sprite.h"
#include "j1Render.h"

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

	current_anim = &idle_anim;
}


Sprite::~Sprite()
{
}

bool Sprite::PostUpdate()
{
	current_anim = &idle_anim;

	//if (in_focus) {
		ComputeRects();
		rect.x = (int)(-anchor_point.x * rect.w) + abs_pos.x;
		rect.y = (int)(-anchor_point.y * rect.h) + abs_pos.y;

		/*int dx = 0, dy = 0, dw = 0, dh = 0;
		if (parent != nullptr) {
			//SDL_IntersectRect(&parent->content_rect, &rect, &result_rect);
			dx = result_rect.x - rect.x;
			dy = result_rect.y - rect.y;
			dw = result_rect.w - rect.w;
			dh = result_rect.h - rect.h;
		}

		SDL_Rect actual_anim_rect = *current_anim;
		actual_anim_rect.w += dw;
		actual_anim_rect.h += dh;
		actual_anim_rect.x += dx;
		actual_anim_rect.y += dy;

		App->render->BlitGui(tex, result_rect.x, result_rect.y, &actual_anim_rect);*/
	//}

	bool ret = InterfaceElement::PostUpdate();
	return ret;
}
