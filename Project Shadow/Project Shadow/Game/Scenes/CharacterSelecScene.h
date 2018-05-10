

#ifndef __CHARACTERSELECSCENE_H__
#define __CHARACTERSELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"
#include "..\..\Engine\ModuleEntityManager.h"
#include "..\..\Engine\UI\InterfaceElement.h"

#include <list>


class Button;
class Sprite;
class Label;



class CharacterSelecScene :
	public Scene
{
public:

	class CharacterToSelect {
	public:
		CharacterToSelect();
		CharacterToSelect(std::string _name, CharacterTypes _type, EntityStats _stats) {
			name = _name;
			type = _type;
			/*animRect = _animRect;*/
			stats = _stats;
		}
		std::string name;
		CharacterTypes type = NONE;
		Button* butt = nullptr;
		std::vector<Label*> labels;

		EntityStats stats;
		SDL_Rect animRect;
		CharacterToSelect* relations[InterfaceElement::Directions::AMOUNT];

	};

	class Player {

	public:
		CharacterToSelect * focusedCharacter = nullptr;
		int playerNum;
		int totalControllersNum;
		Sprite* lockedArrows[2],* lockedLightSprite;
		SDL_Rect arrowLockLeftRect, arrowLockRightRect, lockedLightRect;
		CharacterInfo lockedInfo;
		bool ready = false;

		void LockedArrow(uint lockedNum);
		void RemoveLockedArrow(uint lockedNum);
	};

	int controllersNum;
	CharacterInfo charactersInfo[4];
	CharacterTypes charactersType[3];
	Sprite* characterFrame[4];
	Sprite* characterSprites[4];
	SDL_Rect characterRects[3];
	Label* characterNameLabel[4];
	String characterNameStrings[3];
	int indexSprites[4];

	CharacterInfo character1Info = { WIZARD,{ 100,100 }, Team::BLUE };
	CharacterInfo character2Info = { WARRIOR,{ 100,1000 }, Team::BLUE };
	CharacterInfo character3Info = { WIZARD, { 10000,100 }, Team::RED };
	CharacterInfo character4Info = { WARRIOR,{ 10000,1000 }, Team::RED };

	CharacterSelecScene();
	virtual ~CharacterSelecScene();

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
	void ChangeCharacter();
	bool AllPlayersReady();
	void ApplyCharacterSelection();

	std::vector<Player> players;
	std::list<Button*> buttonsForController;
};

#endif //__CHARACTERSELECSCENE_H__