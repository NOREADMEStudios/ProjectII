#include "MainScene.h"
#include "IntroScene.h"
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


	e = App->entities->CreateCharacter({HERO,{100,100}});
	e2 = App->entities->CreateCharacter({ HERO,{ 100,50 } });
	e3 = App->entities->CreateCharacter({ HERO,{ 50,50 } });
	e4= App->entities->CreateCharacter({ HERO,{ 50,100 } });
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
		App->scenes->ChangeScene(App->scenes->endSc);
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
	App->entities->DestroyEntity(e);
	App->entities->DestroyEntity(e2);	
	App->map->CleanUp(n);
	App->textures->UnLoad(t);

	App->gui->CleanUp(n);


	return true;
}
