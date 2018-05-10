#include "ItemSelecScene.h"
#include "CharacterSelecScene.h"
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
#include "../../Engine/ModuleTransition.h"

void PvPPressCallb(size_t arg_size...);
void ExitPressCallb(size_t arg_size...);
void OnevsPressCallb(size_t arg_size...);
void ChangeSceneCallback(size_t arg_size...);

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

	//Scene::Update(dt);

	return true;
}

bool IntroScene::CleanUp() {
	App->gui->CleanUp();
	UnLoadBackground();
	return true;
}

void PvPPressCallb(size_t arg_size...) {	
	App->scenes->gameMode = GameMode::TWOvsTWO;
	App->transition->MakeTransition(ChangeSceneCallback, ModuleTransition::Transition::SCROLL_LEFT, 1.0f);
}

void OnevsPressCallb(size_t arg_size...) {	
	App->scenes->gameMode = GameMode::ONEvsONE;
	App->transition->MakeTransition(ChangeSceneCallback, ModuleTransition::Transition::FADE_TO_BLACK, 1.0f);
}

void ExitPressCallb(size_t arg_size...) {
	App->Quit();
}

void ChangeSceneCallback(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->characterSc);
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
	pvpButton = App->gui->AddButton((win_size.x / 2) , (win_size.y / 4) * 2.1f, atlas, { 1282,883,400,98 }, true, PvPPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });

	pvpLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string PvPStr = "4vs4";
	pvpLabel->setString(PvPStr);
	pvpLabel->SetParent(pvpButton);
	pvpLabel->culled = false;

	onevsoneButton = App->gui->AddButton( (win_size.x / 2), (win_size.y / 4) * 2.7, atlas, { 1282,883,400,98 }, true, OnevsPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });

	onevsLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string oneStr = "1vs1";
	onevsLabel->setString(oneStr);
	onevsLabel->SetParent(onevsoneButton);
	onevsLabel->culled = false;


	exitButton = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 3.3f, atlas, { 1282,883,400,98 }, true, ExitPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });

	Label* exitLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string ExitStr = "EXIT";
	exitLabel->setString(ExitStr);
	exitLabel->SetParent(exitButton);
	exitLabel->culled = false;


	pvpButton->SetRelation(onevsoneButton, InterfaceElement::Directions::RIGHT);
	pvpButton->SetRelation(exitButton, InterfaceElement::Directions::DOWN);
	exitButton->SetRelation(onevsoneButton, InterfaceElement::Directions::DOWN);
	exitButton->SetRelation(exitButton, InterfaceElement::Directions::DOWN, false);
}

void IntroScene::SetControllerFocus() {
	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}
	App->gui->setFocus(pvpButton);
}

void IntroScene::ManageDisplacement() {
	if (App->input->GetButtonFromController(1) == Input::DOWN) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::DOWN);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetButtonFromController(1) == Input::UP) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::UP);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetButtonFromController(1) == Input::LEFT) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::LEFT);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetButtonFromController(1) == Input::RIGHT) {
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