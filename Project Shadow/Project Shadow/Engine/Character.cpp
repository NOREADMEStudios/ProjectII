#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "App.h"
#include "Character.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"

#include "ModuleMap.h"


class Hero;


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
	LoadBasicStates();

	collAtk = App->collision->CreateCollider({}, "enemy_attack", Collider::ATK);
	collHitBox = App->collision->CreateCollider({}, "player_hitbox", Collider::HITBOX);
	collFeet = App->collision->CreateCollider({}, "player_feet", Collider::FEET);
	collDef = App->collision->CreateCollider({}, "player_shield", Collider::DEF);
	collParry = App->collision->CreateCollider({}, "player_parry", Collider::PARRY);


	App->collision->AddCollider(collAtk, this);
	App->collision->AddCollider(collHitBox, this);
	App->collision->AddCollider(collFeet, this);
	App->collision->AddCollider(collDef, this);
	App->collision->AddCollider(collParry, this);

	invencible.dur = 3;
	invencible.fr = 0.2f;
	stats.spd = 180;
	stats.life = 100;
	stats.atk = 8;
	stats.def = 1;
	char_depth = 20;
	gamepos.y = 0;


	initialpos.x = gamepos.x;
	initialpos.y = gamepos.z;
	initialLife = stats.life;
	lives = maxLives;

	LoadShadow();

	max_speed = stats.spd;

	currentState = IDLE;
	currentAnimation = &states.front()->anim;

	HeroStart();

	active = true;
	return true; 
}

bool Character::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true; 
}

bool Character::Update(float dt)
{ 

	HeroUpdate(dt);

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

bool Character::PostUpdate()
{
	return true; 
}

bool Character::CleanUp(pugi::xml_node&)
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


void Character::RequestState() {
	std::list<Input> inputs;
	int NumControllers = App->input->GetNumControllers();
	if (hero_num <= NumControllers) {
		inputs = App->input->ControllerPlayerConfig(hero_num);
	}
	else {
		if (hero_num - NumControllers == 1) {
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

void Character::UpdateState()
{

	if (currentState == WALK || currentState == IDLE)
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
	if (currentState == JUMP || currentState == ATTACK_J1 || currentState == ATTACK_J2)
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
			lives--;
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

void Character::UpdateCurState(float dt)
{
	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;

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
	else if (gamepos.y < 0)
	{
		gamepos.y = 0;
	}

	switch (currentState)
	{
	case WALK:
	{
		max_speed = stats.spd;
		Accelerate(x_dir * stats.spd, 0, z_dir * stats.spd, dt);
		break;
	}
	case RUN:
	{
		max_speed = stats.spd * 1.5f;
		Accelerate((x_dir * stats.spd), 0, (z_dir * stats.spd), dt);
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

void Character::UpdateAnimation()
{
	for (std::list<State*>::iterator item = states.begin(); item != states.end(); item++) 
	{
		if ((*item)->state == currentState)
		{
			currentAnimation = &(*item)->anim;
			break;
		}
	}
}

void Character::Respawn()
{
	gamepos.x = initialpos.x;
	gamepos.z = initialpos.y;

	stats.life = initialLife;
	invencible.StartInv();
}

void Character::GetHP(int& curr, int& max)
{
	curr = stats.life;
	max = initialLife;
}

void Character::OnCollisionEnter(Collider* _this, Collider* _other)
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
		else if (_this->sTag == "enemy_attack" && _other->sTag == "player_hitbox" && StateisAtk(currentState))
		{
			Attack * atk = GetAtk(currentState);
			if (atk != nullptr)
				_other->entity->stats.life -= _this->entity->stats.atk + atk->damage - _other->entity->stats.def;
		}

	}
}



bool Character::StateisAtk(CharStateEnum state)
{
	return ((state != WALK &&
		state != RUN && state != IDLE &&
		state != JUMP && state != DEATH &&
		state != DEFEND && state != HIT &&
		state != PROTECT && state != TAUNT &&
		state != STOP && state != PARRY) || state == ATTACK_H2);
}

Attack* Character::GetAtk(CharStateEnum atk)
{
	Attack* ret = nullptr;

	for (std::list<Attack*>::iterator item = attacks.begin(); item != attacks.end(); item++) {
		if ((**item).state == atk)
		{
			ret = (*item);
		}
	}
	return ret;
}

uint Character::GetMaxLives() const
{
	return maxLives;
}

uint Character::GetCurrentLives() const
{
	return lives;
}

void Character::SetCombo()
{
	Attack* wanted_atk = GetAtk(wantedState);
	Attack* current_atk = GetAtk(last_attack);

	if (current_atk != nullptr && wanted_atk != nullptr && current_atk->CheckChildInput(wanted_atk->input))
	{
		currentState = current_atk->GetChildInput(wanted_atk->input)->state;
	}
	else
	{
		currentState = wantedState;
	}
}

void Character::LoadState(CharStateEnum _state, std::string animationName)
{
	State* st = new State;
	st->state = _state;

	st->anim.LoadAnimationsfromXML(animationName, HERO_SPRITE_ROOT);
	states.push_back(st);
}

void Character::LoadBasicStates()
{
	LoadState(IDLE, "idle");
	LoadState(WALK, "walking");
	LoadState(JUMP, "jump");
	LoadState(STOP, "stop");
	LoadState(RUN, "run");
	LoadState(HIT, "hit");
	LoadState(DEATH, "death");
	LoadState(TAUNT, "win");

}


		