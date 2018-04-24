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

void OnevsPressCallb(size_t arg_size...);

IntroScene::IntroScene()
{
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Start()
{
	bool test= LoadBackground("UI/MainMenu.png");
	App->audio->PlayMusic("Assets/Audio/BGM/Character_Selection.ogg");
	atlas = App->textures->Load("UI/atlas.png");	
	LoadBackground("UI/MainMenu.png");
	//App->gui->AddSprite(820, 540, bakc_menu, { 0,0,1750,1080 }, true);
 
	App->audio->PlayMusic("Assets/Audio/BGM/Character_Selection.ogg");
	LoadUIButtons();
	SetControllerFocus();
	return true;
}

bool IntroScene::Update(float dt)
{
	DrawBackground();

	//App->input->CheckControllers();

	if (controllersNum != 0) {
		ManageDisplacement();
		ChooseFocus();
	}

	Scene::Update(dt);

	return true;
}

bool IntroScene::CleanUp()
{
	App->gui->CleanUp();
	UnLoadBackground();
	return true;
}

void PvPPressCallb(size_t arg_size...) {

	App->scenes->ChangeScene(App->scenes->itemSc);
	App->scenes->four_players = true;
}

void OnevsPressCallb(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->itemSc);
	App->scenes->four_players = false;

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

void IntroScene::LoadUIButtons() {

	SDL_Texture* atlas = App->textures->Load("UI/atlas.png");
	//pvpButton = App->gui->AddButton(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 4 + 300, atlas, { 50,50,384,186 }, true, PvPPressCallb, { 50,270,384,186 }, { 50,491,384,186 });

	//pvpButton->OnHoverEnter = PvPHoverEnCallb;
	//pvpButton->OnHoverExit = PvPHoverExCallb;
	//Label* pvpLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	//std::string PvPStr = "PvP";
	//pvpLabel->setString(PvPStr);
	//pvpLabel->SetParent(pvpButton);
	//pvpLabel->culled = false;

	uiPoint win_size = App->gui->GetGuiSize();
	pvpButton = App->gui->AddButton((win_size.x / 2) - (win_size.x / 6), win_size.y / 2, atlas, { 50,50,384,195 }, true, PvPPressCallb, { 50,270,384,195 }, { 50,491,384,195 });

	pvpButton->OnHoverEnter = PvPHoverEnCallb;
	pvpButton->OnHoverExit = PvPHoverExCallb;
	pvpLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string PvPStr = "4vs4";
	pvpLabel->setString(PvPStr);
	pvpLabel->SetParent(pvpButton);
	pvpLabel->culled = false;

	onevsoneButton = App->gui->AddButton((win_size.x / 6) + (win_size.x / 2), win_size.y / 2, atlas, { 50,50,384,195 }, true, OnevsPressCallb, { 50,270,384,195 }, { 50,491,384,195 });

	onevsoneButton->OnHoverEnter = PvPHoverEnCallb;
	onevsoneButton->OnHoverExit = PvPHoverExCallb;
	onevsLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string oneStr = "1vs1";
	onevsLabel->setString(oneStr);
	onevsLabel->SetParent(onevsoneButton);
	onevsLabel->culled = false;


	exitButton = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 3, atlas, { 50,50,384,195 }, true, ExitPressCallb, { 50,270,384,195 }, { 50,491,384,195 });
	exitButton->OnHoverEnter = ExitHoverEnCallb;
	exitButton->OnHoverExit = ExitHoverExCallb;
	Label* exitLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string ExitStr = "EXIT";
	exitLabel->setString(ExitStr);
	exitLabel->SetParent(exitButton);
	exitLabel->culled = false;


	pvpButton->SetRelation(onevsoneButton, InterfaceElement::Directions::RIGHT);
	pvpButton->SetRelation(exitButton, InterfaceElement::Directions::DOWN);
	exitButton->SetRelation(onevsoneButton, InterfaceElement::Directions::DOWN);
}

void IntroScene::SetControllerFocus() {
	controllersNum = App->input->GetNumControllers();
	App->gui->setFocus(pvpButton);
	if (controllersNum == 0) {
		return;
	}
	App->gui->setFocus(pvpButton);
}

void IntroScene::ManageDisplacement() {
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::DOWN);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::UP);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_DOWN) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::LEFT);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_DOWN) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::RIGHT);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
}
void IntroScene::ChooseFocus() {
	if (App->input->GetButtonDown(1, SDL_CONTROLLER_BUTTON_A)) {
		((Button*)App->gui->getFocusedItem())->OnClick(0);
	}
}