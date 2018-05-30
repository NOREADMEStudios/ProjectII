#include "Warrior.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "Entity.h"

#include "../Game/Spells/DeathMark.h"


#include "ModuleMap.h"
#include "App.h"




Warrior::Warrior() : Character(CharacterTypes::WARRIOR)
{
}

Warrior::~Warrior()
{
}


bool Warrior::Awake(pugi::xml_node&)
{
	return true;
}

bool Warrior::HeroStart()
{
	partner = (Character*)App->entities->GetSameTeam(this);

	stats.spd = 180;
	stats.life = 100;
	stats.atk = 6;
	stats.def = 2;

	LoadState(PROTECT, "protect");
	LoadState(PARRY, "standup");
	LoadState(RUN, "run");


	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack",animations_name, 2);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "kick", animations_name, 2, 40, false);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, 20, true);
	Attack* light_2 = new Attack(4, LIGHT_ATTACK, "attack_knee", animations_name, 2, 40);
	Attack* heavy_2 = new Attack(5, HEAVY_ATTACK, "strong_attack", animations_name, 2);
	Attack* light_3 = new Attack(6, LIGHT_ATTACK, "attack_2", animations_name, 5);
	Attack* jump_a2 = new Attack(7, LIGHT_ATTACK, "jump_attack", animations_name, 2, 30, true);
	Attack* jump_a3 = new Attack(8, HEAVY_ATTACK, "windwhirl", animations_name, 5, 50, true);
	Attack* ab_1 = new Attack(11, AB_1, "slide", animations_name, 5, 40, false, true);
	Attack* ab_2 = new Attack(12, AB_2, "slide_2", animations_name, 5, 40, false, true);
	Attack* ulti = new Attack(13, AB_3, "intro",animations_name, 0, 20,false, true);

	attacks.push_back(light_1);
	attacks.push_back(light_2);
	attacks.push_back(light_3);
	attacks.push_back(heavy_1);
	attacks.push_back(heavy_2);
	attacks.push_back(jump_a);
	attacks.push_back(jump_a2);
	attacks.push_back(jump_a3);
	attacks.push_back(ab_1);
	attacks.push_back(ab_2);
	attacks.push_back(ulti);

	light_1->AddChild(light_2);
	light_2->AddChild(light_3);
	heavy_1->AddChild(light_3);
	light_2->AddChild(heavy_2);
	heavy_1->AddChild(heavy_2);
	jump_a->AddChild(jump_a2);
	jump_a->AddChild(jump_a3);

	Ability* kick = new Ability(ab_1, 3 - ((stats.cdr / 100) * 3));
	kick->ab_sprite = {202, 115, 50,50};
	AdAbility(*kick);

	Ability* stunt = new Ability(ab_2, 5 - ((stats.cdr / 100) * 5));
	stunt->ab_sprite = { 252, 115, 50,50 };
	AdAbility(*stunt);

	Aulti = new Ability(ulti, 20 - ((stats.cdr / 100) * 20));
	Aulti->ab_sprite = { 102, 165, 50,50 };
	AdAbility(*Aulti);

	ulti_sp = App->entities->CreateSpell({ DEATH_MARK ,team,  {0,0,0} });
	((DeathMark*)ulti_sp)->SetPath("battle_cry");

	return true;
}

bool Warrior::HeroUpdate(float dt)
{

	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;

	partner = (Character*)App->entities->GetSameTeam(this);

	switch (currentState)
	{
		case PROTECT:
		{
			max_speed = stats.spd * 0.5f;
			Accelerate((x_dir * stats.spd), 0, (z_dir * stats.spd), dt);
			break;
		}
		case RUN:
		{
			max_speed = stats.spd * 1.5f;
			Accelerate((x_dir * stats.spd), 0, (z_dir * stats.spd), dt);
			break;
		}
	}


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





	if (GetAbAtk(11)->active && ab_1_bool)
	{
		ab_1_bool = false;
	}


	if (GetAbAtk(12)->active && ab_2_bool)
	{
		ab_2_bool = false;
	}

	if (GetAbAtk(13)->active && buffed)
	{
		buffed = false;
	}

	CreateSounds();

return true;
}


bool Warrior::PostUpdate()
{
	return true;
}

bool Warrior::CleanUp(pugi::xml_node&)
{

	return true;
}

void Warrior::UpdateSpecStates()
{

	int dir = 0;

	if (flip)
		dir = -1;
	else
		dir = 1;

	if (currentState == PARRY)
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
	else if (currentState == RUN)
	{
		if (wantedState != RUN)
			currentState = STOP;
	}

	if (currentTag == 11 && !ab_1_bool)
	{
		Impulsate(3 * dir, 0, 0);
		ab_1_bool = true;
	}

	if (currentTag == 12 && !ab_2_bool)
	{
		Impulsate(3.5f * dir, 0, 0);
		ab_2_bool = true;
	}


	if (currentTag == 13 && !buffed && currentAnimation == &Aulti->atk->anim && currentAnimation->getFrameIndex() >= 6)
	{
		buffed = true;
		AdBuff(10, 100, 10, 10);
		buffed = true;
		DeathMark* bp = new DeathMark{ *(DeathMark*)ulti_sp };
		bp->SetParent(this);
		bp->SetPos(gamepos.x, gamepos.y, gamepos.z - 1);
		bp->cl = true;
		bp->Start();
		bp->SetBack();

		
		if (partner != nullptr)
		{
			partner->AdBuff(10, 100, 10, 10);
			DeathMark* bp_2 = new DeathMark{ *(DeathMark*)ulti_sp };
			bp_2->SetParent(partner);
			bp_2->SetPos(gamepos.x, gamepos.y, gamepos.z - 1);
			bp_2->cl = true;
			bp_2->Start();
			bp->SetBack();
		}
	}


}

void Warrior::OnCollisionEnter(Collider* _this, Collider* _other)
{
	if (_this->entity == _other->entity) return;
	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;


		if (_this->entity->GetGamePos().x - _other->entity->GetGamePos().x > 0)
		{
			hit_dir = -1;
		}
		else
		{
			hit_dir = 1;
		}

		if (_this->type == Collider::DEF && _other->type == Collider::ATK)
		{
			if (_other->entity->breaking)
			{
				currentState = HIT;
				App->SetTimeScale(0.f, hitStopFrames);
				stats.life -= _other->entity->stats.atk;
				hit_bool = true;
			}
			else
			{
				_other->entity->max_speed = 400;
				_other->entity->Accelerate(hit_dir * 400, 0, 0, 1);
			}
			
		}
		else if (_this->type == Collider::ATK && _other->type == Collider::DEF)
		{
			_other->entity->max_speed = 400;
			_other->entity->Accelerate(hit_dir * 400, 0, 0, 1);
		}
		else if (_this->type == Collider::PARRY && _other->type == Collider::ATK)
		{
			currentState = IDLE;
			parried = true;
		}
		else if (_this->type  == Collider::ATK && _other->type == Collider::PARRY)
		{
			currentTag = 0;
			currentState = HIT;
			App->SetTimeScale(0.f, hitStopFrames);
		}
		else if (_this->type == Collider::HITBOX && (_other->type == Collider::ATK || _other->type == Collider::SPELL))
		{
			currentTag = 0;
			currentState = HIT;
			App->SetTimeScale(0.f, hitStopFrames);
			hit_bool = true;
		}
		else if ((_this->type == Collider::ATK || _this->type == Collider::PARRY) && _other->type == Collider::HITBOX && StateisAtk(currentState))
		{

			Attack * atk = GetAtk(currentTag);
			int dmg = _this->entity->stats.atk + atk->damage - _other->entity->stats.def;
			if (dmg <= 0)
			{
				dmg = 1;
			}
			if (atk != nullptr)
				_other->entity->stats.life -= dmg;
			
			if (currentTag == 5 && _other->entity->type == CHARACTER/* &&  ((Character*)(_other->entity))->GetState() != KNOKED*/)
			{
				_other->entity->max_speed = 500;
				_other->entity->max_speed_y = 1000;
				_other->entity->Accelerate(hit_dir * 5, 500, 0, 1);
				_other->entity->SetFlip(hit_dir);
			}

			if (currentTag == 11 && (((Character*)_other->entity)->GetState() != STUNED && ((Character*)_other->entity)->GetState() != HIT))
			{
				_other->entity->AdBuff(3, -_other->entity->stats.spd);
				DeathMark* stun = new DeathMark{ *(DeathMark*)App->entities->stuned };
				stun->SetPos(gamepos.x, gamepos.y + 20, gamepos.z);
				stun->SetParent((Character*)_other->entity);
				stun->Start();
			}
			else if (currentTag == 12)
			{
				_other->entity->max_speed = 800;
				_other->entity->Accelerate(hit_dir * 800, 0, 0, 1);
			}

		}


	
}

void Warrior::CreateSounds()
{
	switch (currentState)
	{
	case IDLE:
		if (!sound_avaliable)
		{
			sound_avaliable = true;
		}
		break;
	case JUMP:
		if (sound_avaliable)
		{
			App->audio->PlayFx(5);
			sound_avaliable = false;
		}
		break;
	case STOP:
		if (sound_avaliable)
		{
			App->audio->PlayFx(9);
			sound_avaliable = false;
		}
	case HIT:
		if (sound_avaliable)
		{
			App->audio->PlayFx(3);
			sound_avaliable = false;
		}
		break;
	case DEATH:
		if (sound_avaliable)
		{
			App->audio->PlayFx(8);
			sound_avaliable = false;
		}
		break;
	case PARRIED:
		if (sound_avaliable)
		{
			App->audio->PlayFx(13);
			sound_avaliable = false;
		}
		break;
	case TAUNT:
		if (sound_avaliable)
		{
			App->audio->PlayFx(12);
			sound_avaliable = false;
		}
		break;
	default:
		break;
	}

	if (currentTag != 0)
	{
		switch (currentTag)
		{
		case 1:
			if (sound_avaliable)
			{
				App->audio->PlayFx(1);
				sound_avaliable = false;
			}
			break;
		case 2:
			if (sound_avaliable)
			{
				App->audio->PlayFx(10);
				sound_avaliable = false;
			}
			break;
		case 4:
			if (sound_avaliable)
			{
				App->audio->PlayFx(1);
				sound_avaliable = false;
			}
			break;
		case 5:
			if (sound_avaliable)
			{
				App->audio->PlayFx(11);
				sound_avaliable = false;
			}
			break;
		case 6:
			if (sound_avaliable)
			{
				App->audio->PlayFx(1);
				sound_avaliable = false;
			}
			break;
		case 7:
			if (sound_avaliable)
			{
				App->audio->PlayFx(1);
				sound_avaliable = false;
			}
			break;
		case 8:
			break;
		case 11:
			if (sound_avaliable)
			{
				App->audio->PlayFx(14);
				sound_avaliable = false;
			}
			break;
		case 12:
			if (sound_avaliable)
			{
				App->audio->PlayFx(15);
				sound_avaliable = false;
			}
			break;
		case 13:
			if (sound_avaliable)
			{
				App->audio->PlayFx(16);
				sound_avaliable = false;
			}
			break;
		default:
			break;
		}
	}
	
}