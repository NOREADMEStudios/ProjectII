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

void item1Callback(...);
void item2Callback(...);
void item3Callback(...);

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
	item1 = App->gui->AddButton(200, 200, NULL, { 0,0,200,200 }, true, item1Callback);
	item2 = App->gui->AddButton(800, 200, NULL, { 0,0,200,200 }, true, item2Callback);
	item3 = App->gui->AddButton (1400, 200, NULL, { 0,0,200,200 }, true, item3Callback);
	itemStats = App->gui->AddLabel(500, 600, 50, path, { 255, 255, 255, 255 });
	itemStats->setString(statsStr);
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
	App->gui->RemoveElement(itemStats);

	Utils::Release(item1);
	Utils::Release(item2);
	Utils::Release(item3);
	Utils::Release(itemStats);

	return true;
}

void item1Callback(...) {
	LOG("PRESSED");
	App->scenes->ChangeScene(App->scenes->mainSc);
}

void item2Callback(...) {
	LOG("PRESSED");
	App->scenes->ChangeScene(App->scenes->mainSc);
}

void item3Callback(...) {
	LOG("PRESSED");
	App->scenes->ChangeScene(App->scenes->mainSc);
}