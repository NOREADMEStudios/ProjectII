#include "Scene.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "App.h"


Scene::Scene(){}


Scene::~Scene(){}

bool Scene::Awake(pugi::xml_node&) {
	loaded = true;
	return true;
}

bool Scene::CleanUp(pugi::xml_node&) {
	loaded = false;
	return true;
}

bool Scene::Update(float dt) {

	/*if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		if (App->scenes->ChangeScene()
		{

		}
		
	}*/
	return true;
}