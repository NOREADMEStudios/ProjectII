#ifndef __ITEMSELECSCENE_H__
#define __ITEMSELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

#include <list>

class Button;
class Sprite;
class Label;
class InterfaceElement;
class Sprite;

enum FrameState
{
	PRESS_BUTTON,
	SELECTING,
	READY
};

class CharacterFrame
{
public:

	Sprite * background = nullptr;
	int characterNumber = 0;
	int item1_id = 1;
	int item2_id = 1;
	int item3_id = 1;

	FrameState currentState = PRESS_BUTTON;

	Label* stateDisplay = nullptr;


};


class Focus {

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


	int controllersNum;

	Button * item1 = nullptr;
	Button * item2 = nullptr;
	Button * item3 = nullptr;
	Button * confirmButton = nullptr;
	Label* confirmLabel = nullptr;
	Label * item1Stats = nullptr;
	Label * item2Stats = nullptr;
	Label * item3Stats = nullptr;
	CharacterFrame* player1 = nullptr;
	CharacterFrame* player2 = nullptr;
	CharacterFrame* player3 = nullptr;
	CharacterFrame* player4 = nullptr;

	String swiftBootsStr; 
	String cursedSwordStr; 
	String paladinsStr; 
	String ringStr; 
	String dragonSlayerStr; 
	String magicRobeStr;


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

	
	void ManageItemChanging();

	void SetControllerFocus();
	void ManageDisplacementFocus();
	void ChooseFocus();
	
	SDL_Texture* atlas;

	std::list<Focus> playersFocus;
	std::list<Button*> buttons;

};

#endif