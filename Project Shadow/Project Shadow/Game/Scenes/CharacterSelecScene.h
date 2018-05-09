

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

		CharacterToSelect* GetRelativeCharacter(InterfaceElement::Directions dir);
		void SetRelation(CharacterToSelect* character, InterfaceElement::Directions direction, bool assignOther = true);
	};

	class Player {

	public:
		CharacterToSelect * focusedCharacter = nullptr;
		int playerNum;
		int totalControllersNum;
		Sprite* arrow = nullptr;
		SDL_Rect arrowRect;
		Sprite* lockedArrows[3];
		SDL_Rect arrowLockRect;
		uint locked = 0;
		CharacterInfo lockedInfo;
		/*Item* playerItems[3];*/
		bool ready = false;

		/*void LockedArrow(uint lockedNum);
		void RemoveLockedArrow(uint lockedNum);
		void LoadArrows();
		void DrawOrderedArrow();*/
	};
	CharacterToSelect* characters[4];
	int controllersNum;
	CharacterInfo charactersInfo[4];
	Sprite* characterFrame[4];
	Label* characterNameLabel[4];
	String characterNameStrings[3];
	int indexSprites[4];

	CharacterInfo character1Info = { WIZARD,{ 100,100 }, Team::BLUE };
	CharacterInfo character2Info = { WIZARD,{ 10000,100 }, Team::RED };
	CharacterInfo character3Info = { WARRIOR,{ 100,1000 }, Team::BLUE };
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
	void FindNextPlayer(uint player, InterfaceElement::Directions direction);
	void ManageDisplacementFocus();
	void AddLabelToButton(CharacterToSelect* character);
	bool AllPlayersReady();
	void ApplyCharacterSelection();

	SDL_Rect characterRects[3];
	Sprite* characterSprites[3];

	/*void LoadSceneUI();
	void FindNextArrowUnlocked(uint player, InterfaceElement::Directions direction);
	void ChooseFocus();
	void RemoveSelectedItem();
	void ApplyItemAttributes();
	bool AllPlayersReady();
	void FindFirstFreeItem(uint playerNum);
	void AddLabelToButton(Item* item);*/

	std::vector<Player> players;
	std::list<Button*> buttonsForController;
};

#endif //__CHARACTERSELECSCENE_H__