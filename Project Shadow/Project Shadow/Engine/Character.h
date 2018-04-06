#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Module.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Entity.h"

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
	HITTED,
	KNOKED,
	DEATH
};

struct Attack
{
	CharStateEnum state;
	int damage = 0;
	uint firstActiveFrame = 0;
	uint activeFrames = 0;

	Attack(CharStateEnum _state, int _damage = 0, uint _firstactive = 0, uint _actives = 0)
	{
		state = _state;
		damage = _damage;
		firstActiveFrame = _firstactive;
		activeFrames = _actives;
	}
};

struct Directions
{
	bool left, down, right, up = false;
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
	
	void LoadAnimations();


	Animation idle;
	Animation walking;

	CharacterTypes charType;

	CharStateEnum currentState;
	CharStateEnum wantedState;
	CharStateEnum last_attack;
	Directions directions;

	LIST(Attack) attacks;

};
#endif


