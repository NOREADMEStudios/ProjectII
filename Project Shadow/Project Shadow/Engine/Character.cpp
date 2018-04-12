#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "App.h"
#include "Character.h"
#include "ModuleCollision.h"


Character::Character(CharacterTypes charType) : Entity(EntityTypes::CHARACTER)
{
}


Character::~Character()
{}

bool Character::Awake(pugi::xml_node&)
{
	return true;
}

bool Character::Start()
{ 
	return true; 
}

bool Character::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true; 
}

bool Character::Update(float dt)
{ 
	return true; 
}

bool Character::PostUpdate()
{
	return true; 
}

bool Character::CleanUp(pugi::xml_node&)
{ 
	
	return true;
}


void Character::ModifyStats(int attack, int defense, int speed, int magic)
{
	//If one new stat = 0 then isn't modified

	if (attack != 0)
	{
		stats.atk = attack;
		
	}
	if (defense != 0)
	{
		stats.def = defense;

	}
	if (speed != 0)
	{
		stats.spd = speed;

	}

	if (magic != 0)
	{
		stats.mgk = magic;

	}

}

void Character::UpdateCollidersPosition() {
	GetCollidersFromAnimation();
	collFeet->collider.x += this->position.x;//pivot
	collFeet->collider.y += this->position.y;//pivot
	collHitBox->collider.x += this->position.x;
	collHitBox->collider.y += this->position.y;
	collAtk->collider.x += this->position.x;
	collAtk->collider.y += this->position.y;
}

void Character::GetCollidersFromAnimation() {
	collFeet->collider = currentAnimation->GetFeetColliderFromFrame();
	collHitBox->collider = currentAnimation->GetHitBoxColliderFromFrame();
	collAtk->collider = currentAnimation->GetAtkColliderFromFrame();
}
