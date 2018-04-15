#ifndef __INTROSCENE_H__
#define __INTROSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

class Button;
class Label;


class IntroScene:
	public Scene
{
public:
	Button * pvpButton;
	Button * onevsoneButton;
	Button * exitButton;

	Label * onevsLabel;
	Label * pvpLabel;
	Label * exitLabel;


	IntroScene();
	virtual ~IntroScene();

	bool Awake(pugi::xml_node&) override { return true; };

	bool Start() override;

	bool PreUpdate() override { return true; }

	bool Update(float dt) override;

	bool PostUpdate() override { return true; }

	bool CleanUp() override;

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };

	void LoadUIButtons();


	//-------CONTROLLER MANAGEMENT
	
	void ChooseFocus();
	void SetControllerFocus();
	void ManageDisplacement();

	int controllersNum;

};

#endif