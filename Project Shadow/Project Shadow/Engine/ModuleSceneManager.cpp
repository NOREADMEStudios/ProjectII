#include "ModuleSceneManager.h"
#include "../Game/Scenes/MainScene.h"
#include "../Game/Scenes/IntroScene.h"
#include "../Game/Scenes/ItemSelecScene.h"
#include "../Game/Scenes/EndScene.h"
#include "ModuleEntityManager.h"



ModuleSceneManager::ModuleSceneManager()
{
	name	=	"scenes";

	introSc	=	new IntroScene();
	itemSc	=	new ItemSelecScene();
	mainSc	=	new MainScene();
	endSc	=	new EndScene();

	AddScene(introSc);
	AddScene(itemSc);
	AddScene(mainSc);
	AddScene(endSc);
}


ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Awake(pugi::xml_node& n) {
	return true;
}

bool ModuleSceneManager::Start() {
	LoadScene(introSc); 
	return true;
}

bool ModuleSceneManager::Update(float dt) {
	if (currentScene != nullptr)
		return currentScene->Update(dt);
	return true;
}

bool ModuleSceneManager::PostUpdate() {
	if (nextScene != nullptr) {
		UnloadScene(currentScene);
		LoadScene(nextScene);
	}
	nextScene = nullptr;
	return true;
}


bool ModuleSceneManager::CleanUp(pugi::xml_node& n) {
	UnloadScene(currentScene);
	return true;
}

void ModuleSceneManager::AddScene(Scene* scene) {
	scenes.push_back(*scene);
}


void ModuleSceneManager::LoadScene(Scene* scene) {
	currentScene = scene;
	currentScene->active = true;
	currentScene->Start();
	App->entities->Start();
}

void ModuleSceneManager::ChangeScene(Scene* scene_to_change) {
	nextScene = scene_to_change;
}

void ModuleSceneManager::UnloadScene(Scene* scene) {
	scene->active = false;
	scene->CleanUp();
	scene = nullptr;
}