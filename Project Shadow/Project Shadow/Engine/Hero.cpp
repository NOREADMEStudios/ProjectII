#include "Hero.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "App.h"

#define HERO_SPRITE_ROOT "Assets/Animations/Characters/Fighter_Animations.tmx"


Hero::Hero() : Character(CharacterTypes::HERO)
{
	idle.name = "idle";
	walking.name = "walking";
}

Hero::~Hero()
{
}


bool Hero::Awake(pugi::xml_node&)
{
	return true;
}

bool Hero::Start()
{
	sprites = App->textures->Load("Characters/Fighter_sprites.png");
	LoadAnimations();

	//Testing things
	collider = { 50 , 50 , 50, 50 };
	stats.spd = 300;

	currentAnimation = &idle;
	return true;
}

bool Hero::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Hero::Update(float dt)
{

	currentAnimation = &idle;


	UpdateInputs(dt);
	Move(dt);
	Break(dt);

	//Only for first testings
	//SDL_Rect provisional_rect{ position.x, position.y,collider.w,collider.h };
	//App->render->DrawQuad(provisional_rect, 255, 0, 0);

	//Draw(dt);
	priority = position.y;
	collider.x = position.x;
	collider.y = position.y;
	App->render->FillQueue(this);

	return true;
}

bool Hero::PostUpdate()
{
	return true;
}

bool Hero::CleanUp(pugi::xml_node&)
{
	App->textures->UnLoad(sprites);
	return true;
}

void Hero::LoadAnimations()
{
	idle.LoadAnimationsfromXML(HERO_SPRITE_ROOT);
	walking.LoadAnimationsfromXML(HERO_SPRITE_ROOT);

}

void Hero::UpdateInputs(float dt)
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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		SetPos(rand() % 1000, 0);
}