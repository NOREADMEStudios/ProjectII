#include "Cleric.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "ModuleMap.h"
#include "App.h"




Cleric::Cleric() : Character(CharacterTypes::CLERIC)
{
}

Cleric::~Cleric()
{
}


bool Cleric::Awake(pugi::xml_node&)
{
	return true;
}

bool Cleric::ClericStart()
{


	return true;
}

bool Cleric::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Cleric::ClericUpdate(float dt)
{

	return true;
}

bool Cleric::PostUpdate()
{
	return true;
}

bool Cleric::CleanUp(pugi::xml_node&)
{

	return true;
}

void Cleric::UpdateSpecStates()
{

}
