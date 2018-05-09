#include "Warrior.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

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

	/*switch (heroNum) {
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
	}*/


	stats.atk += App->entities->items[heroNum - 1].atk;
	stats.def += App->entities->items[heroNum - 1].def;
	stats.spd += App->entities->items[heroNum - 1].spd;
	stats.life += App->entities->items[heroNum - 1].life;



	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack",animations_name, 2);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "kick", animations_name, 2, false);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, true);
	Attack* light_2 = new Attack(4, LIGHT_ATTACK, "attack_knee", animations_name, 2);
	Attack* heavy_2 = new Attack(5, HEAVY_ATTACK, "strong_attack", animations_name, 2);
	Attack* light_3 = new Attack(6, LIGHT_ATTACK, "attack_2", animations_name, 5);
	Attack* jump_a2 = new Attack(7, LIGHT_ATTACK, "jump_attack", animations_name, 2, true);
	Attack* jump_a3 = new Attack(8, HEAVY_ATTACK, "windwhirl", animations_name, 5, true);
	Attack* ab_1 = new Attack(11, AB_1, "slide", animations_name, 5, false, true);
	Attack* ab_2 = new Attack(12, AB_2, "slide_2", animations_name, 5, false, true);
	Attack* ulti = new Attack(13, AB_3, "intro",animations_name, 0,false, true);

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

	Ability* kick = new Ability(ab_1, 3);
	AdAbility(*kick);

	Ability* stunt = new Ability(ab_2, 5);
	AdAbility(*stunt);

	Ability* Aulti = new Ability(ulti, 10);
	AdAbility(*Aulti);

	return true;
}

bool Warrior::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Warrior::HeroUpdate(float dt)
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

	if ((!GetAbAtk(13)->active)|| (!GetAbAtk(2)->active))
	{
		buffed = true;
	}
	
	else
	{
		buffed = false;
	}

	if (!GetAbAtk(12)->active)
	{
		ab_2_bool = true;
	}
	else
	{
		ab_2_bool = false;
	}

	if (!GetAbAtk(11)->active)
	{
		ab_1_bool = true;
	}
	else
	{
		ab_1_bool = false;
	}


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

	if (currentTag == 13 && !buffed)
	{
		AdBuff(5, 300);
		buffed = true;
	}


	int dir = 0;

	if (flip)
		dir = -1;
	else
		dir = 1;

	if (currentTag == 11 && !ab_1_bool)
	Impulsate( 2 * dir, 0, 0);

	else if (currentTag == 12 && !ab_2_bool)
		Impulsate(3 * dir, 0, 0);

}

void Warrior::OnCollisionEnter(Collider* _this, Collider* _other)
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
				_this->entity->Impulsate(hit_dir * 8000, 0, 0);
			}
			//App->audio->PlayFx(10);
		}
		else if (_this->type == Collider::ATK && _other->type == Collider::DEF)
		{
			_this->entity->Impulsate(hit_dir * 8000, 0, 0);
		}
		else if (_this->type == Collider::PARRY && _other->type == Collider::ATK)
		{
			currentState = IDLE;
			parried = true;
		}
		else if (_this->type  == Collider::ATK && _other->type == Collider::PARRY)
		{
			currentState = HIT;
		}
		else if (_this->type == Collider::HITBOX && _other->type == Collider::ATK)
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
		else if (_this->type == Collider::ATK && _other->type == Collider::HITBOX && StateisAtk(currentState))
		{
			Attack * atk = GetAtk(currentState);
			if (atk != nullptr)
				_other->entity->stats.life -= _this->entity->stats.atk + atk->damage - _other->entity->stats.def;

			if (currentTag == 11)
				_other->entity->AdBuff(3, -_other->entity->stats.spd);
			else if (currentTag == 12)
				_other->entity->Impulsate(hit_dir, 0, 0);
		}

	}
}