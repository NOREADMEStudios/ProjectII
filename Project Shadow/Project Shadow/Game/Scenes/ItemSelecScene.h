#ifndef __ITEMSELECSCENE_H__
#define __ITEMSELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

#include <list>

class Button;
class Label;
class InterfaceElement;


class Focus {

public:
	Button* but;
	int playerNum;
	SDL_Rect arrow;
	SDL_Rect arrow_lock;
	void LoadArrows();
	

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
	void DrawArrows();

	SDL_Texture* atlas;

	std::list<Focus> playersFocus;
	std::list<Button*> buttons;

};

#endif