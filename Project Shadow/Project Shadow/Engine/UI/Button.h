#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Sprite.h"
#include "Label.h"
#include "j1Gui.h"

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
	
	void OnHover();
	Callback OnClick = nullptr;
	bool PreUpdate() override;
	void setLabel(Label* label);

protected:
	Label* label = nullptr;
	SDL_Rect pressed_anim;
	SDL_Rect hovered_anim;
	SDL_Rect disabled_anim;
	p2SString idle_string;
	p2SString pressed_string;
	p2SString hovered_string;
	p2SString disabled_string;

};

//typedef Button<Callback_v> Button_regular;

#endif // !__BUTTON_H__