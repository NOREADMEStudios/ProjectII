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
	LoadUIButtons();
	SetControllerFocus();

	return false;
}

bool EndScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->introSc);
	}
	if (controllersNum != 0) {
		
		ChooseFocus();
	}

	return true;
}

bool EndScene::CleanUp()
{
	App->gui->CleanUp();

	return true;
}

void MainButtonPressCallb(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->introSc);
}

void MainButtonHoverEnCallb(size_t arg_size...) {

}

void MainButtonHoverExCallb(size_t arg_size...) {

}

void EndScene::SetControllerFocus() {

	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}
	App->gui->setFocus(mainMenuButton);

}
void EndScene::LoadUIButtons() {

	SDL_Texture* back_end = App->textures->Load("UI/YouWin.png");
	App->gui->AddSprite(820, 540, back_end, { 0,0,1750,1080 }, true);

	mainMenuButton = App->gui->AddButton((SCREEN_WIDTH / 2) + 400, (SCREEN_HEIGHT / 4) * 3, nullptr, { 50,50,384,195 }, true, MainButtonPressCallb, { 50,270,384,195 }, { 50,491,384,195 });
	mainMenuButton->OnHoverEnter = MainButtonHoverEnCallb;
	mainMenuButton->OnHoverExit = MainButtonHoverExCallb;
	mainLabel = App->gui->AddLabel(mainMenuButton->rect.w / 2, mainMenuButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string mainStr = "Main Menu";
	mainLabel->setString(mainStr);
	mainLabel->SetParent(mainMenuButton);
	mainLabel->culled = false;

	winnerLabel = App->gui->AddLabel(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) * 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });

	winnerLabel->setString("WINNER TEAM %d", App->entities->GetWinnerTeam());

}
void EndScene::ChooseFocus() {
	if (App->input->GetButtonDown(1, SDL_CONTROLLER_BUTTON_A)) {
		((Button*)App->gui->getFocusedItem())->OnClick(0);
	}

}