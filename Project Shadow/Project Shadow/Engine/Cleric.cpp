#include "Cleric.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "Entity.h"
#include "../Game/Spells/DeathMark.h"
#include "../Game/Spells/ClericSpells.h"

#include "ModuleMap.h"
#include "App.h"




Cleric::Cleric() : Character(CharacterTypes::CLERIC)
{
}

Cleric::~Cleric()
{
}


bool Cleric::Awake(pugi::xml_node&)
{


	return true;
}

bool Cleric::HeroStart()
{
	partner = (Character*)App->entities->GetSameTeam(this);

	stats.spd = 180;
	stats.life = 100;
	stats.atk = 5;
	stats.def = 3;


	LoadState(PROTECT, "protect");
	LoadState(RUN, "run");
	LoadState(KNOKED, "ko");
	LoadState(STAND_UP, "get_up");


	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack", animations_name, 1);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "attack_2", animations_name, 5);
	Attack* crouch = new Attack(4, LIGHT_ATTACK, "attack_3", animations_name, 2);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, 40, true);
	Attack* jump_a2 = new Attack(5, LIGHT_ATTACK, "death_mark", animations_name, 0, 40, true);
	Attack* ab_1 = new Attack(11, AB_1, "maze", animations_name, 0, 40, false, true);
	Attack* ab_2 = new Attack(12, AB_2, "ab_2", animations_name, 0, 40, false, true);
	Attack* ab_3 = new Attack(13, AB_3, "ab_3", animations_name, 0, 40, false, true);

	attacks.push_back(light_1);
	attacks.push_back(heavy_1);
	attacks.push_back(crouch);
	attacks.push_back(jump_a);
	attacks.push_back(ab_1);
	attacks.push_back(ab_2);
	attacks.push_back(jump_a2);
	attacks.push_back(ab_3);

	light_1->AddChild(crouch);
	jump_a->AddChild(jump_a2);


	knock = App->entities->CreateSpell({ CLERIC_STUN,team,{ gamepos.x + 50, gamepos.y , gamepos.z },{ 1,0 } });
	area = App->entities->CreateSpell({ AREA,team,{ gamepos.x, gamepos.y , gamepos.z },{ 0,0 } });
	ulti = App->entities->CreateSpell({ DEATH_MARK,team,{ gamepos.x, gamepos.y , gamepos.z },{ 1,0 } });
	((DeathMark*)ulti)->SetPath("Healing Wind");


	fire = new Ability(ab_1, 6 - ((stats.cdr/100)*6));
	fire->ab_sprite = { 303,65, 50,50 };
	thunder = new Ability(ab_2, 7 - ((stats.cdr / 100) * 7));
	thunder->ab_sprite = { 303,115, 50,50 };
	ulti_ab = new Ability(ab_3, 15 - ((stats.cdr / 100) * 15));
	ulti_ab->ab_sprite = { 303,165, 50,50 };


	AdAbility(*fire);
	AdAbility(*thunder);
	AdAbility(*ulti_ab);

	return true;
}

bool Cleric::HeroUpdate(float dt)
{


	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;



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
		max_speed = stats.spd * 1.2f;
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

	if (GetAbAtk(11)->active && ab_1_active)
	{
		ab_1_active = false;
	}


	if (GetAbAtk(12)->active && ab_2_active)
	{
		ab_2_active = false;
	}

	if (GetAbAtk(13)->active && ab_3_active)
	{
		ab_3_active = false;
	}

	CreateSounds();

	return true;
}

bool Cleric::PostUpdate()
{
	return true;
}



void Cleric::UpdateSpecStates()
{
	partner = (Character*)App->entities->GetSameTeam(this);
	Point3D pgp;

	int dir = 0;

	if (flip)
		dir = -1;
	else
		dir = 1;


	if (partner != nullptr)
	pgp = partner->GetGamePos();

	if (currentTag == 11 && !ab_1_active && currentAnimation == &fire->atk->anim && currentAnimation->getFrameIndex() >= 6)
	{

		Stun* stun = new Stun{ *(Stun*)knock };
		stun->SetPos(gamepos.x + (20 * dir), gamepos.y + 50, gamepos.z -1);
		stun->SetDir(dir, 0);
		stun->Start();


		ab_1_active = true;
	}
	if (currentTag == 12 && !ab_2_active && currentAnimation == &thunder->atk->anim && currentAnimation->getFrameIndex() >= 6)
	{
		AdBuff(5, 0, 0, 10);

		Area* ar = new Area{ *(Area*)area };
		ar->SetParent(this);
		ar->SetPos(gamepos.x, gamepos.y , gamepos.z - 1);
		ar->Start();
		ab_2_active = true;

	}
	if (currentTag == 13 && !ab_3_active && currentAnimation == &ulti_ab->atk->anim && currentAnimation->getFrameIndex() >= 4)
	{

		AdHp(35);
		AdBuff(10, 50, 0, 10);
		DeathMark* bp = new DeathMark{ *(DeathMark*)ulti };
		bp->SetParent(this);
		bp->SetPos(gamepos.x, gamepos.y, gamepos.z - 1);
		bp->cl = true;
		bp->Start();
		ab_2_active = true;

		if (partner != nullptr && partner->stats.life > 0)
		{
			partner->AdHp(35);
			partner->AdBuff(10, 50, 0, 10);
			DeathMark* bp2 = new DeathMark{ *(DeathMark*)ulti };
			bp2->SetParent(partner);
			bp2->SetPos(gamepos.x, gamepos.y, gamepos.z - 1);
			bp2->cl = true;
			bp2->Start();

		}
		ab_3_active = true;
	}

	if (currentState == PROTECT && wantedState != PROTECT)
	{
		currentState = wantedState;
		currentAnimation->Reset();
	}


	else if (currentState == RUN)
	{
		if (wantedState != RUN)
			currentState = wantedState;
	}


}

void Cleric::OnCollisionEnter(Collider* _this, Collider* _other)
{
	if (_this->entity == _other->entity) return;
	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;


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
			//App->audio->PlayFx(10);
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
			currentTag = 0;
			currentState = HIT;
			hit_bool = true;
			//App->SetTimeScale(0.f, hitStopFrames);


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


		}
}

void Cleric::CreateSounds()
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
			App->audio->PlayFx(31);
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
		case 11:
			if (sound_avaliable)
			{
				App->audio->PlayFx(28);
				sound_avaliable = false;
			}
			break;
		case 12:
			if (sound_avaliable)
			{
				App->audio->PlayFx(29);
				sound_avaliable = false;
			}
			break;
		case 13:
			if (sound_avaliable)
			{
				App->audio->PlayFx(30);
				sound_avaliable = false;
			}
			break;
		default:
			break;
		}
	}

}