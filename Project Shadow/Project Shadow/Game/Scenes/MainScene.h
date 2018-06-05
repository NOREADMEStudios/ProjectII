#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"
#include "..\..\PugiXml\src\pugixml.hpp"
#include "..\..\Engine\UI\InterfaceElement.h"

class Window;
class Slider;
class Button;
class Sprite;
class Label;

class MainScene :
	public Scene
{
public:

	class Player {

	public:
		int			playerNum;
		int			totalControllersNum;
		bool		ready = false;

	};
	
	Entity* e = nullptr,* e2 = nullptr, * e3 = nullptr, *e4 = nullptr;
	int		totalRounds,
			currentRound,
			wonRounds[2];
	Label	*roundsLabel = nullptr,
			*winnerLabel = nullptr;
	Window* pauseWindow = nullptr;
	Button* mainMenuButt = nullptr;
	Button* changeCharactersButt = nullptr;
	Button* settingsButt = nullptr;
	Button* exitButt = nullptr;

	Slider* music_sl = nullptr;
	Sprite* music_sp = nullptr;
	Slider* fx_sl = nullptr;
	Sprite* fx_sp = nullptr;
	Button* fullscrenButt = nullptr;
	Button* settBackButt = nullptr;
			
	bool	combatEndControlBool = false;
	std::vector<Player> players;

	MainScene();
	virtual ~MainScene();

	bool Awake(pugi::xml_node&) override { return true; };

	bool Start() override;

	bool PreUpdate() override { return true; }

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };

	void LoadSceneUI();
	void ManageSettings(bool settingActive);

private:
	void ChooseFocus();
	void SetControllerFocus();
	void ManageDisplacement();
	void CreateSettingsWindow();
	void WindowStates();

	void LoadStartCinematic();
};

#endif