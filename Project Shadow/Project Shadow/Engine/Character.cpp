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
	iPoint pivot_pos = PivotPos();
	collFeet->collider.x += pivot_pos.x;//pivot
	collFeet->collider.y += pivot_pos.y;//pivot
	collHitBox->collider.x += pivot_pos.x;
	collHitBox->collider.y += pivot_pos.y;
	collAtk->collider.x += pivot_pos.x;
	collAtk->collider.y += pivot_pos.y;
	collDef->collider.y += pivot_pos.y;
	collDef->collider.x += pivot_pos.x;
	collParry->collider.y += pivot_pos.y;
	collParry->collider.x += pivot_pos.x;
}

void Character::GetCollidersFromAnimation() {
	collFeet->collider = currentAnimation->GetFeetColliderFromFrame();
	collHitBox->collider = currentAnimation->GetHitBoxColliderFromFrame();
	collAtk->collider = currentAnimation->GetAtkColliderFromFrame();
	collDef->collider = currentAnimation->GetDefColliderFromFrame();
	collParry->collider = currentAnimation->GetParryColliderFromFrame();

	if (flip)
	{
		collFeet->collider.x = currentAnimation->CurrentFrame().rect.w - (collFeet->collider.x + collFeet->collider.w);
		collHitBox->collider.x = currentAnimation->CurrentFrame().rect.w - (collHitBox->collider.x + collHitBox->collider.w);
		collAtk->collider.x = currentAnimation->CurrentFrame().rect.w - (collAtk->collider.x + collAtk->collider.w);
		collDef->collider.x = currentAnimation->CurrentFrame().rect.w - (collDef->collider.x + collDef->collider.w);
		collParry->collider.x = currentAnimation->CurrentFrame().rect.w - (collParry->collider.x + collParry->collider.w);
	}
}
