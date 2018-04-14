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
	SDL_Texture * atlas = App->textures->Load("UI/atlas.png");

	uiPoint win_size = App->gui->GetGuiSize();
	pvpButton = App->gui->AddButton(win_size.x / 4, win_size.y / 4, atlas, { 50,50,384,186 }, true, PvPPressCallb, { 50,270,384,186 }, { 50,491,384,186 });

	pvpButton->OnHoverEnter = PvPHoverEnCallb;
	pvpButton->OnHoverExit = PvPHoverExCallb;
	pvpLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string PvPStr = "4vs4";
	pvpLabel->setString(PvPStr);
	pvpLabel->SetParent(pvpButton);
	pvpLabel->culled = false;

	onevsoneButton = App->gui->AddButton((win_size.x / 4) + (win_size.x/2) , win_size.y / 4, atlas, { 50,50,384,186 }, true, OnevsPressCallb, { 50,270,384,186 }, { 50,491,384,186 });

	onevsoneButton->OnHoverEnter = PvPHoverEnCallb;
	onevsoneButton->OnHoverExit = PvPHoverExCallb;
	onevsLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string oneStr = "1vs1";
	onevsLabel->setString(oneStr);
	onevsLabel->SetParent(onevsoneButton);
	onevsLabel->culled = false;

	exitButton = App->gui->AddButton(win_size.x / 2, (win_size.y / 4) * 3, atlas, { 50,50,384,186 }, true, ExitPressCallb, { 50,270,384,186 }, { 50,491,384,186 });
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
	bool test = DrawBackground();
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->scenes->ChangeScene(App->scenes->itemSc);
	}
	

	return true;
}

bool IntroScene::CleanUp()
{
	xmlNode n;
	App->gui->CleanUp(n);
	bool test = UnLoadBackground();
	return true;
}

void PvPPressCallb(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->itemSc);
	App->scenes->four_players = true;
}

void OnevsPressCallb(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->itemSc);
	App->scenes->four_players = false;
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


