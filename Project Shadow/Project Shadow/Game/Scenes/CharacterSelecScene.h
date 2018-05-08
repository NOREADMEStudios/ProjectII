

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
		CharacterToSelect(std::string _name, CharacterTypes _type, SDL_Rect _animRect, EntityStats _stats) {
			name = _name;
			type = _type;
			animRect = _animRect;
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
	/*	void SetRelation(Item* item, InterfaceElement::Directions direction, bool assignOther = true);*/
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
		/*Item* playerItems[3];*/
		bool ready = false;


		/*void LockedArrow(uint lockedNum);
		void RemoveLockedArrow(uint lockedNum);
		void LoadArrows();
		void DrawOrderedArrow();*/
	};

	int controllersNum;

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
	void SetControllerFocus();
	void FindNextPlayer(uint player, InterfaceElement::Directions direction);
	void ManageDisplacementFocus();
	void ApplyCharacterSelection();

	/*void LoadSceneUI();
	void FindNextArrowUnlocked(uint player, InterfaceElement::Directions direction);
	void ChooseFocus();
	void RemoveSelectedItem();
	void ApplyItemAttributes();
	bool AllPlayersReady();
	void FindFirstFreeItem(uint playerNum);
	void AddLabelToButton(Item* item);*/

	CharacterInfo character1 = { WIZARD,{ 100,100 }, Team::BLUE };
	CharacterInfo character2 = { WIZARD,{ 10000,100 }, Team::RED };
	CharacterInfo character3 = { WARRIOR,{ 100,1000 }, Team::BLUE };
	CharacterInfo character4 = { WARRIOR,{ 10000,1000 }, Team::RED };

	std::vector<Player> players;
	std::list<Button*> buttonsForController;
};

#endif //__CHARACTERSELECSCENE_H__