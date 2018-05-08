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
	LoadBackground("UI/BackgroundItems.png");

	//LoadSceneUI();
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

//bool ItemSelecScene::AllPlayersReady() {
//
//	bool ret = true;
//	for (int i = 0; i < controllersNum; i++) {
//		if (players[i].locked == 3) {
//			players[i].ready = true;
//		}
//	}
//	for (int i = 0; i < controllersNum; i++) {
//		bool ret2 = players[i].ready;
//		ret &= ret2;
//	}
//
//	return ret;//
//}

void CharacterSelecScene::SetControllerFocus() {

	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}

	for (int i = 1; i <= controllersNum; i++) {
		Player player;
		/*player.focusedItem = items[0];*/

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

}
CharacterSelecScene::CharacterToSelect* CharacterSelecScene::CharacterToSelect::GetRelativeCharacter(InterfaceElement::Directions dir) {

	return relations[dir];
}