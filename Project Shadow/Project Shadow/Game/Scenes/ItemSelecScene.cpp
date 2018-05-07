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
	LoadBackground("UI/BackgroundItems.png");

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
	items[i]->butt = App->gui->AddButton(200, 150, atlas, items[i]->animRect, true, nullptr);//0
	AddLabelToButton(items[i]);
	App->gui->setFocus(items[i]->butt);

	items[++i] = new Item("Normal Sword", SWORD, { 480,0,120,120 }, { 0,5,0,0,0 });
	items[i]->butt = App->gui->AddButton(545, 150, atlas, items[i]->animRect, true, nullptr);//1
	AddLabelToButton(items[i]);
	
	items[++i] = new Item("Swift Boots", SWIFT_BOOTS, { 0,0,120,120 }, { 0,0,0,10,0 });
	items[i]->butt = App->gui->AddButton(880, 150, atlas, items[i]->animRect, true, nullptr);//2
	AddLabelToButton(items[i]);

	items[++i] = new Item("Magic Robe", ROBE, { 120,0,120,120 }, { 0,10,0,0 });
	items[i]->butt = App->gui->AddButton(1210, 150, atlas, items[i]->animRect, true, nullptr);//3
	AddLabelToButton(items[i]);

	items[++i] = new Item("Mage Hat", MAGE_HAT, { 240,0,120,120 }, { 0,0,0,0,0 }); // increases stamina??
	items[i]->butt = App->gui->AddButton(1545, 150, atlas, items[i]->animRect, true, nullptr);//4
	AddLabelToButton(items[i]);
	
	//2nd Row	

	items[++i] = new Item("Thief Hood", HOOD, { 360,0,120,120 }, { 0,0,0,10,0 });
	items[i]->butt = App->gui->AddButton(200, 410, atlas, items[i]->animRect, true, nullptr);//5
	AddLabelToButton(items[i]);
	
	items[++i] = new Item("Cleric Hat", CLERIC_HAT, { 0,0,120,120 }, { 0,0,0,0,0 });
	items[i]->butt = App->gui->AddButton(545, 410, atlas, items[i]->animRect, true, nullptr);//6
	AddLabelToButton(items[i]);

	items[++i] = new Item("Ring of Protection", RING, { 360,0,120,120 }, { 0,0,10,0,0 });
	items[i]->butt = App->gui->AddButton(880, 410, atlas, items[i]->animRect, true, nullptr);//7
	AddLabelToButton(items[i]);

	items[++i] = new Item("Tiara", TIARA, { 0,0,120,120 }, { 0,20,0,0,0 });
	items[i]->butt = App->gui->AddButton(1210, 410, atlas, items[i]->animRect, true, nullptr);//8
	AddLabelToButton(items[i]);

	items[++i] = new Item("Cursed Sword",CURSED_SWORD, { 120,0,120,120 }, { 0,10,0,0 });
	items[i]->butt = App->gui->AddButton(1545, 410, atlas, items[i]->animRect, true, nullptr);//9
	AddLabelToButton(items[i]);

	//3rd Row	

	items[++i] = new Item("Boots of Haste", HASTE_BOOTS, { 0,0,120,120 }, { 0,0,0,15,0 });
	items[i]->butt = App->gui->AddButton(200, 665, atlas, items[i]->animRect, true, nullptr);//10
	AddLabelToButton(items[i]);

	items[++i] = new Item("Paladinís Handguards", HANDGUARDS,{ 240,0,120,120 }, { 0,0,15,0,0 });
	items[i]->butt = App->gui->AddButton(545, 665, atlas, items[i]->animRect, true, nullptr);//11
	AddLabelToButton(items[i]);

	items[++i] = new Item("Earrings of Energy", EARRINGS, { 0,0,120,120 }, { 1,1,1,0,1 });
	items[i]->butt = App->gui->AddButton(880, 665, atlas, items[i]->animRect, true, nullptr);//12
	AddLabelToButton(items[i]);

	items[++i] = new Item("Dragon Slayer",DRAGONSLAYER, { 0,0,120,120 }, { 0,20,0,0,0 });
	items[i]->butt = App->gui->AddButton(1210, 665, atlas, items[i]->animRect, true, nullptr);//13
	AddLabelToButton(items[i]);

	items[++i] = new Item("Staff of Thoth", STAFF, { 0,0,120,120 }, { 0,35,0,0,0 });
	items[i]->butt = App->gui->AddButton(1545, 665, atlas, items[i]->animRect, true, nullptr);//14
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
	Label* label1 = App->gui->AddLabel(item->butt->rect.w/2, item->butt->rect.h, fontSize, DEFAULT_FONT, {255,255,255,255});
	label1->setString(item->name);
	item->labels.push_back(label1 );
	int i = 1;
	
	if (item->stats.life > 0) {
		Label* label2 = App->gui->AddLabel(item->butt->rect.w / 2, item->butt->rect.h+ i*fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label2->setString("+" + std::to_string(item->stats.life) + " life");
		item->labels.push_back(label2);
		i++;
	}
	if (item->stats.atk > 0) {
		Label* label3 = App->gui->AddLabel(item->butt->rect.w / 2, item->butt->rect.h + i*fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label3->setString("+" + std::to_string(item->stats.atk) + " atk");
		item->labels.push_back(label3);
		i++;
	}
	if (item->stats.def > 0) {
		Label* label4 = App->gui->AddLabel(item->butt->rect.w / 2, item->butt->rect.h + i*fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label4->setString("+" + std::to_string(item->stats.def) + " def");
		item->labels.push_back(label4);
		i++;
	}
	if (item->stats.spd > 0) {
		Label* label5 = App->gui->AddLabel(item->butt->rect.w / 2, item->butt->rect.h + i*fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label5->setString("+" + std::to_string(item->stats.spd) + " spd");
		item->labels.push_back(label5);
		i++;
	}
	if (item->stats.mgk > 0) {
		Label* label6 = App->gui->AddLabel(item->butt->rect.w / 2, item->butt->rect.h + i*fontSize, fontSize, DEFAULT_FONT, { 255,255,255,255 });
		label6->setString("+" + std::to_string(item->stats.mgk) + " mgk");
		item->labels.push_back(label6);
		i++;
	}
	for (std::vector<Label*>::iterator lab = item->labels.begin(); lab != item->labels.end(); lab++) {
	
		(*lab)->SetParent(item->butt);
	}
	
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

	int initialDisplacement = 130;
	int marginBetweenItemFrames = 17;
	int marginBetweenPlayerFrames = 300;

	for (int i = 0; i < controllersNum; i++) {
		if (players[i].ready) {
			return;
		}
		if (App->input->GetButtonFromController(players[i].playerNum) == Input::BUTTON_A ) {
			
			Item* item = players[i].focusedItem;
			players[i].playerItems[players[i].locked] = item;
			players[i].MiniatureItems[players[i].locked] = App->gui->AddSprite(initialDisplacement + (item->butt->rect.w+ marginBetweenItemFrames)*players[i].locked + marginBetweenPlayerFrames * i, 1000, atlas, item->animRect);//need to check values
			players[i].LockedArrow(players[i].locked);
			players[i].locked++;
			FindFirstFreeItem(i);
		}		
	}
}
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
}

void ItemSelecScene::ManageDisplacementFocus() {
	for (int i = 0; i < controllersNum; i++) {
		if (players[i].ready) {
			return;
		}
		if (App->input->GetButtonFromController(players[i].playerNum) == Input::DOWN) {

			FindNextArrowUnlocked(i, InterfaceElement::Directions::DOWN);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == Input::UP) {
			FindNextArrowUnlocked(i, InterfaceElement::Directions::UP);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == Input::LEFT) {
			FindNextArrowUnlocked(i, InterfaceElement::Directions::LEFT);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == Input::RIGHT) {
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
		EntityStats* item = &App->entities->items[i];
		for (int itemNum = 0; itemNum < 3; itemNum++) {
			item->atk += players[i].playerItems[itemNum]->stats.atk;
			item->def += players[i].playerItems[itemNum]->stats.def;
			item->life += players[i].playerItems[itemNum]->stats.life;
			item->mgk += players[i].playerItems[itemNum]->stats.mgk;
			item->spd += players[i].playerItems[itemNum]->stats.spd;
		}
	}
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