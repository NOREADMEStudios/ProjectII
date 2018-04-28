#ifndef __ITEMSELECSCENE_H__
#define __ITEMSELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

#include <list>


class Button;
class Sprite;
class Label;
class InterfaceElement;


class ItemSelecScene:
	public Scene
{
public:


	


	struct Item {
		Item();
		Item(std::string _name, SDL_Rect _animRect, EntityStats _stats) {
			name = _name;
			animRect = _animRect;
			stats = _stats;
		}
		std::string name;
		Button* butt = nullptr;
		Label* label = nullptr;
		EntityStats stats;
		SDL_Rect animRect;
	};

	class Player {

	public:
		Item* focusedItem = nullptr;
		int playerNum;
		int totalControllersNum;
		Sprite* arrow = nullptr;
		SDL_Rect arrowRect;
		Sprite* lockedArrows[3];		
		SDL_Rect arrowLockRect;
		uint locked = 0;
		Item* playerItems[3];
		bool ready = false;

		void LockedArrow(uint lockedNum);
		void LoadArrows();
		void DrawOrderedArrow();
	};

	Item* items[15];
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

private:
	void LoadSceneUI();

	
	void SetControllerFocus();
	void ManageDisplacementFocus();
	void ChooseFocus();
	void ApplyItemAttributes();
	bool AllPlayersReady();
	
	std::vector<Player> players;
	std::list<Button*> buttonsForController;
};

#endif