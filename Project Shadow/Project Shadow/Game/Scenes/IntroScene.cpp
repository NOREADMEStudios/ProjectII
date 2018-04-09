#include "IntroScene.h"
#include "MainScene.h"
#include "ItemSelecScene.h"
#include "../../Engine/ModuleMap.h"
#include "../../Engine/App.h"
#include "../../Engine/ModuleSceneManager.h"
#include "../../Engine/ModuleEntityManager.h"
#include "../../Engine/ModuleGUI.h"
#include "../../Engine/ModuleTextures.h"
#include "../../Engine/ModuleInput.h"
#include "../../Engine/ModuleRender.h"
#include "../../Engine/UI/Window.h"
#include "../../Engine/ModuleCollision.h"
#include "../../Engine/ModuleAudio.h"


IntroScene::IntroScene()
{
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Start()
{
	App->audio->PlayMusic("Assets/Audio/BGM/Character_Selection.ogg");

	return false;
}

bool IntroScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->itemSc);
	}

	return true;
}

bool IntroScene::CleanUp()
{
	return true;
}


