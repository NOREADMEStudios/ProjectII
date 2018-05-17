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

	return true;
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
	uiPoint screenSize = App->gui->GetGuiSize();
	SDL_Texture* back_end = App->textures->Load("UI/YouWin.png");
	Sprite* background = App->gui->AddSprite(820, 540, back_end, { 0,0,1750,1080 }, true);
	background->setPosition(screenSize.x * 0.5f, screenSize.y * 0.5f);
	background->ComputeRects();
	if (screenSize.y < background->rect.h) {
		background->scale = MAX((float)screenSize.y / (float)background->rect.h, (float)screenSize.x / (float)background->rect.w);
	}
	else {
		background->scale = MIN((float)screenSize.y / (float)background->rect.h, (float)screenSize.x / (float)background->rect.w);
	}
	
	mainMenuButton = App->gui->AddButton((screenSize.x / 2), (screenSize.y / 4) * 3, nullptr, { 1282,883,400,98 }, true, MainButtonPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	mainMenuButton->OnHoverEnter = MainButtonHoverEnCallb;
	mainMenuButton->OnHoverExit = MainButtonHoverExCallb;
	mainLabel = App->gui->AddLabel(mainMenuButton->rect.w * 5.8f / 10.f, mainMenuButton->rect.h / 2.f, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string mainStr = "Main Menu";
	mainLabel->setString(mainStr);
	mainLabel->SetParent(mainMenuButton);
	mainLabel->culled = false;

	winnerLabel = App->gui->AddLabel(screenSize.x / 2, (screenSize.y / 4) * 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });

	winnerLabel->setString("WINNER: TEAM %d", App->entities->GetWinnerTeam());
}

void EndScene::ChooseFocus() {
	if (App->input->GetButtonDown(1, SDL_CONTROLLER_BUTTON_A)) {
		((Button*)App->gui->getFocusedItem())->OnClick(0);
	}
}