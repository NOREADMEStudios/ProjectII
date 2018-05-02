#include "Rogue.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "ModuleMap.h"
#include "App.h"




Rogue::Rogue() : Character(CharacterTypes::ROGUE)
{
}

Rogue::~Rogue()
{
}


bool Rogue::Awake(pugi::xml_node&)
{
	return true;
}

bool Rogue::RogueStart()
{


	return true;
}

bool Rogue::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Rogue::RogueUpdate(float dt)
{

	return true;
}

bool Rogue::PostUpdate()
{
	return true;
}

bool Rogue::CleanUp(pugi::xml_node&)
{

	return true;
}

void Rogue::UpdateSpecStates()
{

}
