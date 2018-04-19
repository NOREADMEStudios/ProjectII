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



	Attack* light_1 = new Attack(ATTACK_LIGHT, LIGHT_ATTACK, 2);
	Attack* light_2 = new Attack(ATTACK_L2, LIGHT_ATTACK, 2);
	Attack* light_3 = new Attack(ATTACK_L3, LIGHT_ATTACK, 5);
	Attack* heavy_1 = new Attack(ATTACK_HEAVY, HEAVY_ATTACK, 2);
	Attack* heavy_2 = new Attack(ATTACK_H2, HEAVY_ATTACK, 2);
	Attack* jump_a = new Attack(JUMP, JUMPINPUT, 0);
	Attack* jump_a2 = new Attack(ATTACK_J1, LIGHT_ATTACK, 2);
	Attack* jump_a3 = new Attack(ATTACK_J2, HEAVY_ATTACK, 5);

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

	if (paused) {		
		return PausedUpdate();
	}
	currentAnimation = &states.front()->anim;

	
	if (stats.life > 0)
		RequestState();
	else
		currentState = DEATH;

	Move(dt);
	Break(dt);


	UpdateAnimation();
	UpdateState();
	UpdateCurState(dt);


	 if (currentState != PROTECT && !StateisAtk(currentState)) {
		if (directions.right - directions.left == 1)
		{
			flip = false;
		}
		else if (directions.right - directions.left == -1)
		{
			flip = true;
		}
	}




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

