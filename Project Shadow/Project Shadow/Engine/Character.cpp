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
	SetAnimations();
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

	if (paused) {
		return PausedUpdate();
	}
	currentAnimation = &states.front()->anim;


	if (stats.life > 0)
		RequestState();
	else
		currentState = DEATH;

	if (!eventstates.empty())
		UpdateEventStates();

	Move(dt);
	Break(dt);

	UpdateAbilities();
	UpdateAnimation();
	UpdateMainStates();
	UpdateSpecStates();
	UpdateCurState(dt);
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
	std::list<CharInput> inputs;
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

	bool run = false,
		block = false;
	directions.down = false;
	directions.up = false;
	directions.left = false;
	directions.right = false;

	wantedState = IDLE;
	wantedTag = 0;

	for (std::list<CharInput>::iterator item = inputs.begin(); item != inputs.end(); item++) {
		CharInput input = *item;
		switch (input)
		{
		case NONECHARINPUT:
			break;
		case CH_UP:
			wantedState = WALK;
			directions.up = true;
			break;
		case CH_DOWN:
			wantedState = WALK;
			directions.down = true;
			break;
		case CH_RIGHT:
			wantedState = WALK;
			directions.right = true;
			break;
		case CH_LEFT:
			wantedState = WALK;
			directions.left = true;
			break;
		case LIGHT_ATTACK:
			wantedState = ATTACK_LIGHT;
			wantedTag = 1;
			break;
		case HEAVY_ATTACK:
			wantedState = ATTACK_HEAVY;
			wantedTag = 2;
			break;
		case JUMPINPUT:
			wantedState = JUMP;
			wantedTag = 3;
			break;
		case RUNINPUT:
			wantedState = RUN;
			break;
		case DEFEND:
			wantedState = PROTECT;
			break;
		case TAUNTINPUT:
			wantedState = TAUNT;
			break;
		case PARRYINPUT:
			wantedState = PARRY;
			break;
		default:
			break;
		}
	}


}

void Character::UpdateMainStates()
{


	if (wantedTag != 0 && GetAtk(wantedTag)->ability)
	{
		if (!GetAbAtk(wantedTag)->active)
		{
			wantedTag = 0;
			wantedState = IDLE;
		}
		else
			GetAbAtk(wantedTag)->Activate();
	}

	if (currentState == WALK || currentState == IDLE)
	{
		currentState = wantedState;
		if (wantedTag != 0)
		{
			if (last_attack != 0)
			{
				SetCombo();
			}
			else
			{
				currentTag = wantedTag;
				last_attack = currentTag;
			}
			time_attack.Start();
		}
		else
		{
			currentTag = 0;
		}
	}
	else if (currentState == RUN)
	{
		if (wantedState != RUN)
			currentState = STOP;
	}
	
	if (currentState == JUMP || (currentState == AD_ACTION && currentTag != 0 &&  GetAtk(currentTag)->air))
	{
		last_attack = 3;
		if (StateisAtk(wantedState))
		{
			SetCombo();
			time_attack.Start();
		}
		if ((currentState == AD_ACTION && GetAtk(currentTag)->air) && currentAnimation->Finished())
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
		
		if (StateisAtk(currentState))
			time_attack.Start();

		currentState = wantedState;

		currentAnimation->Reset();

		if (wantedTag != 0)
		{
			if (last_attack != 0)
			{
				SetCombo();
			}
			else
			{
				currentTag = wantedTag;
				last_attack = currentTag;
			}
			time_attack.Start();
		}
		else
		{
			currentTag = 0;
		}
	}

	if (time_attack.Count(COMBO_MARGIN))
	{
		last_attack = 0;
	}
}

void Character::UpdateCurState(float dt)
{
	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;


	if (gamepos.y > 0)
	{
		if ((currentState != AD_ACTION && currentTag != 0 && !GetAtk(currentTag)->air) ||currentState == JUMP)
			Accelerate(x_dir, -2, z_dir, dt);
		else
			Accelerate(x_dir, -1, z_dir, dt);
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

	}
}

void Character::UpdateAnimation()
{
	if (!StateisAtk(currentState))
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
	else
	{
		for (std::list<Attack*>::iterator item = attacks.begin(); item != attacks.end(); item++)
		{
			if ((*item)->tag == currentTag)
			{
				currentAnimation = &(*item)->anim;
				break;
			}
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
				_this->entity->Impulsate(hit_dir * 8000, 0, 0);
			}
			App->audio->PlayFx(10);
		}
		else if (_this->sTag == "enemy_attack" && _other->sTag == "player_shield")
		{
			_this->entity->Impulsate(hit_dir * 8000, 0, 0);
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
	return (state == ATTACK_LIGHT || state == ATTACK_HEAVY || state == AD_ACTION);
}

Attack* Character::GetAtk(uint atk)
{
	Attack* ret = nullptr;

	for (std::list<Attack*>::iterator item = attacks.begin(); item != attacks.end(); item++) {
		if ((**item).tag == atk)
		{
			ret = (*item);
		}
	}
	return ret;
}

Ability* Character::GetAbAtk(uint atk)
{

	Ability* ret = nullptr;
	for (std::list<Ability>::iterator item = abilities.begin(); item != abilities.end(); item++) 
	{

		if (item->atk->tag == atk)
		{
			ret = &(*item);
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
	Attack* wanted_atk = GetAtk(wantedTag);
	Attack* current_atk = GetAtk(last_attack);

	if (current_atk != nullptr && wanted_atk != nullptr && current_atk->CheckChildInput(wanted_atk->input))
	{
		current_atk = current_atk->GetChildInput(wanted_atk->input);
		currentState = AD_ACTION;
		last_attack = current_atk->tag;
		currentTag = last_attack;
	}
	else
	{
		currentTag = wantedTag;
		last_attack = currentTag;
	}

}

void Character::LoadState(CharStateEnum _state, std::string animationName)
{
	State* st = new State;
	st->state = _state;

	st->anim.LoadAnimationsfromXML(animationName, animations_name);
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
	LoadState(PROTECT, "protect");
	LoadState(PARRY, "standup");

}

void Character::UpdateTag(uint& t)
{
	if (currentState == ATTACK_LIGHT)
		t = 1;
	else if (currentState == ATTACK_HEAVY)
		t = 2;
	else
		t = 0;
}

void Character::UpdateAbilities()
{
	for (std::list<Ability>::iterator item = abilities.begin(); item != abilities.end(); item++)
	{
		if (item->timer.Count(item->cooldown))
		{
			item->active = true;
		}
	}
}

void Character::AdAbility(Ability ab)
{
	abilities.push_back(ab);
}

void Character::UpdateEventStates()
{
	std::list<EventState*>::iterator item = eventstates.begin(); 

	while (item != eventstates.end())
	{
		if ((*item)->timer.Count((*item)->time_active))
		{
			stats = stats - (*item)->stats;
			item = eventstates.erase(item);
		}
		else
		{
			item++;
		}
	}

}

void Character::AdBuff(float time, float spd, float atk, float def)
{
	EventState* buff = new EventState(time, atk, def, spd);
	stats =  stats + buff->stats;

	eventstates.push_back(buff);

}

void Character::SetAnimations()
{
	switch (charType)
	{
	case FIGHTER:
		animations_name = HERO_SPRITE_ROOT;
		sprites = App->textures->Load("Characters/Fighter_sprites_green.png");
		break;
	case ELF:
		animations_name = ELF_SPRITE_ROOT;
		sprites = App->textures->Load("Characters/Elf_sprites.png");
		break;
	}

}

void Character::SetCharType(CharacterTypes type)
{
	charType = type;
}
