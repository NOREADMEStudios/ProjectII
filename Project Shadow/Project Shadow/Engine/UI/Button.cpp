#include "Button.h"
#include "j1Fonts.h"
#include "InterfaceElement.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Scene.h"

Button::Button(uint _x, uint _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim)
	: Sprite(_x, _y, _tex, _anim, _enabled)
{
	type = BUTTON;

	if (_hovered_anim.w == 0 && _hovered_anim.h == 0)
		hovered_anim = _anim;
	else
		hovered_anim = _hovered_anim;

	if (_pressed_anim.w == 0 && _pressed_anim.h == 0)
		pressed_anim = _anim;
	else
		pressed_anim = _pressed_anim;

	current_anim = &idle_anim;

	OnClick = callback;
	interactuable = true;
}

Button::~Button()
{
}

void Button::OnHover()
{
}

bool Button::PreUpdate()
{
	bool ret = InterfaceElement::PreUpdate();

	if (label == nullptr) //Auto set label
	{
		for (p2List_item<InterfaceElement*>* current_element = elements.start;
			current_element != nullptr;
			current_element = current_element->next)
		{
			if (current_element->data->type == LABEL)
				label = (Label*)current_element->data;
		}
	}

	SDL_Rect Mouse;
	App->input->GetMousePosition(Mouse.x, Mouse.y);
	Mouse.w = CURSOR_WIDTH;
	Mouse.h = CURSOR_WIDTH;

	SDL_Rect result;
	if (SDL_IntersectRect(&result_rect, &Mouse, &result) == SDL_TRUE)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			App->input->BlockMouseEvent(SDL_BUTTON_LEFT);
			current_anim = &pressed_anim;
			//SetFocus();
			if (OnClick != nullptr)
			{
				OnClick(0);
				if (type != SLIDER)
					App->audio->PlayFx(App->scene->button_sound);
			}

		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			App->input->BlockMouseEvent(SDL_BUTTON_RIGHT);
			current_anim = &pressed_anim;

			if (OnClick != nullptr)
				OnClick(0);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_IDLE)
		{
			current_anim = &hovered_anim;

			if (label != nullptr)
				label->setColor({ 228, 204, 42, 255 });
			OnHover();
		}
	}
	else
	{
		current_anim = &idle_anim;
		SDL_Color curr;
		label->getColor(&curr);
		if (label != nullptr && (curr.r != 255 || curr.g != 255 || curr.b != 255 || curr.a != 255))
			label->setColor({ 255, 255, 255, 255 });
	}

	return ret;
}

void Button::setLabel(Label * label)
{
	this->label = label;
}
