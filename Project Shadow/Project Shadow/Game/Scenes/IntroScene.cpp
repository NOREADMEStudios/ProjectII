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
#include "../../Engine/UI/Slider.h"
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
	uiPoint screenSize = App->gui->GetGuiSize();

	Sprite* bg = App->gui->AddSprite(0, 0, background, { 0,0,1920,1080 });
	bg->setPosition(screenSize.x * 0.5f, screenSize.y * 0.5f);
	bg->ComputeRects();
	if (screenSize.y < bg->rect.h) {
		bg->scale = MAX((float)screenSize.y / (float)bg->rect.h, (float)screenSize.x / (float)bg->rect.w);
	}
	else {
		bg->scale = MIN((float)screenSize.y / (float)bg->rect.h, (float)screenSize.x / (float)bg->rect.w);
	}

	//App->audio->PlayMusic("Assets/Audio/BGM/Character_Selection.ogg");
	atlas = App->textures->Load("UI/atlas.png");	
	LoadBackground("UI/MainMenu.png");
	//App->gui->AddSprite(820, 540, bakc_menu, { 0,0,1750,1080 }, true);
 
	LoadUIButtons();
	CreateSettingsWindow();
	SetControllerFocus();
	SetDebugLabels();
	ManageSettings(false);
	return true;
}

bool IntroScene::Update(float dt)
{
	//DrawBackground();

	//App->input->CheckControllers();

	if (controllersNum != 0) {
		ManageDisplacement();
		ChooseFocus();
	}
	UpdateDebugLabels();
	//Scene::Update(dt);

	return true;
}

bool IntroScene::CleanUp() {
	App->gui->CleanUp();
	pugi::xml_node n;
	App->entities->CleanUp(n);
	UnLoadBackground();
	return true;
}

void PvPPressCallb(size_t arg_size...) {	
	App->scenes->gameMode = GameMode::TWOvsTWO;
	App->transition->MakeTransition(ChangeSceneCallback, ModuleTransition::Transition::FADE_TO_BLACK, 1.0f);
}

void OnevsPressCallb(size_t arg_size...) {	
	App->scenes->gameMode = GameMode::ONEvsONE;
	App->transition->MakeTransition(ChangeSceneCallback, ModuleTransition::Transition::FADE_TO_BLACK, 1.0f);
}

void SliderMPressCallb(size_t arg_size...) {
	
	Slider* sl = App->scenes->introSc->music_sl;
	float currVol = (sl->GetValue() * 100);
	App->audio->SetMusicVolumePercentage(currVol);
	std::string vol = std::to_string((int)currVol);
	sl->getLabel()->setString(vol);
}
void SliderFPressCallb(size_t arg_size...) {
	Slider* sl = App->scenes->introSc->fx_sl;
	float currVol = (sl->GetValue() * 100);
	App->audio->SetFxVolumePercentage(currVol);
	std::string vol = std::to_string((int)currVol);
	sl->getLabel()->setString(vol);
	App->audio->PlayFx(1);
}

void SettingPressCallb(size_t arg_size...) {
	App->scenes->introSc->ManageSettings(true);
}
void BackPressCallb(size_t arg_size...) {
	App->scenes->introSc->ManageSettings(false);
}
void FullscreenPressCallb(size_t arg_size...) {
	Uint32 winfullscr;
	if (App->win->fullscreen) {
		winfullscr = SDL_WINDOW_MINIMIZED;
		App->win->fullscreen = false;
	}
	else {
		winfullscr = SDL_WINDOW_FULLSCREEN;	
		App->win->fullscreen = true;
	}

	SDL_SetWindowFullscreen(App->win->window, winfullscr);
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
	pvpButton = App->gui->AddButton((win_size.x / 2) , (win_size.y / 4) * 1.5f, atlas, { 1282,883,400,98 }, true, PvPPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });

	Label* pvpLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string PvPStr = "2vs2";
	pvpLabel->setString(PvPStr);
	pvpLabel->SetParent(pvpButton);
	pvpLabel->culled = false;

	onevsoneButton = App->gui->AddButton( (win_size.x / 2), (win_size.y / 4) * 2.1, atlas, { 1282,883,400,98 }, true, OnevsPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });

	Label* onevsLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 75, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string oneStr = "1vs1";
	onevsLabel->setString(oneStr);
	onevsLabel->SetParent(onevsoneButton);
	onevsLabel->culled = false;


	settingButton = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 2.7, atlas, { 1282,883,400,98 }, true, SettingPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	Label* settingLabel = App->gui->AddLabel((settingButton->rect.w / 2)+10, settingButton->rect.h / 2, 45, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string SettingStr = "SETTINGS";
	settingLabel->setString(SettingStr);
	settingLabel->SetParent(settingButton);
	settingLabel->culled = false;


	exitButton = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 3.3f, atlas, { 1282,883,400,98 }, true, ExitPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });

	Label* exitLabel = App->gui->AddLabel(pvpButton->rect.w / 2, pvpButton->rect.h / 2, 45, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string ExitStr = "EXIT";
	exitLabel->setString(ExitStr);
	exitLabel->SetParent(exitButton);
	exitLabel->culled = false;


	pvpButton->SetRelation(onevsoneButton, InterfaceElement::Directions::DOWN);
	onevsoneButton->SetRelation(settingButton, InterfaceElement::Directions::DOWN);
	settingButton->SetRelation(exitButton, InterfaceElement::Directions::DOWN);
	exitButton->SetRelation(pvpButton, InterfaceElement::Directions::DOWN);
	
}

void IntroScene::SetControllerFocus() {
	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}
	App->gui->setFocus(pvpButton);
}

void IntroScene::ManageDisplacement() {
	if (App->input->GetButtonFromController(1, false) == Input::DOWN) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::DOWN);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetButtonFromController(1, false) == Input::UP) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::UP);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetButtonFromController(1, false) == Input::LEFT) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::LEFT);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
	if (App->input->GetButtonFromController(1, false) == Input::RIGHT) {
		InterfaceElement* elem = App->gui->getFocusedItem()->GetRelativeElement(InterfaceElement::Directions::RIGHT);
		if (elem != nullptr)
			App->gui->setFocus(elem);
	}
}
void IntroScene::ChooseFocus() {
	if (App->input->GetButtonDown(1, SDL_CONTROLLER_BUTTON_A)) {
		((Button*)App->gui->getFocusedItem())->OnClick(0);
	}
	if (App->input->GetButtonFromController(1) == Input::RIGHT || App->input->GetButtonFromController(1) == Input::LEFT) {
		if (((Button*)App->gui->getFocusedItem()) == music_sl || ((Button*)App->gui->getFocusedItem()) == fx_sl) {
			((Button*)App->gui->getFocusedItem())->OnClick(0);		
		}
	}
}

void IntroScene::CreateSettingsWindow() {
	uiPoint win_size = App->gui->GetGuiSize();
	//MUSIC SLIDER
	music_sp = App->gui->AddSprite((win_size.x / 2), (win_size.y / 4)*1.5f, atlas, { 455, 268, 427, 27 }, false);

	float musicVol = App->audio->GetMusicVolumePercentage();
	float step = music_sp->rect.w / 100;
	float displacement = musicVol - 50;
	music_sl = App->gui->AddSlider((music_sp->rect.w / 2) + displacement*step , music_sp->rect.h / 2 + 2, atlas, { 1152,197,45,21 }, false, SliderMPressCallb, { 1108, 198, 44, 21 }, { 1108, 198, 44, 21 }, true, music_sp);
	
	Label* MusNumLabel = App->gui->AddLabel((music_sl->rect.w / 2), (music_sl->rect.h / 2)+20, 20, DEFAULT_FONT, { 255, 255, 255, 255 });
	
	std::string MusNumStr = std::to_string((uint)musicVol);
	MusNumLabel->setString(MusNumStr);
	MusNumLabel->SetParent(music_sl);
	MusNumLabel->culled = false;
	
	Label* MusNameLabel = App->gui->AddLabel((music_sp->rect.w / 2), (music_sp->rect.h / 2) - 30, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string MSLStr = "MUSIC";
	MusNameLabel->setString(MSLStr);
	MusNameLabel->SetParent(music_sp);
	MusNameLabel->culled = false;
	music_sl->setLabel(MusNumLabel);

	//FX SLIDER

	fx_sp = App->gui->AddSprite((win_size.x / 2), (win_size.y / 4)*2.1f, atlas, { 455, 268, 427, 27 }, false);

	float FxVol = App->audio->GetFxVolumePercentage();
	float step2 = fx_sp->rect.w / 100;
	float displacement2 = FxVol - 50;
	fx_sl = App->gui->AddSlider((fx_sp->rect.w / 2) + displacement2*step2, fx_sp->rect.h / 2 + 2, atlas, { 1152,197,45,21 }, false, SliderFPressCallb, {1108, 198, 44, 21}, { 1108, 198, 44, 21 }, true, fx_sp);

	Label* FxNumLabel = App->gui->AddLabel((fx_sl->rect.w / 2), (fx_sl->rect.h / 2) + 20, 20, DEFAULT_FONT, { 255, 255, 255, 255 });

	std::string FxNumStr = std::to_string((uint)FxVol);
	FxNumLabel->setString(FxNumStr);
	FxNumLabel->SetParent(fx_sl);
	FxNumLabel->culled = false;

	Label* FxNameLabel = App->gui->AddLabel((fx_sp->rect.w / 2), (fx_sp->rect.h / 2) - 30, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string FXLStr = "FX";
	FxNameLabel->setString(FXLStr);
	FxNameLabel->SetParent(fx_sp);
	FxNameLabel->culled = false;
	fx_sl->setLabel(FxNumLabel);


	// FULLSCREEN BUTTON
	fullscrenBut = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 2.7f, atlas,{ 1282,883,400,98 }, false, FullscreenPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	Label* FSLabel = App->gui->AddLabel((fullscrenBut->rect.w / 2) + 30, fullscrenBut->rect.h / 2, 45, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string FSStr = "FULLSCREEN";
	FSLabel->setString(FSStr);
	FSLabel->SetParent(fullscrenBut);
	FSLabel->culled = false;	
	

	// BACK BUTTON
	settBack = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 3.3f, atlas, { 1282,883,400,98 }, false, BackPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	Label* BckLabel = App->gui->AddLabel((fullscrenBut->rect.w / 2) + 14, fullscrenBut->rect.h / 2, 45, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string BckStr = "BACK";
	BckLabel->setString(BckStr);
	BckLabel->SetParent(settBack);
	BckLabel->culled = false;

	
	music_sl->SetRelation(fx_sl, InterfaceElement::Directions::DOWN);
	fx_sl->SetRelation(fullscrenBut, InterfaceElement::Directions::DOWN);
	fullscrenBut->SetRelation(settBack, InterfaceElement::Directions::DOWN);
	settBack->SetRelation(music_sl, InterfaceElement::Directions::DOWN);
}

void IntroScene::ManageSettings(bool settingActive) {

	pvpButton->Enable(!settingActive);
	onevsoneButton->Enable(!settingActive);
	settingButton->Enable(!settingActive);
	exitButton->Enable(!settingActive);

	fullscrenBut->Enable(settingActive);
	settBack->Enable(settingActive); 
	music_sl->Enable(settingActive);
	music_sp->Enable(settingActive);
	fx_sl->Enable(settingActive);
	fx_sp->Enable(settingActive);

	App->gui->setFocus(settingActive ? music_sl : pvpButton);
}

