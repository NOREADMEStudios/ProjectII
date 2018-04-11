#include "Scene.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "App.h"
#include "ModuleGUI.h"


Scene::Scene(){}


Scene::~Scene(){}

bool Scene::Awake(pugi::xml_node&) {
	loaded = true;
	return true;
}

bool Scene::CleanUp() {
	loaded = false;
	return true;
}

bool Scene::Update(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		App->gui->FocusNext();
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		App->gui->FocusPrev();
	}
	return true;
}