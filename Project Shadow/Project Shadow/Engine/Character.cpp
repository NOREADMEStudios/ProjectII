#include "Character.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "App.h"



Character::Character() :Entity(EntityTypes::CHARACTER)
{
	LoadAnimations();
}


Character::~Character()
{
}

bool Character::Awake(pugi::xml_node&)
{ 
	return true;
}

bool Character::Start()
{ 
	//Meh
	//currentAnimation = &animations.front();
	sprites = App->textures->Load("Fighter_sprites.png");

	//Testing things
	collider = { 50 , 50 , 50, 50 };
	stats.spd = 300;
	currentAnimation = &idle;
	return true; 
}

bool Character::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true; 
}

bool Character::Update(float dt)
{ 
	currentAnimation = &idle;


	UpdateInputs(dt);
	Move(dt);
	Break(dt);

	//Only for first testings
	SDL_Rect provisional_rect{ position.x, position.y,collider.w,collider.h };
	App->render->DrawQuad(provisional_rect, 255, 0, 0);

	Draw(dt);

	return true; 
}

bool Character::PostUpdate()
{
	return true; 
}

bool Character::CleanUp(pugi::xml_node&)
{ 
	App->textures->UnLoad(sprites);
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

void Character::UpdateInputs(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		Accelerate(-stats.spd, 0, dt);
		currentAnimation = &walking;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		currentAnimation = &walking;
		Accelerate(stats.spd, 0, dt);
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		Accelerate(0, -stats.spd, dt);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		Accelerate(0, stats.spd, dt);
}

void Character::LoadAnimations() {

	idle.PushBack({19, 22, 63, 74});
	walking.PushBack({ 21,114,65,75 });
	walking.PushBack({ 94,114,65,75 });
	walking.PushBack({ 167,114,65,75 });
	walking.loop = true;
	walking.speed = 4.0f;
}