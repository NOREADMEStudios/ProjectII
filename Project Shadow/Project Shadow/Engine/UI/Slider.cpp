#include "Slider.h"
#include "Button.h"
#include "j1Input.h"

Slider::Slider(uint _x, uint _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim, bool _axis)
	: Button(_x, _y, _tex, _anim, _enabled, callback, _hovered_anim, _pressed_anim)
{
	axis = _axis;
	type = SLIDER;
}


Slider::~Slider()
{
}

bool Slider::PreUpdate()
{
	bool ret = InterfaceElement::PreUpdate();
	//ComputeRects();

	App->input->GetMousePosition(Mouse.x, Mouse.y);
	Mouse.w = CURSOR_WIDTH;
	Mouse.h = CURSOR_WIDTH;

	//Focus();

	SDL_Rect result, r;
	r = (parent == nullptr) ? rect : result_rect;
	if (SDL_IntersectRect(&r, &Mouse, &result) == SDL_TRUE)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
			current_anim = &pressed_anim;
			App->gui->setFocus(this);
			dragging = true;
			iPoint m = { Mouse.x, Mouse.y };
			delta_pos_mouse = abs_pos - m;
			App->input->BlockMouseEvent(SDL_BUTTON_LEFT);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_IDLE) {
			current_anim = &hovered_anim;

			if (label != nullptr)
				label->setColor({ 228, 204, 42, 255 });
			OnHover();
		}
	}
	else {
		current_anim = &idle_anim;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && in_focus && dragging)
	{
		DragSlider();
		if (OnClick != nullptr) {
			float percent = (axis) ? (float)(rel_pos.x - initial_pos.x + parent->content_rect.w * parent->GetAnchorX()) / (float)parent->content_rect.w :
				(float)(rel_pos.y - initial_pos.y + parent->content_rect.h * parent->GetAnchorY()) / (float)parent->content_rect.h;
			OnClick(1, percent);
		}
		App->input->BlockMouseEvent(SDL_BUTTON_LEFT);
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && dragging)
		dragging = false;

	return ret;
}

void Slider::DragSlider()
{
	if (axis)
	{
		rel_pos.x = Mouse.x + delta_pos_mouse.x - (abs_pos.x - rel_pos.x);
		rel_pos.x = CLAMP(rel_pos.x, 0, parent->content_rect.w);
	}

	else if (!axis)
	{
		rel_pos.y = Mouse.y + delta_pos_mouse.y - (abs_pos.y - rel_pos.y);
		rel_pos.y = CLAMP(rel_pos.y, 0, parent->content_rect.h);
	}
}


void Slider::Focus()
{
	p2List_item<InterfaceElement*>* curr = elements.start;

	while (curr != NULL)
	{
		if (curr->data->in_focus)
		{
			onfocus = curr->data;
			if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
			{
				onfocus->in_focus = false;
				if (curr->next == NULL)
					onfocus = elements.start->data;

				else
					onfocus = curr->next->data;

				onfocus->in_focus = true;
			}
			else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
			{
				onfocus->in_focus = false;
				if (curr->prev == NULL)
					onfocus = elements.end->data;

				else
					onfocus = curr->prev->data;

				onfocus->in_focus = true;

				break;
			}

			break;
		}

		curr = curr->next;
	}

}