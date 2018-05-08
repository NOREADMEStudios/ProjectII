#include "Wizard.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "ModuleMap.h"
#include "App.h"




Wizard::Wizard() : Character(CharacterTypes::WIZARD)
{
}

Wizard::~Wizard()
{
}


bool Wizard::Awake(pugi::xml_node&)
{
	return true;
}

bool Wizard::HeroStart()
{
	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack_1", animations_name, 1);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "attack_dagger", animations_name, 5);
	Attack* crouch = new Attack(4, LIGHT_ATTACK, "attack_crouch", animations_name, 1);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, true);
	Attack* jump_a2 = new Attack(5, LIGHT_ATTACK, "attack_j1", animations_name, 0, true);
	Attack* ab_1 = new Attack(11, AB_1, "attack_m1", animations_name, 0, false, true);
	Attack* ab_2 = new Attack(12, AB_2, "attack_m2", animations_name, 0, false, true);

	attacks.push_back(light_1);
	attacks.push_back(heavy_1);
	attacks.push_back(crouch);
	attacks.push_back(jump_a);
	attacks.push_back(ab_1);
	attacks.push_back(ab_2);
	attacks.push_back(jump_a2);

	light_1->AddChild(crouch);
	jump_a->AddChild(jump_a2);

	Ability* fire = new Ability(ab_1, 3);
	Ability* thunder = new Ability(ab_2, 5);

	AdAbility(*fire);
	AdAbility(*thunder);
	return true;
}

bool Wizard::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Wizard::HeroUpdate(float dt)
{

	if (directions.right - directions.left == 1)
	{
		flip = false;
	}
	else if (directions.right - directions.left == -1)
	{
		flip = true;
	}

	if (!GetAbAtk(11)->active)
	{
		ab_1_active = true;
	}
	else
	{
		ab_1_active = false;
	}

	return true;
}

bool Wizard::PostUpdate()
{
	return true;
}

bool Wizard::CleanUp(pugi::xml_node&)
{

	return true;
}

void Wizard::UpdateSpecStates()
{
	if (currentTag == 11 && !ab_1_active)
	{

		App->entities->CreateSpell({ FIREBALL, gamepos });
		ab_1_active = true;
	}

}
