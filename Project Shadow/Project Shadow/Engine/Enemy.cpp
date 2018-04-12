#include "Enemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
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
	LoadShadow();
	return true;
}

bool Enemy::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Enemy::Update(float dt)
{

	if (paused) {
		return PausedUpdate();
	}
	currentAnimation = &idle;

	priority = position.y;
	collider.x = position.x;
	collider.y = position.y;
	App->render->FillQueue(this);

	// Map boundaries for the Enemies
	if (gamepos.x < App->map->GetMapBorders_X())
		gamepos.x = App->map->GetMapBorders_X();
	if (gamepos.z < App->map->GetMapBorders_Z())
		gamepos.z = App->map->GetMapBorders_Z();
	if (gamepos.x > App->map->GetMapBorders_X() + App->map->GetMapBorders_W())
		gamepos.x = App->map->GetMapBorders_X() + App->map->GetMapBorders_W();
	if (gamepos.z > App->map->GetMapBorders_Z() + App->map->GetMapBorders_H())
		gamepos.z = App->map->GetMapBorders_Z() + App->map->GetMapBorders_H();

	return true;
}

bool Enemy::PostUpdate()
{
	return true;
}

bool Enemy::CleanUp(pugi::xml_node&)
{
	App->textures->UnLoad(sprites);
	UnloadShadow();
	return true;
}

void Enemy::LoadAnimations()
{
	idle.LoadAnimationsfromXML("idle", ENEMY_SPRITE_ROOT);
}