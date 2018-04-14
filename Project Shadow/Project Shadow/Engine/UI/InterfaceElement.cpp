#include "InterfaceElement.h"
#include "..\ModuleRender.h"
#include "..\ModuleWindow.h"
#include "..\ModuleGUI.h"


InterfaceElement::InterfaceElement() : next_frame_enabled(true), scale(App->win->GetScale())
{
	enabled = (true),
	rect = { 0,0,0,0 };
	rel_pos = { 0,0 };
	abs_pos = { 0,0 };
	content_rect = { 0,0,0,0 };
	content_rect_margins = { 0,0,0,0 };
	result_rect = { 0,0,0,0 };
	initial_pos = { 0,0 };
}


InterfaceElement::~InterfaceElement() {
}


bool InterfaceElement::Enable(bool enable) {
	next_frame_enabled = enable;
	if (!enabled && in_focus)
		App->gui->setFocus(nullptr);
	return enabled;
}

bool InterfaceElement::isEnabled() {
	return enabled;
}

bool InterfaceElement::Start() {
	return true;
}

bool InterfaceElement::PreUpdate() {
	bool ret = true;

	ComputeRects();

	for (std::list<InterfaceElement*>::reverse_iterator current_element = elements.rbegin();
		current_element != elements.rend() && ret == true;
		current_element++)
	{
		InterfaceElement* ele = (*current_element);
		ele->enabled = ele->next_frame_enabled;

		if (ele->isEnabled())
			ret = ele->PreUpdate();
	}
	return ret;
}

bool InterfaceElement::Update(float dt) {
	bool ret = true;

	for (std::list<InterfaceElement*>::iterator current_element = elements.begin();
		current_element != elements.end() && ret;
		current_element++)
	{
		if ((*current_element)->isEnabled())
			ret = (*current_element)->Update(dt);
	}

	if (App->debug) {
		DebugDraw();
	}

	return ret;
}

bool InterfaceElement::PostUpdate() {
	bool ret = true;

	for (LIST_ITERATOR(InterfaceElement*) current_element = elements.begin();
		current_element != elements.end() && ret; current_element++) {
		if ((*current_element)->isEnabled())
			ret = (*current_element)->PostUpdate();
	}

	return ret;
}

bool InterfaceElement::CleanUp() {
	bool ret = true;
	
	for (LIST_REVERSE_ITERATOR(InterfaceElement*) current_element = elements.rbegin();
		current_element != elements.rend() && ret; current_element++) {
		ret = (*current_element)->CleanUp();
		Utils::Release(*current_element);
	}

	elements.clear();

	return ret;
}

SDL_Rect InterfaceElement::getRect() const {
	return rect;
}

void InterfaceElement::SetContentRect(int x_margin, int y_margin, int x_margin_2, int y_margin_2) {
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

SDL_Rect InterfaceElement::GetContentRect() const {
	return content_rect;
}

int InterfaceElement::getPositionX() const {
	return rel_pos.x;
}

int InterfaceElement::getPositionY() const {
	return rel_pos.y;
}

void InterfaceElement::setPosition(int x, int y) {
	rel_pos.x = x;
	rel_pos.y = y;
}

void InterfaceElement::setPositionX(int x) {
	rel_pos.x = x;
}

void InterfaceElement::setPositionY(int y) {
	rel_pos.y = y;
}

void InterfaceElement::setScale(float scale) {
	this->scale = scale;
}

float InterfaceElement::getScale() const {
	return scale;
}

InterfaceElement::Interfacetype InterfaceElement::getType() const {
	return type;
}

void InterfaceElement::SetAnchor(float x, float y) {
	anchor_point.x = x;
	anchor_point.y = y;
}

void InterfaceElement::GetAnchor(float & x, float & y) const {
	x = anchor_point.x;
	y = anchor_point.y;
}

float InterfaceElement::GetAnchorX() const {
	return anchor_point.x;
}

float InterfaceElement::GetAnchorY() const {
	return anchor_point.y;
}

void InterfaceElement::DebugDraw() {
	SDL_Rect r = content_rect;

	App->render->DrawQuad(result_rect, 255, 0, 0, 255, false, false);
	App->render->DrawQuad(rect, 0, 0, 255, 255, false, false);
	App->render->DrawQuad(r, 0, 255, 0, 255, false, false);
	App->render->DrawLine(r.x, r.y + (int)(r.h * anchor_point.y), r.x + r.w, r.y + (int)(r.h * anchor_point.y), 0, 128, 128, 255, false);
	App->render->DrawLine(r.x + (int)(r.w * anchor_point.x), r.y, r.x + (int)(r.w * anchor_point.x), r.y + r.h, 0, 128, 128, 255, false);
	if (parent != nullptr)
		App->render->DrawLine(parent->abs_pos.x, parent->abs_pos.y, abs_pos.x, abs_pos.y, 255, 255, 0, 255, false);
}

InterfaceElement * InterfaceElement::AddElement(InterfaceElement * elem) {
	elements.push_back(elem);
	return elem;
}

void InterfaceElement::SetParent(InterfaceElement * parent) {
	if (this->parent != nullptr) {	// Erase the element from its previous parent if it already has one
		Utils::RemoveFromList(this, this->parent->elements);
	} else
		App->gui->RemoveElement(this);

	this->parent = parent;
	if (parent != nullptr) {
		parent->AddElement(this);
	}
	else
		App->gui->AddElement(this);
}

InterfaceElement * InterfaceElement::getNextChild(InterfaceElement * child)
{
	if (elements.size() <= 0) return nullptr;
	size_t pos = Utils::FindInList(child, elements) + 1;
	if (pos >= elements.size())
		pos = 0;
	LIST_ITERATOR(InterfaceElement*) it = elements.begin();
	for (size_t i = 0; i <= pos; it++, i++) {}
	return *it;
}

InterfaceElement * InterfaceElement::getPrevChild(InterfaceElement * child)
{
	if (elements.size() <= 0) return nullptr;
	int pos = (int)Utils::FindInList(child, elements) - 1;
	if (pos < 0)
		pos = elements.size() - 1;
	LIST_ITERATOR(InterfaceElement*) it = elements.begin();
	for (size_t i = 0; i <= pos; it++, i++) {}
	return *it;
}

InterfaceElement * InterfaceElement::getNextSibling()
{
	if (parent == nullptr) return nullptr;
	return parent->getNextChild(this);
}

InterfaceElement * InterfaceElement::getPrevSibling()
{
	if (parent == nullptr) return nullptr;
	return parent->getPrevChild(this);
}

bool InterfaceElement::isInteractuable()
{
	return interactuable;
}

void InterfaceElement::ComputeRects(){
	if (parent != nullptr) {
		parent->ComputeRects();
		parent->SetContentRect();
		abs_pos.x = rel_pos.x + parent->rect.x;
		abs_pos.y = rel_pos.y + parent->rect.y;
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
