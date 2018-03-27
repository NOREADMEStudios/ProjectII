#include "Character.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "App.h"


Character::Character()
{
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

	//Testing things
	collider = { 50 , 50 , 50, 50 };
	stats.spd = 300;
	return true; 
}

bool Character::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true; 
}

bool Character::Update(float dt)
{ 
	//App->render->Blit()


	UpdateInputs(dt);
	Move(dt);
	Break(dt);

	//Only for first testings
	SDL_Rect provisional_rect{ position.x, position.y,collider.w,collider.h };
	App->render->DrawQuad(provisional_rect, 255, 0, 0);


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

void Character::UpdateInputs(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Accelerate(-stats.spd, 0, dt);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Accelerate(stats.spd, 0, dt);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		Accelerate(0, -stats.spd, dt);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		Accelerate(0, stats.spd, dt);
}