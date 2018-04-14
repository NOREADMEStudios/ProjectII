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

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->itemSc);
	}
	if (controllersNum != 0) {
		ManageDisplacement();
		ChooseFocus();

	}

	return true;
}

bool IntroScene::CleanUp()
{
	xmlNode n;
	App->gui->CleanUp(n);
	UnLoadBackground();
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

void IntroScene::LoadUIButtons() {

	SDL_Texture * atlas = App->textures->Load("UI/atlas.png");
	pvpButton = App->gui->AddButton(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 4 + 300, atlas, { 50,50,384,186 }, true, PvPPressCallb, { 50,270,384,186 }, { 50,491,384,186 });

	pvpButton->OnHoverEnter = PvPHoverEnCallb;
	pvpButton->OnHoverExit = PvPHoverExCallb;
	pvpLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string PvPStr = "PvP";
	pvpLabel->setString(PvPStr);
	pvpLabel->SetParent(pvpButton);
	pvpLabel->culled = false;

	exitButton = App->gui->AddButton((SCREEN_WIDTH / 2) - 100, (SCREEN_HEIGHT / 4) * 3, atlas, { 50,50,384,186 }, true, ExitPressCallb, { 50,270,384,186 }, { 50,491,384,186 });
	exitButton->OnHoverEnter = ExitHoverEnCallb;
	exitButton->OnHoverExit = ExitHoverExCallb;
	exitLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string ExitStr = "EXIT";
	exitLabel->setString(ExitStr);
	exitLabel->SetParent(exitButton);
	exitLabel->culled = false;

	buttonsForController.push_back(pvpButton);
	buttonsForController.push_back(exitButton);

}

void IntroScene::SetControllerFocus() {
	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}
	Button* butt = *(buttonsForController.begin());

	player1.but = butt;
	player1.but->in_focus = true;
}
void IntroScene::ManageDisplacement() {

	if (App->input->GetButtonFromController(1) == Input::DOWN) {
		for (std::list<Button*>::iterator button = buttonsForController.begin(); button != buttonsForController.end(); button++) {
			if (*button == player1.but) {
				button++;
				if (button != buttonsForController.end()) {
					player1.but = (*button);
					player1.but->in_focus = true;
					button--;
					(*button)->in_focus = false;
				}
				return;
			}
		}
	}if (App->input->GetButtonFromController(1) == Input::UP) {
		for (std::list<Button*>::iterator button = buttonsForController.begin(); button != buttonsForController.end(); button++) {
			if (*button == player1.but) {
				
				if (button != buttonsForController.begin()) {
					button--;
					player1.but = (*button);
					player1.but->in_focus = true;
					button++;
					(*button)->in_focus = false;
				}
				return;
			}
		}
	}

}
void IntroScene::ChooseFocus() {

	
		if (App->input->GetButtonFromController(1) == Input::JUMPINPUT) {
			LOG("");
			if (player1.but==pvpButton) {
				PvPPressCallb(0);			
			}
			else if (player1.but ==exitButton) {
				ExitPressCallb(0);
			}
		}
	
}