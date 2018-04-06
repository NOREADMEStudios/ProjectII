#include "MainScene.h"
#include "IntroScene.h"
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



MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

bool MainScene::Start()
{

	App->map->Load("map.tmx");


	e = App->entities->CreateCharacter({HERO,{100,100}});
	e2 = App->entities->CreateCharacter({ ENEMY,{ 100,50 } });
	//App->entities->CreateEntity({ CHARACTER,HERO,{ 100,0 } });

	App->debug = true;
	t = App->textures->Load("Maps/map2_spritesheet.png");
  
	/*Window* w = App->gui->AddWindow(500, 500, t, {0,0,500,500}, true);
	Window* s = App->gui->AddWindow(0, 0, t, { 350, 520, 300, 300 }, true);
	s->SetParent(w);
	s->culled = true;
	w->SetContentRect(50, 50, 50, 50);*/
  
	return false;
}

bool MainScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->input->BlockKeyboardEvent(SDL_SCANCODE_P);
		App->scenes->ChangeScene(App->scenes->introSc);
	}
	//App->map->Draw();
	return true;
}

bool MainScene::PostUpdate()
{
	return true;
}

bool MainScene::CleanUp()
{
	pugi::xml_node n;
	App->entities->DestroyEntity(e);
	App->entities->DestroyEntity(e2);
	App->map->CleanUp(n);
	App->textures->UnLoad(t);

	return true;
}
