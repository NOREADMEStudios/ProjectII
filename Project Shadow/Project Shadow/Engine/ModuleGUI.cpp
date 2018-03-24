#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "Sprite.h"
#include "Button.h"
#include "Window.h"
#include "Slider.h"
#include "Brofiler\Brofiler.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	App->win->GetWindowSize(gui_size.x, gui_size.y);
	scale = App->win->GetScale();

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::DarkKhaki);
	p2List_item<InterfaceElement*>* current_element = elements.end;
	while (current_element != NULL)
	{
		current_element->data->enabled = current_element->data->next_frame_enabled;
		if (current_element->data->isEnabled())
			current_element->data->PreUpdate();
		current_element = current_element->prev;
	}

	/*if (focused_item != nullptr && (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN))
		focused_item->NextElement(focused_item)->setFocus();*/

	return true;
}

bool j1Gui::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::DarkMagenta);
	p2List_item<InterfaceElement*>* current_element = elements.start;
	while (current_element != NULL)
	{
		if (current_element->data->isEnabled())
			current_element->data->Update(dt);
		current_element = current_element->next;
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<InterfaceElement*>* current_element = elements.start;
	while (current_element != NULL)
	{
		if (current_element->data->isEnabled())
			current_element->data->PostUpdate();
		current_element = current_element->next;
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp(pugi::xml_node&)
{
	LOG("Freeing GUI");
	p2List_item<InterfaceElement*>* current_element = elements.end;
	while (current_element != NULL)
	{
		current_element->data->CleanUp();
		delete current_element->data;
		current_element = current_element->prev;
	}
	elements.clear();

	if(atlas_texture != nullptr)
		App->tex->UnLoad(atlas_texture);
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas_texture;
}

uiPoint j1Gui::GetGuiSize()
{
	return gui_size;
}

void j1Gui::setFocus(InterfaceElement * elem)
{
	if (focused_item != nullptr)
		focused_item->in_focus = false;
	focused_item = elem;
	if (focused_item != nullptr)
		focused_item->in_focus = true;
}

InterfaceElement * j1Gui::getFocusedItem()
{
	return focused_item;
}

InterfaceElement * j1Gui::AddElement(InterfaceElement * elem)
{
	if (elements.find(elem) < 0)
		elements.add(elem);

	return elem;
}

bool j1Gui::RemoveElement(InterfaceElement * elem)
{
	int index = elements.find(elem);
	if (index > -1)
		elements.del(elements.At(index));

	return (index > -1);
}

Sprite* j1Gui::AddSprite(int x, int y, SDL_Texture* tex, SDL_Rect anim, bool enabled)
{
	Sprite* aux = new Sprite(x, y, tex, anim, enabled);
	elements.add(aux);
	return aux;
}

Label* j1Gui::AddLabel(int x, int y, int psize, const char * font_path, SDL_Color color, Label::RenderMode mode, const char* format, ...)
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

	elements.add(aux);
	return aux;
}

Button* j1Gui::AddButton(int _x, int _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim)
{
	Button* aux = new Button(_x, _y, _tex, _anim, _enabled, callback, _hovered_anim, _pressed_anim);

	elements.add(aux);
	return aux;
}

Slider* j1Gui::AddSlider(int _x, int _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim, bool _axis, InterfaceElement* parent)
{
	Slider* aux = new Slider(_x, _y, _tex, _anim, _enabled, callback, _hovered_anim, _pressed_anim, _axis);
	
	if (parent != NULL)
		aux->SetParent(parent);
	else
		elements.add(aux);

	return aux;
}

Window* j1Gui::AddWindow(int x, int y, SDL_Texture* tex, SDL_Rect anim, bool enabled)
{
	Window* aux = new Window(x, y, tex, anim, enabled);

	elements.add(aux);
	return aux;
}

Window * j1Gui::AddWindow(const Window_Info& info)
{
	Window* aux = new Window(info);

	elements.add(aux);
	return aux;
}
// class Gui ---------------------------------------------------
