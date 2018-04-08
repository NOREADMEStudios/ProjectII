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

void CallbackButton(...);

ItemSelecScene::ItemSelecScene()
{
}

ItemSelecScene::~ItemSelecScene()
{
}

bool ItemSelecScene::Start()
{
	App->debug = true;
	item1 = App->gui->AddButton(200, 200, NULL, { 0,0,200,200 }, true, CallbackButton);
	item2 = App->gui->AddButton(800, 200, NULL, { 0,0,200,200 }, true, CallbackButton);
	item3 = App->gui->AddButton (1400, 200, NULL, { 0,0,200,200 }, true, CallbackButton);
	//Label* l = App->gui->AddLabel()


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
	return true;
}

void CallbackButton(...) {
	LOG("PRESSED");
}