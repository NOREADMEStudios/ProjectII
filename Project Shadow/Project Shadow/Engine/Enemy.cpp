#include "Enemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "App.h"

#define ENEMY_SPRITE_ROOT "Assets/Animations/Characters/BowGnoll_Animations.tmx"

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
	sprites = App->textures->Load("Characters/BowGnoll_sprites.png");
	LoadAnimations();

	collider = { 50 , 50 , 50, 50 };
	currentAnimation = &idle;

	return true;
}

bool Enemy::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Enemy::Update(float dt)
{
	currentAnimation = &idle;

	priority = position.y;
	collider.x = position.x;
	collider.y = position.y;
	App->render->FillQueue(this);
	return true;
}

bool Enemy::PostUpdate()
{
	return true;
}

bool Enemy::CleanUp(pugi::xml_node&)
{
	App->textures->UnLoad(sprites);
	return true;
}

void Enemy::LoadAnimations()
{
	idle.LoadAnimationsfromXML("idle", ENEMY_SPRITE_ROOT);
}