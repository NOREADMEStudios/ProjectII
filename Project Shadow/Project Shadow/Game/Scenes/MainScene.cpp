#include "MainScene.h"
#include "IntroScene.h"
#include "../../Engine/Hero.h"
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



MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

bool MainScene::Start()
{
	App->audio->PlayMusic("Assets/Audio/BGM/Level1.ogg");

	App->map->Load("map.tmx");

	t = App->textures->Load("UI/HealthBars.png");
	Sprite* _bar = App->gui->AddSprite(0, 0, t, { 0, 26, 258, 20 });
	Sprite* _bar2 = App->gui->AddSprite(0, 0, t, { 0, 26, 258, 20 });

	e = App->entities->CreateCharacter({HERO,{100,100}});
	App->gui->AddHealthbar((Hero*)e, 0, _bar, true, 10, 10, t, true, { 0, 0, 264, 26 });
	e2 = App->entities->CreateCharacter({ HERO,{ 100,100 } });
	Entity* e3 =App->entities->CreateCharacter({ ENEMY,{ 100,50 } });
	App->gui->AddHealthbar((Hero*)e2, 1, _bar2, false, 1590, 10, t, true, { 0, 0, 264, 26 });

	App->debug = true;

	//Window* w = App->gui->AddWindow(500, 500, t, {0,0,500,500}, true);
	//Window* s = App->gui->AddWindow(0, 0, t, { 350, 520, 300, 300 }, true);
	//s->SetParent(w);
	//s->culled = true;
	//w->SetContentRect(50, 50, 50, 50);
  
	return false;
}

bool MainScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->input->BlockKeyboardEvent(SDL_SCANCODE_P);
		App->scenes->ChangeScene(App->scenes->endSc);
	}
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		paused = !paused;
		App->PauseGame(paused);
	}

	App->map->Draw();

	if (((Hero*)e)->currHP <= 0)
		((Hero*)e)->currHP = 100;

	if (((Hero*)e2)->currHP <= 0)
		((Hero*)e2)->currHP = 100;

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		((Hero*)e)->currHP -= 10;
		((Hero*)e2)->currHP -= 10;
	}

	return true;
}

bool MainScene::PostUpdate()
{
	return true;
}

bool MainScene::CleanUp()
{
	xmlNode n;
	App->entities->DestroyEntity(e);
	App->entities->DestroyEntity(e2);	
	App->map->CleanUp(n);
	App->textures->UnLoad(t);

	App->gui->CleanUp(n);


	return true;
}
