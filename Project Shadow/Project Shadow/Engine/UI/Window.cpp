
#include "Window.h"
#include "Button.h"
#include "Label.h"
#include "j1Input.h"
#include "j1Window.h"


Window::Window(uint _x, uint _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled) : Sprite(_x,_y, _tex, _anim, _enabled)
{
	type = Interfacetype::WINDOW;
	culled = false;
	interactuable = true;
}

Window::Window(const Window_Info & info) : Window(info.x, info.y, info.tex, info.anim, info.enabled)
{
	OnClose = info.OnClose;
	SetContentRect(info.content_rect_margins.x, info.content_rect_margins.y, info.content_rect_margins.w, info.content_rect_margins.h);
}


Window::~Window()
{
	if (in_focus)
		App->gui->setFocus(parent);
}

bool Window::PreUpdate()
{
	//ComputeRects();
	
	bool ret = InterfaceElement::PreUpdate();

	//Focus();

	App->input->GetMousePosition(Mouse.x, Mouse.y);
	Mouse.w = CURSOR_WIDTH;
	Mouse.h = CURSOR_WIDTH;

	SDL_Rect result, r;
	r = (parent == nullptr) ? rect : result_rect;
	if (SDL_IntersectRect(&r, &Mouse, &result) == SDL_TRUE)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			App->gui->setFocus(this);
			dragging = true;
			iPoint m = { Mouse.x, Mouse.y };
			delta_pos_mouse = abs_pos - m;
			App->input->BlockMouseEvent(SDL_BUTTON_LEFT);
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && in_focus && dragging && !locked)
	{
		App->input->BlockMouseEvent(SDL_BUTTON_LEFT);
		DragWindow();
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && dragging)
		dragging = false;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		Enable(false);
		if (OnClose != nullptr)
			OnClose(0);
	}

	App->input->BlockMouse();
	App->input->BlockKeyboard();

	return ret;
}

void Window::DragWindow()
{
	rel_pos.x = Mouse.x + delta_pos_mouse.x - (abs_pos.x - rel_pos.x);
	rel_pos.y = Mouse.y + delta_pos_mouse.y - (abs_pos.y - rel_pos.y);
}

void Window::Focus()
{
	p2List_item<InterfaceElement*>* curr = elements.start;

	while(curr != NULL)
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

