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

	App->audio->PlayMusic("Assets/Audio/BGM/Character_Selection.ogg");
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
	if (input) {
		ChangeInput();	
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
	App->transition->MakeTransition(ChangeSceneCallback, ModuleTransition::Transition::SCROLL_LEFT, 1.0f);
}

void OnevsPressCallb(size_t arg_size...) {	
	App->scenes->gameMode = GameMode::ONEvsONE;
	App->transition->MakeTransition(ChangeSceneCallback, ModuleTransition::Transition::SCROLL_LEFT, 1.0f);
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
void InputPressCallb(size_t arg_size...) {
	App->scenes->introSc->ManageInput(true);
}

void BackInputPressCallb(size_t arg_size...) {
	App->scenes->introSc->ManageInput(false);

}
void DefaultInputPressCallb(size_t arg_size...) {
	
	
	App->scenes->introSc->DefaultInputList();
	App->scenes->introSc->UpdateInputLabels();
}

void ExitPressCallb(size_t arg_size...) {
	App->Quit();
}

void ChangeSceneCallback(size_t arg_size...) {
	App->scenes->ChangeScene(App->scenes->characterSc);
}

void VoidFuncCallback(size_t arg_size...) {

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
	music_sp = App->gui->AddSprite((win_size.x / 2), (win_size.y / 4)*1.3f, atlas, { 455, 268, 427, 27 }, false);

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

	fx_sp = App->gui->AddSprite((win_size.x / 2), (win_size.y / 4)*1.6f, atlas, { 455, 268, 427, 27 }, false);

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
	
	//INPUT BUTTON
	inputBut = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 2.2f, atlas, { 1282,883,400,98 }, false, InputPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	Label* InLabel = App->gui->AddLabel((inputBut->rect.w / 2) + 30, inputBut->rect.h / 2, 45, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string InStr = "INPUT";
	InLabel->setString(InStr);
	InLabel->SetParent(inputBut);
	InLabel->culled = false;

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
	fx_sl->SetRelation(inputBut, InterfaceElement::Directions::DOWN);
	inputBut->SetRelation(fullscrenBut, InterfaceElement::Directions::DOWN);
	fullscrenBut->SetRelation(settBack, InterfaceElement::Directions::DOWN);
	settBack->SetRelation(music_sl, InterfaceElement::Directions::DOWN);

	CreateInputWindow();
}

void IntroScene::CreateInputWindow() {
	uiPoint win_size = App->gui->GetGuiSize();

	// BACK INPUT BUTTON
	backInputBut = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 3.6f, atlas, { 1282,883,400,98 }, false, BackInputPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	Label* BckILabel = App->gui->AddLabel((backInputBut->rect.w / 2) + 14, backInputBut->rect.h / 2, 45, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string BckIStr = "BACK";
	BckILabel->setString(BckIStr);
	BckILabel->SetParent(backInputBut);
	BckILabel->culled = false;
	backInputBut->Enable(false);

	// DEFAULT INPUT BUTTON
	defaultInput = App->gui->AddButton((win_size.x / 2), (win_size.y / 4) * 3.0f, atlas, { 1282,883,400,98 }, false, DefaultInputPressCallb, { 1283,782,400,100 }, { 1283,982,400,100 });
	Label* DfILabel = App->gui->AddLabel((defaultInput->rect.w / 2), defaultInput->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string DfIStr = "DEFAULT";
	DfILabel->setString(DfIStr);
	DfILabel->SetParent(defaultInput);
	DfILabel->culled = false;
	defaultInput->Enable(false);

	hAtk = App->gui->AddLabel((win_size.x / 3), (win_size.y / 4) * 1.3f, 45, DEFAULT_FONT, { 0, 0, 0, 255 });
	std::string hAStr = "Heavy Attack";
	hAtk->setString(hAStr);
	hAtk->culled = false;
	hAtk->Enable(false);

	lAtk = App->gui->AddLabel((win_size.x / 3), (win_size.y / 4) * 1.5f, 45, DEFAULT_FONT, { 0, 0, 0, 255 });
	std::string lAStr = "Light Attack";
	lAtk->setString(lAStr);
	lAtk->culled = false;
	lAtk->Enable(false);

	ab1 = App->gui->AddLabel((win_size.x/ 3), (win_size.y / 4) * 1.7f, 45, DEFAULT_FONT, { 0, 0, 0, 255 });
	std::string a1Str = "Ability 1";
	ab1->setString(a1Str);
	ab1->culled = false;
	ab1->Enable(false);

	ab2 = App->gui->AddLabel((win_size.x / 3), (win_size.y / 4) * 1.9f, 45, DEFAULT_FONT, { 0, 0, 0, 255 });
	std::string a2Str = "Ability 2";
	ab2->setString(a2Str);
	ab2->culled = false;
	ab2->Enable(false);

	ab3 = App->gui->AddLabel((win_size.x  / 3), (win_size.y / 4) * 2.1f, 45, DEFAULT_FONT, { 0, 0, 0, 255 });
	std::string a3Str = "Ability 3";
	ab3->setString(a3Str);
	ab3->culled = false;
	ab3->Enable(false);

	jump = App->gui->AddLabel((win_size.x / 3), (win_size.y / 4) * 2.3f, 45, DEFAULT_FONT, { 0, 0, 0, 255 });
	std::string jStr = "Jump";
	jump->setString(jStr);
	jump->culled = false;
	jump->Enable(false);

	run = App->gui->AddLabel((win_size.x  / 3), (win_size.y / 4) * 2.5f, 45, DEFAULT_FONT, { 0, 0, 0, 255 });
	std::string rStr = "Run";
	run->setString(rStr);
	run->culled = false;
	run->Enable(false);

	prot = App->gui->AddLabel((win_size.x  / 3), (win_size.y / 4) * 2.7f, 45, DEFAULT_FONT, { 0, 0, 0, 255 });
	std::string pStr = "Protect";
	prot->setString(pStr);
	prot->culled = false;
	prot->Enable(false);


	Button* bb1 = App->gui->AddButton((win_size.x*2 / 3), (win_size.y / 4) * 1.3f, atlas, { 102,396,48,48}, false, VoidFuncCallback, {102,448,48,48}, {102,505,48,48});
	Label* lb1 = App->gui->AddLabel((bb1->rect.w / 2) , bb1->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	InputButton* ib1 = App->input->GetInputButton(Input::BUTTON_Y);	
	std::string b1Str = ib1->name;
	lb1->setString(b1Str);
	lb1->SetParent(bb1);
	lb1->culled = false;
	bb1->Enable(false);
	b1 = new InputFrame(bb1, ib1);
	inFrames.push_back(b1);
	

	Button*  bb2 = App->gui->AddButton((win_size.x * 2 / 3), (win_size.y / 4) * 1.5f, atlas, { 102,396,48,48 }, false, VoidFuncCallback, { 102,448,48,48 }, { 102,505,48,48 });
	Label* lb2 = App->gui->AddLabel((bb2->rect.w / 2), bb2->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	InputButton* ib2 = App->input->GetInputButton(Input::BUTTON_X);
	std::string b2Str = ib2->name;
	lb2->setString(b2Str);
	lb2->SetParent(bb2);
	lb2->culled = false;
	bb2->Enable(false);
	b2 = new InputFrame(bb2, ib2);
	inFrames.push_back(b2);

	Button* bb3 = App->gui->AddButton((win_size.x * 2 / 3), (win_size.y / 4) * 1.7f, atlas, { 102,396,48,48 }, false, VoidFuncCallback, { 102,448,48,48 }, { 102,505,48,48 });
	Label* lb3 = App->gui->AddLabel((bb3->rect.w / 2), bb3->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	InputButton* ib3 = App->input->GetInputButton(Input::BUTTON_B);
	std::string b3Str = ib3->name;
	lb3->setString(b3Str);
	lb3->SetParent(bb3);
	lb3->culled = false;
	bb3->Enable(false);
	b3 = new InputFrame(bb3, ib3);
	inFrames.push_back(b3);

	Button* bb4 = App->gui->AddButton((win_size.x * 2 / 3), (win_size.y / 4) * 1.9f, atlas, { 102,396,48,48 }, false, VoidFuncCallback, { 102,448,48,48 }, { 102,505,48,48 });
	Label* lb4 = App->gui->AddLabel((bb4->rect.w / 2), bb4->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	InputButton* ib4 = App->input->GetInputButton(Input::L2);
	std::string b4Str = ib4->name;
	lb4->setString(b4Str);
	lb4->SetParent(bb4);
	lb4->culled = false;
	bb4->Enable(false);
	b4 = new InputFrame(bb4, ib4);
	inFrames.push_back(b4);

	Button* bb5 = App->gui->AddButton((win_size.x * 2 / 3), (win_size.y / 4) * 2.1f, atlas, { 102,396,48,48 }, false, VoidFuncCallback, { 102,448,48,48 }, { 102,505,48,48 });
	Label* lb5 = App->gui->AddLabel((bb5->rect.w / 2), bb5->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	InputButton* ib5 = App->input->GetInputButton(Input::R2);
	std::string b5Str = ib5->name;
	lb5->setString(b5Str);
	lb5->SetParent(bb5);
	lb5->culled = false;
	bb5->Enable(false);
	b5 = new InputFrame(bb5, ib5);
	inFrames.push_back(b5);

	Button* bb6 = App->gui->AddButton((win_size.x * 2 / 3), (win_size.y / 4) * 2.3f, atlas, { 102,396,48,48 }, false, VoidFuncCallback, { 102,448,48,48 }, { 102,505,48,48 });
	Label* lb6 = App->gui->AddLabel((bb6->rect.w / 2), bb6->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	InputButton* ib6 = App->input->GetInputButton(Input::BUTTON_A);
	std::string b6Str = ib6->name;
	lb6->setString(b6Str);
	lb6->SetParent(bb6);
	lb6->culled = false;
	bb6->Enable(false);
	b6 = new InputFrame(bb6, ib6);
	inFrames.push_back(b6);

	Button* bb7 = App->gui->AddButton((win_size.x * 2 / 3), (win_size.y / 4) * 2.5f, atlas, { 102,396,48,48 }, false, VoidFuncCallback, { 102,448,48,48 }, { 102,505,48,48 });
	Label* lb7 = App->gui->AddLabel((bb7->rect.w / 2), bb7->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	InputButton* ib7 = App->input->GetInputButton(Input::R_SHOULDER);
	std::string b7Str = ib7->name;
	lb7->setString(b7Str);
	lb7->SetParent(bb7);
	lb7->culled = false;
	bb7->Enable(false);
	b7 = new InputFrame(bb7, ib7);
	inFrames.push_back(b7);

	Button* bb8 = App->gui->AddButton((win_size.x * 2 / 3), (win_size.y / 4) * 2.7f, atlas, { 102,396,48,48 }, false, VoidFuncCallback, { 102,448,48,48 }, { 102,505,48,48 });
	Label* lb8 = App->gui->AddLabel((bb8->rect.w / 2), bb8->rect.h / 2, 40, DEFAULT_FONT, { 255, 255, 255, 255 });
	InputButton* ib8 = App->input->GetInputButton(Input::L_SHOULDER);
	std::string b8Str = ib8->name;
	lb8->setString(b8Str);
	lb8->SetParent(bb8);
	lb8->culled = false;
	bb8->Enable(false);
	b8 = new InputFrame(bb8, ib8);
	inFrames.push_back(b8);

	bb1->SetRelation(bb2, InterfaceElement::Directions::DOWN);
	bb2->SetRelation(bb3, InterfaceElement::Directions::DOWN);
	bb3->SetRelation(bb4, InterfaceElement::Directions::DOWN);
	bb4->SetRelation(bb5, InterfaceElement::Directions::DOWN);
	bb5->SetRelation(bb6, InterfaceElement::Directions::DOWN);
	bb6->SetRelation(bb7, InterfaceElement::Directions::DOWN);
	bb7->SetRelation(bb8, InterfaceElement::Directions::DOWN);
	bb8->SetRelation(defaultInput, InterfaceElement::Directions::DOWN);
	defaultInput->SetRelation(backInputBut, InterfaceElement::Directions::DOWN);
	backInputBut->SetRelation(bb1, InterfaceElement::Directions::DOWN);
}


void IntroScene::ManageSettings(bool settingActive) {

	pvpButton->Enable(!settingActive);
	onevsoneButton->Enable(!settingActive);
	settingButton->Enable(!settingActive);
	exitButton->Enable(!settingActive);

	inputBut->Enable(settingActive);
	fullscrenBut->Enable(settingActive);
	settBack->Enable(settingActive); 
	music_sl->Enable(settingActive);
	music_sp->Enable(settingActive);
	fx_sl->Enable(settingActive);
	fx_sp->Enable(settingActive);

	App->gui->setFocus(settingActive ? music_sl : pvpButton);
}

void IntroScene::ManageInput(bool inputActive) {

	inputBut->Enable(!inputActive);
	fullscrenBut->Enable(!inputActive);
	settBack->Enable(!inputActive);
	music_sl->Enable(!inputActive);
	music_sp->Enable(!inputActive);
	fx_sl->Enable(!inputActive);
	fx_sp->Enable(!inputActive);

	backInputBut->Enable(inputActive);
	defaultInput->Enable(inputActive);
	hAtk->Enable(inputActive);
	lAtk->Enable(inputActive);
	ab1->Enable(inputActive);
	ab2->Enable(inputActive);
	ab3->Enable(inputActive);
	jump->Enable(inputActive);
	run->Enable(inputActive);
	prot->Enable(inputActive);
	for(int i = 0; i<inFrames.size();i++){
	inFrames[i]->but->Enable(inputActive);
	}

	App->gui->setFocus(inputActive ? backInputBut : music_sl);
	input = inputActive;
}

void IntroScene::ChangeInput() {
	for (int i = 0; i < inFrames.size(); i++) {
		if (inFrames[i]->but->in_focus) {
			Input in = App->input->GetButtonFromController(1);			
			if (in == BUTTON_A || in == BUTTON_B || in == BUTTON_X || in == BUTTON_Y || in == L_SHOULDER || in == R_SHOULDER || in == L2 || in == R2) {
				InputButton* inb = App->input->GetInputButton(in);
				InputFrame* ifr = GetInputFrame(inb);				
				App->input->SwapInputs(inFrames[i]->inBut, ifr->inBut);
				UpdateInputLabels();
			}
		}
	}
}

void IntroScene::UpdateInputLabels() const{

	for (int i = 0; i < inFrames.size(); i++) {
		inFrames[i]->but->getLabel()->setString(inFrames[i]->inBut->name);
	}
}

void IntroScene::DefaultInputList() {
	for (int i = 0; i < inFrames.size(); i++) {
		inFrames[i]->inBut = App->input->default_list[i];
	}
	App->input->ResetConfig();
}

InputFrame* IntroScene::GetInputFrame(InputButton* ibut) {
	InputFrame* ret = nullptr;
	
	for (int i = 0; i < inFrames.size(); i++) {
		if (inFrames[i]->inBut->input==ibut->input)
		{
			ret = inFrames[i];
		}
	}
	return ret;
}
