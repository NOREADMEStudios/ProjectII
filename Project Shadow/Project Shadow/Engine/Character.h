#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Module.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Entity.h"
#include "ModuleCollision.h"

enum CharacterTypes;


enum Input
{
	NONEINPUT,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	LIGHT_ATTACK,
	HEAVY_ATTACK,
	JUMPINPUT,
	RUNINPUT,
	DEFEND
};

enum CharStateEnum
{
	IDLE,
	JUMP,
	WALK,
	RUN,
	DASH,
	ATTACK_LIGHT,
	ATTACK_HEAVY,
	ATTACK_L2,
	ATTACK_L3,
	STOP,
	HIT,
	KNOKED,
	DEATH
};

struct Attack
{
	CharStateEnum state;
	int damage = 0;
	LIST(Attack*) childs;
	Input input;

	Attack(CharStateEnum _state, Input _input, int _damage = 0)
	{
		state = _state;
		input = _input;
		damage = _damage;

	}

	void AddChild(Attack* _child)
	{
		childs.push_back(_child);
	}

	bool CheckChild(Attack* _child)
	{
		for (std::list<Attack*>::const_iterator item = childs.begin(); item != childs.end(); item++) {
			if (*item == _child)
			{
				return true;
			}
		}

		return false;
	}

	bool CheckChildInput(Input input)
	{

		for (std::list<Attack*>::const_iterator item = childs.begin(); item != childs.end(); item++) {
			if ((*item)->input == input)
			{
				return true;
			}
		}

		return false;
	}

	Attack* GetChildInput(Input input)
	{

		for (std::list<Attack*>::const_iterator item = childs.begin(); item != childs.end(); item++) {
			if ((*item)->input == input)
			{
				return *item;
			}
		}

		return nullptr;
	}

};



class Character : public Entity
{
public:
	Character(CharacterTypes charType);
	~Character();


	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override;

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&) override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void ModifyStats(int attack, int defense = 0, int speed = 0, int magic = 0);
	

	
protected:
	void GetCollidersFromAnimation();
	void UpdateCollidersPosition();

	void LoadAnimations();
	

	Animation idle, walking, attack , death;

	Collider	*collFeet		=	nullptr,
				*collHitBox		=	nullptr,
				*collAtk		=	nullptr;

	CharacterTypes charType;

	CharStateEnum currentState;
	CharStateEnum wantedState;
	CharStateEnum last_attack;


	LIST(Attack*) attacks;

};
#endif


