#include "ItemSelecScene.h"
#include "EndScene.h"
#include "IntroScene.h"
#include "MainScene.h"
#include "../../Engine/ModuleMap.h"
#include "../../Engine/App.h"
#include "../../Engine/ModuleSceneManager.h"
#include "../../Engine/ModuleEntityManager.h"
#include "../../Engine/ModuleGUI.h"
#include "../../Engine/ModuleTextures.h"
#include "../../Engine/ModuleWindow.h"
#include "../../Engine/ModuleInput.h"
#include "../../Engine/ModuleRender.h"
#include "../../Engine/UI/Window.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/UI/Label.h"
#include "../../Engine/ModuleCollision.h"
#include "../../Engine/ModuleAudio.h"
#include "../../Engine/ModuleFonts.h"

void PvPPressCallb(size_t arg_size...);
void PvPHoverEnCallb(size_t arg_size...);
void PvPHoverExCallb(size_t arg_size...);
void ExitPressCallb(size_t arg_size...);
void ExitHoverEnCallb(size_t arg_size...);
void ExitHoverExCallb(size_t arg_size...);


IntroScene::IntroScene()
{
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Start()
{
	//App->audio->PlayMusic("Assets/Audio/BGM/Character_Selection.ogg");
	SDL_Texture * atlas = App->textures->Load("UI/atlas.png");
	pvpButton = App->gui->AddButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, atlas, { 50,50,384,186 }, true, PvPPressCallb, { 50,270,384,186 }, { 50,491,384,186 });

	pvpButton->OnHoverEnter = PvPHoverEnCallb;
	pvpButton->OnHoverExit = PvPHoverExCallb;
	pvpLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string PvPStr = "PvP";
	pvpLabel->setString(PvPStr);
	pvpLabel->SetParent(pvpButton);
	pvpLabel->culled = false;

	exitButton = App->gui->AddButton(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) * 3, atlas, { 50,50,384,186 }, true, ExitPressCallb, { 50,270,384,186 }, { 50,491,384,186 });
	exitButton->OnHoverEnter = ExitHoverEnCallb;
	exitButton->OnHoverExit = ExitHoverExCallb;
	exitLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 }); 
	std::string ExitStr = "EXIT";
	exitLabel->setString(ExitStr);
	exitLabel->SetParent(exitButton);
	exitLabel->culled = false;
	return true;
}

bool IntroScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->itemSc);
	}

	return true;
}

bool IntroScene::CleanUp()
{
	xmlNode n;
	App->gui->CleanUp(n);

	return true;
}

void PvPPressCallb(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->itemSc);
}

void PvPHoverEnCallb(size_t arg_size...) {

}

void PvPHoverExCallb(size_t arg_size...) {

}

void ExitPressCallb(size_t arg_size...) {
	App->Quit();
}

void ExitHoverEnCallb(size_t arg_size...) {

}

void ExitHoverExCallb(size_t arg_size...) {

}


