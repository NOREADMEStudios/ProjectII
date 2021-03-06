#include "Rogue.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "Spells.h"
#include "../Game/Spells/DeathMark.h"
#include "../Game/Spells/Dagger.h"

#include "ModuleMap.h"
#include "App.h"




Rogue::Rogue() : Character(CharacterTypes::ROGUE)
{
}

Rogue::~Rogue()
{
}


bool Rogue::Awake(pugi::xml_node&)
{
	return true;
}

bool Rogue::HeroStart()
{
	LoadState(RUN, "run");
	LoadState(PROTECT, "protect");
	LoadState(KNOKED, "ko");
	LoadState(STAND_UP, "roll");

	stats.spd = 180;
	stats.life = 100;
	stats.atk = 8;
	stats.def = 1;


	dagger = App->entities->CreateSpell({ DAGGER,team ,{ gamepos.x, gamepos.y + 50, gamepos.z },{ 0,0 } });
	dagger->SetParent(this);

	death_m = App->entities->CreateSpell({ DEATH_MARK , team,{ 0,0,0 } });
	((DeathMark*)death_m)->SetPath("death_mark");


	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "L_Attack_2", animations_name, 1);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "H_Attack", animations_name, 5, 40);
	Attack* heavy_2 = new Attack(7, HEAVY_ATTACK, "H_Attack2", animations_name, 5, 40);
	Attack* heavy_3 = new Attack(6, HEAVY_ATTACK, "H_Attack3", animations_name, 5, 40);
	Attack* crouch = new Attack(4, LIGHT_ATTACK, "L_Attack_3", animations_name, 1);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, 30, true);
	Attack* jump_a2 = new Attack(5, LIGHT_ATTACK, "jump_attack", animations_name, 0, 40, true);


	Attack* ab_1 = new Attack(11, AB_1, "stop", animations_name, 0, 20, false, true);
	Attack* ab_2 = new Attack(12, AB_2, "dagger", animations_name, 0,20, false, true);


	Attack* ab_3 = new Attack(13, AB_3, "dash", animations_name, 0, 30, false, true);

	attacks.push_back(light_1);
	attacks.push_back(heavy_1);
	attacks.push_back(heavy_2);
	attacks.push_back(heavy_3);
	attacks.push_back(crouch);
	attacks.push_back(jump_a);
	attacks.push_back(ab_1);
	attacks.push_back(ab_2);
	attacks.push_back(ab_3);
	attacks.push_back(jump_a2);

	light_1->AddChild(crouch);
	heavy_1->AddChild(heavy_2);
	heavy_2->AddChild(heavy_3);
	crouch->AddChild(heavy_2);
	jump_a->AddChild(jump_a2);

	behindU = new Ability(ab_2, 4 - ((stats.cdr / 100) * 4));
	behindU->ab_sprite = { 202,165, 50,50 };
	parry = new Ability(ab_1, 3 - ((stats.cdr / 100) * 3));
	parry->ab_sprite = { 152, 165, 50, 50 };

	ulti = new Ability(ab_3, 8 - ((stats.cdr / 100) * 8));
	ulti->ab_sprite = { 253, 165, 50,50 };

	AdAbility(*parry);
	AdAbility(*behindU);
	AdAbility(*ulti);
	return true;
}

bool Rogue::HeroUpdate(float dt)
{
	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;
	switch (currentState)
	{
	case RUN:
	{
		max_speed = stats.spd * 1.5f;
		Accelerate((x_dir * stats.spd), 0, (z_dir * stats.spd), dt);
		break;
	}
	case AD_ACTION:
	case ATTACK_LIGHT:
	case ATTACK_HEAVY:
	{
		int dir = 0;

		if (flip)
			dir = 1;
		else
			dir = -1;

		if (currentTag == 1 ||  currentTag == 2 || currentTag == 4)
		{
			max_speed = stats.spd * 0.3f;
			Accelerate((x_dir * stats.spd), 0, (z_dir * stats.spd), dt);
		}
		else if (currentTag == 7 && currentAnimation->getFrameIndex() == 4)
		{
			max_speed = stats.spd;
			Accelerate((dir * stats.spd), 0, (z_dir * stats.spd), dt);
		}
		else if (currentTag == 6 && currentAnimation->getFrameIndex() == 3)
		{
			max_speed = stats.spd * 1.5f;
			Accelerate((dir * stats.spd), 0, (z_dir * stats.spd), dt);
		}

		break;
	}
	}



	if (currentState != PROTECT && !StateisAtk(currentState)) {
		if (directions.right - directions.left == 1)
		{
			flip = true;
		}
		else if (directions.right - directions.left == -1)
		{
			flip = false;
		}
	}



	if (GetAbAtk(12)->active && ab_2_active)
	{
		ab_2_active = false;
	}

	if (!GetAbAtk(11)->active)
	{
		ab_1_active = true;
	}
	else
	{
		ab_1_active = false;
	}

	if (!GetAbAtk(13)->active)
	{
		ab_3_active = true;
	}
	else
	{
		ab_3_active = false;
	}

	CreateSounds();

	return true;
}

bool Rogue::PostUpdate()
{
	return true;
}



void Rogue::UpdateSpecStates()
{
	int dir = 0;

	if (flip)
		dir = 1;
	else
		dir = -1;

	if (currentState == PROTECT && wantedState != PROTECT)
	{
		currentState = wantedState;
		currentAnimation->Reset();
	}


	if (currentTag == 12 && !ab_2_active && currentAnimation == &behindU->atk->anim && currentAnimation->getFrameIndex() >= 8)
	{
		Dagger* dag = new Dagger{ *(Dagger*)dagger };
		dag->SetPos(gamepos.x, gamepos.y + 50, gamepos.z);
		dag->SetDir(dir ,0 );
		dag->Start();

		ab_2_active = true;
		
	}
	else if (currentTag == 11 && !ab_1_active)
	{

		//Impulsate(2 * dir, 0, 0);
		ab_1_active = true;
	}
	else if (currentTag == 13 && !ab_3_active)
	{

		Impulsate(4 * dir, 0, 0);
		ab_3_active = true;
	}

	else if (currentState == RUN)
	{
		if (wantedState != RUN)
			currentState = wantedState;
	}
}

void Rogue::OnCollisionEnter(Collider* _this, Collider* _other)
{
	if (_this->entity == _other->entity) return;
	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;

	{
		if (_this->collider.x - _other->collider.x > 0)
		{
			hit_dir = 1;
		}
		else
		{
			hit_dir = -1;
		}

		if (_this->type == Collider::DEF && _other->type == Collider::ATK)
		{
			if (_other->entity->breaking)
			{
				currentAnimation->Reset();
				currentState = HIT;
				//App->SetTimeScale(0.f, hitStopFrames);
				stats.life -= _other->entity->stats.atk;
				hit_bool = true;
			}
			else
			{
				_this->entity->Impulsate(hit_dir * 10, 0, 0);
			}
			
		}
		else if (_this->type == Collider::ATK && _other->type == Collider::DEF)
		{
			_this->entity->Impulsate(hit_dir * 10, 0, 0);
		}
		else if (_this->type == Collider::PARRY && _other->type == Collider::ATK)
		{
			currentState = IDLE;
			parried = true;
		}
		else if (_this->type == Collider::ATK && _other->type == Collider::PARRY)
		{
			currentAnimation->Reset();
			currentTag = 0;
			currentState = HIT;
			//App->SetTimeScale(0.f, hitStopFrames);
		}
		else if (_this->type == Collider::HITBOX && (_other->type == Collider::ATK || _other->type == Collider::SPELL))
		{
			currentAnimation->Reset();
			currentState = HIT;
			//App->SetTimeScale(0.f, hitStopFrames);
			currentTag = 0;
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
		else if (_this->type == Collider::ATK && _other->type == Collider::HITBOX && StateisAtk(currentState))
		{
			Attack * atk = GetAtk(currentTag);
			int dmg = _this->entity->stats.atk + atk->damage - _other->entity->stats.def;
			if (dmg <= 0)
			{
				dmg = 1;
			}

			if (atk != nullptr)
				_other->entity->stats.life -= dmg;

			if (currentTag == 11)
				_other->entity->AdBuff(3 - ((_other->entity->stats.ccr/100)*3), -_other->entity->stats.spd);
			else if (currentTag == 12)
				_other->entity->Impulsate(hit_dir, 0, 0);
			else if (currentTag == 13)
			{
				if (stats.spd > _other->entity->stats.spd)
					dmg += stats.spd - _other->entity->stats.spd;
				
				DeathMark* death = new DeathMark{ *(DeathMark*)death_m };
				death->SetParent((Character*)_other->entity);
				death->Start();
				death->SetLifeTime((10 - ((_other->entity->stats.ccr / 100) * 10)) * 1000);

				_other->entity->AdBuff(10 - ((_other->entity->stats.ccr / 100) *10), 0, -5, -5);
			}

		}

	}
}

void Rogue::CreateSounds()
{
	switch (currentState)
	{
	case IDLE:
	case WALK:
		if (!sound_avaliable)
		{
			sound_avaliable = true;
		}
		break;
	case JUMP:
		if (sound_avaliable)
		{
			App->audio->PlayFx(23);
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
			App->audio->PlayFx(21);
			sound_avaliable = false;
		}
		break;
	case DEATH:
		if (sound_avaliable)
		{
			App->audio->PlayFx(22);
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
			App->audio->PlayFx(26);
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
				App->audio->PlayFx(24);
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
				App->audio->PlayFx(1);
				sound_avaliable = false;
			}
			break;
		case 6:
			if (sound_avaliable)
			{
				App->audio->PlayFx(25);
				sound_avaliable = false;
			}
			break;
		case 7:
			if (sound_avaliable)
			{
				App->audio->PlayFx(24);
				sound_avaliable = false;
			}
			break;
		case 11:
			if (sound_avaliable)
			{
				App->audio->PlayFx(13);
				sound_avaliable = false;
			}
			break;
		case 12:
			if (sound_avaliable)
			{
				App->audio->PlayFx(27);
				sound_avaliable = false;
			}
			break;
		case 13:
			if (sound_avaliable)
			{
				App->audio->PlayFx(7);
				sound_avaliable = false;
			}
			break;
		default:
			break;
		}
	}

}