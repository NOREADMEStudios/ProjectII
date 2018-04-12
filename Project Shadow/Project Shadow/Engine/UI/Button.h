#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Sprite.h"
#include "Label.h"
#include "..\ModuleGui.h"

struct Mix_Chunk;

class Button :
	public Sprite
{
public:
	Button(uint _x, uint _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim = { 0, 0, 0, 0 }, SDL_Rect _pressed_anim = { 0, 0, 0, 0 });
	~Button();
	
	enum STATE {
		IDLE,
		PRESSED,
		HOVERED,
		DISABLED
	} state;
	
	Callback OnHover;
	Callback OnClick;
	bool PreUpdate() override;
	void setLabel(Label* label);

protected:
	Label* label = nullptr;
	SDL_Rect pressed_anim;
	SDL_Rect hovered_anim;
	SDL_Rect disabled_anim;
	std::string idle_string;
	std::string pressed_string;
	std::string hovered_string;
	std::string disabled_string;
	int clickSound = -1;
};

//typedef Button<Callback_v> Button_regular;

#endif // !__BUTTON_H__