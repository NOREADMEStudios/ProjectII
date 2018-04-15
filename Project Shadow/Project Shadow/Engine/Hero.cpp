#include "Hero.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "ModuleMap.h"
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
	switch (hero_num) {
	case 1: 
		sprites = App->textures->Load("Characters/Fighter_sprites_red.png");
		break;
	case 2:
		sprites = App->textures->Load("Characters/Fighter_sprites_green.png");
		break;
	case 3:
		sprites = App->textures->Load("Characters/Fighter_sprites_blue.png");
		break;
	case 4:
		sprites = App->textures->Load("Characters/Fighter_sprites_grey.png");
		break;
	}

	LoadAnimations();

	collAtk = App->collision->CreateCollider({}, "enemy_attack", Collider::ATK);
	collHitBox = App->collision->CreateCollider({}, "player_hitbox", Collider::HITBOX);
	collFeet = App->collision->CreateCollider({}, "player_feet", Collider::FEET);
	collDef = App->collision->CreateCollider({}, "player_shield", Collider::DEF);
	collParry = App->collision->CreateCollider({}, "player_parry", Collider::PARRY);

	//Testing things
	App->collision->AddCollider(collAtk, this);
	App->collision->AddCollider(collHitBox, this);
	App->collision->AddCollider(collFeet, this);
	App->collision->AddCollider(collDef, this);
	App->collision->AddCollider(collParry, this);

	invencible.dur = 3;
	invencible.fr = 0.2f;
	collider = { 50 , 50 , 50, 50 };
	stats.spd = 180;
	stats.life = 100;
	stats.atk = 8;
	stats.def = 1;
	char_depth = 20;
	gamepos.y = 0;

	stats.atk += App->entities->items[hero_num-1].atk;
	stats.def += App->entities->items[hero_num-1].def;
	stats.spd += App->entities->items[hero_num-1].spd;
	stats.life += App->entities->items[hero_num-1].life;

	initialpos.x = gamepos.x;
	initialpos.y = gamepos.y;
	initialLife = stats.life;
	lives = 2;

	Attack* light_1 = new Attack(ATTACK_LIGHT, LIGHT_ATTACK, 2);
	Attack* light_2 = new Attack(ATTACK_L2, LIGHT_ATTACK, 2);
	Attack* light_3 = new Attack(ATTACK_L3, LIGHT_ATTACK, 5);
	Attack* heavy_1 = new Attack(ATTACK_HEAVY, HEAVY_ATTACK, 2);
	Attack* heavy_2 = new Attack(ATTACK_H2, HEAVY_ATTACK, 2);
	Attack* jump_a = new Attack(JUMP, JUMPINPUT, 0);
	Attack* jump_a2 = new Attack(ATTACK_J1, LIGHT_ATTACK, 2);
	Attack* jump_a3 = new Attack(ATTACK_J2, HEAVY_ATTACK, 5);

	attacks.push_back(light_1);
	attacks.push_back(light_2);
	attacks.push_back(light_3);
	attacks.push_back(heavy_1);
	attacks.push_back(heavy_2);
	attacks.push_back(jump_a);
	attacks.push_back(jump_a2);
	attacks.push_back(jump_a3);

	light_1->AddChild(light_2);
	light_2->AddChild(light_3);
	heavy_1->AddChild(light_3);
	light_2->AddChild(heavy_2);
	heavy_1->AddChild(heavy_2);
	jump_a->AddChild(jump_a2);
	jump_a->AddChild(jump_a3);

	LoadShadow();

	max_speed = stats.spd;

	currentState = IDLE;

	currentAnimation = &idle;
	active = true;
	return true;
}

bool Hero::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Hero::Update(float dt)
{

	if (paused) {		
		return PausedUpdate();
	}
	currentAnimation = &idle;

	
	if (stats.life > 0)
		RequestState();
	else
		currentState = DEATH;

	Move(dt);
	Break(dt);


	UpdateAnimation();
	UpdateState();
	UpdateCurState(dt);


	 if (currentState != PROTECT && !StateisAtk(currentState)) {
		if (directions.right - directions.left == 1)
		{
			flip = false;
		}
		else if (directions.right - directions.left == -1)
		{
			flip = true;
		}
	}



	priority = gamepos.z;
	collider.x = position.x;
	collider.y = position.y;



	CalcRealPos();
	UpdateCollidersPosition();

	if (!invencible.active)
	App->render->FillQueue(this);

	else if (invencible.active)
	{
		invencible.UpdateInv();

		if (invencible.on)
		App->render->FillQueue(this);
	}
	
	// Map boundaries for the Heroes
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

bool Hero::PostUpdate()
{
	return true;
}

bool Hero::CleanUp(pugi::xml_node&)
{
	
	App->textures->UnLoad(sprites);

	UnloadShadow();

	if (collAtk) {
		App->collision->RemoveCollider(collAtk);
	}
	if (collDef) {
		App->collision->RemoveCollider(collDef);
	}
	if (collParry) {
		App->collision->RemoveCollider(collParry);
	}

	Utils::ClearList(attacks);

	App->collision->RemoveCollider(collHitBox);
	App->collision->RemoveCollider(collFeet);
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
	hit.LoadAnimationsfromXML("hit", HERO_SPRITE_ROOT);
	kick.LoadAnimationsfromXML("kick", HERO_SPRITE_ROOT);
	attack.LoadAnimationsfromXML("attack", HERO_SPRITE_ROOT);
	death.LoadAnimationsfromXML("death", HERO_SPRITE_ROOT);
	attack_l2.LoadAnimationsfromXML("attack_knee", HERO_SPRITE_ROOT);
	attack_l3.LoadAnimationsfromXML("attack_2", HERO_SPRITE_ROOT);
	protect.LoadAnimationsfromXML("protect", HERO_SPRITE_ROOT);
	taunt.LoadAnimationsfromXML("win", HERO_SPRITE_ROOT);
	attack_s2.LoadAnimationsfromXML("strong_attack", HERO_SPRITE_ROOT);
	parry.LoadAnimationsfromXML("standup", HERO_SPRITE_ROOT);
	attack_j2.LoadAnimationsfromXML("windwhirl", HERO_SPRITE_ROOT);
}

void Hero::RequestState() {
	std::list<Input> inputs;
	int NumControllers = App->input->GetNumControllers();
	if (hero_num <= NumControllers) {
		inputs = App->input->ControllerPlayerConfig(hero_num);
	}
	else {
		if (hero_num-NumControllers == 1) {
			inputs = App->input->FirstPlayerConfig();
		}

		else if (hero_num - NumControllers == 2)
			inputs = App->input->SecondPlayerConfig();
	}
	

	bool l_attack = false, 
		s_attack = false, 
		jump = false, 
		block = false, 
		run = false,
		taunt_b = false,
		parry_b = false;

	directions.down = false;
	directions.up = false;
	directions.left = false;
	directions.right = false;

	for (std::list<Input>::iterator item = inputs.begin(); item != inputs.end(); item++) {
		Input input = *item;
		switch (input)
		{
		case NONEINPUT:
			break;
		case UP:
			directions.up = true;
			break;
		case DOWN:
			directions.down = true;
			break;
		case RIGHT:
			directions.right = true;
			break;
		case LEFT:
			directions.left = true;
			break;
		case LIGHT_ATTACK:
			l_attack = true;
			break;
		case HEAVY_ATTACK:
			s_attack = true;
			break;
		case JUMPINPUT:
			jump = true;
			break;
		case RUNINPUT:
			run = true;
			break;
		case DEFEND:
			block = true;
			break;
		case TAUNTINPUT:
			taunt_b = true;
			break;
		case PARRYINPUT:
			parry_b = true;
			break;
		default:
			break;
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

	if (taunt_b)
		wantedState = TAUNT;

	if (jump)
	{
		wantedState = JUMP;
	}
	if (l_attack)
	{
		wantedState = ATTACK_LIGHT;
	}
	else if (s_attack)
	{
		wantedState = ATTACK_HEAVY;
	}
	else if (block)
		wantedState = PROTECT;
	else if (parry_b)
		wantedState = PARRY;
	


}

void Hero::UpdateState()
{

	if (currentState == WALK  || currentState == IDLE)
	{
		currentState = wantedState;

		if (StateisAtk(wantedState) && last_attack != IDLE)
		{
			SetCombo();
			time_attack.Start();
		}	
	}
	else if (currentState == RUN)
	{
		if (wantedState != RUN)
			currentState = STOP;
	}
	else if (currentState == PARRY)
	{
		if (parried)
		{
			currentAnimation->Reset();
			currentState = wantedState;
			parried = false;
		}
		else if (currentAnimation->Finished())
		{
			currentAnimation->Reset();
			currentState = wantedState;
		}
	}
	else if (currentState == PROTECT && wantedState != PROTECT)
	{
			currentState = wantedState;
			currentAnimation->Reset();
		
	}
	if (currentState == JUMP ||currentState == ATTACK_J1 ||currentState == ATTACK_J2)
	{
		last_attack = JUMP;
		if (StateisAtk(wantedState))
		{
			SetCombo();
			time_attack.Start();
		}
		if ((currentState == ATTACK_J1 || currentState == ATTACK_J2) && currentAnimation->Finished())
		{
			currentAnimation->Reset();
			currentState = JUMP;
		}
		if (gamepos.y <= 0)
		{
			jumping = false;
			speedVector.y = 0;
			currentAnimation->Reset();
			currentState = wantedState;
		}
	}
	else if (currentAnimation->Finished())
	{	
	
		if (currentState == DEATH)
		{
			if (lives > 0)
				Respawn();

			else
				active = false;
		}
;

		last_attack = currentState;
		currentState = wantedState;

		currentAnimation->Reset();

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
	int z_dir =  directions.down - directions.up;
	int x_dir =	 directions.right - directions.left;

	if (currentState == ATTACK_H2)
	{
		breaking = true;
	}
	else
	{
		breaking = false;
	}

	if (gamepos.y > 0 && (currentState != ATTACK_J1 && currentState != ATTACK_J2))
	{
		Accelerate(x_dir, -2, z_dir, dt);
	}

	switch (currentState)
	{
		case WALK:
		{
			max_speed = stats.spd;
			Accelerate(x_dir * stats.spd,0, z_dir * stats.spd, dt);
			break;
		}
		case RUN:
		{
			max_speed = stats.spd * 1.5f;
			Accelerate((x_dir * stats.spd), 0,(z_dir * stats.spd), dt);
			break;
		}
		case PROTECT:
		{
			max_speed = stats.spd * 0.5f;
			Accelerate((x_dir * stats.spd), 0, (z_dir * stats.spd), dt);
			break;
		}
		case HIT:
		{
			if (hit_bool)
			{
				Accelerate(hit_dir, 0, 0, dt);
				hit_bool = false;
			}
			break;
		}
		case JUMP:
		{	
			if (!jumping)
			{
				jumping = true;
				max_speed = 1000;
				Accelerate(x_dir, 500, z_dir, dt);
			}	
			break;
		}
		case ATTACK_J1:
		case ATTACK_J2:
		{	
				Accelerate(x_dir, -1, z_dir, dt);
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

		if (!sound_avaliable)
			sound_avaliable = true;
	}
	else if (currentState == ATTACK_LIGHT)
	{
		currentAnimation = &attack;

		if (sound_avaliable)
		{
			App->audio->PlayFx(1);
			sound_avaliable = false;
		}
	}
	else if (currentState == ATTACK_L2)
	{
		currentAnimation = &attack_l2;

		if (sound_avaliable)
		{
			App->audio->PlayFx(1);
			sound_avaliable = false;
		}
	}
	else if (currentState == HIT)
	{
		currentAnimation = &hit;

		if (sound_avaliable)
			{
				App->audio->PlayFx(3);
				sound_avaliable = false;
			}
	}
	else if (currentState == DEATH)
	{
		currentAnimation = &death;
		if (sound_avaliable)
		{
			App->audio->PlayFx(8);
			sound_avaliable = false;
		}
	}
	else if (currentState == JUMP)
	{
		currentAnimation = &jump;

		if (sound_avaliable)
		{
			App->audio->PlayFx(5);
			sound_avaliable = false;
		}
	}
	else if (currentState == RUN)
	{
		currentAnimation = &run;
	}
	else if (currentState == ATTACK_HEAVY)
	{
		currentAnimation = &kick;

		if (sound_avaliable)
		{
			App->audio->PlayFx(11);
			sound_avaliable = false;
		}
	}
	else if (currentState == ATTACK_L3)
	{
		currentAnimation = &attack_l3;
		if (sound_avaliable)
		{
			App->audio->PlayFx(1);
			sound_avaliable = false;
		}
	}
	else if (currentState == STOP)
	{
		currentAnimation = &stop;
	}
	else if (currentState == PROTECT)
	{
		currentAnimation = &protect;
	}
	else if (currentState == TAUNT)
	{
		currentAnimation = &taunt;

		if (sound_avaliable)
		{
			App->audio->PlayFx(13);
			sound_avaliable = false;
		}
	}
	else if (currentState == ATTACK_H2)
	{
		currentAnimation = &attack_s2;

		if (sound_avaliable)
		{
			App->audio->PlayFx(12);
			sound_avaliable = false;
		}
	}
	else if (currentState == PARRY)
	{
		currentAnimation = &parry;

		if (sound_avaliable)
		{
			App->audio->PlayFx(14);
			sound_avaliable = false;
		}
	}
	else if (currentState == ATTACK_J1)
	{
		currentAnimation = &jumpAtk;
	}
	else if (currentState == ATTACK_J2)
	{
		currentAnimation = &attack_j2;
	}
}

void Hero::Respawn()
{
	gamepos.x = initialpos.x;
	gamepos.z = initialpos.y;

	stats.life = initialLife;
	lives--;
	invencible.StartInv();
}

void Hero::GetHP(int& curr, int& max)
{
	curr = stats.life;
	max = initialLife;
}

void Hero::OnCollisionEnter(Collider* _this, Collider* _other)
{
	if (_this->entity == _other->entity) return;

	int z1 = _this->entity->GetGamePos().z;
	int d1 = _this->entity->GetCharDepth();

	int z2 = _other->entity->GetGamePos().z;
	int d2 = _other->entity->GetCharDepth();

	int p11 = z1 - (d1 / 2);
	int p12 = z1 + (d1 / 2);
	int p21 = z2 - (d2 / 2);
	int p22 = z2 + (d2 / 2);

	if ((p11 <= p21 && p21 <= p12) || (p11 <= p22 && p22 <= p12) || (p21 <= p11 && p11 <= p22) || (p21 <= p12 && p12 <= p22))
	{
		if (_this->collider.x - _other->collider.x > 0)
		{
			hit_dir = 1;
		}
		else
		{
			hit_dir = -1;
		}

		if (_this->sTag == "player_shield" && _other->sTag == "enemy_attack")
		{
			if (_other->entity->breaking)
			{
				currentState = HIT;
				stats.life -= _other->entity->stats.atk;
				hit_bool = true;

				
			}
			else
			{
				max_speed = 600;
				_this->entity->Accelerate(hit_dir * 1000, 0, 0, 1);
			}
			App->audio->PlayFx(10);
		}
		else if (_this->sTag == "enemy_attack" && _other->sTag == "player_shield")
		{
			max_speed = 400;
			_this->entity->Accelerate(hit_dir * 800, 0, 0, 1);



		}
		else if (_this->sTag == "player_parry" && _other->sTag == "enemy_attack")
		{
			currentState = IDLE;
			parried = true;
		}
		else if (_this->sTag == "enemy_attack" && _other->sTag == "player_parry")
		{
			currentState = HIT;

		}
		else if (_this->sTag == "player_hitbox" && _other->sTag == "enemy_attack")
		{

			currentState = HIT;
			hit_bool = true;


			if (_this->collider.x - _other->collider.x > 0)
			{
				hit_dir = 1 * _other->entity->stats.atk;
			}
			else
			{
				hit_dir = -1 * _other->entity->stats.atk;
			}
						
		}
		else if (_this->sTag == "enemy_attack" && _other->sTag == "player_hitbox")
		{
			_other->entity->stats.life -= _this->entity->stats.atk + GetAtk(currentState)->damage - _other->entity->stats.def;
		}

	}
}



bool Hero::StateisAtk(CharStateEnum State)
{
	return (State != WALK && State != RUN && State != IDLE && State != JUMP && State != DEATH && State != DEFEND && State != HIT && State != PROTECT && State != TAUNT && State != STOP && State != PARRY);
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