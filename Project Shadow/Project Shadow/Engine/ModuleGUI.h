#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "UI\InterfaceElement.h"
#include "UI\Label.h"
#include <list>

class Sprite;
class Label;
class Button;
class Window;
class Slider;
class Healthbar;
class Character;
struct Window_Info;


// ---------------------------------------------------
class ModuleGUI : public Module
{
public:

	ModuleGUI();

	// Destructor
	virtual ~ModuleGUI();

	// Called when before render is available
	bool Awake(pugi::xml_node&) override;

	// Call before first frame
	bool Start() override;

	// Called before all Updates
	bool PreUpdate() override;

	// Called every frame
	bool Update(float dt) override;

	// Called after all Updates
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp(pugi::xml_node&) override;
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	Sprite* AddSprite(int x, int y, SDL_Texture* tex, SDL_Rect anim = { 0, 0, 0, 0 }, bool enabled = true);
	Label* AddLabel(int x, int y, int psize, const char* font_path = nullptr, SDL_Color color = {0,0,0,255}, Label::RenderMode mode = Label::BLENDED, const char* format = nullptr, ...);
	Button* AddButton(int _x, int _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim = { 0, 0, 0, 0 }, SDL_Rect _pressed_anim = { 0, 0, 0, 0 });
	Window* AddWindow(int x, int y, SDL_Texture* tex, SDL_Rect anim, bool enabled = true);
	Window* AddWindow(const Window_Info& info);
	Slider* AddSlider(int _x, int _y, SDL_Texture* _tex, SDL_Rect _anim, bool _enabled, Callback callback, SDL_Rect _hovered_anim = { 0, 0, 0, 0 }, SDL_Rect _pressed_anim = { 0, 0, 0, 0 }, bool _axis = 1, InterfaceElement* parent = nullptr);
	Healthbar* AddHealthbar(Character* character, int charNum, bool leftSide = true, uint _x = 0, uint _y = 0, SDL_Texture* _tex = nullptr, bool _enabled = false, SDL_Rect _anim = { 0, 0, 0, 0 });
	const SDL_Texture* GetAtlas() const;
	std::string atlas_file_name;

	uiPoint GetGuiSize();
	void setFocus(InterfaceElement* elem);
	InterfaceElement* getFocusedItem();
	void FocusNext();
	void FocusPrev();

	InterfaceElement* AddElement(InterfaceElement* elem);
	void RemoveElement(InterfaceElement* elem);

	std::list<InterfaceElement*> elements;

private:
	SDL_Texture* atlas_texture = nullptr;
	SDL_Texture* characterFace = nullptr;
	uiPoint gui_size;
	float scale;


	InterfaceElement* focused_item = nullptr;
};

#endif // __j1GUI_H__