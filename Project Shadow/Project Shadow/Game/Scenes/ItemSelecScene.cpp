#include "ItemSelecScene.h"
#include "EndScene.h"
#include "IntroScene.h"
#include "MainScene.h"
#include "../../Engine/ModuleMap.h"
#include "../../Engine/App.h"
#include "../../Engine/ModuleSceneManager.h"
#include "../../Engine/ModuleEntityManager.h"
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

void item1PressCallb(size_t arg_size...);
void item1HoverEnCallb(size_t arg_size...);
void item1HoverExCallb(size_t arg_size...);
void item2PressCallb(size_t arg_size...);
void item2HoverEnCallb(size_t arg_size...);
void item2HoverExCallb(size_t arg_size...);
void item3PressCallb(size_t arg_size...);
void item3HoverEnCallb(size_t arg_size...);
void item3HoverExCallb(size_t arg_size...);
void item4PressCallb(size_t arg_size...);
void item4HoverEnCallb(size_t arg_size...);
void item4HoverExCallb(size_t arg_size...);
void item5PressCallb(size_t arg_size...);
void item5HoverEnCallb(size_t arg_size...);
void item5HoverExCallb(size_t arg_size...);
void item6PressCallb(size_t arg_size...);
void item6HoverEnCallb(size_t arg_size...);
void item6HoverExCallb(size_t arg_size...);
void confirmCallback(size_t arg_size...);

ItemSelecScene::ItemSelecScene()
{
}

ItemSelecScene::~ItemSelecScene()
{
}

bool ItemSelecScene::Start()
{
	LoadBackground("UI/BasicMenuScene.png");
	
	
	App->debug = true;
	
	LoadSceneUI();
	
	SetControllerFocus();

	return false;
}

bool ItemSelecScene::Update(float dt)
{
	if (AllItemsSelected()) {//ALL ARROWS LOCKED 
		ApplyItemAttributes();						 // NEED TO FILL 
		App->scenes->ChangeScene(App->scenes->mainSc);
	}


	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->introSc);
	}
	DrawBackground();
	
	if (controllersNum != 0) {
		ManageDisplacementFocus();
		ChooseFocus();
		
	}

	return true;
}

bool ItemSelecScene::CleanUp()
{
	xmlNode n;
	
	App->gui->CleanUp(n);
	UnLoadBackground();
	return true;
}

void ItemSelecScene::LoadSceneUI() {

	atlas = App->textures->Load("UI/atlas.png");
	items_atlas = App->textures->Load("UI/items.png");
	uiPoint sizeScreen = App->gui->GetGuiSize();
	
	swiftBoots = App->gui->AddButton(sizeScreen.x / 4, sizeScreen.y / 3 + 50, items_atlas, { 0,0,120,120 }, true, item1PressCallb);
	cursedSword = App->gui->AddButton(sizeScreen.x / 2, sizeScreen.y / 3 + 50, items_atlas, { 120,0,120,120 }, true, item2PressCallb);
	paladinsHandguards = App->gui->AddButton(sizeScreen.x / 4 * 3, sizeScreen.y / 3 + 50, items_atlas, { 240,0,120,120 }, true, item3PressCallb);
	ringProtection = App->gui->AddButton(sizeScreen.x /  4, sizeScreen.y / 2 + 50, items_atlas, { 360,0,120,120 }, true, item4PressCallb);
	dragonSlayer = App->gui->AddButton(sizeScreen.x / 2, sizeScreen.y / 2 + 50, items_atlas, { 480,0,120,120 }, true, item5PressCallb);
	magicRobe = App->gui->AddButton(sizeScreen.x / 4 * 3, sizeScreen.y / 2 + 50, items_atlas, { 600,0,120,120 }, true, item6PressCallb);
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
	item6Stats->culled = false;
}

bool ItemSelecScene::AllItemsSelected() {

	bool ret = true;
	for (std::list<Selection>::iterator focus = playersSelections.begin(); focus != playersSelections.end(); focus++) {
		bool ret2 = (*focus).locked;
		ret &= ret2;
	}

	return ret;
}

void ItemSelecScene::SetControllerFocus() {

	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}
	
	Button* butt = *(buttonsForController.begin());

	for (int i = 1; i <= controllersNum; i++) {
	
	Selection focus;		
	focus.but = butt;
	focus.playerNum = i;
	focus.totalControllersNum = controllersNum;
	focus.LoadArrows(atlas);
	
	
	playersSelections.push_back(focus);
	}
}




void ItemSelecScene::ChooseFocus() {

	for (std::list<Selection>::iterator focus = playersSelections.begin(); focus != playersSelections.end(); focus++) {
		if (App->input->GetButtonFromController((*focus).playerNum) == Input::JUMPINPUT) {
			LOG("");
			(*focus).arrow->ChangeAnimation((*focus).arrowLockRect);
			(*focus).locked = true;			
		}
	}
}

void ItemSelecScene::ManageDisplacementFocus() {

	for (std::list<Selection>::iterator foc = playersSelections.begin(); foc != playersSelections.end(); foc++) {
		if (!(*foc).locked) {
			if (App->input->GetButtonFromController((*foc).playerNum) == Input::RIGHT) {
				for (std::list<Button*>::iterator button = buttonsForController.begin(); button != buttonsForController.end(); button++) {
					if (*button == (*foc).but) {
						button++;
						if (button != buttonsForController.end()) {
							(*foc).but = (*button);
							button--;
							(*foc).DrawOrderedArrow();

						}
						return;
					}
				}
			}

			else if (App->input->GetButtonFromController((*foc).playerNum) == Input::LEFT) {
				for (std::list<Button*>::iterator button = buttonsForController.begin(); button != buttonsForController.end(); button++) {
					if (!(*foc).locked) {
						if (*button == (*foc).but) {
							if (button != buttonsForController.begin()) {
								button--;
								(*foc).but = (*button);
								button++;
								(*foc).arrow->setPosition((*foc).but->getPositionX(), (*foc).but->getPositionY() - (*foc).but->rect.h / 2);
								(*foc).DrawOrderedArrow();

							}
							return;
						}
					}
				}
			}
		}
	}
}

void item1PressCallb(size_t arg_size...) {
	LOG("PRESSED");
}

void item1HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}

void item1HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}

void item2PressCallb(size_t arg_size...) {
	LOG("PRESSED");
}

void item2HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}

void item2HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}

void item3PressCallb(size_t arg_size...) {
	LOG("PRESSED");
}

void item3HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}

void item3HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}

void item4PressCallb(size_t arg_size...) {
	LOG("PRESSED");
}

void item4HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}

void item4HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}

void item5PressCallb(size_t arg_size...) {
	LOG("PRESSED");
}

void item5HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}

void item5HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}

void item6PressCallb(size_t arg_size...) {
	LOG("PRESSED");
}

void item6HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}

void item6HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}
void confirmCallback(size_t arg_size...) {
	LOG("BUTTON CONFIRM PRESSED");
	if (App->scenes->itemSc->enableMouse) { App->scenes->ChangeScene(App->scenes->mainSc); }
}

void Selection::LoadArrows(SDL_Texture* tex) {
	
	switch (playerNum) {
	case 0: return;
	case 1: 
		arrowRect = { 754, 50, 22,19 }; // Red
		arrowLockRect = { 754, 87, 23,21 };
		break;
	case 2:
		arrowRect = { 825, 50, 22,19 }; //Yellow 
		arrowLockRect = { 825, 87, 23,21 };
		break;
	case 3:
		arrowRect = { 778, 50, 22,19 }; //Blue
		arrowLockRect = { 778, 87, 23,21 };
		break;
	case 4:
		arrowRect = { 802, 50, 22,19 }; // Green
		arrowLockRect = { 802, 87, 23,21 };
		break;
	}
	
	arrow = App->gui->AddSprite(but->getPositionX(), but->getPositionY() - but->rect.h/2,tex, arrowRect);
	DrawOrderedArrow();
}

void Selection::DrawOrderedArrow() {
	
	
	int distance = but->rect.w / (totalControllersNum+1);
	 
	int x = but->getPositionX() - (but->rect.w/2) + (distance * playerNum);
	arrow->setPosition(x, but->getPositionY() - but->rect.h / 2);
}

void ItemSelecScene::ApplyItemAttributes() {

	for (std::list<Selection>::iterator focus = playersSelections.begin(); focus != playersSelections.end(); focus++) {
		
		if ((*focus).but == swiftBoots) {
			//Apply the effect to the player num ( (*focus).playerNum  )
		
		}
		else if ((*focus).but == cursedSword) {
			//Apply the effect to the player num ( (*focus).playerNum  )

		}
		else if ((*focus).but == paladinsHandguards) {
			//Apply the effect to the player num ( (*focus).playerNum  )

		}
		else if ((*focus).but == ringProtection) {
			//Apply the effect to the player num ( (*focus).playerNum  )

		}
		else if ((*focus).but == dragonSlayer) {
			//Apply the effect to the player num ( (*focus).playerNum  )

		}
		else if ((*focus).but == magicRobe) {
			//Apply the effect to the player num ( (*focus).playerNum  )

		}
	}
}