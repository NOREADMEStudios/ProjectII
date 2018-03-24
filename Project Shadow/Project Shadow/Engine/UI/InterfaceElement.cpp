#include "InterfaceElement.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"



InterfaceElement::InterfaceElement() : enabled(true), next_frame_enabled(true), scale(App->win->GetScale())
{
	rect = { 0,0,0,0 };
	rel_pos = { 0,0 };
	abs_pos = { 0,0 };
	content_rect = { 0,0,0,0 };
	content_rect_margins = { 0,0,0,0 };
	result_rect = { 0,0,0,0 };
	initial_pos = { 0,0 };
}


InterfaceElement::~InterfaceElement()
{
}


bool InterfaceElement::Enable(bool enable)
{
	next_frame_enabled = enable;
	if (!enabled && in_focus)
		App->gui->setFocus(nullptr);
	return enabled;
}

bool InterfaceElement::isEnabled()
{
	return enabled;
}

bool InterfaceElement::Start()
{
	return true;
}

bool InterfaceElement::PreUpdate()
{
	bool ret = true;

	ComputeRects();

	for (p2List_item<InterfaceElement*>* current_element = elements.end;
		current_element != nullptr && ret;
		current_element = current_element->prev)
	{
		current_element->data->enabled = current_element->data->next_frame_enabled;

		if (current_element->data->isEnabled())
			ret = current_element->data->PreUpdate();
	}
	return ret;
}

bool InterfaceElement::Update(float dt)
{
	bool ret = true;

	for (p2List_item<InterfaceElement*>* current_element = elements.start;
		current_element != nullptr && ret;
		current_element = current_element->next)
	{
		if (current_element->data->isEnabled())
			ret = current_element->data->Update(dt);
	}

	if (App->debug) {
		SDL_Rect r = content_rect;
		App->render->DrawQuad(result_rect, 255, 0, 0, 255, 0.0f, false, false, false);
		App->render->DrawQuad(rect, 0, 0, 255, 255, 0.0f, false, false, false);
		App->render->DrawQuad(r, 0, 255, 0, 255, 0.0f, false, false, false);
		App->render->DrawLine(r.x, r.y + (int)(r.h * anchor_point.y), r.x + r.w, r.y + (int)(r.h * anchor_point.y), 0, 0, 255, 255, false, false);
		App->render->DrawLine(r.x + (int)(r.w * anchor_point.x), r.y, r.x + (int)(r.w * anchor_point.x), r.y + r.h, 0, 0, 255, 255, false, false);
		if (parent != nullptr)
			App->render->DrawLine(parent->abs_pos.x, parent->abs_pos.y, abs_pos.x, abs_pos.y, 255, 255, 0, 255, false, false);
	}

	return ret;
}

bool InterfaceElement::PostUpdate()
{
	bool ret = true;

	for (p2List_item<InterfaceElement*>* current_element = elements.start;
		current_element != nullptr && ret;
		current_element = current_element->next)
	{
		if (current_element->data->isEnabled())
			ret = current_element->data->PostUpdate();
	}

	return ret;
}

bool InterfaceElement::CleanUp()
{
	bool ret = true;
	for (p2List_item<InterfaceElement*>* current_element = elements.end;
		current_element != nullptr && ret;
		current_element = current_element->prev)
	{
		ret = current_element->data->CleanUp();
		delete current_element->data;
	}

	elements.clear();

	return ret;
}

SDL_Rect InterfaceElement::getRect() const
{
	return rect;
}

void InterfaceElement::SetContentRect(int x_margin, int y_margin, int x_margin_2, int y_margin_2)
{
	if (x_margin == INT_MAX) x_margin = content_rect_margins.x;
	if (y_margin == INT_MAX) y_margin = content_rect_margins.y;
	if (x_margin_2 == INT_MAX) x_margin_2 = content_rect_margins.w;
	if (y_margin_2 == INT_MAX) y_margin_2 = content_rect_margins.h;
	content_rect_margins = { x_margin, y_margin, x_margin_2, y_margin_2 };
	content_rect.x = rect.x + x_margin;
	content_rect.y = rect.y + y_margin;
	content_rect.w = rect.w - x_margin - x_margin_2;
	content_rect.h = rect.h - y_margin - y_margin_2;
}

SDL_Rect InterfaceElement::GetContentRect() const
{
	return content_rect;
}

int InterfaceElement::getPositionX() const
{
	return rel_pos.x;
}

int InterfaceElement::getPositionY() const
{
	return rel_pos.y;
}

void InterfaceElement::setPosition(int x, int y)
{
	rel_pos.x = x;
	rel_pos.y = y;
}

void InterfaceElement::setPositionX(int x)
{
	rel_pos.x = x;
}

void InterfaceElement::setPositionY(int y)
{
	rel_pos.y = y;
}

void InterfaceElement::setScale(float scale)
{
	this->scale = scale;
}

float InterfaceElement::getScale() const
{
	return scale;
}

InterfaceElement::Interfacetype InterfaceElement::getType() const
{
	return type;
}

void InterfaceElement::SetAnchor(float x, float y)
{
	anchor_point.x = x;
	anchor_point.y = y;
}

void InterfaceElement::GetAnchor(float & x, float & y) const
{
	x = anchor_point.x;
	y = anchor_point.y;
}

float InterfaceElement::GetAnchorX() const
{
	return anchor_point.x;
}

float InterfaceElement::GetAnchorY() const
{
	return anchor_point.y;
}

InterfaceElement * InterfaceElement::AddElement(InterfaceElement * elem)
{
	return elements.add(elem)->data;
}

void InterfaceElement::SetParent(InterfaceElement * parent)
{
	if (this->parent != nullptr)	// Erase the element from its previous parent if it already has one
	{
		int index = this->parent->elements.find(this);
		if (index > -1)
			this->parent->elements.del(this->parent->elements.At(index));
	}

	this->parent = parent;
	if (parent != nullptr) {
		parent->AddElement(this);
		App->gui->RemoveElement(this);
	}
	else
		App->gui->AddElement(this);
}

/*void InterfaceElement::SetFocus()
{
	p2List_item<InterfaceElement*>* curr = nullptr;

	if (parent == NULL)
		curr = App->gui->elements.start;
	else
		curr = parent->elements.start;

	while (curr != NULL)
	{
		curr->data->in_focus = false;
		curr = curr->next;
	}
	this->in_focus = true;
}*/

/*InterfaceElement * InterfaceElement::NextElement(InterfaceElement* curr_child)
{
	if (elements.start == nullptr) {
		return parent->NextElement(this);
	}
	
	int index = elements.find(curr_child);
	if (index < 0) {
		LOG("Could not find desired object %d in this obejct", curr_child);
		return nullptr;
	}

	if (elements.At(index)->next == nullptr) {
		return elements.At(0)->data;
	}
	else return elements.At(index)->data;
}*/

void InterfaceElement::ComputeRects()
{
	if (parent != nullptr) {
		parent->ComputeRects();
		parent->SetContentRect();
		abs_pos.x = rel_pos.x + parent->content_rect.x;
		abs_pos.y = rel_pos.y + parent->content_rect.y;
		SetContentRect();
		if (culled)
			SDL_IntersectRect(&parent->result_rect, &rect, &result_rect);
		else result_rect = rect;
	}
	else {
		abs_pos = rel_pos;
		SetContentRect();
		result_rect = content_rect;
	}

	rect.x = (int)(-anchor_point.x * rect.w) + abs_pos.x;
	rect.y = (int)(-anchor_point.y * rect.h) + abs_pos.y;
}
