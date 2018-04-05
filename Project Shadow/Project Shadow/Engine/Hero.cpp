#include "Hero.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "App.h"


#define HERO_SPRITE_ROOT "Assets/Animations/Characters/Fighter_Animations.tmx"


Hero::Hero() : Character(CharacterTypes::HERO)
{
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
	sprites = App->textures->Load("Characters/new_sprites.png");
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
	
	
	RequestState();
	UpdateState();
	UpdateCurState(dt);
	UpdateAnimation();

	Move(dt);
	Break(dt);



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
	idle.LoadAnimationsfromXML("idle",HERO_SPRITE_ROOT);
	walking.LoadAnimationsfromXML("walking",HERO_SPRITE_ROOT);

}

void Hero::UpdateInputs(float dt)
{
	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
	//	Accelerate(-stats.spd, 0, dt);
	//	currentAnimation = &walking;
	//}
	//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
	//	currentAnimation = &walking;
	//	Accelerate(stats.spd, 0, dt);
	//}
	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	Accelerate(0, -stats.spd, dt);

	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	Accelerate(0, stats.spd, dt);

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//	SetPos(rand() % 1000, 0);
}



void Hero::RequestState() {


	std::list<Input> inputs; 

	if (hero_num == 1)
	inputs = App->input->FirstPlayerConfig();

	else if (hero_num == 2)
	inputs = App->input->SecondPlayerConfig();

	

	bool l_attack = false, 
		s_attack = false, 
		jump = false, 
		block = false, 
		run = false;

	directions.down = false;
	directions.up = false;
	directions.left = false;
	directions.right = false;

	for (std::list<Input>::iterator item = inputs.begin(); item != inputs.end(); item++) {
		
		if ((*item) == DOWN)
		{
			directions.down = true;
		}
		else if ((*item) == LEFT)
		{
			directions.left = true;
		}
		else if ((*item) == RIGHT)
		{
			directions.right = true;
		}
		else if ((*item) == UP)
		{
			directions.up = true;
		}
		else if ((*item) == LIGHT_ATTACK)
		{
			l_attack = true;
		}
		else if ((*item) == HEAVY_ATTACK)
		{
			s_attack = true;
		}
		else if ((*item) == JUMPINPUT)
		{
			jump = true;
		}
		else if ((*item) == DEFEND)
		{
			block = true;
		}
		else if ((*item) == RUN)
		{
			run = true;
		}

	}

	wantedState = IDLE;

	if (directions.down || directions.left || directions.right || directions.up)
	{
		if (run)
			wantedState = RUN;
		else
			wantedState = WALK;
	}

	if (l_attack)
	{
		if (last_attack == ATTACK_LIGHT)
		{
			wantedState = ATTACK_L2;
		}
		else
		{
			wantedState = ATTACK_LIGHT;
		}
	
	}
		
	else if (s_attack)
		wantedState = ATTACK_HEAVY;



}


void Hero::UpdateState()
{
	if (currentState == WALK || currentState == RUN || currentState == IDLE)
	{
		currentState = wantedState;

		if (time_light_attack.Count(COMBO_MARGIN))
		{
			last_attack = IDLE;
		}
	}
	else if (currentAnimation->Finished())
	{
		last_attack = currentState;
		if (currentState == ATTACK_LIGHT || currentState == ATTACK_L2)
			time_light_attack.Start();

		currentState = IDLE;
	}
}

void Hero::UpdateCurState(float dt)
{
	int y_dir =  directions.down - directions.up;
	int x_dir =	 directions.right - directions.left;
	switch (currentState) 
	{
	case WALK:
		{
			Accelerate(x_dir * stats.spd, y_dir * stats.spd, dt);
		}
	}

}
void Hero::UpdateAnimation()
{
	if (currentState == WALK)
	{
		currentAnimation = &walking;
	}
	else if (currentState == IDLE)
	{
		currentAnimation = &idle;
	}
}


void Hero::PushInBuffer(Input state)
{
	inputBuffer.pop_front();
	inputBuffer.push_back(state);
}