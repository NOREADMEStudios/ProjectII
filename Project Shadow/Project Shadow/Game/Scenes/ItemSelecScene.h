#ifndef __ITEMSELECSCENE_H__
#define __ITEMSELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

#include <list>

class Button;
class Sprite;
class Label;
class InterfaceElement;


class Selection {

public:
	Button* but;
	int playerNum;
	int totalControllersNum;
	Sprite* arrow;	
	SDL_Rect arrowRect;
	SDL_Rect arrowLockRect;
	bool locked = false;

	void LoadArrows(SDL_Texture* tex);
	void DrawOrderedArrow();
	

};

class ItemSelecScene:
	public Scene
{
public:

	Button *	item1 = nullptr;
	Button *	item2 = nullptr;
	Button *	item3 = nullptr;
	Button *	confirmButton = nullptr;
	Label*		confirmLabel = nullptr;
	Label *		item1Stats = nullptr;
	Label *		item2Stats = nullptr;
	Label *		item3Stats = nullptr;

	int controllersNum;

	ItemSelecScene();
	virtual ~ItemSelecScene();

	bool Awake(pugi::xml_node&) override { return true; };

	bool Start() override;

	bool PreUpdate() override { return true; }

	bool Update(float dt) override;

	bool PostUpdate() override { return true; }

	bool CleanUp() override;

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };


private:
	void LoadSceneUI();

	
	void SetControllerFocus();
	void ManageDisplacementFocus();
	void ChooseFocus();
	
	SDL_Texture* atlas;

	std::list<Selection> playersFocus;
	std::list<Button*> buttons;

};

#endif