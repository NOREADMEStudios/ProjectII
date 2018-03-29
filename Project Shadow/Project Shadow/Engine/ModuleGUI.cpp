#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "UI\Sprite.h"
#include "UI\Label.h"
#include "UI\Window.h"
#include "UI\Slider.h"
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

Sprite* ModuleGUI::AddSprite(int x, int y, SDL_Texture* tex, SDL_Rect anim, bool enabled)
{
	Sprite* aux = new Sprite(x, y, tex, enabled, &anim);
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

	AddElement(aux);
	return aux;
}

Button* ModuleGUI::AddButton(int _x, int _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim)
{
	Button* aux = new Button(_x, _y, _tex, _anim, _enabled, callback, _hovered_anim, _pressed_anim);

	AddElement(aux);
	return aux;
}

Slider* ModuleGUI::AddSlider(int _x, int _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim, SDL_Rect _pressed_anim, bool _axis, InterfaceElement* parent)
{
	Slider* aux = new Slider(_x, _y, _tex, _anim, _enabled, callback, _hovered_anim, _pressed_anim, _axis);
	
	if (parent != NULL)
		aux->SetParent(parent);
	else
		AddElement(aux);
	return aux;
}

Window* ModuleGUI::AddWindow(int x, int y, SDL_Texture* tex, SDL_Rect anim, bool enabled)
{
	Window* aux = new Window(x, y, tex, anim, enabled);

	AddElement(aux);
	return aux;
}

Window * ModuleGUI::AddWindow(const Window_Info& info)
{
	Window* aux = new Window(info);

	AddElement(aux);
	return aux;
}
// class Gui ---------------------------------------------------