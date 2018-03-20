#include "TestScene.h"
#include "../Engine/ModuleMap.h"
#include "../Engine/App.h"
#include "../Engine/ModuleSceneManager.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

bool TestScene::Start()
{
	App->map->Load("map.tmx");
	return false;
}

bool TestScene::Update(float dt)
{
	App->map->Draw();
	return true;
}
