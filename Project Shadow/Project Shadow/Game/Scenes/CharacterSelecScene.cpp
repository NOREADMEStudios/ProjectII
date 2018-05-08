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
	
	/*for (int i = 0; i < controllersNum; i++) {
		if (i = 0)
			charactersInfo[i] = { WIZARD,{ 100,100 }, Team::BLUE };
		else if (i = 1)
			charactersInfo[i] = { WIZARD,{ 10000,100 }, Team::RED };
		else if (i = 2)
			charactersInfo[i] = { WARRIOR,{ 100,1000 }, Team::BLUE };
		else if (i = 3)
			charactersInfo[i] = { WARRIOR,{ 10000,1000 }, Team::RED };
	}*/

	LoadSceneUI();
	//SetControllerFocus();

	return true;
}

bool CharacterSelecScene::Update(float dt)
{

	DrawBackground();
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
		player.focusedCharacter = characters[0];

		player.playerNum = i;
		player.totalControllersNum = controllersNum;
		/*player.LoadArrows();*/
		players.push_back(player);
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
		characterFrame[0] = App->gui->AddSprite(sizeScreen.x / 8, sizeScreen.y / 2, atlas, { 1275, 50, 194, 373 });
		characterFrame[1] = App->gui->AddSprite(sizeScreen.x / 8 * 3, sizeScreen.y / 2, atlas, { 1275, 50, 194, 373 });
		characterFrame[2] = App->gui->AddSprite((sizeScreen.x / 8) * 5, sizeScreen.y / 2, atlas, { 1275, 50, 194, 373 });
		characterFrame[3] = App->gui->AddSprite((sizeScreen.x / 8) * 7, sizeScreen.y / 2, atlas, { 1275, 50, 194, 373 });
	}
	else if (App->scenes->gameMode == GameMode::ONEvsONE){
		characterFrame[0] = App->gui->AddSprite(sizeScreen.x / 4, sizeScreen.y / 2, atlas, { 1275, 50, 194, 373 });
		characterFrame[1] = App->gui->AddSprite(sizeScreen.x / 4 * 3, sizeScreen.y / 2, atlas, { 1275, 50, 194, 373 });
	}

	int i = 0;

	characters[i] = new CharacterToSelect("WARRIOR", WARRIOR, { 480,0,120,120 }, { 0,0,5,0,0 });
	characters[i]->butt = App->gui->AddButton(200, 150, atlas, characters[i]->animRect, true, nullptr);//0
	AddLabelToButton(characters[i]);
	App->gui->setFocus(characters[i]->butt);

	characters[++i] = new CharacterToSelect("ROGUE", ROGUE, { 480,0,120,120 }, { 0,0,5,0,0 });
	characters[i]->butt = App->gui->AddButton(200, 150, atlas, characters[i]->animRect, true, nullptr);//1
	AddLabelToButton(characters[i]);
	App->gui->setFocus(characters[i]->butt);

	characters[++i] = new CharacterToSelect("WIZARD", WIZARD, { 480,0,120,120 }, { 0,0,5,0,0 });
	characters[i]->butt = App->gui->AddButton(200, 150, atlas, characters[i]->animRect, true, nullptr);//2
	AddLabelToButton(characters[i]);
	App->gui->setFocus(characters[i]->butt);

	characters[++i] = new CharacterToSelect("CLERIC", CLERIC, { 480,0,120,120 }, { 0,0,5,0,0 });
	characters[i]->butt = App->gui->AddButton(200, 150, atlas, characters[i]->animRect, true, nullptr);//3
	AddLabelToButton(characters[i]);
	App->gui->setFocus(characters[i]->butt);

	characters[0]->SetRelation(characters[1], InterfaceElement::Directions::RIGHT);
	characters[1]->SetRelation(characters[2], InterfaceElement::Directions::RIGHT);
	characters[2]->SetRelation(characters[3], InterfaceElement::Directions::RIGHT);
	characters[3]->SetRelation(characters[0], InterfaceElement::Directions::RIGHT);



}