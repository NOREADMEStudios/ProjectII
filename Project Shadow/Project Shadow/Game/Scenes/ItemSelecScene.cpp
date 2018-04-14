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
		ApplyItemAttributes();
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

	std::string statsStr = "Attack + ???";
	item1 = App->gui->AddButton(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 4, atlas, { 0,0,200,200 }, true, item1PressCallb, { 50,270,384,186 }, { 50,491,384,186 });
	item2 = App->gui->AddButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, atlas, { 0,0,200,200 }, true, item2PressCallb, { 50,270,384,186 }, { 50,491,384,186 });
	item3 = App->gui->AddButton(SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT / 4, atlas, { 0,0,200,200 }, true, item3PressCallb, { 50,270,384,186 }, { 50,491,384,186 });
	item1->OnHoverEnter = item1HoverEnCallb;
	item1->OnHoverExit = item1HoverExCallb;
	item2->OnHoverEnter = item2HoverEnCallb;
	item2->OnHoverExit = item2HoverExCallb;
	item3->OnHoverEnter = item3HoverEnCallb;
	item3->OnHoverExit = item3HoverExCallb;

	buttons.push_back(item1);
	buttons.push_back(item2);
	buttons.push_back(item3);

	item1Stats = App->gui->AddLabel(item1->rect.w / 2, item1->rect.h, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	item1Stats->Enable(false);
	item1Stats->setString(statsStr);
	item1Stats->SetParent(item1);
	item1Stats->SetAnchor(0.5f, 0);
	item1Stats->culled = false;
	item2Stats = App->gui->AddLabel(item1->rect.w / 2, item1->rect.h, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	item2Stats->Enable(false);
	item2Stats->setString(statsStr);
	item2Stats->SetParent(item2);
	item2Stats->SetAnchor(0.5f, 0);
	item2Stats->culled = false;
	item3Stats = App->gui->AddLabel(item1->rect.w / 2, item1->rect.h, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	item3Stats->Enable(false);
	item3Stats->setString(statsStr);
	item3Stats->SetParent(item3);
	item3Stats->SetAnchor(0.5f, 0);
	item3Stats->culled = false;

	
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
	
	Button* butt = *(buttons.begin());

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
			// PUT HERE THE RESULT OF PRESSING A ITEM NORMAN&JOEL



		}
	}
}

void ItemSelecScene::ManageDisplacementFocus() {

	for (std::list<Selection>::iterator foc = playersSelections.begin(); foc != playersSelections.end(); foc++) {
		if (!(*foc).locked) {
			if (App->input->GetButtonFromController((*foc).playerNum) == Input::RIGHT) {
				for (std::list<Button*>::iterator button = buttons.begin(); button != buttons.end(); button++) {
					if (*button == (*foc).but) {
						button++;
						if (button != buttons.end()) {
							(*foc).but = (*button);
							button--;
							(*foc).DrawOrderedArrow();

						}
						return;
					}
				}
			}

			else if (App->input->GetButtonFromController((*foc).playerNum) == Input::LEFT) {
				for (std::list<Button*>::iterator button = buttons.begin(); button != buttons.end(); button++) {
					if (!(*foc).locked) {
						if (*button == (*foc).but) {
							if (button != buttons.begin()) {
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
	
	App->scenes->itemSc->confirmButton->Enable(true);
	App->scenes->itemSc->confirmLabel->Enable(true);

	if (App->scenes->itemSc->item2->pressed || App->scenes->itemSc->item3->pressed) {
		App->scenes->itemSc->item2->pressed = false;
		App->scenes->itemSc->item2Stats->Enable(false);
		App->scenes->itemSc->item3->pressed = false;
		App->scenes->itemSc->item3Stats->Enable(false);
	}
	va_list args;
	va_start(args, arg_size);

	Button* id = va_arg(args, Button*);
	id->pressed = true;
}

void item1HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);
	if (!b->pressed)
		b->getLabel()->Enable(true);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item1HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	if (!b->pressed)
		b->getLabel()->Enable(false);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item2PressCallb(size_t arg_size...) {
	LOG("PRESSED");
	App->scenes->itemSc->confirmButton->Enable(true);
	App->scenes->itemSc->confirmLabel->Enable(true);

	va_list args;
	va_start(args, arg_size);

	Button* id = va_arg(args, Button*);
	id->pressed = true;

	if (App->scenes->itemSc->item1->pressed || App->scenes->itemSc->item3->pressed) {
		App->scenes->itemSc->item1->pressed = false;
		App->scenes->itemSc->item1Stats->Enable(false);
		App->scenes->itemSc->item3->pressed = false;
		App->scenes->itemSc->item3Stats->Enable(false);
	}

}

void item2HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);
	if (!b->pressed)
		b->getLabel()->Enable(true);

	va_end(args);
}

void item2HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);
	if (!b->pressed)
		b->getLabel()->Enable(false);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item3PressCallb(size_t arg_size...) {
	LOG("PRESSED");
	App->scenes->itemSc->confirmButton->Enable(true);
	App->scenes->itemSc->confirmLabel->Enable(true);

	va_list args;
	va_start(args, arg_size);

	Button* id = va_arg(args, Button*);
	id->pressed = true;

	if (App->scenes->itemSc->item1->pressed || App->scenes->itemSc->item2->pressed) {
		App->scenes->itemSc->item1->pressed = false;
		App->scenes->itemSc->item1Stats->Enable(false);
		App->scenes->itemSc->item2->pressed = false;
		App->scenes->itemSc->item2Stats->Enable(false);
	}

}

void item3HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	if (!b->pressed)
		b->getLabel()->Enable(true);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item3HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	if (!b->pressed)
		b->getLabel()->Enable(false);

	va_end(args);
}

void confirmCallback(size_t arg_size...) {
	LOG("BUTTON CONFIRM PRESSED");
	//HERE THE ITEMS WILL BE ADDED IF ITEM ? IS PRESSED
	App->scenes->ChangeScene(App->scenes->mainSc);
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


}