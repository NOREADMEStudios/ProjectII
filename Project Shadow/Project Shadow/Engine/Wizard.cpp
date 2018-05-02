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

bool Wizard::WizardStart()
{

	
	return true;
}

bool Wizard::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Wizard::WizardUpdate(float dt)
{

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
