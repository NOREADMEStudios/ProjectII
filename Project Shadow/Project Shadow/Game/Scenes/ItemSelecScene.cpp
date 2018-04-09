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


ItemSelecScene::ItemSelecScene()
{
}

ItemSelecScene::~ItemSelecScene()
{
}

bool ItemSelecScene::Start()
{
	
	App->debug = true;
	const char * path  = "Assets/Textures/UI/TTF/Vecna.ttf";
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
	item1Stats = App->gui->AddLabel(0, 200, 50, path, { 255, 255, 255, 255 });
	item1Stats->setString(statsStr);
	item1Stats->SetParent(item1);
	item1Stats->SetAnchor(0, 0);
	item1Stats->culled = false;
	item2Stats = App->gui->AddLabel(0, 200, 50, path, { 255, 255, 255, 255 });
	item2Stats->setString(statsStr);
	item2Stats->SetParent(item2);
	item2Stats->SetAnchor(0, 0);
	item2Stats->culled = false;
	item3Stats = App->gui->AddLabel(0, 200, 50, path, { 255, 255, 255, 255 });
	item3Stats->setString(statsStr);
	item3Stats->SetParent(item3);
	item3Stats->SetAnchor(0, 0);
	item3Stats->culled = false;

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
	App->gui->RemoveElement(item1);
	App->gui->RemoveElement(item2);
	App->gui->RemoveElement(item3);
	App->gui->RemoveElement(item1Stats);

	//Utils::Release(item1Stats);
	Utils::Release(item1);
	Utils::Release(item2);
	Utils::Release(item3);
	
	return true;
}

void item1PressCallb(size_t arg_size...) {
	LOG("PRESSED");
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item1HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	b->getLabel()->Enable(true);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item1HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	b->getLabel()->Enable(false);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item2PressCallb(size_t arg_size...) {
	LOG("PRESSED");
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item2HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	b->getLabel()->Enable(true);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item2HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	b->getLabel()->Enable(false);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item3PressCallb(size_t arg_size...) {
	LOG("PRESSED");
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item3HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	b->getLabel()->Enable(true);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}

void item3HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
	va_list args;
	va_start(args, arg_size);

	Button* b = va_arg(args, Button*);

	b->getLabel()->Enable(false);

	va_end(args);
	//App->scenes->ChangeScene(App->scenes->mainSc);
}