

#include "../Scenes/CharSelecScene.h"
#include "../../Engine/ModuleSceneManager.h"
#include "../../Engine/ModuleEntityManager.h"
#include "../../Engine/Entity.h"
#include "../../Engine/ModuleGUI.h"
#include "../../Engine/ModuleTextures.h"
#include "../../Engine/ModuleInput.h"
#include "../../Engine/ModuleRender.h"
#include "../../Engine/UI/Window.h"
#include "../../Engine/ModuleWindow.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/UI/Label.h"

#include "../../Engine/ModuleFonts.h"



CharSelecScene::CharSelecScene()
{
}

CharSelecScene::~CharSelecScene()
{
}

bool CharSelecScene::Start()
{
	//LoadBackground("UI/BasicMenuScene.png");

	LoadSceneUI();
	SetControllerFocus();

	return false;
}

bool CharSelecScene::Update(float dt)
{
	
	return true;
}

bool CharSelecScene::CleanUp()
{
	App->gui->CleanUp();
	App->textures->UnLoad(atlas);
	UnLoadBackground();
	return true;
}

void CharSelecScene::LoadSceneUI() {
	atlas = App->textures->Load("UI/atlas.png");
	uiPoint sizeScreen = App->gui->GetGuiSize();

}

void CharSelecScene::SetControllerFocus() {

	/*controllersNum = App->input->GetNumControllers();
	if (controllersNum == 0) {
		return;
	}

	Button* butt = *(buttonsForController.begin());

	for (int i = 1; i <= controllersNum; i++) {
		Selection focus;
		focus.but = butt;
		focus.playerNum = i;
		focus.totalControllersNum = controllersNum;
		focus.LoadArrows();

		playersSelections.push_back(focus);
	}*/
}




void CharSelecScene::ChooseFocus() {
	/*
	for (std::list<Selection>::iterator focus = playersSelections.begin(); focus != playersSelections.end(); focus++) {
		if (App->input->GetButtonFromController((*focus).playerNum) == CharInput::JUMPINPUT) {
			LOG("");
			(*focus).arrow->ChangeAnimation((*focus).arrowLockRect);
			(*focus).locked = true;
		}
	}*/
}

void CharSelecScene::ManageDisplacementFocus() {
	/*for (std::list<Selection>::iterator foc = playersSelections.begin(); foc != playersSelections.end(); foc++) {
		if (!(*foc).locked) {
			if (App->input->GetButtonFromController((*foc).playerNum) == CharInput::CH_RIGHT) {
				for (std::list<Button*>::iterator button = buttonsForController.begin(); button != buttonsForController.end(); button++) {
					if (*button == (*foc).but) {
						button++;
						if (button != buttonsForController.end()) {
							(*foc).but = (*button);
							button--;
							(*foc).DrawOrderedArrow();

						}
						return;
					}
				}
			}

			else if (App->input->GetButtonFromController((*foc).playerNum) == CharInput::CH_LEFT) {
				for (std::list<Button*>::iterator button = buttonsForController.begin(); button != buttonsForController.end(); button++) {
					if (!(*foc).locked) {
						if (*button == (*foc).but) {
							if (button != buttonsForController.begin()) {
								button--;
								(*foc).but = (*button);
								button++;
								(*foc).arrow->setPosition((*foc).but->getPositionX(), (*foc).but->getPositionY() - (*foc).but->rect.h / 2);
								(*foc).DrawOrderedArrow();

							}
							return;
						}
					}
				}
			}
		}
	}*/
}