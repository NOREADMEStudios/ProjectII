#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Sprite.h"

struct Window_Info {
	uint x = 0;
	uint y = 0;
	SDL_Texture* tex = nullptr;
	SDL_Rect anim = { 0, 0, 0, 0 };
	SDL_Rect content_rect_margins = { 0, 0, 0, 0 };
	bool enabled = false;
	Callback OnClose = nullptr;
};

class Window :
	public Sprite
{
public:
	Window(uint _x = 0, uint _y = 0, SDL_Texture* _tex = nullptr, const SDL_Rect _anim = { 0, 0, 0, 0 }, bool _enabled = false);
	Window(const Window_Info& info);
	~Window();

	void DragWindow();
	bool PreUpdate() override;

	void Focus();

	InterfaceElement* onfocus = nullptr;

	SDL_Rect Mouse;
	bool locked = true;
	bool dragging = false;
	iPoint delta_pos_mouse;

	Callback OnClose = nullptr;
};
#endif