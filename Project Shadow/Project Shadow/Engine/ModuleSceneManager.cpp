#include "ModuleSceneManager.h"



ModuleSceneManager::ModuleSceneManager()
{
	name = "scenes";
}


ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Awake(pugi::xml_node&) {}

bool ModuleSceneManager::Start() {}

bool ModuleSceneManager::Update(float dt) {

	currentScene->Update(dt);
}

bool ModuleSceneManager::CleanUp(pugi::xml_node& n) {

	currentScene->CleanUp(n);
}

void ModuleSceneManager::LoadScene(Scene* scene) {

}

Scene* ModuleSceneManager::ChangeScene(Scene* scene_to_change) {

}

void ModuleSceneManager::UnloadScene(Scene* scene) {


}