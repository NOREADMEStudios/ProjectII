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

	LoadSceneUI();
	SetControllerFocus();

	return true;
}

bool CharacterSelecScene::Update(float dt)
{
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
	for (int i = 0; i < controllersNum; i++) {
		if (players[i].locked == 1) {
			players[i].ready = true;
		}
	}
	for (int i = 0; i < controllersNum; i++) {
		bool ret2 = players[i].ready;
		ret &= ret2;
	}

	return ret;//
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
		players.push_back(player);
	}
}
void CharacterSelecScene::ChangeCharacter()
{
	for (int i = 0; i < controllersNum; i++) {
		Player* player = &players[i];

		if (App->input->GetButtonFromController(player->playerNum) == Input::RIGHT) {
			if (indexSprites[i] < 2) {
				characterSprites[i]->rect = characterRects[i + 1];
				indexSprites[i]++;
			}
			else {
				characterSprites[i]->rect = characterRects[0];
				indexSprites[i] = 0;
			}
		}
		else if (App->input->GetButtonFromController(player->playerNum) == Input::LEFT) {
			if (indexSprites[i] > 0) {
				characterSprites[i]->rect = characterRects[i - 1];
				indexSprites[i]--;
			}
			else {
				characterSprites[i]->rect = characterRects[2];
				indexSprites[i] = 2;
			}
		}
	}
}
/*
void ItemSelecScene::RemoveSelectedItem() {

	for (int i = 0; i < controllersNum; i++) {
		Player* player = &players[i];

		if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_B) {
			if (player->locked > 0) {
				if (player->ready) {
					player->ready = false;
				}
				player->locked--;
				player->playerItems[player->locked] = nullptr;
				App->gui->RemoveElement(player->MiniatureItems[player->locked]);
				player->MiniatureItems[player->locked] = nullptr;
				player->RemoveLockedArrow(player->locked);

			}
		}
	}
}*/

void CharacterSelecScene::AddLabelToButton(CharacterToSelect* character) {
	int fontSize = 25;
	Label* label1 = App->gui->AddLabel(character->butt->rect.w / 2, character->butt->rect.h, fontSize, DEFAULT_FONT, { 255,255,255,255 });
	label1->setString(character->name);
	character->labels.push_back(label1);
	int i = 1;

	if (character->stats.life > 0) {
		Label* label2 = App->gui->AddLabel(character->butt->rect.w / 2, character->butt->rect.h + i * fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label2->setString("+" + std::to_string(character->stats.life) + " life");
		character->labels.push_back(label2);
		i++;
	}
	if (character->stats.atk > 0) {
		Label* label3 = App->gui->AddLabel(character->butt->rect.w / 2, character->butt->rect.h + i * fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label3->setString("+" + std::to_string(character->stats.atk) + " atk");
		character->labels.push_back(label3);
		i++;
	}
	if (character->stats.def > 0) {
		Label* label4 = App->gui->AddLabel(character->butt->rect.w / 2, character->butt->rect.h + i * fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label4->setString("+" + std::to_string(character->stats.def) + " def");
		character->labels.push_back(label4);
		i++;
	}
	if (character->stats.spd > 0) {
		Label* label5 = App->gui->AddLabel(character->butt->rect.w / 2, character->butt->rect.h + i * fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label5->setString("+" + std::to_string(character->stats.spd) + " spd");
		character->labels.push_back(label5);
		i++;
	}
	if (character->stats.mgk > 0) {
		Label* label6 = App->gui->AddLabel(character->butt->rect.w / 2, character->butt->rect.h + i * fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label6->setString("+" + std::to_string(character->stats.mgk) + " mgk");
		character->labels.push_back(label6);
		i++;
	}
	for (std::vector<Label*>::iterator lab = character->labels.begin(); lab != character->labels.end(); lab++) {

		(*lab)->SetParent(character->butt);
	}

}

void CharacterSelecScene::ManageDisplacementFocus() {
	for (int i = 0; i < controllersNum; i++) {
		if (players[i].ready) {
			return;
		}
		if (App->input->GetButtonFromController(players[i].playerNum) == Input::LEFT) {
			FindNextPlayer(i, InterfaceElement::Directions::LEFT);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == Input::RIGHT) {
			FindNextPlayer(i, InterfaceElement::Directions::RIGHT);
		}

	}
}

void CharacterSelecScene::FindNextPlayer(uint playerNum, InterfaceElement::Directions direction) {
	CharacterToSelect* nextCharacterToSelect = players[playerNum].focusedCharacter->GetRelativeCharacter(direction);

	if (nextCharacterToSelect != nullptr) {
		players[playerNum].focusedCharacter = nextCharacterToSelect;
	}
}

void CharacterSelecScene::ApplyCharacterSelection() {
	for (int i = 0; i < controllersNum; i++) {
		charactersInfo[i].chType = players[i].lockedInfo.chType;
	}
}

void CharacterSelecScene::CharacterToSelect::SetRelation(CharacterToSelect* character, InterfaceElement::Directions direction, bool assignOther) {

	relations[direction] = character;
	if (assignOther)
		character->relations[direction < 2 ? direction + 2 : direction - 2] = this;
}

CharacterSelecScene::CharacterToSelect* CharacterSelecScene::CharacterToSelect::GetRelativeCharacter(InterfaceElement::Directions dir) {

	return relations[dir];
}

void CharacterSelecScene::LoadSceneUI() {
	atlas = App->textures->Load("UI/atlas.png");
	uiPoint sizeScreen = App->gui->GetGuiSize();

	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		characterFrame[0] = App->gui->AddSprite(sizeScreen.x / 8, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[0] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[0]->SetParent(characterFrame[0]);
		characterSprites[0]->SetAnchor(0, 0);
		characterSprites[0]->setPosition(80, 89);
		characterNameLabel[0] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[0]->setString(characterNameStrings[0]);
		characterNameLabel[0]->SetParent(characterFrame[0]);
		characterNameLabel[0]->SetAnchor(0, 0);
		characterNameLabel[0]->setPosition(75, 14);
		Label* stats1Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats1Label->setString("STATS");
		stats1Label->SetParent(characterFrame[0]);
		stats1Label->setPosition(171, 462);

		characterFrame[1] = App->gui->AddSprite(sizeScreen.x / 8 * 3, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[1] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[1]->SetParent(characterFrame[1]);
		characterSprites[1]->SetAnchor(0, 0);
		characterSprites[1]->setPosition(80, 89);
		characterNameLabel[1] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[1]->setString(characterNameStrings[0]);
		characterNameLabel[1]->SetParent(characterFrame[1]);
		characterNameLabel[1]->SetAnchor(0, 0);
		characterNameLabel[1]->setPosition(75, 14);
		Label* stats2Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats2Label->setString("STATS");
		stats2Label->SetParent(characterFrame[1]);
		stats2Label->setPosition(171, 462);

		characterFrame[2] = App->gui->AddSprite((sizeScreen.x / 8) * 5, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[2] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[2]->SetParent(characterFrame[2]);
		characterSprites[2]->SetAnchor(0, 0);
		characterSprites[2]->setPosition(80, 89);
		characterNameLabel[2] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[2]->setString(characterNameStrings[0]);
		characterNameLabel[2]->SetParent(characterFrame[2]);
		characterNameLabel[2]->SetAnchor(0, 0);
		characterNameLabel[2]->setPosition(75, 14);
		Label* stats3Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats3Label->setString("STATS");
		stats3Label->SetParent(characterFrame[2]);
		stats3Label->setPosition(171, 462);

		characterFrame[3] = App->gui->AddSprite((sizeScreen.x / 8) * 7, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[3] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[3]->SetParent(characterFrame[3]);
		characterSprites[3]->SetAnchor(0, 0);
		characterSprites[3]->setPosition(80, 89);
		characterNameLabel[3] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[3]->setString(characterNameStrings[0]);
		characterNameLabel[3]->SetParent(characterFrame[3]);
		characterNameLabel[3]->SetAnchor(0, 0);
		characterNameLabel[3]->setPosition(75, 14);
		Label* stats4Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats4Label->setString("STATS");
		stats4Label->SetParent(characterFrame[3]);
		stats4Label->setPosition(171, 462);
	}

	else if (App->scenes->gameMode == GameMode::ONEvsONE){
		characterFrame[0] = App->gui->AddSprite(sizeScreen.x / 4, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[0] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[0]->SetParent(characterFrame[0]);
		characterSprites[0]->SetAnchor(0, 0);
		characterSprites[0]->setPosition(80, 89);
		characterNameLabel[0] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[0]->setString(characterNameStrings[0]);
		characterNameLabel[0]->SetParent(characterFrame[0]);
		characterNameLabel[0]->SetAnchor(0, 0);
		characterNameLabel[0]->setPosition(75, 14);
		Label* stats1Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats1Label->setString("STATS");
		stats1Label->SetParent(characterFrame[0]);
		stats1Label->setPosition(171, 462);

		characterFrame[1] = App->gui->AddSprite(sizeScreen.x / 4 * 3, 20 + sizeScreen.y / 5 * 3, atlas, { 1296, 50, 343, 659 });
		characterSprites[1] = App->gui->AddSprite(0, 0, atlas, characterRects[0]);
		characterSprites[1]->SetParent(characterFrame[1]);
		characterSprites[1]->SetAnchor(0, 0);
		characterSprites[1]->setPosition(80, 89);
		characterNameLabel[1] = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT);
		characterNameLabel[1]->setString(characterNameStrings[0]);
		characterNameLabel[1]->SetParent(characterFrame[1]);
		characterNameLabel[1]->SetAnchor(0, 0);
		characterNameLabel[1]->setPosition(75, 14);
		Label* stats2Label = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 80, 80, 80, 255 });
		stats2Label->setString("STATS");
		stats2Label->SetParent(characterFrame[1]);
		stats2Label->setPosition(171, 462);

	}

	//int i = 0;

	/*characters[i] = new CharacterToSelect("WARRIOR", WARRIOR, { 480,0,120,120 }, { 0,0,5,0,0 });
	characters[++i] = new CharacterToSelect("ROGUE", ROGUE, { 480,0,120,120 }, { 0,0,5,0,0 });
	characters[++i] = new CharacterToSelect("WIZARD", WIZARD, { 480,0,120,120 }, { 0,0,5,0,0 });
	characters[++i] = new CharacterToSelect("CLERIC", CLERIC, { 480,0,120,120 }, { 0,0,5,0,0 });*/
	
	/*characters[0]->SetRelation(characters[1], InterfaceElement::Directions::RIGHT);
	characters[1]->SetRelation(characters[2], InterfaceElement::Directions::RIGHT);
	characters[2]->SetRelation(characters[3], InterfaceElement::Directions::RIGHT);
	characters[3]->SetRelation(characters[0], InterfaceElement::Directions::RIGHT);*/



}