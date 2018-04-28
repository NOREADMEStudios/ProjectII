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



	items[0]->butt->SetRelation(items[1]->butt, InterfaceElement::Directions::RIGHT);
	items[1]->butt->SetRelation(items[2]->butt, InterfaceElement::Directions::RIGHT);
	items[2]->butt->SetRelation(items[3]->butt, InterfaceElement::Directions::RIGHT);
	items[3]->butt->SetRelation(items[4]->butt, InterfaceElement::Directions::RIGHT);
	items[4]->butt->SetRelation(items[0]->butt, InterfaceElement::Directions::RIGHT);
	/*
	swiftBoots = App->gui->AddButton(sizeScreen.x / 4, sizeScreen.y / 3 + 50, atlas, { 451,492,130,130 }, true, item1PressCallb);
	cursedSword = App->gui->AddButton(sizeScreen.x / 2, sizeScreen.y / 3 + 50, atlas, { 581,492,130,130 }, true, item2PressCallb);
	paladinsHandguards = App->gui->AddButton(sizeScreen.x / 4 * 3, sizeScreen.y / 3 + 50, atlas, { 711,492,130,130 }, true, item3PressCallb);
	ringProtection = App->gui->AddButton(sizeScreen.x /  4, sizeScreen.y / 2 + 50, atlas, { 841,492,130,130 }, true, item4PressCallb);
	dragonSlayer = App->gui->AddButton(sizeScreen.x / 2, sizeScreen.y / 2 + 50, atlas, { 971,492,130,130 }, true, item5PressCallb);
	magicRobe = App->gui->AddButton(sizeScreen.x / 4 * 3, sizeScreen.y / 2 + 50, atlas, { 1101,492,130,130 }, true, item6PressCallb);
	swiftBoots->OnHoverEnter = item1HoverEnCallb;
	swiftBoots->OnHoverExit = item1HoverExCallb;
	cursedSword->OnHoverEnter = item2HoverEnCallb;
	cursedSword->OnHoverExit = item2HoverExCallb;
	paladinsHandguards->OnHoverEnter = item3HoverEnCallb;
	paladinsHandguards->OnHoverExit = item3HoverExCallb;

	buttonsForController.push_back(swiftBoots);
	buttonsForController.push_back(cursedSword);
	buttonsForController.push_back(paladinsHandguards);
	buttonsForController.push_back(ringProtection);
	buttonsForController.push_back(dragonSlayer);
	buttonsForController.push_back(magicRobe);

	item1Stats = App->gui->AddLabel(swiftBoots->rect.w / 2, swiftBoots->rect.h, 30, DEFAULT_FONT, { 255, 255, 255, 255 });
	item1Stats->setString(swiftBootsStr);
	item1Stats->SetParent(swiftBoots);
	item1Stats->SetAnchor(0.5f, 0);
	item1Stats->culled = false;
	item2Stats = App->gui->AddLabel(swiftBoots->rect.w / 2, swiftBoots->rect.h, 30, DEFAULT_FONT, { 255, 255, 255, 255 });
	item2Stats->setString(cursedSwordStr);
	item2Stats->SetParent(cursedSword);
	item2Stats->SetAnchor(0.5f, 0);
	item2Stats->culled = false;
	item3Stats = App->gui->AddLabel(swiftBoots->rect.w / 2, swiftBoots->rect.h, 30, DEFAULT_FONT, { 255, 255, 255, 255 });
	item3Stats->setString(paladinsStr);
	item3Stats->SetParent(paladinsHandguards);
	item3Stats->SetAnchor(0.5f, 0);
	item3Stats->culled = false;
	item4Stats = App->gui->AddLabel(swiftBoots->rect.w / 2, swiftBoots->rect.h, 30, DEFAULT_FONT, { 255, 255, 255, 255 });
	item4Stats->setString(ringStr);
	item4Stats->SetParent(ringProtection);
	item4Stats->SetAnchor(0.5f, 0);
	item4Stats->culled = false;
	item5Stats = App->gui->AddLabel(swiftBoots->rect.w / 2, swiftBoots->rect.h, 30, DEFAULT_FONT, { 255, 255, 255, 255 });
	item5Stats->setString(dragonSlayerStr);
	item5Stats->SetParent(dragonSlayer);
	item5Stats->SetAnchor(0.5f, 0);
	item5Stats->culled = false;
	item6Stats = App->gui->AddLabel(swiftBoots->rect.w / 2, swiftBoots->rect.h, 30, DEFAULT_FONT, { 255, 255, 255, 255 });
	item6Stats->setString(magicRobeStr);
	item6Stats->SetParent(magicRobe);
	item6Stats->SetAnchor(0.5f, 0);
	item6Stats->culled = false;*/
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

	
	Button* butt = (Button*)App->gui->getFocusedItem();

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
			
			players[i].locked++;
			players[i].playerItems[players[i].locked] = players[i].focusedItem;
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

			InterfaceElement* elem = players[i].focusedItem->butt->GetRelativeElement(InterfaceElement::Directions::DOWN);			
			if (elem != nullptr) {
				players[i].focusedItem->butt = (Button*)elem;
				players[i].DrawOrderedArrow();
				App->gui->setFocus(elem);
			}
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == CharInput::CH_UP) {
			InterfaceElement* elem = players[i].focusedItem->butt->GetRelativeElement(InterfaceElement::Directions::UP);
			if (elem != nullptr) {
				players[i].focusedItem->butt = (Button*)elem;
				players[i].DrawOrderedArrow();
				App->gui->setFocus(elem);
			}
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == CharInput::CH_LEFT) {
			InterfaceElement* elem = players[i].focusedItem->butt->GetRelativeElement(InterfaceElement::Directions::LEFT);						if (elem != nullptr)
				if (elem != nullptr) {
					players[i].focusedItem->butt = (Button*)elem;
					players[i].DrawOrderedArrow();
					App->gui->setFocus(elem);
				}
		}
		else if (App->input->GetButtonFromController(players[i].playerNum) == CharInput::CH_RIGHT) {
			InterfaceElement* elem = players[i].focusedItem->butt->GetRelativeElement(InterfaceElement::Directions::RIGHT);						if (elem != nullptr)
				if (elem != nullptr) {
					players[i].focusedItem->butt = (Button*)elem;
					players[i].DrawOrderedArrow();
					App->gui->setFocus(elem);
					
				}
		}
		
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