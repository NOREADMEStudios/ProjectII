#include "Button.h"
#include "..\ModuleGUI.h"
#include "..\ModuleInput.h"
#include "..\ModuleRender.h"
#include "..\ModuleAudio.h"

Button::Button(uint _x, uint _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim)
	: Sprite(_x, _y, _tex, _enabled, &_anim) {
	
	type = BUTTON;
	state = IDLE;

	if (_hovered_anim.w == 0 && _hovered_anim.h == 0)
		hovered_anim = _anim;
	else
		hovered_anim = _hovered_anim;

	if (_pressed_anim.w == 0 && _pressed_anim.h == 0)
		pressed_anim = _anim;
	else
		pressed_anim = _pressed_anim;

	currentAnim = &idle_anim;

	OnClick = callback;
	interactuable = true;
}

Button::~Button() {
}

bool Button::PreUpdate() {
	bool ret = InterfaceElement::PreUpdate();
	
	if (label == nullptr) //Auto set label
	{
		for (std::list<InterfaceElement*>::iterator current_element = elements.begin(); current_element != elements.end(); current_element++)
		{
			if ((*current_element)->type == LABEL)
				label = (Label*)(*current_element);
		}
	}

	SDL_Rect Mouse;
	App->input->GetMousePosition(Mouse.x, Mouse.y);
	Mouse.w = CURSOR_WIDTH;
	Mouse.h = CURSOR_WIDTH;

	SDL_Rect result;
	if (SDL_IntersectRect(&result_rect, &Mouse, &result) == SDL_TRUE || in_focus) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			App->input->BlockMouseEvent(SDL_BUTTON_LEFT);
			currentAnim = &pressed_anim;

			//SetFocus();
			if (OnClick != nullptr) {
				OnClick(1, this);
				if (type != SLIDER && clickSound >= 0)
					App->audio->PlayFx(clickSound);
			}
		}
		else {
			currentAnim = &hovered_anim;
			
			if (state != HOVERED)
				if (OnHoverEnter != nullptr)
					OnHoverEnter(1, this);

			state = HOVERED;
		}
	}
	else {
		currentAnim = &idle_anim;

		if (state == HOVERED)
			if (OnHoverExit != nullptr)
				OnHoverExit(1, this);

		state = IDLE;
		/*SDL_Color curr;
		label->getColor(&curr);
		if (label != nullptr && (curr.r != 255 || curr.g != 255 || curr.b != 255 || curr.a != 255))
			label->setColor({ 255, 255, 255, 255 });*/
	}
	if (in_focus) {
		currentAnim = &hovered_anim;
	}
	return ret;
}

void Button::setLabel(Label * label) {
	this->label = label;
}

Label * Button::getLabel()
{
	return label;
}
