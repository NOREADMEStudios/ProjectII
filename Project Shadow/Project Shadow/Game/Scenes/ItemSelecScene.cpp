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
	
	App->debug = true;
	std::string statsStr = "Attack + ???";
	item1 = App->gui->AddButton(200, 200, NULL, { 0,0,200,200 }, true, item1PressCallb);
	item2 = App->gui->AddButton(800, 200, NULL, { 0,0,200,200 }, true, item2PressCallb);
	item3 = App->gui->AddButton (1400, 200, NULL, { 0,0,200,200 }, true, item3PressCallb);
	item1->OnHoverEnter = item1HoverEnCallb;
	item1->OnHoverExit = item1HoverExCallb;
	item2->OnHoverEnter = item2HoverEnCallb;
	item2->OnHoverExit = item2HoverExCallb;
	item3->OnHoverEnter = item3HoverEnCallb;
	item3->OnHoverExit = item3HoverExCallb;
	
	item1Stats = App->gui->AddLabel(item1->rect.w/2, item1->rect.h, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
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

	SDL_Texture * atlas = App->textures->Load("UI/ButtonsContinueExit.png");
	confirmButton = App->gui->AddButton(App->gui->GetGuiSize().x / 2, App->gui->GetGuiSize().y / 2, atlas, { 1, 72, 250, 61 }, false, confirmCallback);
	confirmLabel = App->gui->AddLabel(confirmButton->rect.w / 2, confirmButton->rect.h / 2, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string confirmStr = "CONFIRM";
	confirmLabel->setString(confirmStr);
	confirmLabel->SetParent(confirmButton);
	confirmLabel->culled = false;

	return false;
}

bool ItemSelecScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->introSc);
	}

	return true;
}

bool ItemSelecScene::CleanUp()
{
	xmlNode n;
	App->gui->CleanUp(n);

	return true;
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