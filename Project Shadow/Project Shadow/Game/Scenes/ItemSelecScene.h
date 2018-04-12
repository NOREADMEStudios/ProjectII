#ifndef __ITEMSELECSCENE_H__
#define __ITEMSELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

class Button;
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

	FrameState currentState = PRESS_BUTTON;

	Label* stateDisplay = nullptr;
};

class ItemSelecScene:
	public Scene
{
public:

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


};

#endif