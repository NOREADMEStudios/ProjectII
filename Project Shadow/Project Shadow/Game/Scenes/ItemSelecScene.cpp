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
#include "../../Engine/ModuleInput.h"
#include "../../Engine/ModuleRender.h"
#include "../../Engine/UI/Window.h"
#include "../../Engine/ModuleWindow.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/UI/Label.h"
#include "../../Engine/ModuleCollision.h"
#include "../../Engine/ModuleFonts.h"

void item1PressCallb(size_t arg_size...);
void item1HoverEnCallb(size_t arg_size...);
void item1HoverExCallb(size_t arg_size...);
void item2PressCallb(size_t arg_size...);
void item2HoverEnCallb(size_t arg_size...);
void item2HoverExCallb(size_t arg_size...);
void item3PressCallb(size_t arg_size...);
void item3HoverEnCallb(size_t arg_size...);
void item3HoverExCallb(size_t arg_size...);
void confirmCallback(size_t arg_size...);

ItemSelecScene::ItemSelecScene()
{
}

ItemSelecScene::~ItemSelecScene()
{
}

bool ItemSelecScene::Start()
{
	SDL_Texture * atlas = App->textures->Load("UI/atlas.png");
	SDL_Texture * characterFrame = App->textures->Load("UI/Character Frame.png");
	SDL_Texture * item_atlas = App->textures->Load("UI/items.png");
	player1 = new CharacterFrame();
	player2 = new CharacterFrame();
	player3 = new CharacterFrame();
	player4 = new CharacterFrame();

	Sprite* bgsprite1 = App->gui->AddSprite(0, 0, characterFrame, { 0,0,480, 1080 });
	Sprite* bgsprite2 = App->gui->AddSprite((App->gui->GetGuiSize().x / 4), 0, characterFrame, { 0,0,480, 1080 });
	Sprite* bgsprite3 = App->gui->AddSprite(((App->gui->GetGuiSize().x / 4) * 2), 0, characterFrame, { 0,0,480, 1080 });
	Sprite* bgsprite4 = App->gui->AddSprite(((App->gui->GetGuiSize().x / 4) * 3), 0, characterFrame, { 0,0,480, 1080 });

	bgsprite1->SetAnchor(0, 0);
	player1->background = bgsprite1;
	player1->stateDisplay = App->gui->AddLabel(0, 0, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	player1->stateDisplay->SetAnchor(0, 0);
	player1->stateDisplay->setString("PRESS A");

	bgsprite2->SetAnchor(0, 0);
	player2->background = bgsprite2;
	player2->stateDisplay = App->gui->AddLabel((App->gui->GetGuiSize().x / 4), 0, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	player2->stateDisplay->SetAnchor(0, 0);
	player2->stateDisplay->setString("PRESS A");

	bgsprite3->SetAnchor(0, 0);
	player3->background = bgsprite3;
	player3->stateDisplay = App->gui->AddLabel(((App->gui->GetGuiSize().x / 4) * 2), 0, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	player3->stateDisplay->SetAnchor(0, 0);
	player3->stateDisplay->setString("PRESS A");

	bgsprite4->SetAnchor(0, 0);
	player4->background = bgsprite4;
	player4->stateDisplay = App->gui->AddLabel(((App->gui->GetGuiSize().x / 4) * 3), 0, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	player4->stateDisplay->SetAnchor(0, 0);
	player4->stateDisplay->setString("PRESS A");

	App->debug = true;

	swiftBootsStr = "SPEED + 10"; //1
	cursedSwordStr = "ATTACK + 10"; //2
	paladinsStr = "DEFENSE + 10"; //3
	ringStr = "LIFE + 10"; //4
	dragonSlayerStr = "ATK.+ 5 DEF.+ 5"; //5
	magicRobeStr = "ATK.+ 5 SPD.+ 5"; //6

	item1 = App->gui->AddButton(40, 866, item_atlas, { 0,0,120,120 }, true, item1PressCallb);
	item1->SetAnchor(0, 0);
	item2 = App->gui->AddButton(179, 866, item_atlas, { 0,0,120,120 }, true, item2PressCallb);
	item2->SetAnchor(0, 0);
	item3 = App->gui->AddButton(318, 866, item_atlas, { 0,0,120,120 }, true, item3PressCallb);
	item3->SetAnchor(0, 0);
	item1->OnHoverEnter = item1HoverEnCallb;
	item1->OnHoverExit = item1HoverExCallb;
	item2->OnHoverEnter = item2HoverEnCallb;
	item2->OnHoverExit = item2HoverExCallb;
	item3->OnHoverEnter = item3HoverEnCallb;
	item3->OnHoverExit = item3HoverExCallb;
	
	item1Stats = App->gui->AddLabel(item1->rect.w/2, -12, 19, DEFAULT_FONT, { 255, 255, 255, 255 });
	item1Stats->setString(swiftBootsStr);
	item1Stats->SetParent(item1);
	item1Stats->culled = false;
	item2Stats = App->gui->AddLabel(item1->rect.w / 2, -12, 19, DEFAULT_FONT, { 255, 255, 255, 255 });
	item2Stats->setString(swiftBootsStr);
	item2Stats->SetParent(item2);
	item2Stats->culled = false;
	item3Stats = App->gui->AddLabel(item1->rect.w / 2, -12, 19, DEFAULT_FONT, { 255, 255, 255, 255 });
	item3Stats->setString(swiftBootsStr);
	item3Stats->SetParent(item3);
	item3Stats->culled = false;
	
	confirmButton = App->gui->AddButton(App->gui->GetGuiSize().x / 2, App->gui->GetGuiSize().y / 2, atlas, { 450, 50, 250, 61 }, false, confirmCallback, { 450, 120, 250, 61 }, { 450, 189, 250, 61 });
	confirmLabel = App->gui->AddLabel(confirmButton->rect.w / 2, confirmButton->rect.h / 2, 50, DEFAULT_FONT, { 255, 255, 255, 255 });
	std::string confirmStr = "CONFIRM";
	confirmLabel->setString(confirmStr);
	confirmLabel->SetParent(confirmButton);
	confirmLabel->culled = false;

	return false;
}

bool ItemSelecScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && !item1->pressed) {
		if (player1->item1_id == 0) {
			item1->idle_anim = { 0, 0, 120, 120 };
			item1->setHoveredAnim(item1->idle_anim);
			item1->setPressedAnim(item1->idle_anim);
			item1Stats->setString(swiftBootsStr);
			player1->item1_id++;
		}
		else if (player1->item1_id == 1) {
			item1->idle_anim = { 0 + 120, 0, 120, 120 };
			item1->setHoveredAnim(item1->idle_anim);
			item1->setPressedAnim(item1->idle_anim);
			item1Stats->setString(cursedSwordStr);
			player1->item1_id++;
		}
		else if (player1->item1_id == 2) {
			item1->idle_anim = { 0 + 120 * player1->item1_id, 0, 120, 120 };
			item1->setHoveredAnim(item1->idle_anim);
			item1->setPressedAnim(item1->idle_anim);
			item1Stats->setString(paladinsStr);
			player1->item1_id++;
		}
		else if (player1->item1_id == 3) {
			item1->idle_anim = { 0 + 120 * player1->item1_id, 0, 120, 120 };
			item1->setHoveredAnim(item1->idle_anim);
			item1->setPressedAnim(item1->idle_anim);
			item1Stats->setString(ringStr);
			player1->item1_id++;
		}
		else if (player1->item1_id == 4) {
			item1->idle_anim = { 0 + 120 * player1->item1_id, 0, 120, 120 };
			item1->setHoveredAnim(item1->idle_anim);
			item1->setPressedAnim(item1->idle_anim);
			item1Stats->setString(dragonSlayerStr);
			player1->item1_id++;
		}
		else if (player1->item1_id == 5) {
			item1->idle_anim = { 0 + 120 * player1->item1_id, 0, 120, 120 };
			item1->setHoveredAnim(item1->idle_anim);
			item1->setPressedAnim(item1->idle_anim);
			item1Stats->setString(magicRobeStr);
			player1->item1_id = 0;
		};
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && item1->pressed && !item2->pressed) {
		if (player1->item2_id == 0) {
			item2->idle_anim = { 0, 0, 120, 120 };
			item2->setHoveredAnim(item2->idle_anim);
			item2->setPressedAnim(item2->idle_anim);
			item2Stats->setString(swiftBootsStr);
			player1->item2_id++;
		}
		else if (player1->item2_id == 1) {
			item2->idle_anim = { 0 + 120, 0, 120, 120 };
			item2->setHoveredAnim(item2->idle_anim);
			item2->setPressedAnim(item2->idle_anim);
			item2Stats->setString(cursedSwordStr);
			player1->item2_id++;
		}
		else if (player1->item2_id == 2) {
			item2->idle_anim = { 0 + 120 * player1->item2_id, 0, 120, 120 };
			item2->setHoveredAnim(item2->idle_anim);
			item2->setPressedAnim(item2->idle_anim);
			item2Stats->setString(paladinsStr);
			player1->item2_id++;
		}
		else if (player1->item2_id == 3) {
			item2->idle_anim = { 0 + 120 * player1->item2_id, 0, 120, 120 };
			item2->setHoveredAnim(item2->idle_anim);
			item2->setPressedAnim(item2->idle_anim);
			item2Stats->setString(ringStr);
			player1->item2_id++;
		}
		else if (player1->item2_id == 4) {
			item2->idle_anim = { 0 + 120 * player1->item2_id, 0, 120, 120 };
			item2->setHoveredAnim(item2->idle_anim);
			item2->setPressedAnim(item2->idle_anim);
			item2Stats->setString(dragonSlayerStr);
			player1->item2_id++;
		}
		else if (player1->item2_id == 5) {
			item2->idle_anim = { 0 + 120 * player1->item2_id, 0, 120, 120 };
			item2->setHoveredAnim(item2->idle_anim);
			item2->setPressedAnim(item2->idle_anim);
			item2Stats->setString(magicRobeStr);
			player1->item2_id = 0;
		};
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && item1->pressed && item2->pressed && !item3->pressed) {
		if (player1->item3_id == 0) {
			item3->idle_anim = { 0, 0, 120, 120 };
			item3->setHoveredAnim(item3->idle_anim);
			item3->setPressedAnim(item3->idle_anim);
			item3Stats->setString(swiftBootsStr);
			player1->item3_id++;
		}
		else if (player1->item3_id == 1) {
			item3->idle_anim = { 0 + 120, 0, 120, 120 };
			item3->setHoveredAnim(item3->idle_anim);
			item3->setPressedAnim(item3->idle_anim);
			item3Stats->setString(cursedSwordStr);
			player1->item3_id++;
		}
		else if (player1->item3_id == 2) {
			item3->idle_anim = { 0 + 120 * player1->item3_id, 0, 120, 120 };
			item3->setHoveredAnim(item3->idle_anim);
			item3->setPressedAnim(item3->idle_anim);
			item3Stats->setString(paladinsStr);
			player1->item3_id++;
		}
		else if (player1->item3_id == 3) {
			item3->idle_anim = { 0 + 120 * player1->item3_id, 0, 120, 120 };
			item3->setHoveredAnim(item3->idle_anim);
			item3->setPressedAnim(item3->idle_anim);
			item3Stats->setString(ringStr);
			player1->item3_id++;
		}
		else if (player1->item3_id == 4) {
			item3->idle_anim = { 0 + 120 * player1->item3_id, 0, 120, 120 };
			item3->setHoveredAnim(item3->idle_anim);
			item3->setPressedAnim(item3->idle_anim);
			item3Stats->setString(dragonSlayerStr);
			player1->item3_id++;
		}
		else if (player1->item3_id == 5) {
			item3->idle_anim = { 0 + 120 * player1->item3_id, 0, 120, 120 };
			item3->setHoveredAnim(item3->idle_anim);
			item3->setPressedAnim(item3->idle_anim);
			item3Stats->setString(magicRobeStr);
			player1->item3_id = 0;
		};
	}

	//IF CLICKING BACKSPACE YOU CAN RETURN TO THE ITEM BEFORE
	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) {
		if (item1->pressed && !item2->pressed){
			item1->pressed = false;
		}
		else if (item1->pressed && item2->pressed && !item3->pressed){
			item2->pressed = false;
		}
		else if (item1->pressed && item2->pressed && item3->pressed){
			item3->pressed = false;
		}
	}
	return true;
}

bool ItemSelecScene::CleanUp()
{
	xmlNode n;
	App->gui->CleanUp(n);

	return true;
}

void item1PressCallb(size_t arg_size...) {
	LOG("PRESSED");
	
	//App->scenes->itemSc->confirmButton->Enable(true);
	//App->scenes->itemSc->confirmLabel->Enable(true);

	//if (App->scenes->itemSc->item2->pressed || App->scenes->itemSc->item3->pressed) {
	//	App->scenes->itemSc->item2->pressed = false;
	//	App->scenes->itemSc->item2Stats->Enable(false);
	//	App->scenes->itemSc->item3->pressed = false;
	//	App->scenes->itemSc->item3Stats->Enable(false);
	//}
	va_list args;
	va_start(args, arg_size);

	Button* id = va_arg(args, Button*);
	id->pressed = true;

	va_end (args);
}

void item1HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}

void item1HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}
void item2PressCallb(size_t arg_size...) {
	LOG("PRESSED");
	va_list args;
	va_start(args, arg_size);

	Button* id = va_arg(args, Button*);
	id->pressed = true;

	va_end(args);
}
void item2HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}
void item2HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}
void item3PressCallb(size_t arg_size...) {
	LOG("PRESSED");
	va_list args;
	va_start(args, arg_size);

	Button* id = va_arg(args, Button*);
	id->pressed = true;

	va_end(args);
}
void item3HoverEnCallb(size_t arg_size...) {
	LOG("HOVERENTER");
}

void item3HoverExCallb(size_t arg_size...) {
	LOG("HOVEREXIT");
}

void confirmCallback(size_t arg_size...) {
	LOG("BUTTON CONFIRM PRESSED");
	//HERE THE ITEMS WILL BE ADDED IF ITEM ? IS PRESSED
	App->scenes->ChangeScene(App->scenes->mainSc);
}