#include "ModuleSceneManager.h"
#include "../Game/TestScene.h"



ModuleSceneManager::ModuleSceneManager()
{
	name = "scenes";
}


ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Awake(pugi::xml_node& n) {
	return true;
}

bool ModuleSceneManager::Start() {
	LoadScene(new TestScene());
	return true;
}

bool ModuleSceneManager::Update(float dt) {
	if (currentScene != nullptr)
		return currentScene->Update(dt);
	return true;
}

bool ModuleSceneManager::CleanUp(pugi::xml_node& n) {
	if (currentScene != nullptr)
		return currentScene->CleanUp(n);
	return true;
}

void ModuleSceneManager::LoadScene(Scene* scene) {
	currentScene = scene;
	currentScene->active = true;
	currentScene->Start();
}

void ModuleSceneManager::ChangeScene(Scene* scene_to_change) {
	
}

void ModuleSceneManager::UnloadScene(Scene* scene) {
	
}