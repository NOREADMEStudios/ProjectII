#ifndef __ITEMSELECSCENE_H__
#define __ITEMSELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

class Button;
class Label;
class InterfaceElement;

struct Focus {

	Button* focused;
	Controller* controller;

};

class ItemSelecScene:
	public Scene
{
public:

	Button * item1;
	Button * item2;
	Button * item3;
	Button * confirmButton;
	Label* confirmLabel;
	Label * item1Stats;
	Label * item2Stats;
	Label * item3Stats;

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

	void LoadSceneUI();

	void SetControllerFocus();

	std::vector<Button*> buttons;

};

#endif