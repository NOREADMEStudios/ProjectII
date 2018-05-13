#include "Wizard.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "ModuleMap.h"
#include "App.h"
#include "..\Game\Spells\Lightning.h"




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

	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack_1", animations_name, 3);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "attack_dagger", animations_name, 5);
	Attack* crouch = new Attack(4, LIGHT_ATTACK, "attack_crouch", animations_name, 1);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", animations_name, 0, true);
	Attack* jump_a2 = new Attack(5, LIGHT_ATTACK, "attack_j1", animations_name, 0, true);
	Attack* ab_1 = new Attack(11, AB_1, "attack_m1", animations_name, 0, false, true);
	Attack* ab_2 = new Attack(12, AB_2, "attack_m2", animations_name, 0, false, true);
	Attack* ab_3 = new Attack(13, AB_3, "win", animations_name, 0, false, true);

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

	Ability* icicle = new Ability(ab_1, 3);
	icicle->ab_sprite = {152,65, 50,50 };
	Ability* thunder = new Ability(ab_2, 5);
	thunder->ab_sprite = { 202, 65,50,50 };
	Ability* ulti = new Ability(ab_3, 10);
	ulti->ab_sprite = { 102, 115, 50,50 };

	AdAbility(*icicle);
	AdAbility(*thunder);
	AdAbility(*ulti);
	return true;
}

bool Wizard::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Wizard::HeroUpdate(float dt)
{
	if (!noMove.IsZero() && noMove.Read() > LIGHTNING_MS_LIFETIME) {
		noMove.SetZero();	
	}
	if (directions.right - directions.left == 1)
	{
		flip = false;
	}
	else if (directions.right - directions.left == -1)
	{
		flip = true;
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
		ab_2_bool = true;
	}
	else
	{
		ab_2_bool = false;
	}
	if (!GetAbAtk(13)->active)
	{
		ab_3_bool = true;
	}
	else
	{
		ab_3_bool = false;
	}
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
		dir = 1;
	else
		dir = -1;

	if (currentTag == 11 && !ab_1_active)
	{

		App->entities->CreateSpell({ ICICLE,team, {gamepos.x, gamepos.y+75, gamepos.z} });
		ab_1_active = true;
	}
	if (currentTag == 12 && !ab_2_bool )
	{
	
		App->entities->CreateSpell({ LIGHTING,team,{ gamepos.x + (50 * -dir), gamepos.y + 40, gamepos.z },{flip,0} });
		ab_2_bool = true;
		//noMove.Start();
			
	}
	if (currentTag == 13 && !ab_3_bool)
	{

		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x + 50, gamepos.y , gamepos.z },{ 1,0 } });
		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x - 50, gamepos.y , gamepos.z },{ -1,0 } });
		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x, gamepos.y , gamepos.z + 50 },{ 0,1 } });
		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x, gamepos.y , gamepos.z - 50},{ 0,-1 } });
		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x + 30, gamepos.y , gamepos.z + 30 },{ 1,1 } });
		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x - 30, gamepos.y , gamepos.z + 30 },{ -1,1 } });
		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x + 30, gamepos.y , gamepos.z - 30 },{ 1,-1 } });
		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x - 30, gamepos.y , gamepos.z - 30 },{ -1,-1 } });
		App->entities->CreateSpell({ FIRE_DEMON,team,{ gamepos.x, gamepos.y + 50 , gamepos.z} });
		ab_3_bool = true;
		//noMove.Start();

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
		else if (_this->type == Collider::ATK && _other->type == Collider::PARRY)
		{
			currentState = HIT;
		}
		else if (_this->type == Collider::HITBOX && (_other->type == Collider::ATK || _other->type == Collider::SPELL))
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

