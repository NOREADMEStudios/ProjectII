#include "Scene.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "App.h"
#include "ModuleGUI.h"
#include "ModuleFonts.h"
#include "UI\Label.h"
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

	uiPoint s;
	s = App->gui->GetGuiSize();
	bool ret = App->render->Blit(background,  s.x - w, s.y - h, &rect.toSDL_Rect());

	return ret;
}

bool Scene::UnLoadBackground() {

	bool ret = App->textures->UnLoad(background);
	return ret;
}

void Scene::SetDebugLabels()
{
	uiPoint gui = App->gui->GetGuiSize();
	fps = App->gui->AddLabel(200, gui.y - 40 , 35, DEFAULT_FONT);
	fps->setAlignment(Label::LEFT);
	fps->Enable(false);
	time_since_start = App->gui->AddLabel(500, gui.y - 40, 35, DEFAULT_FONT);
	time_since_start->setAlignment(Label::LEFT);
	time_since_start->Enable(false);
}

void Scene::UpdateDebugLabels()
{
	if (App->debug)
	{
		char fpsChar[256];
		sprintf_s(fpsChar, 256, "FPS: %.2f", App->avg_fps);
		fps->setString(fpsChar);

		char timeChar[256];
		sprintf_s(timeChar, 256, "TIME: %.2f", App->seconds_since_startup);
		time_since_start->setString(timeChar);
	}
}

void Scene::EnableDebugLabels()
{
	fps->Enable(!fps->enabled);
	time_since_start->Enable(!time_since_start->enabled);
}