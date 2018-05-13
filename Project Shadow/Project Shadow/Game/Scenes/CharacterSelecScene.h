

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

	class Player {

	public:
		int			playerNum;
		int			totalControllersNum;
		Sprite*		lockedArrows[2],
			  *		lockedLightSprite;
		SDL_Rect	arrowLockLeftRect,
					arrowLockRightRect,
					lockedLightRect;
		bool		ready = false;

		void LockedArrow(uint lockedNum);
		void RemoveLockedArrow(uint lockedNum);
	};

	int controllersNum;
	CharacterInfo charactersInfo[4];
	CharacterTypes charactersType[4];
	Sprite* characterFrame[4];
	Sprite* characterSprites[4];
	SDL_Rect characterRects[4];
	Label* characterNameLabel[4];
	String characterNameStrings[4];
	int indexSprites[4];

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
	void SetCharactersInfo();
	void ChangeCharacter();
	bool AllPlayersReady();
	void ApplyCharacterSelection();

	std::vector<Player> players;
};

#endif //__CHARACTERSELECSCENE_H__