#include "CharacterSelecScene.h"
#include "ItemSelecScene.h"
#include "EndScene.h"
#include "IntroScene.h"
#include "MainScene.h"
#include "../../Engine/ModuleMap.h"
#include "../../Engine/App.h"
#include "../../Engine/ModuleSceneManager.h"
#include "../../Engine/ModuleEntityManager.h"
#include "../../Engine/Entity.h"
#include "../../Engine/ModuleGUI.h"
#include "../../Engine/ModuleTextures.h"
#include "../../Engine/ModuleInput.h"
#include "../../Engine/ModuleRender.h"
#include "../../Engine/UI/Window.h"
#include "../../Engine/ModuleWindow.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/UI/Label.h"
#include "../../Engine/ModuleCollision.h"
#include "../../Engine/ModuleFonts.h"

CharacterSelecScene::CharacterSelecScene()
{
}


CharacterSelecScene::~CharacterSelecScene()
{
}

bool CharacterSelecScene::Start()
{
	LoadBackground("UI/BasicMenuScene.png");
	indexSprites[0] = indexSprites[1] = indexSprites[2] = indexSprites[3] = 0;

	characterRects[0] = { 1676, 156, 188, 275 }; // WIZARD For changing the sprite directly
	characterRects[1] = { 1676, 431, 188, 275 }; //ROGUE
	characterRects[2] = { 1864, 431, 188, 275 }; //WARRIOR
	characterNameStrings[0] = "WIZARD";
	characterNameStrings[1] = "ROGUE";
	characterNameStrings[2] = "WARRIOR";
	charactersType[0] = WIZARD;
	charactersType[1] = ROGUE;
	charactersType[2] = WARRIOR;

	LoadSceneUI();
	SetControllerFocus();
	SetCharactersInfo();

	return true;
}

bool CharacterSelecScene::Update(float dt)
{
	if (AllPlayersReady())
	{
		ApplyCharacterSelection();
		App->scenes->ChangeScene(App->scenes->itemSc);
	}

	DrawBackground();
	ChangeCharacter();

	//App->input->CheckControllers();
	return true;
}

bool CharacterSelecScene::CleanUp()
{
	App->gui->CleanUp();
	App->textures->UnLoad(atlas);
	//UnLoadBackground();
	return true;
}


bool CharacterSelecScene::AllPlayersReady() {

	bool ret = true;
	int index = controllersNum;
	for (int i = 0; i < controllersNum; i++) {
		if (players[i].ready == true) {
			index--;
		}
	}
	if (index != 0)
		ret = false;

	return ret;
}

void CharacterSelecScene::SetControllerFocus() {

	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}

	for (int i = 1; i <= controllersNum; i++) {
		Player player;

		player.playerNum = i;
		player.totalControllersNum = controllersNum;
		player.arrowLockLeftRect = { 1718,74,29,38 };
		player.arrowLockRightRect = { 1792,74,29,38 };
		player.lockedLightRect = { 50,1200,290,348 };

		players.push_back(player);
	}
}
void CharacterSelecScene::SetCharactersInfo(){
	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		charactersInfo[0] = { WIZARD,{ 100,100 }, Team::BLUE };
		charactersInfo[1] = { WARRIOR,{ 100,1000 }, Team::BLUE };
		charactersInfo[2] = { WIZARD,{ 10000,100 }, Team::RED };
		charactersInfo[3] = { WARRIOR,{ 10000,1000 }, Team::RED };
	}
	else if (App->scenes->gameMode == GameMode::ONEvsONE) {
		charactersInfo[0] = { WIZARD,{ 100,100 }, Team::BLUE };
		charactersInfo[1] = { WARRIOR,{ 10000,100 }, Team::RED };
	}

}
void CharacterSelecScene::ChangeCharacter()
{
	for (int i = 0; i < controllersNum; i++) {
		Player* player = &players[i];

		if (App->input->GetButtonFromController(player->playerNum, false) == Input::RIGHT && !player->ready) {
			if (indexSprites[i] < 2) {
				characterSprites[i]->idle_anim = characterRects[indexSprites[i] + 1];
				characterNameLabel[i]->setString(characterNameStrings[indexSprites[i] + 1]);
				indexSprites[i]++;
			}
			else {
				characterSprites[i]->idle_anim = characterRects[0];
				characterNameLabel[i]->setString(characterNameStrings[0]);
				indexSprites[i] = 0;
			}
		}
		else if (App->input->GetButtonFromController(player->playerNum, false) == Input::LEFT && !player->ready) {
			if (indexSprites[i] > 0) {
				characterSprites[i]->idle_anim = characterRects[indexSprites[i] - 1];
				characterNameLabel[i]->setString(characterNameStrings[indexSprites[i] - 1]);
				indexSprites[i]--;
			}
			else {
				characterSprites[i]->idle_anim = characterRects[2];
				characterNameLabel[i]->setString(characterNameStrings[2]);
				indexSprites[i] = 2;
			}
		}
		else if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_A && !player->ready ) {
			player->ready = true;
			
			player->lockedArrows[0] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockLeftRect);
			player->lockedArrows[0]->SetParent(characterFrame[i]);
			player->lockedArrows[0]->SetAnchor(0, 0);
			player->lockedArrows[0]->setPosition(39, 213);

			player->lockedArrows[1] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockRightRect);
			player->lockedArrows[1]->SetParent(characterFrame[i]);
			player->lockedArrows[1]->SetAnchor(0, 0);
			player->lockedArrows[1]->setPosition(275, 214);

			player->lockedLightSprite = App->gui->AddSprite(-500, 0, atlas, player->lockedLightRect);
			player->lockedLightSprite->SetParent(characterFrame[i]);
			player->lockedLightSprite->SetAnchor(0, 0);
			player->lockedLightSprite->setPosition(24, 72);
		}
		else if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_B && player->ready) {
			player->ready = false;
			player->lockedArrows[0]->Enable(false);
			player->lockedArrows[1]->Enable(false);
			player->lockedLightSprite->Enable(false);
		}
	}
}

void CharacterSelecScene::ApplyCharacterSelection() {
	for (int i = 0; i < controllersNum; i++) {
		charactersInfo[i].chType = charactersType[indexSprites[i]];
	}
}

void CharacterSelecScene::LoadSceneUI() {
	atlas = App->textures->Load("UI/atlas.png");
	uiPoint sizeScreen = App->gui->GetGuiSize();

	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		Sprite* crossedSwordsSprite = App->gui->AddSprite(sizeScreen.x / 2 , sizeScreen.y / 5 * 3 + 20, atlas, { 1700, 782,187,175 });
		characterFrame[0] = App->gui->AddSprite(sizeScreen.x / 8 - 30, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[0] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[0]->idle_anim = characterRects[0];
		characterSprites[0]->SetParent(characterFrame[0]);
		characterSprites[0]->SetAnchor(0, 0);
		characterSprites[0]->setPosition(80, 89);
		characterNameLabel[0] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[0]->setString(characterNameStrings[0]);
		characterNameLabel[0]->SetParent(characterFrame[0]);
		characterNameLabel[0]->SetAnchor(0, 0);
		characterNameLabel[0]->setPosition(65, 14);
		Label* stats1Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats1Label->setString("STATS");
		stats1Label->SetParent(characterFrame[0]);
		stats1Label->setPosition(171, 462);
		Sprite* blueTeam1Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 741, 412,53,53 });
		blueTeam1Sprite->SetParent(characterFrame[0]);
		blueTeam1Sprite->setPosition(172, 416);

		characterFrame[1] = App->gui->AddSprite(sizeScreen.x / 8 * 3 - 60, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[1] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[1]->SetParent(characterFrame[1]);
		characterSprites[1]->SetAnchor(0, 0);
		characterSprites[1]->setPosition(80, 89);
		characterNameLabel[1] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[1]->setString(characterNameStrings[0]);
		characterNameLabel[1]->SetParent(characterFrame[1]);
		characterNameLabel[1]->SetAnchor(0, 0);
		characterNameLabel[1]->setPosition(65, 14);
		Label* stats2Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats2Label->setString("STATS");
		stats2Label->SetParent(characterFrame[1]);
		stats2Label->setPosition(171, 462);
		Sprite* blueTeam2Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 741, 412,53,53 });
		blueTeam2Sprite->SetParent(characterFrame[1]);
		blueTeam2Sprite->setPosition(172, 416);

		characterFrame[2] = App->gui->AddSprite((sizeScreen.x / 8) * 5 + 60, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[2] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[2]->SetParent(characterFrame[2]);
		characterSprites[2]->SetAnchor(0, 0);
		characterSprites[2]->setPosition(80, 89);
		characterNameLabel[2] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[2]->setString(characterNameStrings[0]);
		characterNameLabel[2]->SetParent(characterFrame[2]);
		characterNameLabel[2]->SetAnchor(0, 0);
		characterNameLabel[2]->setPosition(65, 14);
		Label* stats3Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats3Label->setString("STATS");
		stats3Label->SetParent(characterFrame[2]);
		stats3Label->setPosition(171, 462);
		Sprite* redTeam1Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 676, 412,53,53 });
		redTeam1Sprite->SetParent(characterFrame[2]);
		redTeam1Sprite->setPosition(172, 416);

		characterFrame[3] = App->gui->AddSprite((sizeScreen.x / 8) * 7 + 30, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[3] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[3]->SetParent(characterFrame[3]);
		characterSprites[3]->SetAnchor(0, 0);
		characterSprites[3]->setPosition(80, 89);
		characterNameLabel[3] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[3]->setString(characterNameStrings[0]);
		characterNameLabel[3]->SetParent(characterFrame[3]);
		characterNameLabel[3]->SetAnchor(0, 0);
		characterNameLabel[3]->setPosition(65, 14);
		Label* stats4Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats4Label->setString("STATS");
		stats4Label->SetParent(characterFrame[3]);
		stats4Label->setPosition(171, 462);
		Sprite* redTeam2Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 676, 412,53,53 });
		redTeam2Sprite->SetParent(characterFrame[3]);
		redTeam2Sprite->setPosition(172, 416);
	}

	else if (App->scenes->gameMode == GameMode::ONEvsONE){
		Sprite* crossedSwordsSprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 1700, 782,187,175 });
		characterFrame[0] = App->gui->AddSprite(sizeScreen.x / 4, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[0] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[0]->SetParent(characterFrame[0]);
		characterSprites[0]->SetAnchor(0, 0);
		characterSprites[0]->setPosition(80, 89);
		characterNameLabel[0] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[0]->setString(characterNameStrings[0]);
		characterNameLabel[0]->SetParent(characterFrame[0]);
		characterNameLabel[0]->SetAnchor(0, 0);
		characterNameLabel[0]->setPosition(65, 14);
		Label* stats1Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats1Label->setString("STATS");
		stats1Label->SetParent(characterFrame[0]);
		stats1Label->setPosition(171, 462);
		Sprite* blueTeam1Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 741, 412,53,53 });
		blueTeam1Sprite->SetParent(characterFrame[0]);
		blueTeam1Sprite->setPosition(172, 416);

		characterFrame[1] = App->gui->AddSprite(sizeScreen.x / 4 * 3, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[1] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[1]->SetParent(characterFrame[1]);
		characterSprites[1]->SetAnchor(0, 0);
		characterSprites[1]->setPosition(80, 89);
		characterNameLabel[1] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[1]->setString(characterNameStrings[0]);
		characterNameLabel[1]->SetParent(characterFrame[1]);
		characterNameLabel[1]->SetAnchor(0, 0);
		characterNameLabel[1]->setPosition(65, 14);
		Label* stats2Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats2Label->setString("STATS");
		stats2Label->SetParent(characterFrame[1]);
		stats2Label->setPosition(171, 462);
		Sprite* redTeam1Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 676, 412,53,53 });
		redTeam1Sprite->SetParent(characterFrame[1]);
		redTeam1Sprite->setPosition(172, 416);

	}
}