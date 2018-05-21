#include "Cleric.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "Entity.h"
#include "../Game/Spells/DeathMark.h"

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

	ab_duration = 5;

	LoadState(PROTECT, "protect");
	LoadState(PARRY, "standup");
	LoadState(RUN, "run");

	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack", animations_name, 1);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "attack_2", animations_name, 5);
	Attack* crouch = new Attack(4, LIGHT_ATTACK, "attack_3", animations_name, 2);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, true);
	Attack* jump_a2 = new Attack(5, LIGHT_ATTACK, "attack_j1", animations_name, 0, true);
	Attack* ab_1 = new Attack(11, AB_1, "ab_1", animations_name, 0, 20, false, true);
	Attack* ab_2 = new Attack(12, AB_2, "ab_2", animations_name, 0, 20, false, true);
	Attack* ab_3 = new Attack(13, AB_3, "ab_3", animations_name, 0, 20, false, true);

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

	Ability* fire = new Ability(ab_1, 6 - ((stats.cdr/100)*6));
	fire->ab_sprite = { 303,65, 50,50 };
	Ability* thunder = new Ability(ab_2, 7 - ((stats.cdr / 100) * 7));
	thunder->ab_sprite = { 303,115, 50,50 };
	Ability* ulti = new Ability(ab_3, 15 - ((stats.cdr / 100) * 15));
	ulti->ab_sprite = { 303,165, 50,50 };

	AdAbility(*fire);
	AdAbility(*thunder);
	AdAbility(*ulti);

	return true;
}

bool Cleric::HeroUpdate(float dt)
{


	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;

	if (!ab_timer.Count(ab_duration))
	{
		if (partner  != nullptr)
		partner->cleric_ab = true;

		cleric_ab = true;
	}
	else
	{
		if (partner != nullptr)
		partner->cleric_ab = false;

		cleric_ab = false;
	}

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

	if (!GetAbAtk(11)->active)
	{
		ab_1_active = true;
	}
	else
	{
		ab_1_active = false;
	}

	if (!GetAbAtk(12)->active)
	{
		ab_2_active = true;
	}
	else
	{
		ab_2_active = false;
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

bool Cleric::PostUpdate()
{
	return true;
}

bool Cleric::CleanUp(pugi::xml_node&)
{

	return true;
}

void Cleric::UpdateSpecStates()
{
	partner = (Character*)App->entities->GetSameTeam(this);
	Point3D pgp;

	if (partner != nullptr)
	pgp = partner->GetGamePos();

	if (currentTag == 11 && !ab_1_active)
	{
		Spells* dm = App->entities->CreateSpell({ DEATH_MARK , team,{ 0,0,0 } });
		dm->SetParent(this);
		((DeathMark*)dm)->SetPath("Blessing_Protection");
		((DeathMark*)dm)->cl = true;

		if (partner != nullptr)
		{
		Spells* dm2 = App->entities->CreateSpell({ DEATH_MARK , team,pgp });
		dm2->SetParent(partner);

		((DeathMark*)dm2)->SetPath("Blessing_Protection");
		((DeathMark*)dm2)->cl = true;
		}

		ab_timer.Start();
	}
	if (currentTag == 12 && !ab_2_active)
	{
		AdBuff(5, 0, 0, 10);

		Spells* bs = App->entities->CreateSpell({ DEATH_MARK , team,{ 0,0,0 } });
		bs->SetParent(this);
		((DeathMark*)bs)->SetPath("Energy Shield");
		((DeathMark*)bs)->cl = true;

		if (partner != nullptr)
		{
			partner->AdBuff(5, 0, 0, 10);

			Spells* es = App->entities->CreateSpell({ DEATH_MARK , team,pgp });
			es->SetParent(partner);
			((DeathMark*)es)->SetPath("Energy Shield");
			((DeathMark*)es)->cl = true;
		}

	}
	if (currentTag == 13 && !ab_3_active)
	{

		AdHp(35);
		AdBuff(10, 50, 0, 10);
		Spells* hw = App->entities->CreateSpell({ DEATH_MARK , team,{ 0,0,0 } });
		hw->SetParent(this);
		((DeathMark*)hw)->SetPath("Healing Wind");
		((DeathMark*)hw)->cl = true;


		if (partner != nullptr)
		{
			partner->AdHp(35);
			partner->AdBuff(10, 50, 0, 10);
			Spells* hw2 = App->entities->CreateSpell({ DEATH_MARK , team,pgp });
			hw2->SetParent(partner);
			((DeathMark*)hw2)->SetPath("Healing Wind");
			((DeathMark*)hw2)->cl = true;
		}
	}

	if (currentState == PROTECT && wantedState != PROTECT)
	{
		currentState = wantedState;
		currentAnimation->Reset();
	}

}

void Cleric::OnCollisionEnter(Collider* _this, Collider* _other)
{
	if (_this->entity == _other->entity) return;
	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;

	/*int z1 = _this->entity->GetGamePos().z;
	int d1 = _this->entity->GetCharDepth();

	int z2 = _other->entity->GetGamePos().z;
	int d2 = _other->entity->GetCharDepth();

	int p11 = z1 - (d1 / 2);
	int p12 = z1 + (d1 / 2);
	int p21 = z2 - (d2 / 2);
	int p22 = z2 + (d2 / 2);

	if ((p11 <= p21 && p21 <= p12) || (p11 <= p22 && p22 <= p12) || (p21 <= p11 && p11 <= p22) || (p21 <= p12 && p12 <= p22))*/
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
				currentState = HIT;
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
			currentTag = 0;
			currentState = HIT;
		}
		else if (_this->type == Collider::HITBOX && (_other->type == Collider::ATK || _other->type == Collider::SPELL))
		{
			currentTag = 0;
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
		else if (_this->type == Collider::ATK && _other->type == Collider::HITBOX && StateisAtk(currentState))
		{
			Attack * atk = GetAtk(currentTag);
			if (atk != nullptr)
				_other->entity->stats.life -= _this->entity->stats.atk + atk->damage - _other->entity->stats.def;

			//if (currentTag == 11)
			//	_other->entity->AdBuff(3, -_other->entity->stats.spd);
			//else if (currentTag == 12)
			//	_other->entity->Impulsate(hit_dir, 0, 0);
		}

	}
}

void Cleric::CreateSounds()
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