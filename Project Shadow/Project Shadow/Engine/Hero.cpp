#include "Hero.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"

#include "ModuleCollision.h"

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
	sprites = App->textures->Load("Characters/Fighter_sprites_red.png");
	LoadAnimations();

	collAtk = App->collision->CreateCollider({}, "enemy_attack", Collider::ATK);
	collHitBox = App->collision->CreateCollider({}, "player_hitbox", Collider::HITBOX);
	collFeet = App->collision->CreateCollider({}, "player_feet", Collider::FEET);
	//Testing things
	App->collision->AddCollider(collAtk, this);
	App->collision->AddCollider(collHitBox, this);
	App->collision->AddCollider(collFeet, this);
	

	collider = { 50 , 50 , 50, 50 };
	stats.spd = 300;
	stats.life = 100;

	initialpos = position;
	initiallife = stats.life;
	lives = 2;

	Attack* light_1 = new Attack(ATTACK_LIGHT, LIGHT_ATTACK, 10);
	Attack* light_2 = new Attack(ATTACK_L2, LIGHT_ATTACK, 12);
	attacks.push_back(light_1);
	attacks.push_back(light_2);

	light_1->AddChild(light_2);

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
	
	if (stats.life > 0)
		RequestState();
	else
		currentState = DEATH;

	UpdateState();
	UpdateCurState(dt);
	UpdateAnimation();

	if (StateisAtk(currentState)) {
		CalculateAtk();
	}
	else {
		stats.atk = 0;
	}

	Move(dt);
	Break(dt);



	priority = position.y;
	collider.x = position.x;
	collider.y = position.y;

	UpdateCollidersPosition();//Needed to change by the pivot position
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
	jump.LoadAnimationsfromXML("jump", HERO_SPRITE_ROOT);
	stop.LoadAnimationsfromXML("stop", HERO_SPRITE_ROOT);
	run.LoadAnimationsfromXML("run", HERO_SPRITE_ROOT);
	jumpAtk.LoadAnimationsfromXML("jump_attack", HERO_SPRITE_ROOT);
	jumpProt.LoadAnimationsfromXML("jump_protect", HERO_SPRITE_ROOT);
	kick.LoadAnimationsfromXML("kick", HERO_SPRITE_ROOT);
	attack.LoadAnimationsfromXML("attack", HERO_SPRITE_ROOT);

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
		wantedState = ATTACK_LIGHT;
	}
				
	
		
	else if (s_attack)
	{

		wantedState = ATTACK_HEAVY;
	}
		



}


void Hero::UpdateState()
{

	if (currentState == WALK || currentState == RUN || currentState == IDLE)
	{
		currentState = wantedState;

		if (StateisAtk(wantedState) && last_attack != IDLE)
		{
			SetCombo();

			time_attack.Start();
		}
	
	}
	else if (currentAnimation->Finished())
	{	
		last_attack = currentState;

		if (!StateisAtk(last_attack))
		{
			currentState = wantedState;
		}
		else 
		{
			SetCombo();

			time_attack.Start();
	
		}

	}

	if (time_attack.Count(COMBO_MARGIN))
	{
		last_attack = IDLE;
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
			break;
		}
		case DEATH:
		{
			Respawn();
			break;
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


void Hero::Respawn()
{
	position = initialpos;
	stats.life = initiallife;
	lives--;
}
void Hero::OnCollisionEnter(Collider* _this, Collider* _other)
{
	//Dont know the hit tag
	if (_this->sTag == "player_hitbox" && _other->sTag == "enemy_attack")
	{
		currentState = HIT;
		stats.life -= _other->entity->stats.atk;
	}
}

void Hero::CalculateAtk()
{
	stats.atk = GetAtk(currentState)->damage;		
}

bool Hero::StateisAtk(CharStateEnum State)
{
	return (State != WALK && State != RUN && State != IDLE && State != JUMP && State != DEATH && State != DEFEND);
}

Attack* Hero::GetAtk(CharStateEnum atk)
{
	Attack* ret = nullptr;

	for (std::list<Attack*>::iterator item = attacks.begin(); item != attacks.end(); item++) {
		if ((**item).state == atk)
		{
			ret = &(**item);
		}
	}
	return ret;
}

void Hero::SetCombo()
{
	Attack* wanted_atk = GetAtk(wantedState);
	Attack* current_atk = GetAtk(last_attack);

	if (wanted_atk != nullptr && current_atk->CheckChildInput(wanted_atk->input))
	{
		currentState = current_atk->GetChildInput(wanted_atk->input)->state;
	}
	else
	{
		currentState = wantedState;
	}
}
