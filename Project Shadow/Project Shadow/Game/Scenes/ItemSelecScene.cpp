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
	SDL_Texture* bakc_items = App->textures->Load("UI/BasicMenuScene.png");
	App->gui->AddSprite(820, 540, bakc_items, { 0,0,1750,1080 }, true);
	
	App->debug = true;
	
	LoadSceneUI();
	
	SetControllerFocus();

	return false;
}

bool ItemSelecScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->introSc);
	}

	ManageDisplacementFocus();
	ChooseFocus();
	DrawArrows();

	return true;
}

bool ItemSelecScene::CleanUp()
{
	xmlNode n;
	App->gui->CleanUp(n);

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

	

	confirmButton = App->gui->AddButton(App->gui->GetGuiSize().x / 2, App->gui->GetGuiSize().y / 2, atlas, { 450, 50, 250, 61 }, false, confirmCallback, { 450, 120, 250, 61 }, { 450, 189, 250, 61 });

	

	confirmLabel = App->gui->AddLabel(confirmButton->rect.w / 2, confirmButton->rect.h / 2, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string confirmStr = "CONFIRM";
	confirmLabel->setString(confirmStr);
	confirmLabel->SetParent(confirmButton);
	confirmLabel->culled = false;
}

void ItemSelecScene::DrawArrows() {

	switch (playersFocus.size()) {
	case 0:
		break;
	case 1: 
		App->render->Blit(atlas, (*playersFocus.begin()).but->getPositionX(), (*playersFocus.begin()).but->getPositionY()-10, &(*playersFocus.begin()).arrow);
		break;
	}
}


void ItemSelecScene::SetControllerFocus() {

	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}
	Button* butt = *(buttons.begin());
	//App->gui->setFocus(butt);
	for (int i = 1; i <= controllersNum; i++) {
	
	Focus focus;		
	focus.but = butt;
	focus.playerNum = controllersNum;
	focus.LoadArrows();
	
	playersFocus.push_back(focus);
	}
}


void ItemSelecScene::ChooseFocus() {

	for (std::list<Focus>::iterator focus = playersFocus.begin(); focus != playersFocus.end(); focus++) {
		if (App->input->GetButtonFromController((*focus).playerNum) == Input::JUMPINPUT) {
			LOG("");
			
			// PUT HERE THE RESULT OF PRESSING A ITEM



		}
	}
}

void ItemSelecScene::ManageDisplacementFocus() {

	for (std::list<Focus>::iterator foc = playersFocus.begin(); foc != playersFocus.end(); foc++) {
		if(App->input->GetButtonFromController((*foc).playerNum) == Input::RIGHT){			
			for (std::list<Button*>::iterator button = buttons.begin(); button != buttons.end(); button++) {
				if (*button == (*foc).but) {
					button++;
					if (button != buttons.end()) {
						(*foc).but = (*button);
						button--;
						//App->gui->setFocus((*foc).but);
					}
					return;
				}
			}
		}
		else if (App->input->GetButtonFromController((*foc).playerNum) == Input::LEFT) {
			for (std::list<Button*>::iterator button = buttons.begin(); button != buttons.end(); button++) {
				if (*button == (*foc).but) {
					if (button != buttons.begin()) {
						button--;
						(*foc).but = (*button);
						button++;
						//App->gui->setFocus((*foc).but);
					}
					return;
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

void Focus::LoadArrows() {

	switch (playerNum) {
	case 1: 
		arrow = { 730, 50, 22,19 }; //Black
		break;
	case 2:
		arrow = { 754, 50, 22,19 }; // Red
		break;
	case 3:
		arrow = { 778, 50, 22,19 }; //Blue
		break;
	case 4:
		arrow = { 802, 50, 22,19 }; // Green
		break;
	}
}