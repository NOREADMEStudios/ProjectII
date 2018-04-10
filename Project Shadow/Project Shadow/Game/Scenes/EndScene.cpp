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

void MainButtonPressCallb(size_t arg_size...);
void MainButtonHoverEnCallb(size_t arg_size...);
void MainButtonHoverExCallb(size_t arg_size...);

EndScene::EndScene()
{
}

EndScene::~EndScene()
{
}

bool EndScene::Start()
{
	atlas = App->textures->Load("UI/Buttons.png");
	mainMenuButton = App->gui->AddButton(SCREEN_WIDTH / 2, 700, atlas, { 68,70,384,186 }, true, MainButtonPressCallb);
	mainMenuButton->OnHoverEnter = MainButtonHoverEnCallb;
	mainMenuButton->OnHoverExit = MainButtonHoverExCallb;
	mainLabel = App->gui->AddLabel(mainMenuButton->rect.w / 2, mainMenuButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string mainStr = "Main Menu";
	mainLabel->setString(mainStr);
	mainLabel->SetParent(mainMenuButton);
	mainLabel->culled = false;

	winnerLabel = App->gui->AddLabel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string winnerStr = "WINNER: CHARACTER 1";
	winnerLabel->setString(winnerStr);
	



	return false;
}

bool EndScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->introSc);
	}

	return true;
}

bool EndScene::CleanUp()
{
	xmlNode n;
	App->gui->CleanUp(n);

	return true;
}

void MainButtonPressCallb(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->introSc);
}

void MainButtonHoverEnCallb(size_t arg_size...) {

}

void MainButtonHoverExCallb(size_t arg_size...) {

}
