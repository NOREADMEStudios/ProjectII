#include "Scene.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "App.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"

#define BACKGROUND_RECT { 0,0,1750,1080 }

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
	

	int w, h;
	SDL_QueryTexture(background, NULL, NULL, &w, &h);
	iRect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = w;
	rect.h = h;

	uint s_w;
	uint s_h;
	App->win->GetWindowSize(s_w, s_h);
	bool ret = App->render->Blit(background,  s_w - w, s_h - h, &rect.toSDL_Rect());

	return ret;
}

bool Scene::UnLoadBackground() {

	bool ret = App->textures->UnLoad(background);
	return ret;
}