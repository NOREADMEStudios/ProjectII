

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
		Sprite*		lockedArrows[4];
		Sprite*		arrows[4];
		Sprite*		lockedLightSprite;
		SDL_Rect	arrowLeftRect,
					arrowRightRect,
					arrowLockLeftRect,
					arrowLockRightRect,
					lockedLightRect;
		bool		teamSelected = false;
		bool		ready = false;

		void LockedArrow(uint lockedNum);
		void RemoveLockedArrow(uint lockedNum);
	};

	CharacterInfo charactersInfo[4];
	CharacterTypes charactersType[4];
	Team charactersTeam[2];

	Sprite* characterFrame[4];
	Sprite* characterSprites[4];
	Sprite* statsSprites[16];
	Sprite* characTeamSprite[4];
	SDL_Rect statsRects[5];
	SDL_Rect characterRects[4];
	SDL_Rect teamRects[4];
	Label* characterNameLabel[4];
	String characterNameStrings[4];
	int indexSprites[4];
	int teamIndex[4];


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
	void ChangeTeam();
	void ChangeStats(int playerNum, int index);
	bool AllPlayersReady();
	void ApplyCharacterSelection();
	void LoadArrows();
	bool loadedArrows = false;
	int redTeamMembers = 0;
	int blueTeamMembers = 0;

	Sprite* characterIndicatorSquare[4];
	std::vector<Player> players;
};

#endif //__CHARACTERSELECSCENE_H__