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
	DEATH
};

struct CharState
 {
	CharStateEnum state = IDLE;
	std::list<CharStateEnum> linkers;


	CharState(CharStateEnum init_state = IDLE, CharStateEnum first_linker = IDLE)
	{
		state = init_state;
		if (first_linker != IDLE)
		{
			linkers.push_back(first_linker);
		}
	}

	bool CanLink(CharStateEnum wanted_state)
	{
		for (std::list<CharStateEnum>::iterator item = linkers.begin(); item != linkers.end(); item++)
		{
			if (*item == wanted_state)
				return true;
		}
		return false;
	}

	void AddLink(CharStateEnum linker)
	{
		linkers.push_back(linker);
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

	//Attacks

	std::list<CharState> attacks;


};
#endif


