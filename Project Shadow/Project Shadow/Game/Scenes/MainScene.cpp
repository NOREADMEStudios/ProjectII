#include "MainScene.h"
#include "IntroScene.h"
#include "CharacterSelecScene.h"
#include "../../Engine/Warrior.h"
#include "EndScene.h"
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
#include "../../Engine/ModuleTransition.h"


void Reload(size_t i, ...) {
	App->scenes->UnloadScene(App->scenes->mainSc);
	App->scenes->LoadScene(App->scenes->mainSc);
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
	App->audio->PlayMusic("Assets/Audio/BGM/Level1.ogg");

	App->map->Load("map2small.tmx");

	atlas = App->textures->Load("UI/atlas.png");

	e = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[0]);
	e2 = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[1]);

	App->gui->AddHealthbar((Character*)e, 0, true, 10, 10, atlas, true, { 451, 271, 264, 26 });
	App->gui->AddHealthbar((Character*)e2, 1, false, 1590, 10, atlas, true, { 451, 271, 264, 26 });

	uiPoint dims = App->gui->GetGuiSize();
	roundsLabel = App->gui->AddLabel(0.5f * dims.x, 0.1f * dims.y, 70, "Assets/Textures/UI/TTF/Vecna Bold.ttf",
										{ 255, 255, 255, 255 }, Label::BLENDED, "Round %d", currentRound);


	if (App->scenes->gameMode==GameMode::TWOvsTWO)
	{
		e3 = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[2]);
		e4 = App->entities->CreateCharacter(App->scenes->characterSc->charactersInfo[3]);
		App->gui->AddHealthbar((Character*)e3, 2, true, 1590, 10, atlas, true, { 451, 271, 264, 26 });
		App->gui->AddHealthbar((Character*)e4, 3, false, 1590, 10, atlas, true, { 451, 271, 264, 26 });
	}
	return false;
}

bool MainScene::Update(float dt)
{
	if (App->entities->finish || App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		uint winner = App->entities->GetWinnerTeam();
		wonRounds[winner - 1]++;
		uiPoint dims = App->gui->GetGuiSize();
		winnerLabel = App->gui->AddLabel(dims.x * 0.5f, dims.y * 0.5f, 150, "Assets/Textures/UI/TTF/Vecna Bold.ttf",
			{ 255, 255, 255, 255 }, Label::BLENDED, "Winner team %d!", winner);

		if (wonRounds[winner - 1] <= totalRounds / 2) {
			currentRound++;
			App->transition->MakeTransition(Reload, ModuleTransition::Transition::FADE_TO_BLACK, 1.5f);
		}
		else {
			totalRounds = 3;
			currentRound = 1;
			wonRounds[0] = 0;
			wonRounds[1] = 0;
			App->scenes->ChangeScene(App->scenes->endSc);
		}
		App->entities->finish = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->input->BlockKeyboardEvent(SDL_SCANCODE_P);
		App->scenes->ChangeScene(App->scenes->endSc);
	}
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		paused = !paused;
		App->PauseGame(paused);
	}
	App->map->Draw();

	return true;
}

bool MainScene::PostUpdate()
{
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
	return true;
}