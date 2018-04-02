#include "TestScene.h"
#include "../Engine/ModuleMap.h"
#include "../Engine/App.h"
#include "../Engine/ModuleSceneManager.h"
#include "../Engine/ModuleEntityManager.h"
#include "../Engine/ModuleGUI.h"
#include "../Engine/ModuleTextures.h"
#include "../Engine/UI/Window.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

bool TestScene::Start()
{

	App->map->Load("map.tmx");


	App->entities->CreateEntity({ CHARACTER,{100,100} });
	App->entities->CreateEntity({ CHARACTER,{ 100,50 } });
	App->entities->CreateEntity({ CHARACTER,{ 100,0 } });

	//App->map->Load("map.tmx");
	App->debug = true;
	SDL_Texture* t = App->textures->Load("map2_spritesheet.png");
	//Window* w = App->gui->AddWindow(500, 500, t, {0,0,500,500}, true);
	//Window* s = App->gui->AddWindow(0, 0, t, { 350, 520, 300, 300 }, true);
	//s->SetParent(w);
	//s->culled = true;
	//w->SetContentRect(50, 50, 50, 50);

	return false;
}

bool TestScene::Update(float dt)
{
	App->map->Draw();
	return true;
}
