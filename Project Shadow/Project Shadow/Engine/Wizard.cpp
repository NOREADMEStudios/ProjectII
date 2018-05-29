#include "Wizard.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

#include "ModuleMap.h"
#include "App.h"
#include "..\Game\Spells\Lightning.h"
#include "..\Game\Spells\Icicle.h"
#include "..\Game\Spells\FireBall.h"
#include "..\Game\Spells\FireDemon.h"



Wizard::Wizard() : Character(CharacterTypes::WIZARD)
{
}

Wizard::~Wizard()
{
}


bool Wizard::Awake(pugi::xml_node&)
{
	return true;
}

bool Wizard::HeroStart()
{

	stats.spd = 180;
	stats.life = 100;
	stats.atk = 2;
	stats.def = 0;

	icicle = App->entities->CreateSpell({ ICICLE,team,{ gamepos.x, gamepos.y , gamepos.z} });

	lighting = (Lightning*)App->entities->CreateSpell({ LIGHTING,team,{ gamepos.x, gamepos.y + 75, gamepos.z } });

	fireball = App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x + 50, gamepos.y , gamepos.z },{ 1,0 } });

	fireDemon = App->entities->CreateSpell({ FIRE_DEMON,team,{ gamepos.x, gamepos.y + 50 , gamepos.z } });

	light_aura = App->entities->CreateSpell({ SpellsType::LIGHTNING_AURA, team,{ gamepos.x, gamepos.y, gamepos.z } });

	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack_1", animations_name, 3);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "attack_dagger", animations_name, 5);
	Attack* crouch = new Attack(4, LIGHT_ATTACK, "attack_crouch", animations_name, 1);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, 20, true);
	Attack* jump_a2 = new Attack(5, LIGHT_ATTACK, "attack_j1", animations_name, 0, 20, true);
	Attack* ab_1 = new Attack(11, AB_1, "attack_m1", animations_name, 0, 20, false, true);
	Attack* ab_2 = new Attack(12, AB_2, "attack_m2", animations_name, 0, 20, false, true);
	Attack* ab_3 = new Attack(13, AB_3, "win", animations_name, 0,20, false, true);

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

	icicle_ab = new Ability(ab_1, 3 - ((stats.cdr / 100) * 3));
	icicle_ab->ab_sprite = {152,65, 50,50 };

	thunder = new Ability(ab_2, 5 - ((stats.cdr / 100) * 5));
	thunder->ab_sprite = { 202, 65,50,50 };
	ulti = new Ability(ab_3, 10 - ((stats.cdr / 100) * 10));
	ulti->ab_sprite = { 102, 115, 50,50 };

	AdAbility(*icicle_ab);
	AdAbility(*thunder);
	AdAbility(*ulti);
	return true;
}

bool Wizard::HeroUpdate(float dt)
{
	if (!noMove.IsZero() && noMove.Read() > LIGHTNING_MS_LIFETIME) {
		noMove.SetZero();	
	}

	if (!StateisAtk(currentState)) 
	{
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


    if (GetAbAtk(12)->active && ab_2_bool)
	{
		ab_2_bool = false;
	}

	if (GetAbAtk(13)->active && ab_3_bool )
	{
		ab_3_bool = false;
	}



	CreateSounds();

	return true;
}

bool Wizard::PostUpdate()
{
	return true;
}

bool Wizard::CleanUp(pugi::xml_node&)
{

	return true;
}

void Wizard::UpdateSpecStates()
{
	int dir = 0;

	if (flip)
		dir = -1;
	else
		dir = 1;

	if (start_pose)
	{
		if (pose_bool)
		{
			pose_timer.Start();
			pose_bool = false;
		}

		if (pose_timer.Count(pose_wait))
		{
			currentAnimation->speed = prevAnimSpeed;
			start_pose = false;
		}
		else
		{
			currentAnimation->speed = 0;
		}
	}

	if (currentTag == 11 && !ab_1_active && currentAnimation == &icicle_ab->atk->anim && currentAnimation->getFrameIndex() >= 6)
	{
		Icicle* ice = new Icicle{ *(Icicle*)icicle };
		ice->SetPos(gamepos.x+(100*dir), gamepos.y + 400, gamepos.z);
		ice->SetDir(flip, 0);
		ice->Start();
		ab_1_active = true;
	}
	if (currentTag == 12 && !ab_2_bool && currentAnimation == &thunder->atk->anim && currentAnimation->getFrameIndex() >= 3)
	{
		Lightning* light = nullptr;
		light = new Lightning( *lighting );
		light->SetPos(gamepos.x + (50 * dir), gamepos.y + 50 , gamepos.z);
		light->SetDir(flip, 0);
		light->Start();

		Aura* light_a = nullptr;
		light_a = new Aura{ *(Aura*)light_aura };
		light_a->SetPos(gamepos.x + (50 * dir), gamepos.y + 50, gamepos.z + 1);
		light_a->SetDir(flip, 0);
		light_a->Start();
		ab_2_bool = true;
		
		pose_bool = true;
		start_pose = true;
		pose_wait = 1.5f;
		prevAnimSpeed = thunder->atk->anim.speed;
			
	}
	if (currentTag == 13 && !ab_3_bool && currentAnimation == &ulti->atk->anim && currentAnimation->getFrameIndex() >= 12)
	{
		int map_border_x = App->map->GetMapBorders_X();
		int map_border_z = App->map->GetMapBorders_Z();


		for (int i = 0; i <= 10; i++)
		{
			FireBall* fb = new FireBall{ *(FireBall*)fireball };
			fb->SetPos( map_border_x + 50 , gamepos.y,  map_border_z + (i * 25));
			fb->SetDir(1, 0);
			fb->Start();
		}

		FireDemon* fired = new FireDemon{ *(FireDemon*)fireDemon };
		fired->SetPos(gamepos.x, gamepos.y + 50, gamepos.z);
		fired->Start();
		ab_3_bool = true;


	}
}

void Wizard::OnCollisionEnter(Collider* _this, Collider* _other)
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
		if (_this->entity->GetGamePos().x - _other->entity->GetGamePos().x > 0)
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
				App->SetTimeScale(0.f, hitStopFrames);
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
			App->SetTimeScale(0.f, hitStopFrames);
		}
		else if (_this->type == Collider::HITBOX && (_other->type == Collider::ATK || _other->type == Collider::SPELL))
		{
			currentTag = 0;
			currentState = HIT;
			App->SetTimeScale(0.f, hitStopFrames);
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

			if (currentTag == 11)
				_other->entity->AdBuff(3, -_other->entity->stats.spd);
			else if (currentTag == 12)
				_other->entity->Impulsate(hit_dir, 0, 0);
		}

	}
}

void Wizard::CreateSounds()
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
				App->audio->PlayFx(1);
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
				App->audio->PlayFx(17);
				sound_avaliable = false;
			}
			break;
		case 12:
			if (sound_avaliable)
			{
				App->audio->PlayFx(19);
				sound_avaliable = false;
			}
			break;
		case 13:
			if (sound_avaliable)
			{
				App->audio->PlayFx(20);
				sound_avaliable = false;
			}
			break;
		default:
			break;
		}
	}

}