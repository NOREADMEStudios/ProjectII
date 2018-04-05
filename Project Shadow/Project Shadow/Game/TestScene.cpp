#include "TestScene.h"
#include "../Engine/ModuleMap.h"
#include "../Engine/App.h"
#include "../Engine/ModuleSceneManager.h"
#include "../Engine/ModuleGUI.h"
#include "../Engine/ModuleTextures.h"
#include "../Engine/UI/Window.h"
#include "../Engine/ModuleCollision.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

bool TestScene::Start()
{
	//App->map->Load("map.tmx");
	App->debug = true;
	/*SDL_Texture* t = App->textures->Load("map2_spritesheet.png");
	Window* w = App->gui->AddWindow(500, 500, t, {0,0,500,500}, true);
	Window* s = App->gui->AddWindow(0, 0, t, { 350, 520, 300, 300 }, true);
	s->SetParent(w);
	s->culled = true;
	w->SetContentRect(50, 50, 50, 50);*/

	c.collider = { 100, 100, 50, 50 };
	c.type = Collider::TRIGGER;
	c.tag = 0;

	App->collision->AddCollider(&c, &e);

	c2.collider = { 100, 100, 50, 50 };
	c2.type = Collider::TRIGGER;
	c2.tag = 1;
	
	App->collision->AddCollider(&c2, &e2);


	return false;
}

bool TestScene::Update(float dt)
{
	App->map->Draw();
	c2.collider.x += 10 * sin(angle);
	angle += 0.01f;
	return true;
}
