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

	void LoadArrows();
	void DrawOrderedArrow();
};

class ItemSelecScene:
	public Scene
{
public:

	Button *	swiftBoots = nullptr;
	Button *	cursedSword = nullptr;
	Button *	paladinsHandguards = nullptr;
	Button *	ringProtection = nullptr;
	Button *	dragonSlayer = nullptr;
	Button *	magicRobe = nullptr;
	Button *	confirmButton = nullptr;
	Label *		confirmLabel = nullptr;
	Label *		item1Stats = nullptr;
	Label *		item2Stats = nullptr;
	Label *		item3Stats = nullptr;
	Label *		item4Stats = nullptr;
	Label *		item5Stats = nullptr;
	Label *		item6Stats = nullptr;

	String swiftBootsStr = "SPEED + 10"; //1
	String cursedSwordStr = "ATTACK + 10"; //2
	String paladinsStr = "DEFENSE + 10"; //3
	String ringStr = "LIFE + 10"; //4
	String dragonSlayerStr = "ATK.+ 5 DEF.+ 5"; //5
	String magicRobeStr = "ATK.+ 5 SPD.+ 5"; //6

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
	void ApplyItemAttributes();
	bool AllItemsSelected();
	
	SDL_Texture* items_atlas; //NEEDS TO BE REMOVED ONCE THERE IS ONLY ONE ATLAS

	std::list<Selection> playersSelections;
	std::list<Button*> buttonsForController;
};

#endif