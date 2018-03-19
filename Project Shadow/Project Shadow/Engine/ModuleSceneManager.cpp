#include "ModuleSceneManager.h"



ModuleSceneManager::ModuleSceneManager()
{
	name = "scenes";
}


ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Awake(pugi::xml_node&) {

	return true;
}

bool ModuleSceneManager::Start() {
	return true;
}

bool ModuleSceneManager::Update(float dt) {

	if (currentScene)
		currentScene->Update(dt);
	return true;
}

bool ModuleSceneManager::CleanUp(pugi::xml_node& n) {

	currentScene->CleanUp(n);
	return true;
}

void ModuleSceneManager::LoadScene(Scene* scene) {

}

void ModuleSceneManager::ChangeScene(Scene* scene_to_change) {
	
}

void ModuleSceneManager::UnloadScene(Scene* scene) {


}