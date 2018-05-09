#include "MainScene.h"
#include "IntroScene.h"
#include "../../Engine/Warrior.h"
#include "EndScene.h"
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
#include "../../Engine/ModuleWindow.h"


MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

bool MainScene::Start()
{
	App->audio->PlayMusic("Assets/Audio/BGM/Level1.ogg");

	App->map->Load("map2small.tmx");

	atlas = App->textures->Load("UI/atlas.png");



	e = App->entities->CreateCharacter({WARRIOR,{100, 0 ,100}, Team::BLUE});
	e2 = App->entities->CreateCharacter({ WARRIOR,{ 10000, 0, 100 }, Team::RED });

	App->gui->AddHealthbar((Character*)e, 0, true, 10, 10, atlas, true, { 451, 271, 264, 26 });
	App->gui->AddHealthbar((Character*)e2, 1, false, 1590, 10, atlas, true, { 451, 271, 264, 26 });


	if (App->scenes->gameMode==GameMode::TWOvsTWO)
	{
		e3 = App->entities->CreateCharacter({ WARRIOR,{ 100,1000 }, Team::BLUE });
		e4 = App->entities->CreateCharacter({ WARRIOR,{ 10000,1000 }, Team::RED });
		App->gui->AddHealthbar((Character*)e3, 2, true, 1590, 10, atlas, true, { 451, 271, 264, 26 });
		App->gui->AddHealthbar((Character*)e4, 3, false, 1590, 10, atlas, true, { 451, 271, 264, 26 });
	}


	return false;
}

bool MainScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN ||App->entities->finish) {
		App->input->BlockKeyboardEvent(SDL_SCANCODE_P);
		App->scenes->ChangeScene(App->scenes->endSc);
		App->entities->finish = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		paused = !paused;
		App->PauseGame(paused);
	}


	App->map->Draw();

	return true;
}

bool MainScene::PostUpdate()
{
	return true;
}

bool MainScene::CleanUp()
{
	xmlNode n;
	App->win->SetScale(1.0f);
	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		App->entities->DestroyEntity(e4);
		App->entities->DestroyEntity(e3);
	}
	App->entities->DestroyEntity(e2);
	App->entities->DestroyEntity(e);

	App->map->CleanUp(n);
	App->textures->UnLoad(atlas);
	App->collision->CleanUp(n);
	App->gui->CleanUp();
	return true;
}
