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
	characterRects[3] = { 1864, 156, 188, 275 }; //CLERIC
	statsRects[0] = { 1298, 1126, 255, 15 }; // 1/5
	statsRects[1] = { 1298, 1147, 255, 15 }; // 2/5
	statsRects[2] = { 1298, 1170, 255, 15 }; // 3/5
	statsRects[3] = { 1298, 1191, 255, 15 }; // 4/5
	statsRects[4] = { 1298, 1210, 255, 15 }; // 5/5
	characterNameStrings[0] = "WIZARD";
	characterNameStrings[1] = "ROGUE";
	characterNameStrings[2] = "WARRIOR";
	characterNameStrings[3] = "CLERIC";
	charactersType[0] = WIZARD;
	charactersType[1] = ROGUE;
	charactersType[2] = WARRIOR;
	charactersType[3] = CLERIC;

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
	players.clear();
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
		player.ready = false;
		player.totalControllersNum = controllersNum;
		player.arrowLockLeftRect = { 1713,68,39,51 };
		player.arrowLockRightRect = { 1784,68,39,51 };
		player.lockedLightRect = { 50,1200,216,298 };

		players.push_back(player);
	}
}
void CharacterSelecScene::SetCharactersInfo(){
	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		charactersInfo[0] = { ROGUE,{ 100,100 }, Team::BLUE };
		charactersInfo[1] = { WARRIOR,{ 100,1000 }, Team::BLUE };
		charactersInfo[2] = { WIZARD,{ 10000,100 }, Team::RED };
		charactersInfo[3] = { WARRIOR,{ 10000,1000 }, Team::RED };
	}
	else if (App->scenes->gameMode == GameMode::ONEvsONE) {
		charactersInfo[0] = { WARRIOR,{ 100,100 }, Team::BLUE };
		charactersInfo[1] = { WARRIOR,{ 10000,100 }, Team::RED };
	}

}
void CharacterSelecScene::ChangeCharacter()
{
	for (int i = 0; i < controllersNum; i++) {
		Player* player = &players[i];

		if (App->input->GetButtonFromController(player->playerNum, false) == Input::RIGHT && !player->ready) {
			if (indexSprites[i] < 3) {
				characterSprites[i]->idle_anim = characterRects[indexSprites[i] + 1];
				characterNameLabel[i]->setString(characterNameStrings[indexSprites[i] + 1]);
				indexSprites[i]++;
				ChangeStats(i, indexSprites[i]);
			}
			else {
				characterSprites[i]->idle_anim = characterRects[0];
				characterNameLabel[i]->setString(characterNameStrings[0]);
				indexSprites[i] = 0;
				ChangeStats(i, indexSprites[i]);
			}
		}
		else if (App->input->GetButtonFromController(player->playerNum, false) == Input::LEFT && !player->ready) {
			if (indexSprites[i] > 0) {
				characterSprites[i]->idle_anim = characterRects[indexSprites[i] - 1];
				characterNameLabel[i]->setString(characterNameStrings[indexSprites[i] - 1]);
				indexSprites[i]--;
				ChangeStats(i, indexSprites[i]);
			}
			else {
				characterSprites[i]->idle_anim = characterRects[3];
				characterNameLabel[i]->setString(characterNameStrings[3]);
				indexSprites[i] = 3;
				ChangeStats(i, indexSprites[i]);
			}
		}
		else if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_A && !player->ready ) {
			player->ready = true;
			
			player->lockedArrows[0] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockLeftRect);
			player->lockedArrows[0]->SetParent(characterFrame[i]);
			player->lockedArrows[0]->SetAnchor(0, 0);
			player->lockedArrows[0]->setPosition(17, 220);

			player->lockedArrows[1] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockRightRect);
			player->lockedArrows[1]->SetParent(characterFrame[i]);
			player->lockedArrows[1]->SetAnchor(0, 0);
			player->lockedArrows[1]->setPosition(284, 220);

			player->lockedLightSprite = App->gui->AddSprite(-500, 0, atlas, player->lockedLightRect);
			player->lockedLightSprite->SetParent(characterFrame[i]);
			player->lockedLightSprite->SetAnchor(0, 0);
			player->lockedLightSprite->setPosition(62, 95);
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

void CharacterSelecScene::ChangeStats(int playerNum, int index) {
	if (playerNum == 0) {
		if (index == 0) {
			statsSprites[0]->idle_anim = statsRects[4];
			statsSprites[1]->idle_anim = statsRects[0];
			statsSprites[2]->idle_anim = statsRects[1];
			statsSprites[3]->idle_anim = statsRects[3];
		}
		else if (index == 1) {
			statsSprites[0]->idle_anim = statsRects[3];
			statsSprites[1]->idle_anim = statsRects[4];
			statsSprites[2]->idle_anim = statsRects[0];
			statsSprites[3]->idle_anim = statsRects[1];
		}
		else if (index == 2) {
			statsSprites[0]->idle_anim = statsRects[2];
			statsSprites[1]->idle_anim = statsRects[3];
			statsSprites[2]->idle_anim = statsRects[4];
			statsSprites[3]->idle_anim = statsRects[2];
		}
		else if (index == 3) {
			statsSprites[0]->idle_anim = statsRects[1];
			statsSprites[1]->idle_anim = statsRects[4];
			statsSprites[2]->idle_anim = statsRects[4];
			statsSprites[3]->idle_anim = statsRects[1];
		}
	}

	else if (playerNum == 1) {
		if (index == 0) {
			statsSprites[4]->idle_anim = statsRects[4];
			statsSprites[5]->idle_anim = statsRects[0];
			statsSprites[6]->idle_anim = statsRects[1];
			statsSprites[7]->idle_anim = statsRects[3];
		}
		else if (index == 1) {
			statsSprites[4]->idle_anim = statsRects[3];
			statsSprites[5]->idle_anim = statsRects[4];
			statsSprites[6]->idle_anim = statsRects[0];
			statsSprites[7]->idle_anim = statsRects[1];
		}
		else if (index == 2) {
			statsSprites[4]->idle_anim = statsRects[2];
			statsSprites[5]->idle_anim = statsRects[3];
			statsSprites[6]->idle_anim = statsRects[4];
			statsSprites[7]->idle_anim = statsRects[2];
		}
		else if (index == 3) {
			statsSprites[4]->idle_anim = statsRects[1];
			statsSprites[5]->idle_anim = statsRects[4];
			statsSprites[6]->idle_anim = statsRects[4];
			statsSprites[7]->idle_anim = statsRects[1];
		}
	}

	else if (playerNum == 2) {
		if (index == 0) {
			statsSprites[8]->idle_anim = statsRects[4];
			statsSprites[9]->idle_anim = statsRects[0];
			statsSprites[10]->idle_anim = statsRects[1];
			statsSprites[11]->idle_anim = statsRects[3];
		}
		else if (index == 1) {
			statsSprites[8]->idle_anim = statsRects[3];
			statsSprites[9]->idle_anim = statsRects[4];
			statsSprites[10]->idle_anim = statsRects[0];
			statsSprites[11]->idle_anim = statsRects[1];
		}
		else if (index == 2) {
			statsSprites[8]->idle_anim = statsRects[2];
			statsSprites[9]->idle_anim = statsRects[3];
			statsSprites[10]->idle_anim = statsRects[4];
			statsSprites[11]->idle_anim = statsRects[2];
		}
		else if (index == 3) {
			statsSprites[8]->idle_anim = statsRects[1];
			statsSprites[9]->idle_anim = statsRects[4];
			statsSprites[10]->idle_anim = statsRects[4];
			statsSprites[11]->idle_anim = statsRects[1];
		}
	}

	else if (playerNum == 3) {
		if (index == 0) {
			statsSprites[12]->idle_anim = statsRects[4];
			statsSprites[13]->idle_anim = statsRects[0];
			statsSprites[14]->idle_anim = statsRects[1];
			statsSprites[15]->idle_anim = statsRects[3];
		}
		else if (index == 1) {
			statsSprites[12]->idle_anim = statsRects[3];
			statsSprites[13]->idle_anim = statsRects[4];
			statsSprites[14]->idle_anim = statsRects[0];
			statsSprites[15]->idle_anim = statsRects[1];
		}
		else if (index == 2) {
			statsSprites[12]->idle_anim = statsRects[2];
			statsSprites[13]->idle_anim = statsRects[3];
			statsSprites[14]->idle_anim = statsRects[4];
			statsSprites[15]->idle_anim = statsRects[2];
		}
		else if (index == 3) {
			statsSprites[12]->idle_anim = statsRects[1];
			statsSprites[13]->idle_anim = statsRects[4];
			statsSprites[14]->idle_anim = statsRects[4];
			statsSprites[15]->idle_anim = statsRects[1];
		}
	}
}

void CharacterSelecScene::LoadSceneUI() {
	atlas = App->textures->Load("UI/atlas.png");
	uiPoint sizeScreen = App->gui->GetGuiSize();

	Sprite* crossedSwordsSprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 1700, 782,187,175 });
	characterFrame[0] = App->gui->AddSprite(sizeScreen.x / 4, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 43, 343, 659 });
	characterSprites[0] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
	characterSprites[0]->idle_anim = characterRects[0];
	characterSprites[0]->SetParent(characterFrame[0]);
	characterSprites[0]->SetAnchor(0, 0);
	characterSprites[0]->setPosition(80, 105);
	characterNameLabel[0] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
	characterNameLabel[0]->setString(characterNameStrings[0]);
	characterNameLabel[0]->SetParent(characterFrame[0]);
	characterNameLabel[0]->setPosition(characterFrame[0]->idle_anim.w / 2, 35);
	Label* stats1Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
	stats1Label->setString("STATS");
	stats1Label->SetParent(characterFrame[0]);
	stats1Label->setPosition(171, 462);
	Sprite* blueTeam1Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 741, 412,53,53 });
	blueTeam1Sprite->SetParent(characterFrame[0]);
	blueTeam1Sprite->setPosition(172, 416);
	statsSprites[0] = App->gui->AddSprite(0, 0, atlas, statsRects[4]);
	statsSprites[0]->SetParent(characterFrame[0]);
	statsSprites[0]->SetAnchor(0, 0);
	statsSprites[0]->setPosition(45, 530);
	statsSprites[1] = App->gui->AddSprite(0, 0, atlas, statsRects[0]);
	statsSprites[1]->SetParent(characterFrame[0]);
	statsSprites[1]->SetAnchor(0, 0);
	statsSprites[1]->setPosition(45, 562);
	statsSprites[2] = App->gui->AddSprite(0, 0, atlas, statsRects[1]);
	statsSprites[2]->SetParent(characterFrame[0]);
	statsSprites[2]->SetAnchor(0, 0);
	statsSprites[2]->setPosition(45, 594);
	statsSprites[3] = App->gui->AddSprite(0, 0, atlas, statsRects[3]);
	statsSprites[3]->SetParent(characterFrame[0]);
	statsSprites[3]->SetAnchor(0, 0);
	statsSprites[3]->setPosition(45, 626);

	characterFrame[1] = App->gui->AddSprite(sizeScreen.x / 4 * 3, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 43, 343, 659 });
	characterSprites[1] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
	characterSprites[1]->SetParent(characterFrame[1]);
	characterSprites[1]->SetAnchor(0, 0);
	characterSprites[1]->setPosition(80, 105);
	characterNameLabel[1] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
	characterNameLabel[1]->setString(characterNameStrings[0]);
	characterNameLabel[1]->SetParent(characterFrame[1]);
	characterNameLabel[1]->setPosition(characterFrame[0]->idle_anim.w / 2, 35);
	Label* stats2Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
	stats2Label->setString("STATS");
	stats2Label->SetParent(characterFrame[1]);
	stats2Label->setPosition(171, 462);
	Sprite* blueTeam2Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 741, 412,53,53 });
	blueTeam2Sprite->SetParent(characterFrame[1]);
	blueTeam2Sprite->setPosition(172, 416);
	statsSprites[4] = App->gui->AddSprite(0, 0, atlas, statsRects[4]);
	statsSprites[4]->SetParent(characterFrame[1]);
	statsSprites[4]->SetAnchor(0, 0);
	statsSprites[4]->setPosition(45, 530);
	statsSprites[5] = App->gui->AddSprite(0, 0, atlas, statsRects[0]);
	statsSprites[5]->SetParent(characterFrame[1]);
	statsSprites[5]->SetAnchor(0, 0);
	statsSprites[5]->setPosition(45, 562);
	statsSprites[6] = App->gui->AddSprite(0, 0, atlas, statsRects[1]);
	statsSprites[6]->SetParent(characterFrame[1]);
	statsSprites[6]->SetAnchor(0, 0);
	statsSprites[6]->setPosition(45, 594);
	statsSprites[7] = App->gui->AddSprite(0, 0, atlas, statsRects[3]);
	statsSprites[7]->SetParent(characterFrame[1]);
	statsSprites[7]->SetAnchor(0, 0);
	statsSprites[7]->setPosition(45, 626);

	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		characterFrame[0]->setPositionX(sizeScreen.x / 8 - 30);
		characterFrame[1]->setPositionX(sizeScreen.x / 8 * 3 - 60);

		characterFrame[2] = App->gui->AddSprite((sizeScreen.x / 8) * 5 + 60, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 43, 343, 659 });
		characterSprites[2] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[2]->SetParent(characterFrame[2]);
		characterSprites[2]->SetAnchor(0, 0);
		characterSprites[2]->setPosition(80, 105);
		characterNameLabel[2] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[2]->setString(characterNameStrings[0]);
		characterNameLabel[2]->SetParent(characterFrame[2]);
		characterNameLabel[2]->setPosition(characterFrame[0]->idle_anim.w / 2, 35);
		Label* stats3Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats3Label->setString("STATS");
		stats3Label->SetParent(characterFrame[2]);
		stats3Label->setPosition(171, 462);
		Sprite* redTeam1Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 676, 412,53,53 });
		redTeam1Sprite->SetParent(characterFrame[2]);
		redTeam1Sprite->setPosition(172, 416);
		statsSprites[8] = App->gui->AddSprite(0, 0, atlas, statsRects[4]);
		statsSprites[8]->SetParent(characterFrame[2]);
		statsSprites[8]->SetAnchor(0, 0);
		statsSprites[8]->setPosition(45, 530);
		statsSprites[9] = App->gui->AddSprite(0, 0, atlas, statsRects[0]);
		statsSprites[9]->SetParent(characterFrame[2]);
		statsSprites[9]->SetAnchor(0, 0);
		statsSprites[9]->setPosition(45, 562);
		statsSprites[10] = App->gui->AddSprite(0, 0, atlas, statsRects[1]);
		statsSprites[10]->SetParent(characterFrame[2]);
		statsSprites[10]->SetAnchor(0, 0);
		statsSprites[10]->setPosition(45, 594);
		statsSprites[11] = App->gui->AddSprite(0, 0, atlas, statsRects[3]);
		statsSprites[11]->SetParent(characterFrame[2]);
		statsSprites[11]->SetAnchor(0, 0);
		statsSprites[11]->setPosition(45, 626);

		characterFrame[3] = App->gui->AddSprite((sizeScreen.x / 8) * 7 + 30, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 43, 343, 659 });
		characterSprites[3] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[3]->SetParent(characterFrame[3]);
		characterSprites[3]->SetAnchor(0, 0);
		characterSprites[3]->setPosition(80, 105);
		characterNameLabel[3] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[3]->setString(characterNameStrings[0]);
		characterNameLabel[3]->SetParent(characterFrame[3]);
		characterNameLabel[3]->setPosition(characterFrame[0]->idle_anim.w / 2, 35);
		Label* stats4Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats4Label->setString("STATS");
		stats4Label->SetParent(characterFrame[3]);
		stats4Label->setPosition(171, 462);
		Sprite* redTeam2Sprite = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, { 676, 412,53,53 });
		redTeam2Sprite->SetParent(characterFrame[3]);
		redTeam2Sprite->setPosition(172, 416);
		statsSprites[12] = App->gui->AddSprite(0, 0, atlas, statsRects[4]);
		statsSprites[12]->SetParent(characterFrame[3]);
		statsSprites[12]->SetAnchor(0, 0);
		statsSprites[12]->setPosition(45, 530);
		statsSprites[13] = App->gui->AddSprite(0, 0, atlas, statsRects[0]);
		statsSprites[13]->SetParent(characterFrame[3]);
		statsSprites[13]->SetAnchor(0, 0);
		statsSprites[13]->setPosition(45, 562);
		statsSprites[14] = App->gui->AddSprite(0, 0, atlas, statsRects[1]);
		statsSprites[14]->SetParent(characterFrame[3]);
		statsSprites[14]->SetAnchor(0, 0);
		statsSprites[14]->setPosition(45, 594);
		statsSprites[15] = App->gui->AddSprite(0, 0, atlas, statsRects[3]);
		statsSprites[15]->SetParent(characterFrame[3]);
		statsSprites[15]->SetAnchor(0, 0);
		statsSprites[15]->setPosition(45, 626);
	}
}