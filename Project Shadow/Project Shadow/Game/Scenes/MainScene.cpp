#include "MainScene.h"
#include "IntroScene.h"
#include "CharacterSelecScene.h"
#include "../../Engine/Warrior.h"
#include "EndScene.h"
#include "ItemSelecScene.h"
#include "../../Engine/Warrior.h"
#include "../../Engine/ModuleMap.h"
#include "../../Engine/App.h"
#include "../../Engine/ModuleSceneManager.h"
#include "../../Engine/ModuleEntityManager.h"
#include "../../Engine/ModuleGUI.h"
#include "../../Engine/ModuleTextures.h"
#include "../../Engine/ModuleInput.h"
#include "../../Engine/ModuleRender.h"
#include "../../Engine/UI/Window.h"
#include "../../Engine/ModuleCollision.h"
#include "../../Engine/ModuleAudio.h"
#include "../../Engine/ModuleWindow.h"
#include "../../Engine/UI/Label.h"
#include "../../Engine/UI/Slider.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/ModuleFonts.h"
#include "../../Engine/ModuleTransition.h"
#include <time.h>

void MainMenuPressCallb(size_t arg_size...);
void ChangeCharactersPressCallb(size_t arg_size...);
void SettingsPressCallb(size_t arg_size...);
void ExitPressCallb(size_t arg_size...);
void SliderMusicPressCallb(size_t arg_size...);
void SliderFxPressCallb(size_t arg_size...);
void FullscreenPressCallb(size_t arg_size...);
void BackButtPressCallb(size_t arg_size...);

void Reload(size_t i, ...) {
	App->scenes->ChangeScene(App->scenes->mainSc);
	App->entities->finish = false;
}


MainScene::MainScene()
{
	totalRounds = 3;
	currentRound = 1;
	wonRounds[0] = 0;
	wonRounds[1] = 0;
}

MainScene::~MainScene()
{
}

bool MainScene::Start()
{

	uiPoint sizeScreen = App->gui->GetGuiSize(); 
	

	combatEndControlBool = false;
	App->audio->PlayMusic("Assets/Audio/BGM/Level1.ogg");

	srand(time(NULL));
	int rand_map = rand() % 2;
	if (rand_map == 0)
		App->map->Load("map2small.tmx");
	else
		App->map->Load("mapSmall.tmx");

	atlas = App->textures->Load("UI/atlas.png");

	uiPoint dims = App->gui->GetGuiSize();
	roundsLabel = App->gui->AddLabel(0.5f * dims.x, 0.1f * dims.y, 70, DEFAULT_FONT,
										{ 255, 255, 255, 255 }, Label::BLENDED, "Round %d", currentRound);
	uint healthbarMargin = 10;
	uint secHealtbarMargin = 180;
	if (App->scenes->gameMode == GameMode::ONEvsONE) {
		e = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[0]);
		e2 = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[1]);
		App->gui->AddHealthbar((Character*)e, 0, true, healthbarMargin, healthbarMargin, atlas, true, { 304, 271, 577, 26 });
		App->gui->AddHealthbar((Character*)e2, 2, false, sizeScreen.x - healthbarMargin, healthbarMargin, atlas, true, { 304, 271, 577, 26 });
	}
	else if (App->scenes->gameMode == GameMode::TWOvsTWO)
	{
		e = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[0]);
		e2 = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[1]);
		App->gui->AddHealthbar((Character*)e, 0, true,healthbarMargin, healthbarMargin, atlas, true, { 304, 271, 577, 26 });
		App->gui->AddHealthbar((Character*)e2, 1, true, secHealtbarMargin, 240, atlas, true, { 304, 271, 577, 26 });

		e3 = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[2]);
		e4 = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[3]);
		App->gui->AddHealthbar((Character*)e3, 2, false, sizeScreen.x - healthbarMargin, healthbarMargin, atlas, true, { 304, 271, 577, 26 });
		App->gui->AddHealthbar((Character*)e4, 3, false, sizeScreen.x -secHealtbarMargin, 240, atlas, true, { 304, 271, 577, 26 });
	}

	LoadSceneUI();
	CreateSettingsWindow();
	SetControllerFocus();
	SetDebugLabels();
	
	return false;
}

bool MainScene::Update(float dt)
{
	WindowStates();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		paused = !paused;
		App->PauseGame(paused);
	}

	if (controllersNum != 0) {
		ManageDisplacement();
		ChooseFocus();
	}

#ifdef _DEBUG
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		((Character*)e)->AdHp(-100);
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		((Character*)e2)->AdHp(-100);
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && App->scenes->gameMode == GameMode::TWOvsTWO) {
		((Character*)e3)->AdHp(-100);
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && App->scenes->gameMode == GameMode::TWOvsTWO) {
		((Character*)e4)->AdHp(-100);
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->input->BlockKeyboardEvent(SDL_SCANCODE_P);
		App->scenes->ChangeScene(App->scenes->endSc);
	}

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		((Character*)e)->AdHp(100);
		((Character*)e2)->AdHp(100);
		if (App->scenes->gameMode == GameMode::TWOvsTWO)
		{
			((Character*)e3)->AdHp(100);
			((Character*)e4)->AdHp(100);
		}
	}

#endif
	UpdateDebugLabels();
	App->map->Draw();

	return true;
}

bool MainScene::PostUpdate()
{
	if (App->entities->finish && !combatEndControlBool) {
		combatEndControlBool = true;
		uint winner = App->entities->GetWinnerTeam();
		wonRounds[winner - 1]++;
		uiPoint dims = App->gui->GetGuiSize();
		winnerLabel = App->gui->AddLabel(dims.x * 0.5f, dims.y * 0.5f, 150, "Assets/Textures/UI/TTF/Vecna Bold.ttf",
			{ 255, 255, 255, 255 }, Label::BLENDED, "Winner team %d!", winner);

		if (wonRounds[winner - 1] <= totalRounds / 2) {
			currentRound++;
			App->transition->MakeTransition(Reload, ModuleTransition::Transition::FADE_TO_BLACK, 3.5f);
		}
		else {
			totalRounds = 3;
			currentRound = 1;
			wonRounds[0] = 0;
			wonRounds[1] = 0;
			App->scenes->ChangeScene(App->scenes->endSc);
		}
	}
	return true;
}

bool MainScene::CleanUp()
{
	xmlNode n;
	App->win->SetScale(1.0f);
	if (App->scenes->gameMode == GameMode::TWOvsTWO) {
		App->entities->DestroyEntity(e4);
		App->entities->DestroyEntity(e3);
	}
	App->entities->DestroyEntity(e2);
	App->entities->DestroyEntity(e);

	App->map->CleanUp(n);
	App->textures->UnLoad(atlas);
	App->collision->CleanUp(n);
	App->gui->CleanUp();
	App->entities->CleanUp(n);
	App->SetTimeScale(1.f);

	App->scenes->itemSc->players.clear();
	return true;
}

void MainScene::SetControllerFocus(){
	controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}

	for (int i = 1; i <= controllersNum; i++) {
		Player player;

		player.playerNum = i;
		player.ready = false;
		player.totalControllersNum = controllersNum;
		
		players.push_back(player);
	}
	App->gui->setFocus(mainMenuButt);
}

void MainScene::WindowStates(){
	for (int i = 0; i < controllersNum; i++) {
		Player* player = &players[i];
		if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_START && !paused) {
			paused = true;
			ManageSettings(false);
			App->PauseGame(paused);			
			pauseWindow->Enable(true);
		}
		else if (App->input->GetButtonFromController(player->playerNum) == Input::BUTTON_START && paused) {
			pauseWindow->Enable(false);
			paused = false;
			ManageSettings(false);
			App->PauseGame(false);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)//ONLY FOR TESTING WITHOUT CONTROLLER
		pauseWindow->Enable(true);
}

void MainScene::ManageDisplacement() {
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
void MainScene::ChooseFocus() {
	if (App->input->GetButtonDown(1, SDL_CONTROLLER_BUTTON_A) && paused) {
		((Button*)App->gui->getFocusedItem())->OnClick(0);
	}
}

void MainScene::CreateSettingsWindow() {
	uiPoint win_size = App->gui->GetGuiSize();

	//MUSIC SLIDER
	music_sp = App->gui->AddSprite((win_size.x / 2), (win_size.y / 4)*1.5f - 100, atlas, { 455, 268, 427, 27 }, false);

	float musicVol = App->audio->GetMusicVolumePercentage();
	float step = music_sp->rect.w / 100;
	float displacement = musicVol - 50;
	music_sl = App->gui->AddSlider((music_sp->rect.w / 2) + displacement * step, music_sp->rect.h / 2 + 2, atlas, { 456, 298, 28,15 }, false, SliderMusicPressCallb, {}, {}, true, music_sp);

	Label* MusNumLabel = App->gui->AddLabel((music_sl->rect.w / 2), (music_sl->rect.h / 2) + 20, 20, DEFAULT_FONT, { 255, 255, 255, 255 });

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

	fx_sp = App->gui->AddSprite((win_size.x / 2), (win_size.y / 4)*2.1f - 100, atlas, { 455, 268, 427, 27 }, false);

	float FxVol = App->audio->GetFxVolumePercentage();
	float step2 = fx_sp->rect.w / 100;
	float displacement2 = FxVol - 50;
	fx_sl = App->gui->AddSlider((fx_sp->rect.w / 2) + displacement2 * step2, fx_sp->rect.h / 2 + 2, atlas, { 456, 298, 28,15 }, false, SliderFxPressCallb, {}, {}, true, fx_sp);

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
	fullscrenButt = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 2.7f - 100, atlas, { 1282,883,400,98 }, false, FullscreenPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	Label* FSLabel = App->gui->AddLabel(mainMenuButt->rect.w * 4 / 7, mainMenuButt->rect.h / 2, 25, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string FSStr = "FULLSCREEN";
	FSLabel->setString(FSStr);
	FSLabel->SetParent(fullscrenButt);
	FSLabel->culled = false;


	// BACK BUTTON
	settBackButt = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 3.3f - 100, atlas, { 1282,883,400,98 }, false, BackButtPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	Label* BckLabel = App->gui->AddLabel(mainMenuButt->rect.w * 4 / 7, mainMenuButt->rect.h / 2, 25, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string BckStr = "BACK";
	BckLabel->setString(BckStr);
	BckLabel->SetParent(settBackButt);
	BckLabel->culled = false;

	music_sp->SetParent(pauseWindow); 
	fx_sp->SetParent(pauseWindow);
	fullscrenButt->SetParent(pauseWindow);
	settBackButt->SetParent(pauseWindow);
	pauseWindow->Enable(false);
	

	fullscrenButt->SetRelation(settBackButt, InterfaceElement::Directions::DOWN);
	settBackButt->SetRelation(fullscrenButt, InterfaceElement::Directions::DOWN);
	
}

void MainScene::ManageSettings(bool settingActive){
	if (paused) {
		mainMenuButt->Enable(!settingActive);
		changeCharactersButt->Enable(!settingActive);
		settingsButt->Enable(!settingActive);
		exitButt->Enable(!settingActive);

		fullscrenButt->Enable(settingActive);
		settBackButt->Enable(settingActive);
		music_sl->Enable(settingActive);
		music_sp->Enable(settingActive);
		fx_sl->Enable(settingActive);
		fx_sp->Enable(settingActive);

		App->gui->setFocus(settingActive ? fullscrenButt : mainMenuButt);
	}
	else {
		fullscrenButt->Enable(settingActive);
		settBackButt->Enable(settingActive);
		music_sl->Enable(settingActive);
		music_sp->Enable(settingActive);
		fx_sl->Enable(settingActive);
		fx_sp->Enable(settingActive);
	}

}


void MainMenuPressCallb(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->introSc);
}

void ChangeCharactersPressCallb(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->characterSc);
}

void SliderMusicPressCallb(size_t arg_size...) {

	Slider* sl = App->scenes->introSc->music_sl;
	float currVol = (sl->GetValue() * 100);
	App->audio->SetMusicVolumePercentage(currVol);
	std::string vol = std::to_string((int)currVol);
	sl->getLabel()->setString(vol);
}
void SliderFxPressCallb(size_t arg_size...) {
	Slider* sl = App->scenes->introSc->fx_sl;
	float currVol = (sl->GetValue() * 100);
	App->audio->SetFxVolumePercentage(currVol);
	std::string vol = std::to_string((int)currVol);
	sl->getLabel()->setString(vol);
	App->audio->PlayFx(1);
}

void SettingsPressCallb(size_t arg_size...) {
	App->scenes->mainSc->ManageSettings(true);
}

void BackButtPressCallb(size_t arg_size...) {
	App->scenes->mainSc->ManageSettings(false);
}

//void ExitPressCallb(size_t arg_size...) {
//
//}

void MainScene::LoadSceneUI() {
	uiPoint sizeScreen = App->gui->GetGuiSize();

	paused = false;

	pauseWindow = App->gui->AddWindow(sizeScreen.x / 2, sizeScreen.y / 2, nullptr, { 2130, 42, 1294, 743 }, false);
	mainMenuButt = App->gui->AddButton(0, 0, nullptr, { 1282, 883, 400, 98 }, true, MainMenuPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	mainMenuButt->SetParent(pauseWindow);
	mainMenuButt->setPosition(pauseWindow->rect.w / 2, pauseWindow->rect.h / 6);
	Label* mainMenuLabel = App->gui->AddLabel(mainMenuButt->rect.w * 4 / 7, mainMenuButt->rect.h / 2, 25, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string mainMenuStr = "MAIN MENU";
	mainMenuLabel->setString(mainMenuStr);
	mainMenuLabel->SetParent(mainMenuButt);
	mainMenuLabel->culled = false;

	changeCharactersButt = App->gui->AddButton(0, 0, nullptr, { 1282, 883, 400, 98 }, true, ChangeCharactersPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	changeCharactersButt->SetParent(pauseWindow);
	changeCharactersButt->setPosition(pauseWindow->rect.w / 2, pauseWindow->rect.h * 3 / 8);
	Label* changeCharacterLabel = App->gui->AddLabel(mainMenuButt->rect.w * 4 / 7, mainMenuButt->rect.h / 2, 25, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string changeCharacterStr = "CHANGE CHARACTERS";
	changeCharacterLabel->setString(changeCharacterStr);
	changeCharacterLabel->SetParent(changeCharactersButt);
	changeCharacterLabel->culled = false;
	
	settingsButt = App->gui->AddButton(0, 0, nullptr, { 1282, 883, 400, 98 }, true, SettingsPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	settingsButt->SetParent(pauseWindow);
	settingsButt->setPosition(pauseWindow->rect.w / 2, pauseWindow->rect.h * 5 / 8);
	Label* settingsLabel = App->gui->AddLabel(mainMenuButt->rect.w * 4 / 7, mainMenuButt->rect.h / 2, 25, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string settingsStr = "SETTINGS";
	settingsLabel->setString(settingsStr);
	settingsLabel->SetParent(settingsButt);
	settingsLabel->culled = false;

	exitButt = App->gui->AddButton(0, 0, nullptr, { 1282, 883, 400, 98 }, true, ExitPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	exitButt->SetParent(pauseWindow);
	exitButt->setPosition(pauseWindow->rect.w / 2, pauseWindow->rect.h * 5 / 6);
	Label* exitLabel = App->gui->AddLabel(mainMenuButt->rect.w * 4 / 7, mainMenuButt->rect.h / 2, 25, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string exitStr = "EXIT";
	exitLabel->setString(exitStr);
	exitLabel->SetParent(exitButt);
	exitLabel->culled = false;

	mainMenuButt->SetRelation(changeCharactersButt, InterfaceElement::Directions::DOWN);
	changeCharactersButt->SetRelation(settingsButt, InterfaceElement::Directions::DOWN);
	settingsButt->SetRelation(exitButt, InterfaceElement::Directions::DOWN);
	exitButt->SetRelation(mainMenuButt, InterfaceElement::Directions::DOWN);

}

