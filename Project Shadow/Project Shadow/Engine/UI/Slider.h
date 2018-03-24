#pragma once
#include "Button.h"
#include "Rect.h"
class Slider :
	public Button
{
public:
	Slider(uint _x = 0, uint _y = 0, SDL_Texture* _tex = nullptr, SDL_Rect _anim = { 0, 0, 0, 0 }, bool _enabled = false, Callback callback = nullptr, SDL_Rect _hovered_anim = { 0, 0, 0, 0 }, SDL_Rect _pressed_anim = { 0, 0, 0, 0 }, bool _axis = 1);
	~Slider();

	iRect upper_rect;

	void DragSlider();
	bool PreUpdate() override;

	void Focus();

	InterfaceElement* onfocus = nullptr;
	bool axis;
	bool dragging = false;
	SDL_Rect Mouse;
	bool locked = false;
	iPoint delta_pos_mouse;

};

