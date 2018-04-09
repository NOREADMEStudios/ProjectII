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
	PvPButton = App->gui->AddButton(SCREEN_WIDTH / 2, 200, atlas, { 0,0,200,200 }, true, PvPPressCallb);
	PvPButton->OnHoverEnter = PvPHoverEnCallb;
	PvPButton->OnHoverExit = PvPHoverExCallb;
	return false;
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
	
}

void ExitHoverEnCallb(size_t arg_size...) {

}

void ExitHoverExCallb(size_t arg_size...) {

}


