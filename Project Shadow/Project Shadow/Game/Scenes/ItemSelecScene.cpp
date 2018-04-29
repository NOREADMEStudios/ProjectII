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
		RemoveSelectedItem();
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

	items[i] = new Item("Plate Mail", PLATE, { 480,0,120,120 }, { 0,0,5,0,0 });
	items[i]->butt = App->gui->AddButton(100, 100, atlas, items[i]->animRect, true, nullptr);//0
	AddLabelToButton(items[i]);
	App->gui->setFocus(items[i]->butt);

	items[++i] = new Item("Normal Sword", SWORD, { 480,0,120,120 }, { 0,5,0,0,0 });
	items[i]->butt = App->gui->AddButton(300, 100, atlas, items[i]->animRect, true, nullptr);//1
	AddLabelToButton(items[i]);
	
	items[++i] = new Item("Swift Boots", SWIFT_BOOTS, { 0,0,120,120 }, { 0,0,0,10,0 });
	items[i]->butt = App->gui->AddButton(500, 100, atlas, items[i]->animRect, true, nullptr);//2
	AddLabelToButton(items[i]);

	items[++i] = new Item("Magic Robe", ROBE, { 120,0,120,120 }, { 0,10,0,0 });
	items[i]->butt = App->gui->AddButton(700, 100, atlas, items[i]->animRect, true, nullptr);//3
	AddLabelToButton(items[i]);

	items[++i] = new Item("Mage Hat", MAGE_HAT, { 240,0,120,120 }, { 0,0,0,0,0 }); // increases stamina??
	items[i]->butt = App->gui->AddButton(900, 100, atlas, items[i]->animRect, true, nullptr);//4
	AddLabelToButton(items[i]);


	items[++i] = new Item("Thief Hood", HOOD, { 360,0,120,120 }, { 0,0,0,10,0 });
	items[i]->butt = App->gui->AddButton(100, 300, atlas, items[i]->animRect, true, nullptr);//5
	AddLabelToButton(items[i]);
	
	items[++i] = new Item("Cleric Hat", CLERIC_HAT, { 0,0,120,120 }, { 0,0,0,0,0 });
	items[i]->butt = App->gui->AddButton(300,300, atlas, items[i]->animRect, true, nullptr);//6
	AddLabelToButton(items[i]);

	items[++i] = new Item("Ring of Protection", RING, { 360,0,120,120 }, { 0,0,10,0,0 });
	items[i]->butt = App->gui->AddButton(500, 300, atlas, items[i]->animRect, true, nullptr);//7
	AddLabelToButton(items[i]);

	items[++i] = new Item("Tiara", TIARA, { 0,0,120,120 }, { 0,20,0,0,0 });
	items[i]->butt = App->gui->AddButton(700, 300, atlas, items[i]->animRect, true, nullptr);//8
	AddLabelToButton(items[i]);

	items[++i] = new Item("Cursed Sword",CURSED_SWORD, { 120,0,120,120 }, { 0,10,0,0 });
	items[i]->butt = App->gui->AddButton(900, 300, atlas, items[i]->animRect, true, nullptr);//9
	AddLabelToButton(items[i]);


	items[++i] = new Item("Boots of Haste", HASTE_BOOTS, { 0,0,120,120 }, { 0,0,0,15,0 });
	items[i]->butt = App->gui->AddButton(100, 500, atlas, items[i]->animRect, true, nullptr);//10
	AddLabelToButton(items[i]);

	items[++i] = new Item("Paladin’s Handguards", HANDGUARDS,{ 240,0,120,120 }, { 0,0,15,0,0 });
	items[i]->butt = App->gui->AddButton(300, 500, atlas, items[i]->animRect, true, nullptr);//11
	AddLabelToButton(items[i]);

	items[++i] = new Item("Earrings of Energy", EARRINGS, { 0,0,120,120 }, { 0,0,0,0,0 });
	items[i]->butt = App->gui->AddButton(500, 500, atlas, items[i]->animRect, true, nullptr);//12
	AddLabelToButton(items[i]);

	items[++i] = new Item("Dragon Slayer",DRAGONSLAYER, { 0,0,120,120 }, { 0,20,0,0,0 });
	items[i]->butt = App->gui->AddButton(700, 500, atlas, items[i]->animRect, true, nullptr);//13
	AddLabelToButton(items[i]);

	items[++i] = new Item("Staff of Thoth", STAFF, { 0,0,120,120 }, { 0,35,0,0,0 });
	items[i]->butt = App->gui->AddButton(900, 500, atlas, items[i]->animRect, true, nullptr);//14
	AddLabelToButton(items[i]);

	items[0]->SetRelation(items[1], InterfaceElement::Directions::RIGHT);
	items[1]->SetRelation(items[2], InterfaceElement::Directions::RIGHT);
	items[2]->SetRelation(items[3], InterfaceElement::Directions::RIGHT);
	items[3]->SetRelation(items[4], InterfaceElement::Directions::RIGHT);
	items[4]->SetRelation(items[0], InterfaceElement::Directions::RIGHT);
	
	items[5]->SetRelation(items[6], InterfaceElement::Directions::RIGHT);
	items[6]->SetRelation(items[7], InterfaceElement::Directions::RIGHT);
	items[7]->SetRelation(items[8], InterfaceElement::Directions::RIGHT);
	items[8]->SetRelation(items[9], InterfaceElement::Directions::RIGHT);
	items[9]->SetRelation(items[5], InterfaceElement::Directions::RIGHT);

	items[10]->SetRelation(items[11], InterfaceElement::Directions::RIGHT);
	items[11]->SetRelation(items[12], InterfaceElement::Directions::RIGHT);
	items[12]->SetRelation(items[13], InterfaceElement::Directions::RIGHT);
	items[13]->SetRelation(items[14], InterfaceElement::Directions::RIGHT);
	items[14]->SetRelation(items[10], InterfaceElement::Directions::RIGHT);

	items[0]->SetRelation(items[5], InterfaceElement::Directions::DOWN);
	items[1]->SetRelation(items[6], InterfaceElement::Directions::DOWN);
	items[2]->SetRelation(items[7], InterfaceElement::Directions::DOWN);
	items[3]->SetRelation(items[8], InterfaceElement::Directions::DOWN);
	items[4]->SetRelation(items[9], InterfaceElement::Directions::DOWN);
	
	items[5]->SetRelation(items[10], InterfaceElement::Directions::DOWN);
	items[6]->SetRelation(items[11], InterfaceElement::Directions::DOWN);
	items[7]->SetRelation(items[12], InterfaceElement::Directions::DOWN);
	items[8]->SetRelation(items[13], InterfaceElement::Directions::DOWN);
	items[9]->SetRelation(items[14], InterfaceElement::Directions::DOWN);

	items[10]->SetRelation(items[0], InterfaceElement::Directions::DOWN);
	items[11]->SetRelation(items[1], InterfaceElement::Directions::DOWN);
	items[12]->SetRelation(items[2], InterfaceElement::Directions::DOWN);
	items[13]->SetRelation(items[3], InterfaceElement::Directions::DOWN);
	items[14]->SetRelation(items[4], InterfaceElement::Directions::DOWN);

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

void ItemSelecScene::AddLabelToButton(Item* item) {
	int fontSize = 25;
	item->label = App->gui->AddLabel(item->butt->getPositionX(), item->butt->getPositionY() + item->butt->rect.w / 2, fontSize, DEFAULT_FONT, {255,255,255,255});
	std::string string = item->name;
	
	if (item->stats.life > 0) {
		string += "\n + %d life", item->stats.life;		
	}
	if (item->stats.atk > 0) {
		string += "\n + %d atk", item->stats.atk;
	}
	if (item->stats.def > 0) {
		string += "\n + %d def", item->stats.def;
	}
	if (item->stats.spd > 0) {
		string += "\n + %d spd", item->stats.spd;
	}
	if (item->stats.mgk > 0) {
		string += "\n + %d mgk", item->stats.mgk;
	}
	item->label->setString(string);
	//item->label->SetParent(item->butt);
}

void ItemSelecScene::SetControllerFocus() {

	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}

	for (int i = 1; i <= controllersNum; i++) {
		Player player ;		
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
			
			Item* item = players[i].focusedItem;
			players[i].playerItems[players[i].locked] = item;
			players[i].MiniatureItems[players[i].locked] = App->gui->AddSprite(50 + item->butt->rect.w*players[i].locked + 300 * i, 800, atlas, item->animRect);//need to check values
			players[i].LockedArrow(players[i].locked);
			players[i].locked++;
			FindFirstFreeItem(i);
		}		
	}
}
void ItemSelecScene::RemoveSelectedItem() {


	for (int i = 0; i < controllersNum; i++) {
		Player* player = &players[i];
		
		if (App->input->GetButtonFromController(player->playerNum) == CharInput::PARRYINPUT) {			
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

void ItemSelecScene::FindFirstFreeItem(uint playerNum) {
	Player* player = &players[playerNum];

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < player->locked; j++) { 
			if (player->playerItems[j] != items[i]) {			
				if (j == player->locked-1) {
					player->focusedItem = items[i];
					player->DrawOrderedArrow();
					return;
				}
			}
			else {
				break;
			}
		}
	}
}

void ItemSelecScene::ApplyItemAttributes() {
	for (int i = 0; i < controllersNum; i++) {
		switch (players[i].focusedItem->type)
		{
		case ItemType::PLATE:
		{}
		}
	}
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

void ItemSelecScene::Player::RemoveLockedArrow(uint lockedNum) {
	App->gui->RemoveElement(lockedArrows[lockedNum - 1]);
}
void ItemSelecScene::Item::SetRelation(Item* item, InterfaceElement::Directions direction, bool assignOther){
	
	relations[direction] = item;
	if (assignOther)
		item->relations[direction < 2 ? direction + 2 : direction - 2] = this;
}

ItemSelecScene::Item* ItemSelecScene::Item::GetRelativeItem(InterfaceElement::Directions dir){

	return relations[dir];
}