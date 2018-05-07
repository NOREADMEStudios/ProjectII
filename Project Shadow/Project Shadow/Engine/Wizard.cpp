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
	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack_1", animations_name, 2);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "attack_dagger", animations_name, 2);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, true);
	

	attacks.push_back(light_1);
	attacks.push_back(heavy_1);
	attacks.push_back(jump_a);

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

}
