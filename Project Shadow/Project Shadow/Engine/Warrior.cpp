#include "Warrior.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "ModuleMap.h"
#include "App.h"




Warrior::Warrior() : Character(CharacterTypes::WARRIOR)
{
}

Warrior::~Warrior()
{
}


bool Warrior::Awake(pugi::xml_node&)
{
	return true;
}

bool Warrior::WarriorStart()
{

	/*switch (heroNum) {
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
	}*/


	stats.atk += App->entities->items[heroNum - 1].atk;
	stats.def += App->entities->items[heroNum - 1].def;
	stats.spd += App->entities->items[heroNum - 1].spd;
	stats.life += App->entities->items[heroNum - 1].life;



	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack",animations_name, 2);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "kick", animations_name, 2, false, true);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, true);
	Attack* light_2 = new Attack(4, LIGHT_ATTACK, "attack_knee", animations_name, 2);
	Attack* heavy_2 = new Attack(5, HEAVY_ATTACK, "strong_attack", animations_name, 2);
	Attack* light_3 = new Attack(6, LIGHT_ATTACK, "attack_2", animations_name, 5);
	Attack* jump_a2 = new Attack(7, LIGHT_ATTACK, "jump_attack", animations_name, 2, true);
	Attack* jump_a3 = new Attack(8, HEAVY_ATTACK, "windwhirl", animations_name, 5, true);
	//Attack* ulti = new Attack(9, TAUNTINPUT, "win", 0, 0, true);

	attacks.push_back(light_1);
	attacks.push_back(light_2);
	attacks.push_back(light_3);
	attacks.push_back(heavy_1);
	attacks.push_back(heavy_2);
	attacks.push_back(jump_a);
	attacks.push_back(jump_a2);
	attacks.push_back(jump_a3);
	//attacks.push_back(ulti);

	light_1->AddChild(light_2);
	light_2->AddChild(light_3);
	//heavy_1->AddChild(light_3);
	light_2->AddChild(heavy_2);
	//heavy_1->AddChild(heavy_2);
	jump_a->AddChild(jump_a2);
	jump_a->AddChild(jump_a3);

	Ability* kick = new Ability(heavy_1, 3);
	AdAbility(*kick);


	Ability* Aulti = new Ability(light_2, 3);
	AdAbility(*Aulti);

	return true;
}

bool Warrior::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Warrior::WarriorUpdate(float dt)
{

	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;

	switch (currentState)
	{

	case PROTECT:
	{
		max_speed = stats.spd * 0.5f;
		Accelerate((x_dir * stats.spd), 0, (z_dir * stats.spd), dt);
		break;
	}
	}


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

	if (currentTag == 4 && GetAbAtk(currentTag)->active)
	{
		buffed = true;
	}
	else
	{
		buffed = false;
	}
	
	return true;
}

bool Warrior::PostUpdate()
{
	return true;
}

bool Warrior::CleanUp(pugi::xml_node&)
{

	return true;
}

void Warrior::UpdateSpecStates()
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

	if (currentTag == 4 && !buffed)
	{
		AdBuff(5, 300);
		buffed = true;
	}
	
}
