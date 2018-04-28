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


ItemSelecScene::ItemSelecScene()
{
}

ItemSelecScene::~ItemSelecScene()
{
}

bool ItemSelecScene::Start()
{
	LoadBackground("UI/BasicMenuScene.png");

	LoadSceneUI();
	SetControllerFocus();

	return true;
}

bool ItemSelecScene::Update(float dt)
{
	if (AllPlayersReady()) {//ALL ARROWS LOCKED 
		ApplyItemAttributes();						 // NEED TO FILL 
		App->scenes->ChangeScene(App->scenes->mainSc);
	}

	DrawBackground();
	//App->input->CheckControllers();

	if (controllersNum != 0) {
		ManageDisplacementFocus();
		ChooseFocus();
	}

	return true;
}

bool ItemSelecScene::CleanUp()
{	
	App->gui->CleanUp();
	App->textures->UnLoad(atlas);
	UnLoadBackground();
	return true;
}

void ItemSelecScene::LoadSceneUI() {
	atlas = App->textures->Load("UI/items.png");
	uiPoint sizeScreen = App->gui->GetGuiSize();

	int i = 0;

	items[i] = new Item("firstItem", { 0,0,120,120}, { 10,20,30,50 });
	items[i]->butt = App->gui->AddButton(100,100,atlas, items[i]->animRect, true, nullptr );//0	
	App->gui->setFocus(items[i]->butt);

	items[++i] = new Item("secItem", { 120,0,120,120 }, { 11,20,30,50 });
	items[i]->butt = App->gui->AddButton(300, 100, atlas, items[i]->animRect, true, nullptr);//1

	items[++i] = new Item("thirdItem", { 240,0,120,120 }, { 12,20,30,50 });
	items[i]->butt = App->gui->AddButton(500, 100, atlas, items[i]->animRect, true, nullptr);//2

	items[++i] = new Item("fourthItem", { 360,0,120,120 }, { 13,20,30,50 });
	items[i]->butt = App->gui->AddButton(700, 100, atlas, items[i]->animRect, true, nullptr);//3

	items[++i] = new Item("fifthItem", { 480,0,120,120 }, { 14,20,30,50 });
	items[i]->butt = App->gui->AddButton(900, 100, atlas, items[i]->animRect, true, nullptr);//4



	items[0]->SetRelation(items[1], InterfaceElement::Directions::RIGHT);
	items[1]->SetRelation(items[2], InterfaceElement::Directions::RIGHT);
	items[2]->SetRelation(items[3], InterfaceElement::Directions::RIGHT);
	items[3]->SetRelation(items[4], InterfaceElement::Directions::RIGHT);
	items[4]->SetRelation(items[0], InterfaceElement::Directions::RIGHT);
	
}

bool ItemSelecScene::AllPlayersReady() {
	
	bool ret = true;
	for (int i = 0; i < controllersNum; i++) {
		if (players[i].locked == 3) {
			players[i].ready = true;
		}
	}
	for (int i = 0; i < controllersNum; i++) {
		bool ret2 = players[i].ready;
		ret &= ret2;
	}
	
	return ret;//
}

void ItemSelecScene::SetControllerFocus() {

	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}

	for (int i = 1; i <= controllersNum; i++) {
		Player player;		
		player.focusedItem = items[0];
	
		player.playerNum = i;
		player.totalControllersNum = controllersNum;
		player.LoadArrows();
		players.push_back(player);
	}
}




void ItemSelecScene::ChooseFocus() {

	for (int i = 0; i < controllersNum; i++) {
		if (players[i].ready) {
			return;
		}
		if (App->input->GetButtonFromController(players[i].playerNum) == CharInput::JUMPINPUT) {
			LOG("");

			players[i].playerItems[players[i].locked] = players[i].focusedItem;
			players[i].locked++;			
			players[i].LockedArrow(players[i].locked);
			
		}
	}
}

void ItemSelecScene::ManageDisplacementFocus() {
	for (int i = 0; i < controllersNum; i++) {
		if (players[i].ready) {
			return;
		}
		if (App->input->GetButtonFromController(players[i].playerNum) == CharInput::CH_DOWN) {

			FindNextArrowUnlocked(i, InterfaceElement::Directions::DOWN);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == CharInput::CH_UP) {
			FindNextArrowUnlocked(i, InterfaceElement::Directions::UP);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == CharInput::CH_LEFT) {
			FindNextArrowUnlocked(i, InterfaceElement::Directions::LEFT);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == CharInput::CH_RIGHT) {
			FindNextArrowUnlocked(i, InterfaceElement::Directions::RIGHT);
		}
		
	}
}


void ItemSelecScene::FindNextArrowUnlocked(uint playerNum, InterfaceElement::Directions direction) {
	Item* nextItem = players[playerNum].focusedItem->GetRelativeItem(direction);
	
		if (nextItem != nullptr) {
			
			players[playerNum].focusedItem = nextItem;

			if (players[playerNum].locked!=0) {
				for (int i = 0; i <= players[playerNum].locked; i++) { // check if the item is already taken
					if (players[playerNum].playerItems[i] == players[playerNum].focusedItem) {
						FindNextArrowUnlocked(playerNum, direction);
					}
				}
			}
			
			players[playerNum].DrawOrderedArrow();
			//App->gui->setFocus(nextElem);

		}

}

void ItemSelecScene::Player::LoadArrows() {
	
	switch (playerNum) {
	case 0: return;
	case 1: 
		arrowRect = { 754, 50, 22,19 }; // Red
		arrowLockRect = { 754, 87, 23,21 };
		break;
	case 2:
		arrowRect = { 825, 50, 22,19 }; // Green
		arrowLockRect = { 825, 87, 23,21 };
		break;
	case 3:
		arrowRect = { 778, 50, 22,19 }; //Blue
		arrowLockRect = { 778, 87, 23,21 };
		break;
	case 4:
		arrowRect = { 802, 50, 22,19 }; // Yellow (should be gray)
		arrowLockRect = { 802, 87, 23,21 };
		break;
	}
	
	arrow = App->gui->AddSprite(focusedItem->butt->getPositionX(), focusedItem->butt->getPositionY() - focusedItem->butt->rect.h/2, nullptr, arrowRect);
	DrawOrderedArrow();
}

void ItemSelecScene::Player::DrawOrderedArrow() {		
	int distance = focusedItem->butt->rect.w / (totalControllersNum+1);
	 
	int x = focusedItem->butt->getPositionX() - (focusedItem->butt->rect.w/2) + (distance * playerNum);
	arrow->setPosition(x, focusedItem->butt->getPositionY() - focusedItem->butt->rect.h / 2);
}

void ItemSelecScene::ApplyItemAttributes() {

	/*
	for (std::list<Selection>::iterator focus = playersSelections.begin(); focus != playersSelections.end(); focus++) {
		EntityStats* item = &App->entities->items[(*focus).playerNum-1];

		if ((*focus).but == swiftBoots) {
			//Apply the effect to the player num ( (*focus).playerNum  )
			item->spd += 5;
		}
		else if ((*focus).but == cursedSword) {
			//Apply the effect to the player num ( (*focus).playerNum  )
			item->atk += 5;
		}
		else if ((*focus).but == paladinsHandguards) {
			//Apply the effect to the player num ( (*focus).playerNum  )
			item->def += 5;
		}
		else if ((*focus).but == ringProtection) {
			//Apply the effect to the player num ( (*focus).playerNum  )
			item->life += 10;
		}
		else if ((*focus).but == dragonSlayer) {
			//Apply the effect to the player num ( (*focus).playerNum  )
			item->atk += 2;
			item->def += 2;
		}
		else if ((*focus).but == magicRobe) {
			//Apply the effect to the player num ( (*focus).playerNum  )
			item->atk += 2;
			item->spd += 2;
		}
	}*/
}

void ItemSelecScene::Player::LockedArrow(uint lockedNum) {
	lockedArrows[lockedNum-1] = App->gui->AddSprite(arrow->getPositionX(), arrow->getPositionY() , nullptr, arrowLockRect);

}
void ItemSelecScene::Item::SetRelation(Item* item, InterfaceElement::Directions direction, bool assignOther){
	
	relations[direction] = item;
	if (assignOther)
		item->relations[direction < 2 ? direction + 2 : direction - 2] = this;
}

ItemSelecScene::Item* ItemSelecScene::Item::GetRelativeItem(InterfaceElement::Directions dir){

	return relations[dir];
}