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
#include "../../Engine/ModuleCollision.h"


ItemSelecScene::ItemSelecScene()
{
	
}

ItemSelecScene::~ItemSelecScene()
{
}

bool ItemSelecScene::Start()
{
	Callback call;
	item1 = App->gui->AddButton(500, 500, NULL, { 0,0,500,500 }, true, call);
	item2 = App->gui->AddButton(100, 500, NULL, { 0,0,500,500 }, true, call);
	item3 = App->gui->AddButton(1500, 500, NULL, { 0,0,500,500 }, true, call);
	return true;
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
