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
	uiPoint screenSize = App->gui->GetGuiSize();
	LoadBackground("UI/BackgroundItems.png");

	Sprite* bg = App->gui->AddSprite(0, 0, background, { 0,0,1920,1080 });
	bg->setPosition(screenSize.x * 0.5f, screenSize.y * 0.5f);
	bg->ComputeRects();
	if (screenSize.y < bg->rect.h) {
		bg->scale = MAX((float)screenSize.y / (float)bg->rect.h, (float)screenSize.x / (float)bg->rect.w);
	}
	else {
		bg->scale = MIN((float)screenSize.y / (float)bg->rect.h, (float)screenSize.x / (float)bg->rect.w);
	}

	LoadSceneUI();
	SetControllerFocus();
	SetDebugLabels();

	return true;
}

bool ItemSelecScene::Update(float dt)
{
	if (AllPlayersReady()) {//ALL ARROWS LOCKED 
		App->scenes->ChangeScene(App->scenes->mainSc);
	}

	//DrawBackground();
	//App->input->CheckControllers();

	if (controllersNum != 0) {
		ManageDisplacementFocus();
		ChooseFocus();
		RemoveSelectedItem();
	}
	UpdateDebugLabels();
	return true;
}

bool ItemSelecScene::CleanUp()
{	

	App->gui->CleanUp();
	App->textures->UnLoad(atlas);
	
	UnLoadBackground();
	//players.clear();
	return true;
}

void ItemSelecScene::LoadSceneUI() {

	uiPoint sizeScreen = App->gui->GetGuiSize();

	int i = 0;

	items[i] = new Item("Staff of Thoth", STAFF, { 1282,1568,180,180 }, { 360,1825,90,90 }, { 0,0,0,0,0,20,0 });
	items[i]->butt = App->gui->AddButton(sizeScreen.x / 4, sizeScreen.y / 3, nullptr, items[i]->animRect, true, nullptr);
	App->gui->setFocus(items[i]->butt);
	AddLabelToButton(items[i]);

	items[++i] = new Item("Dragon Slayer", DRAGONSLAYER, { 1467,1568,180,180 }, { 360,1735,90,90 }, { 0,30,0,0,0,0,0,0 });
	items[i]->butt = App->gui->AddButton(sizeScreen.x / 2, sizeScreen.y / 3, nullptr, items[i]->animRect, true, nullptr);
	AddLabelToButton(items[i]);

	items[++i] = new Item("Plate Mail", PLATE, { 1652,1568,180,180 }, { 0,1645,90,90 }, { 0,0,30,0,0,0,0,0 });
	items[i]->butt = App->gui->AddButton(sizeScreen.x * 3 / 4, sizeScreen.y / 3, nullptr, items[i]->animRect, true, nullptr);
	AddLabelToButton(items[i]);

	items[++i] = new Item("Ring of Protection", RING, { 1282,1753,180,180 }, { 180,1735,90,90 }, { 0,0,0,0,0,0,30,0 });
	items[i]->butt = App->gui->AddButton(sizeScreen.x / 4, sizeScreen.y * 2 / 3, nullptr, items[i]->animRect, true, nullptr);
	AddLabelToButton(items[i]);

	items[++i] = new Item("Swift Boots", SWIFT_BOOTS, { 1467,1753,180,180 }, { 180,1645,90,90 }, { 0,0,0,50,0,0,0,0 });
	items[i]->butt = App->gui->AddButton(sizeScreen.x / 2, sizeScreen.y * 2 / 3, nullptr, items[i]->animRect, true, nullptr);
	AddLabelToButton(items[i]);

	items[++i] = new Item("Tiara", TIARA, { 1652,1753,180,180 }, { 270,1735,90,90 }, { 0,0,0,0,0,0,0,1 });
	items[i]->butt = App->gui->AddButton(sizeScreen.x * 3 / 4, sizeScreen.y * 2 / 3, nullptr, items[i]->animRect, true, nullptr);
	AddLabelToButton(items[i]);


	items[0]->SetRelation(items[1], InterfaceElement::Directions::RIGHT);
	items[1]->SetRelation(items[2], InterfaceElement::Directions::RIGHT);
	items[2]->SetRelation(items[0], InterfaceElement::Directions::RIGHT);

	items[3]->SetRelation(items[4], InterfaceElement::Directions::RIGHT);
	items[4]->SetRelation(items[5], InterfaceElement::Directions::RIGHT);
	items[5]->SetRelation(items[3], InterfaceElement::Directions::RIGHT);

	items[0]->SetRelation(items[3], InterfaceElement::Directions::DOWN);
	items[1]->SetRelation(items[4], InterfaceElement::Directions::DOWN);
	items[2]->SetRelation(items[5], InterfaceElement::Directions::DOWN);

	items[3]->SetRelation(items[0], InterfaceElement::Directions::DOWN);
	items[4]->SetRelation(items[1], InterfaceElement::Directions::DOWN);
	items[5]->SetRelation(items[2], InterfaceElement::Directions::DOWN);
}

bool ItemSelecScene::AllPlayersReady() {
	
	bool ret = true;
	for (int i = 0; i < players.size(); i++) {
		if (players[i].locked == 2) {
			players[i].ready = true;
		}
	}
	for (int i = 0; i < players.size(); i++) {
		bool ret2 = players[i].ready;
		ret &= ret2;
	}
	
	return ret;//
}

void ItemSelecScene::AddLabelToButton(Item* item) {
	int fontSize = 27;
	int offset = item->butt->rect.h + (item->butt->rect.h * 0.2f);
	Label* label1 = App->gui->AddLabel(item->butt->rect.w / 2, offset, 35, DEFAULT_FONT, { 255,255,255,255 });
	label1->setString(item->name);
	item->labels.push_back(label1 );
	int i = 1;
	
	if (item->stats.hpRecover == true) {
		Label* label2 = App->gui->AddLabel(item->butt->rect.w / 2, offset + i*fontSize, fontSize, DEFAULT_FONT, { 218,165,32,255 });
		label2->setString(std::string("Recovers 5\% Health each 5 seconds"));
		item->labels.push_back(label2);
		i++;
	}
	if (item->stats.atk > 0) {
		Label* label3 = App->gui->AddLabel(item->butt->rect.w / 2, offset + i*fontSize, fontSize, DEFAULT_FONT, { 218,165,32,255 });
		label3->setString("+" + std::to_string(item->stats.atk) + "\% Attack");
		item->labels.push_back(label3);
		i++;
	}
	if (item->stats.def > 0) {
		Label* label4 = App->gui->AddLabel(item->butt->rect.w / 2, offset + i*fontSize, fontSize, DEFAULT_FONT, { 218,165,32,255 });
		label4->setString("+" + std::to_string(item->stats.def) + "\% Defense");
		item->labels.push_back(label4);
		i++;
	}
	if (item->stats.spd > 0) {
		Label* label5 = App->gui->AddLabel(item->butt->rect.w / 2, offset + i*fontSize, fontSize, DEFAULT_FONT, { 218,165,32,255 });
		label5->setString("+" + std::to_string(item->stats.spd) + "\% Speed");
		item->labels.push_back(label5);
		i++;
	}
	if (item->stats.cdr > 0) {
		Label* label6 = App->gui->AddLabel(item->butt->rect.w / 2, offset + i*fontSize, fontSize, DEFAULT_FONT, { 218,165,32,255 });
		label6->setString("+" + std::to_string(item->stats.cdr) + "\% Cooldown Reduction"); //COOLDOWN REDUCTION
		item->labels.push_back(label6);
		i++;
	}
	if (item->stats.ccr > 0) {
		Label* label7 = App->gui->AddLabel(item->butt->rect.w / 2, offset + i * fontSize, fontSize, DEFAULT_FONT, { 218,165,32,255 });
		label7->setString("+" + std::to_string(item->stats.ccr) + "\% Resistance to Crowd control"); //RESISTANCE TO CROWD CONTROL
		item->labels.push_back(label7);
		i++;
	}
	for (std::vector<Label*>::iterator lab = item->labels.begin(); lab != item->labels.end(); lab++) {
	
		(*lab)->SetParent(item->butt);
	}
	
}

void ItemSelecScene::SetControllerFocus() {

	uiPoint sizeScreen = App->gui->GetGuiSize();
	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}

	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		if (controllersNum != 4)
			return;
		for (int i = 1; i < 5; i++) {
			Player player;
			player.focusedItem = items[0];

			int marginBetweenNames = sizeScreen.x * 0.20f;

			player.playerNum = i;
			player.miniatureItemsFrame = App->gui->AddSprite((sizeScreen.x * ((i - 1) * 2 + 1)) / 8, sizeScreen.y * 7 / 8 + 15, nullptr, { 4500,43000,392,150 });//only used for having empty sprite
			player.playerName = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 255,255,255,255 });
			player.playerName->SetParent(player.miniatureItemsFrame);
			player.playerName->setPosition(player.miniatureItemsFrame->rect.w / 2, 15);
			player.playerName->setString("Player " + std::to_string(player.playerNum));

			player.totalControllersNum = 4;
			player.LoadArrows();
			players.push_back(player);
		}
	}

	else if(App->scenes->gameMode == GameMode::ONEvsONE) {
		if (controllersNum < 2)
			return;
		for (int i = 1; i < 3; i++) {
			Player player;
			player.focusedItem = items[0];

			int marginBetweenNames = sizeScreen.x * 0.20f;

			player.playerNum = i;
			player.miniatureItemsFrame = App->gui->AddSprite((sizeScreen.x * i / 3), sizeScreen.y * 7 / 8 + 15, nullptr, { 4500,43000,392,150 });//only used for having empty sprite
			player.playerName = App->gui->AddLabel(0, 0, 30, DEFAULT_FONT, { 255,255,255,255 });
			player.playerName->SetParent(player.miniatureItemsFrame);
			player.playerName->setPosition(player.miniatureItemsFrame->rect.w / 2, 15);
			player.playerName->setString("Player " + std::to_string(player.playerNum));

			player.totalControllersNum = 2;
			player.LoadArrows();
			players.push_back(player);
		}
	}

}

void ItemSelecScene::ChooseFocus() {

	uiPoint sizeScreen = App->gui->GetGuiSize();
	int initialDisplacement = sizeScreen.x * 0.1f;
	int marginBetweenItemFrames = sizeScreen.x * 0.005f;
	int marginBetweenPlayerFrames = sizeScreen.x * 0.085f;

	for (int i = 0; i < players.size(); i++) {
		if (players[i].ready) {
			players[i].arrow->Enable(false);
			continue;
		}
		if (App->input->GetButtonFromController(players[i].playerNum) == Input::BUTTON_A ) {
			
			Item* item = players[i].focusedItem;
			players[i].playerItems[players[i].locked] = item;
			players[i].miniatureItems[players[i].locked] = App->gui->AddSprite(-300, 0, nullptr, item->lockedRect);
			players[i].miniatureItems[players[i].locked]->SetParent(players[i].miniatureItemsFrame);
			players[i].miniatureItems[players[i].locked]->setPosition((players[i].miniatureItemsFrame->rect.w * (1 + players[i].locked) / 3), players[i].miniatureItemsFrame->rect.h * 2 / 3);
			players[i].LockedArrow(players[i].locked);
			players[i].locked++;
			FindFirstFreeItem(i);
		}		
	}
}
void ItemSelecScene::RemoveSelectedItem() {

	for (int i = 0; i < players.size(); i++) {
		Player* player = &players[i];
		
		if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_B) {			
			if (player->locked > 0) {
				if (player->ready) {
					player->ready = false;
				}
				player->locked--;
				player->playerItems[player->locked] = nullptr;
				App->gui->RemoveElement(player->miniatureItems[player->locked]);
				player->miniatureItems[player->locked] = nullptr;				
				player->RemoveLockedArrow(player->locked);

			}
		}
	}
}

void ItemSelecScene::ManageDisplacementFocus() {
	for (int i = 0; i < players.size(); i++) {
		if (players[i].ready) {
			continue;
		}
		if (App->input->GetButtonFromController(players[i].playerNum, false) == Input::DOWN) {

			FindNextArrowUnlocked(i, InterfaceElement::Directions::DOWN);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum, false) == Input::UP) {
			FindNextArrowUnlocked(i, InterfaceElement::Directions::UP);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum, false) == Input::LEFT) {
			FindNextArrowUnlocked(i, InterfaceElement::Directions::LEFT);
		}
		else if (App->input->GetButtonFromController(players[i].playerNum, false) == Input::RIGHT) {
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
		arrowRect = { 450, 1672, 42,27 }; // Red
		arrowLockRect = { 1082, 129, 51,45 };
		break;
	case 2:
		arrowRect = { 492,1726, 42,27 }; // Green
		arrowLockRect = { 1133, 129, 51,45 };
		break;
	case 3:
		arrowRect = { 492,1753, 42,27 }; //Blue
		arrowLockRect = { 1184, 129, 51,45 };
		break;
	case 4:
		arrowRect = { 450, 1807, 42,27 }; // Yellow (should be gray)
		arrowLockRect = { 1235, 129, 51,45 };
		break;
	}
	arrow = App->gui->AddSprite(focusedItem->butt->getPositionX(), focusedItem->butt->getPositionY() - focusedItem->butt->rect.h/2,nullptr, arrowRect);
	DrawOrderedArrow();
}

void ItemSelecScene::Player::DrawOrderedArrow() {		

	switch (playerNum) {
	case 0: return;
	case 1:
		arrow->SetAnchor(0, 1);
		arrow->setPosition(focusedItem->butt->getPositionX() - focusedItem->butt->rect.w / 2, focusedItem->butt->getPositionY() + focusedItem->butt->rect.h / 2);
		break;
	case 2:
		arrow->SetAnchor(1, 1);
		arrow->setPosition(focusedItem->butt->getPositionX() + focusedItem->butt->rect.w / 2, focusedItem->butt->getPositionY() + focusedItem->butt->rect.h / 2);
		break;
	case 3:
		arrow->SetAnchor(1, 0);
		arrow->setPosition(focusedItem->butt->getPositionX() + focusedItem->butt->rect.w / 2, focusedItem->butt->getPositionY() - focusedItem->butt->rect.h / 2);
		break;
	case 4:
		arrow->SetAnchor(0, 0);
		arrow->setPosition(focusedItem->butt->getPositionX() - focusedItem->butt->rect.w / 2, focusedItem->butt->getPositionY() - focusedItem->butt->rect.h / 2);
		break;
	}
}

void ItemSelecScene::FindFirstFreeItem(uint playerNum) {
	Player* player = &players[playerNum];
	if (player->locked == MAX_CHARACTER_ITEMS) {
		return;
	}
	for (int i = 0; i < 6; i++) {
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



void ItemSelecScene::Player::LockedArrow(uint lockedNum) {
	int distance = focusedItem->butt->rect.w / (totalControllersNum+1);

	int x = focusedItem->butt->getPositionX() - (focusedItem->butt->rect.w/2) + (distance * playerNum);
	lockedArrows[lockedNum - 1] = App->gui->AddSprite(x, focusedItem->butt->getPositionY() - focusedItem->butt->rect.h / 2, nullptr, arrowLockRect);

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

