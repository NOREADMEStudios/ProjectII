#include "Scene.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "App.h"
#include "ModuleGUI.h"

#define BACKGROUND_RECT {0,0,3555,2005}

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

bool Scene::LoadBackground(std::string path) {
	bool ret = true;
	background = App->textures->Load(path.c_str());
	
	if (!background) {
		ret = false;
	}
	return ret;
}

bool Scene::DrawBackground() {
	
	iRect rect = BACKGROUND_RECT;
	bool ret = App->render->Blit(background, 0,0, &rect.toSDL_Rect());

	return ret;
}

bool Scene::UnLoadBackground() {

	bool ret = App->textures->UnLoad(background);
	return ret;
}