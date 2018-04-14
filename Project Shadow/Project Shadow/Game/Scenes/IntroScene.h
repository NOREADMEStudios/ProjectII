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
	Button * exitButton;
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
	class Selected {
	public:
		Button* but;
		
		

	};
	void ChooseFocus();
	void SetControllerFocus();
	void ManageDisplacement();

	int controllersNum;

	std::list<Button*> buttonsForController;
	Selected player1;
};

#endif