#ifndef __INTROSCENE_H__
#define __INTROSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

class Button;
class Label; 
class Slider;
class Sprite;
class InputButton;

struct InputFrame {

	InputFrame(Button* b, InputButton* ib) { but = b; inBut = ib; }
	Button* but;
	InputButton* inBut;

};

class IntroScene:
	public Scene
{
public:
	Button * pvpButton;
	Button * onevsoneButton;
	Button * settingButton;
	Button * exitButton;

	Slider* music_sl;
	Sprite* music_sp;
	Slider* fx_sl;
	Sprite* fx_sp;
	Button* fullscrenBut ;
	Button* settBack;
	Button* inputBut;
	
	Button* backInputBut;
	Button* defaultInput;
	Label* hAtk, *lAtk, *ab1, *ab2, *ab3, *jump,* run, *prot;
	InputFrame* b1, *b2, *b3, *b4, *b5, *b6, *b7, *b8;
	bool input = false;

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
	void ManageSettings(bool settingActive);
	void ManageInput(bool inputActive);
	void ChangeInput();
	void UpdateInputLabels()const ;
	void DefaultInputList();

	//-------CONTROLLER MANAGEMENT
private:
	void ChooseFocus();
	void SetControllerFocus();
	void ManageDisplacement();
	void CreateSettingsWindow();
	void CreateInputWindow();
	

	std::vector<InputFrame*> inFrames;
};

#endif