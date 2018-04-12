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

	App->map->Load("map2small.tmx");

	t = App->textures->Load("UI/HealthBars.png");
	SDL_Texture* f = App->textures->Load("UI/FaceArt.png");
	Sprite* _bar = App->gui->AddSprite(0, 0, t, { 0, 26, 258, 20 });
	Sprite* _bar2 = App->gui->AddSprite(0, 0, t, { 0, 26, 258, 20 });
	Sprite* charFace = App->gui->AddSprite(10, 35, f, { 0, 0, 128, 64 });
	Sprite* charFace2 = App->gui->AddSprite(10, 35, f, { 0, 0, 128, 64 });
	Label* charNumber = App->gui->AddLabel(0, 0, 2, "UI/TTF/Vecna Bold.ttf", { 255, 255, 255, 255 }, Label::BLENDED, "hola");

	e = App->entities->CreateCharacter({HERO,{100,100}});
	e2 = App->entities->CreateCharacter({ HERO,{ 100,50 } });
	e3 = App->entities->CreateCharacter({ HERO,{ 50,50 } });
	e4= App->entities->CreateCharacter({ HERO,{ 50,100 } });

	App->gui->AddHealthbar((Hero*)e, 0, _bar, charFace, charNumber, true, 10, 10, t, true, { 0, 0, 264, 26 });
	App->gui->AddHealthbar((Hero*)e2, 1, _bar2, charFace2, charNumber, false, 1590, 10, t, true, { 0, 0, 264, 26 });

	App->debug = true;
	t = App->textures->Load("Maps/map2_spritesheet.png");
  
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
	App->entities->DestroyEntity(e4);
	App->entities->DestroyEntity(e3);	
	App->entities->DestroyEntity(e2);
	App->entities->DestroyEntity(e);

	App->map->CleanUp(n);
	App->textures->UnLoad(t);
	App->collision->CleanUp(n);
	App->gui->CleanUp(n);
	return true;
}
