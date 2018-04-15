#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "UI\InterfaceElement.h"
#include "UI\Sprite.h"
#include "UI\Label.h"
#include "UI\Window.h"
#include "UI\Slider.h"
#include "UI\Healthbar.h"
#include "..\Brofiler\Brofiler.h"

ModuleGUI::ModuleGUI() : Module()
{
	name ="gui";
}

// Destructor
ModuleGUI::~ModuleGUI()
{}

// Called before render is available
bool ModuleGUI::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	App->win->GetWindowSize(gui_size.x, gui_size.y);
	scale = App->win->GetScale();

	return ret;
}

// Called before the first frame
bool ModuleGUI::Start()
{
	atlas_texture = App->textures->Load("UI/atlas.png");
	return true;
}

// Update all guis
bool ModuleGUI::PreUpdate()
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::DarkKhaki);
	std::list<InterfaceElement*>::reverse_iterator current_element;
	
	for (current_element = elements.rbegin(); current_element != elements.rend(); current_element++) {
		(*current_element)->enabled = (*current_element)->next_frame_enabled;
		if ((*current_element)->isEnabled())
			(*current_element)->PreUpdate();
	}
	/*if (focused_item != nullptr && (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN))
		focused_item->NextElement(focused_item)->setFocus();*/

	return true;
}

bool ModuleGUI::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::DarkMagenta);
	std::list<InterfaceElement*>::iterator current_element;

	for (current_element = elements.begin(); current_element != elements.end(); current_element++) {
		
		if ((*current_element)->isEnabled())
			(*current_element)->Update(dt);
	}
	return true;
}

// Called after all Updates
bool ModuleGUI::PostUpdate()
{
	std::list<InterfaceElement*>::iterator current_element;

	for (current_element = elements.begin(); current_element != elements.end(); current_element++) {
		
		if ((*current_element)->isEnabled())
			(*current_element)->PostUpdate();
	}
	return true;
}

// Called before quitting
bool ModuleGUI::CleanUp(pugi::xml_node& )
{
	LOG("Freeing GUI");
	std::list<InterfaceElement*>::reverse_iterator current_element;

	for (current_element = elements.rbegin(); current_element != elements.rend(); current_element++) {
		(*current_element)->enabled = (*current_element)->next_frame_enabled;
		(*current_element)->CleanUp();
	}
	elements.clear();

	if(atlas_texture != nullptr)
		App->textures->UnLoad(atlas_texture);
	return true;
}

bool ModuleGUI::CleanUp() {
	std::list<InterfaceElement*>::reverse_iterator current_element;

	for (current_element = elements.rbegin(); current_element != elements.rend(); current_element++) {
		(*current_element)->enabled = (*current_element)->next_frame_enabled;
		(*current_element)->CleanUp();
	}
	elements.clear();
	return true;
}

// const getter for atlas
const SDL_Texture* ModuleGUI::GetAtlas() const
{
	return atlas_texture;
}

uiPoint ModuleGUI::GetGuiSize()
{
	return gui_size;
}

void ModuleGUI::setFocus(InterfaceElement * elem)
{
	if (focused_item != nullptr)
		focused_item->in_focus = false;
	focused_item = elem;
	if (focused_item != nullptr)
		focused_item->in_focus = true;
}

InterfaceElement * ModuleGUI::getFocusedItem()
{
	return focused_item;
}

InterfaceElement * ModuleGUI::AddElement(InterfaceElement * elem)
{
	elements.push_back(elem);
	return elem;
}

void ModuleGUI::RemoveElement(InterfaceElement * elem)
{
	elements.remove(elem);
}

void ModuleGUI::FocusNext()
{
	if (focused_item != nullptr) {
		size_t times = 0;
		InterfaceElement* item = focused_item;
		do {
			item = item->getNextSibling();
			if (item == nullptr) {
				size_t pos = Utils::FindInList(focused_item, elements) + 1 + times;
				if (pos >= elements.size())
					pos = 0;
				LIST_ITERATOR(InterfaceElement*) it = elements.begin();
				for (size_t i = 0; i < pos; it++, i++) {
				}
				item = *it;
				times++;
			}
		} while (!item->isInteractuable());
		setFocus(item);
	}
}

void ModuleGUI::FocusPrev()
{
	if (focused_item != nullptr) {
		size_t times = 0;
		InterfaceElement* item = focused_item;
		do {
			item = item->getPrevSibling();
			if (item == nullptr) {
				int pos = Utils::FindInList(focused_item, elements) - 1 - times;
				if (pos < 0)
					pos = elements.size() - 1;
				LIST_ITERATOR(InterfaceElement*) it = elements.begin();
				for (size_t i = 0; i < pos; it++, i++) {
				}
				item = *it;
				times++;
			}
		} while (!item->isInteractuable());
		setFocus(item);
	}
}

Sprite* ModuleGUI::AddSprite(int x, int y, SDL_Texture* tex, SDL_Rect anim, bool enabled)
{
	if (tex == nullptr)
		tex = atlas_texture;

	Sprite* aux = new Sprite(x, y, tex, enabled, &anim);
	aux->ComputeRects();

	AddElement(aux);
	return aux;
}

Label* ModuleGUI::AddLabel(int x, int y, int psize, const char * font_path, SDL_Color color, Label::RenderMode mode, const char* format, ...)
{
	Label* aux = new Label(x, y, font_path, psize, mode);
	aux->setColor(color);

	if (format != NULL)
	{
		va_list  ap;
		char buffer[TMP_STRING_SIZE];

		va_start(ap, format);
		int res = vsprintf_s(buffer, TMP_STRING_SIZE, format, ap);
		va_end(ap);

		if (res > 0) {
			aux->setString(buffer);
		}
	}

	aux->ComputeRects();

	AddElement(aux);
	return aux;
}

Button* ModuleGUI::AddButton(int _x, int _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim)
{
	if (_tex == nullptr)
		_tex = atlas_texture;

	Button* aux = new Button(_x, _y, _tex, _anim, _enabled, callback, _hovered_anim, _pressed_anim);
	aux->ComputeRects();

	AddElement(aux);
	return aux;
}

Slider* ModuleGUI::AddSlider(int _x, int _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim, bool _axis, InterfaceElement* parent)
{
	if (_tex == nullptr)
		_tex = atlas_texture;

	Slider* aux = new Slider(_x, _y, _tex, _anim, _enabled, callback, _hovered_anim, _pressed_anim, _axis);
	aux->ComputeRects();
	
	if (parent != NULL)
		aux->SetParent(parent);
	else
		AddElement(aux);
	return aux;
}

Healthbar * ModuleGUI::AddHealthbar(Hero * character, int charNum, bool leftSide, uint _x, uint _y, SDL_Texture * _tex, bool _enabled, SDL_Rect _anim)
{
	if (_tex == nullptr)
		_tex = atlas_texture;

	Healthbar* aux = new Healthbar(character, charNum, leftSide, _x, _y, _tex, _enabled, &_anim);
	Sprite* bar = App->gui->AddSprite(0, 0, _tex, { 451, 297, 258, 20 });
	Label* charNumber = App->gui->AddLabel(0, 0, 48, "Assets/Textures/UI/TTF/Vecna Bold.ttf", { 255, 255, 255, 255 }, Label::BLENDED, "");

	aux->characterText = charNumber;
	charNumber->SetParent(aux);
	charNumber->setString("%d", charNum+1);

	aux->bar = bar;
	bar->SetParent(aux);
	bar->setPosition(aux->rect.w / 2, aux->rect.h / 2);


	iRect charFaceRect;
	uiPoint screenDims = GetGuiSize();
	fPoint anchor;
	uint screenMargin = 10;

	switch(charNum)
	{
	case 0:
		anchor.x = 0;
		anchor.y = 0;

		charFaceRect = { 890, 335, 91, 91 };
		break;

	case 1:
		anchor.x = 1;
		anchor.y = 0;

		charFaceRect = { 890, 240, 91, 91 };
		break;

	case 2:
		anchor.x = 0;
		anchor.y = 1;

		charFaceRect = { 890, 145, 91, 91 };
		break;

	case 3:
		anchor.x = 1;
		anchor.y = 1;

		charFaceRect = { 890, 50, 91, 91 };
		break;
	}

	Sprite* charFace = App->gui->AddSprite(0, 0, atlas_texture, charFaceRect.toSDL_Rect());

	aux->SetAnchor(anchor.x, anchor.y);
	charFace->SetAnchor(anchor.x, anchor.y);
	charNumber->SetAnchor(anchor.x, anchor.y);

	aux->setPositionX(screenMargin + ((screenDims.x - (screenMargin * 2)) * anchor.x));
	aux->setPositionY(screenMargin + ((screenDims.y - (screenMargin * 2)) * anchor.y));

	charFace->setPosition(aux->rect.w * anchor.x, aux->rect.h * (1 - anchor.y));
	charFace->culled = false;

	aux->characterMugShot = charFace;
	charFace->SetParent(aux);

	charNumber->setPositionX(charFace->getPositionX() + (charFace->rect.w + screenMargin) * (leftSide ? 1 : -1));
	charNumber->setPositionY(charFace->getPositionY() +  screenMargin * (anchor.y == 0.f ? 1 : -1));

	bar->SetAnchor(0.5f, 0.5f);

	iPoint margins;
	margins.x = (aux->rect.w - bar->rect.w) / 2;
	margins.y = (aux->rect.h - bar->rect.h) / 2;
	aux->SetContentRect(margins.x, margins.y, margins.x, margins.y);

	bar->culled = true;
	bar->ComputeRects();

	AddElement(aux);
	return aux;
}

Window* ModuleGUI::AddWindow(int x, int y, SDL_Texture* tex, SDL_Rect anim, bool enabled)
{
	Window* aux = new Window(x, y, tex, anim, enabled);
	aux->ComputeRects();

	AddElement(aux);
	return aux;
}

Window * ModuleGUI::AddWindow(const Window_Info& info)
{
	Window* aux = new Window(info);
	aux->ComputeRects();

	AddElement(aux);
	return aux;
}
// class Gui ---------------------------------------------------
