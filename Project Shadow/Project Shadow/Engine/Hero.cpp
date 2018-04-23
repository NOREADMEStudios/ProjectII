#include "Hero.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "ModuleMap.h"
#include "App.h"

#define HERO_SPRITE_ROOT "Assets/Animations/Characters/Fighter_Animations.tmx"


Hero::Hero() : Character(CharacterTypes::HERO)
{
}

Hero::~Hero()
{
}


bool Hero::Awake(pugi::xml_node&)
{
	return true;
}

bool Hero::HeroStart()
{
	switch (hero_num) {
	case 1: 
		sprites = App->textures->Load("Characters/Fighter_sprites_red.png");
		break;
	case 2:
		sprites = App->textures->Load("Characters/Fighter_sprites_green.png");
		break;
	case 3:
		sprites = App->textures->Load("Characters/Fighter_sprites_blue.png");
		break;
	case 4:
		sprites = App->textures->Load("Characters/Fighter_sprites_grey.png");
		break;
	}



	stats.atk += App->entities->items[hero_num-1].atk;
	stats.def += App->entities->items[hero_num-1].def;
	stats.spd += App->entities->items[hero_num-1].spd;
	stats.life += App->entities->items[hero_num-1].life;



	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack", 2);
	Attack* light_2 = new Attack(4, LIGHT_ATTACK, "attack_knee", 2);
	Attack* light_3 = new Attack(6, LIGHT_ATTACK, "attack_2", 5);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK,"kick", 2);
	Attack* heavy_2 = new Attack(5, HEAVY_ATTACK,"strong_attack", 2);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", 0, true);
	Attack* jump_a2 = new Attack(7, LIGHT_ATTACK,"jump_attack", 2, true);
	Attack* jump_a3 = new Attack(8, HEAVY_ATTACK, "windwhirl", 5, true);

	attacks.push_back(light_1);
	attacks.push_back(light_2);
	attacks.push_back(light_3);
	attacks.push_back(heavy_1);
	attacks.push_back(heavy_2);
	attacks.push_back(jump_a);
	attacks.push_back(jump_a2);
	attacks.push_back(jump_a3);

	light_1->AddChild(light_2);
	light_2->AddChild(light_3);
	heavy_1->AddChild(light_3);
	light_2->AddChild(heavy_2);
	heavy_1->AddChild(heavy_2);
	jump_a->AddChild(jump_a2);
	jump_a->AddChild(jump_a3);

	
	return true;
}

bool Hero::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Hero::HeroUpdate(float dt)
{

	return true;
}

bool Hero::PostUpdate()
{
	return true;
}

bool Hero::CleanUp(pugi::xml_node&)
{
	
	return true;
}

void Hero::UpdateSpecStates()
{
	if (currentState == PARRY)
	{
		if (parried)
		{
			currentAnimation->Reset();
			currentState = wantedState;
			parried = false;
		}
		else if (currentAnimation->Finished())
		{
			currentAnimation->Reset();
			currentState = wantedState;
		}
	}
	else if (currentState == PROTECT && wantedState != PROTECT)
	{
		currentState = wantedState;
		currentAnimation->Reset();

	}
}
