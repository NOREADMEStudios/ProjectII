#include "Enemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "App.h"

Enemy::Enemy(): Character(CharacterTypes::ENEMY)
{
}

Enemy::~Enemy()
{
}


bool Enemy::Awake(pugi::xml_node&)
{
	return true;
}

bool Enemy::Start()
{

	return true;
}

bool Enemy::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Enemy::Update(float dt)
{
	return true;
}

bool Enemy::PostUpdate()
{
	return true;
}

bool Enemy::CleanUp(pugi::xml_node&)
{
	
	return true;
}

void Enemy::LoadAnimations()
{
	
}