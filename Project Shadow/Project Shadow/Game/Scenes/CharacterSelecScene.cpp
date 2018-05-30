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
#include "../../Engine/ModuleTransition.h"


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
	teamRects[0] = { 675, 430,54,54 }; //RED
	teamRects[1] = { 738, 430,54,54 }; //BLUE
	teamRects[2] = { 801, 430,54,54 }; //RED LIGHTED
	teamRects[3] = { 864, 430,54,54 }; //BLUE LIGHTED
	characterNameStrings[0] = "WIZARD";
	characterNameStrings[1] = "ROGUE";
	characterNameStrings[2] = "WARRIOR";
	characterNameStrings[3] = "CLERIC";
	charactersType[0] = WIZARD;
	charactersType[1] = ROGUE;
	charactersType[2] = WARRIOR;
	charactersType[3] = CLERIC;
	charactersTeam[0] = RED;
	charactersTeam[1] = BLUE;


	LoadSceneUI();
	SetControllerFocus();
	SetCharactersInfo();
	LoadArrows();

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
	ChangeTeam();

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
		player.arrowLeftRect = { 1713,14,39,51 };
		player.arrowRightRect = { 1784,14,39,51 };
		player.arrowLockLeftRect = { 1713,68,39,51 };
		player.arrowLockRightRect = { 1784,68,39,51 };
		player.lockedLightRect = { 50,1200,216,298 };

		players.push_back(player);
	}
}

void CharacterSelecScene::SetCharactersInfo(){

	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		charactersInfo[0] = { WIZARD,{ 150,0,150 }, Team::BLUE };
		charactersInfo[1] = { ROGUE,{ 100,0,500 }, Team::BLUE };
		charactersInfo[2] = { WIZARD,{ 700,0,150 }, Team::RED };
		charactersInfo[3] = { CLERIC,{ 750,0,500 }, Team::RED };
	}
	else if (App->scenes->gameMode == GameMode::ONEvsONE) {

		charactersInfo[0] = { CLERIC,{ 100 ,0,250 }, Team::BLUE };
		charactersInfo[1] = { WARRIOR,{ 800, 0 ,250 }, Team::RED };

	}

}

void CharacterSelecScene::ChangeCharacter()
{
	for (int i = 0; i < controllersNum; i++) {
		Player* player = &players[i];

		if (App->input->GetButtonFromController(player->playerNum, false) == Input::RIGHT && !player->ready && player->teamSelected) { //CHANGE CHARACTER
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
		else if (App->input->GetButtonFromController(player->playerNum, false) == Input::LEFT  && !player->ready && player->teamSelected) { //CHANGE CHARACTER
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
		else if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_A &&  !player->ready && player->teamSelected) { //SELECT CHARACTER
			
			player->lockedArrows[2] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockLeftRect);
			player->lockedArrows[2]->SetParent(characterFrame[i]);
			player->lockedArrows[2]->SetAnchor(0, 0);
			player->lockedArrows[2]->setPosition(17, 220);

			player->lockedArrows[3] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockRightRect);
			player->lockedArrows[3]->SetParent(characterFrame[i]);
			player->lockedArrows[3]->SetAnchor(0, 0);
			player->lockedArrows[3]->setPosition(284, 220);

			player->lockedLightSprite = App->gui->AddSprite(-500, 0, atlas, player->lockedLightRect);
			player->lockedLightSprite->SetParent(characterFrame[i]);
			player->lockedLightSprite->SetAnchor(0, 0);
			player->lockedLightSprite->setPosition(62, 95);
			player->ready = true;
		}
		else if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_B && player->ready) {
			player->ready = false;
			player->lockedArrows[2]->Enable(false);
			player->lockedArrows[3]->Enable(false);
			player->lockedLightSprite->Enable(false);
		}
	}
}

void CharacterSelecScene::ChangeTeam(){
	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		for (int i = 0; i < controllersNum; i++) {
			Player* player = &players[i];
			if ((App->input->GetButtonFromController(player->playerNum, false) == Input::RIGHT || App->input->GetButtonFromController(player->playerNum, false) == Input::LEFT) && !player->teamSelected) {
				if (teamIndex[i] == 1) {
					characTeamSprite[i]->idle_anim = teamRects[0];
					teamIndex[i]--;
				}
				else if (teamIndex[i] == 0) {
					characTeamSprite[i]->idle_anim = teamRects[1];
					teamIndex[i]++;
				}
			}
			else if (App->input->GetButtonFromController(player->playerNum, false) == Input::BUTTON_A && !player->teamSelected) {
				if (teamIndex[i] == 1 && blueTeamMembers < 2) {
					characTeamSprite[i]->idle_anim = teamRects[3];
					player->lockedArrows[0] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockLeftRect);
					player->lockedArrows[0]->SetParent(characterFrame[i]);
					player->lockedArrows[0]->setPosition(121, 414);
					player->lockedArrows[1] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockRightRect);
					player->lockedArrows[1]->SetParent(characterFrame[i]);
					player->lockedArrows[1]->setPosition(220, 414);
					blueTeamMembers++;
					player->arrows[2]->Enable(true);
					player->arrows[3]->Enable(true);
				}
				else if (teamIndex[i] == 0 && redTeamMembers < 2) {
					characTeamSprite[i]->idle_anim = teamRects[2];
					player->lockedArrows[0] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockLeftRect);
					player->lockedArrows[0]->SetParent(characterFrame[i]);
					player->lockedArrows[0]->setPosition(121, 414);
					player->lockedArrows[1] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockRightRect);
					player->lockedArrows[1]->SetParent(characterFrame[i]);
					player->lockedArrows[1]->setPosition(220, 414);
					redTeamMembers++;
					player->arrows[2]->Enable(true);
					player->arrows[3]->Enable(true);
				}
				player->teamSelected = true;
			}
			else if (!player->teamSelected && blueTeamMembers == 2) {
				characTeamSprite[i]->idle_anim = teamRects[2];
				player->lockedArrows[0] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockLeftRect);
				player->lockedArrows[0]->SetParent(characterFrame[i]);
				player->lockedArrows[0]->setPosition(121, 414);
				player->lockedArrows[1] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockRightRect);
				player->lockedArrows[1]->SetParent(characterFrame[i]);
				player->lockedArrows[1]->setPosition(220, 414);
				redTeamMembers++;
				teamIndex[i] = 0;
				player->teamSelected = true;
				player->arrows[2]->Enable(true);
				player->arrows[3]->Enable(true);
			}
			else if (!player->teamSelected && redTeamMembers == 2) {
				characTeamSprite[i]->idle_anim = teamRects[3];
				player->lockedArrows[0] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockLeftRect);
				player->lockedArrows[0]->SetParent(characterFrame[i]);
				player->lockedArrows[0]->setPosition(121, 414);
				player->lockedArrows[1] = App->gui->AddSprite(-100, 0, atlas, player->arrowLockRightRect);
				player->lockedArrows[1]->SetParent(characterFrame[i]);
				player->lockedArrows[1]->setPosition(220, 414);
				blueTeamMembers++;
				teamIndex[i] = 1;
				player->teamSelected = true;
				player->arrows[2]->Enable(true);
				player->arrows[3]->Enable(true);
			}
		}
	}
	else
	{
		for (int i = 0; i < controllersNum; i++) {
			Player* player = &players[i];
			player->teamSelected = true;
			player->arrows[0]->Enable(false);
			player->arrows[1]->Enable(false);
			player->arrows[2]->Enable(true);
			player->arrows[3]->Enable(true);
		}
	}
}



void CharacterSelecScene::ApplyCharacterSelection() {
	for (int i = 0; i < controllersNum; i++) {
		charactersInfo[i].chType = charactersType[indexSprites[i]];
		if (App->scenes->gameMode == GameMode::TWOvsTWO)
			charactersInfo[i].chTeam = charactersTeam[teamIndex[i]];
	}
}

void CharacterSelecScene::LoadArrows(){
	for (int i = 0; i < controllersNum; i++) {
		Player* player = &players[i];
		player->arrows[0] = App->gui->AddSprite(0, 0, nullptr, player->arrowLeftRect);//LEFT TEAM ARROW
		player->arrows[0]->SetParent(characterFrame[i]);
		player->arrows[0]->setPosition(121, 414);

		player->arrows[1] = App->gui->AddSprite(0, 0, nullptr, player->arrowRightRect);//RIGHT TEAM ARROW
		player->arrows[1]->SetParent(characterFrame[i]);
		player->arrows[1]->setPosition(220, 414);

		player->arrows[2] = App->gui->AddSprite(0, 0, nullptr, player->arrowLeftRect, false);//LEFT FRAME ARROW
		player->arrows[2]->SetParent(characterFrame[i]);
		player->arrows[2]->SetAnchor(0, 0);
		player->arrows[2]->setPosition(17, 220);

		player->arrows[3] = App->gui->AddSprite(0, 0, nullptr, player->arrowRightRect, false);//RIGHT FRAME ARROW
		player->arrows[3]->SetParent(characterFrame[i]);
		player->arrows[3]->SetAnchor(0, 0);
		player->arrows[3]->setPosition(284, 220);
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

	/*Label* backLabel = App->gui->AddLabel(sizeScreen.x*0.15f, sizeScreen.y*0.95f, 50, DEFAULT_FONT);
	backLabel->setString("Press B to go back");*/

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
	characTeamSprite[0] = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, teamRects[0]);
	characTeamSprite[0]->SetParent(characterFrame[0]);
	characTeamSprite[0]->setPosition(172, 416);
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
	characTeamSprite[1] = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, teamRects[0]);
	characTeamSprite[1]->SetParent(characterFrame[1]);
	characTeamSprite[1]->setPosition(172, 416);
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
		characTeamSprite[2] = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, teamRects[0]);
		characTeamSprite[2]->SetParent(characterFrame[2]);
		characTeamSprite[2]->setPosition(172, 416);
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
		characTeamSprite[3] = App->gui->AddSprite(sizeScreen.x / 2, sizeScreen.y / 5 * 3 + 20, atlas, teamRects[0]);
		characTeamSprite[3]->SetParent(characterFrame[3]);
		characTeamSprite[3]->setPosition(172, 416);
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