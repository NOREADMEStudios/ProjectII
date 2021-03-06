#ifndef __ITEMSELECSCENE_H__
#define __ITEMSELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"
#include "..\..\Engine\UI\InterfaceElement.h"

#include <list>


class Button;
class Sprite;
class Label;

enum ItemType { 
	NO_ITEM,
	PLATE,
	DRAGONSLAYER,
	TIARA,
	SWIFT_BOOTS,
	RING,
	STAFF
};


class ItemSelecScene:
	public Scene
{
public:
	
	class Item {
	public:
		Item();
		Item(std::string _name, ItemType _type, SDL_Rect _animRect, SDL_Rect _lockedRect, EntityStats _stats) {
			name = _name;
			type = _type;
			animRect = _animRect;
			lockedRect = _lockedRect;
			stats = _stats;
		}
		std::string name;
		ItemType type= NO_ITEM;
		Button* butt = nullptr;
		std::vector<Label*> labels;
		
		EntityStats stats;
		SDL_Rect animRect;
		SDL_Rect lockedRect;
		Item* relations[InterfaceElement::Directions::AMOUNT];

		Item* GetRelativeItem(InterfaceElement::Directions dir);
		void SetRelation(Item* item, InterfaceElement::Directions direction, bool assignOther = true);
	};

	class Player {

	public:
		Item* focusedItem = nullptr;
		int playerNum;
		int totalControllersNum;
		Sprite* arrow = nullptr;
		SDL_Rect arrowRect;
		Sprite* lockedArrows[2];		
		SDL_Rect arrowLockRect;
		uint locked = 0;
		Item* playerItems[2];
		Sprite* miniatureItemsFrame;
		Sprite* miniatureItems[2];
		Label* playerName;
		bool ready = false;

		
		void LockedArrow(uint lockedNum);
		void RemoveLockedArrow(uint lockedNum);
		void LoadArrows();
		void DrawOrderedArrow();
	};

	Item* items[6];
	std::vector<Player> players;
	/*String* statsItemsStr[6];
	Label* stats*/

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
	void FindNextArrowUnlocked(uint player, InterfaceElement::Directions direction);
	
	void SetControllerFocus();
	void ManageDisplacementFocus();
	void ChooseFocus();
	void RemoveSelectedItem();
	bool AllPlayersReady();
	void FindFirstFreeItem(uint playerNum);
	void AddLabelToButton(Item* item);


	std::list<Button*> buttonsForController;

};

#endif