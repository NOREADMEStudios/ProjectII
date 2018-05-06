#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Module.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Entity.h"
#include "ModuleCollision.h"

#include <list>

enum Team {
	BLUE,
	RED
};

#define COMBO_MARGIN 1
#define HERO_SPRITE_ROOT "Assets/Animations/Characters/Fighter_Animations.tmx"
#define ELF_SPRITE_ROOT "Assets/Animations/Characters/Elf_Animations.tmx"


enum CharInput
{
	NONECHARINPUT,
	CH_UP,
	CH_DOWN,
	CH_RIGHT,
	CH_LEFT,
	LIGHT_ATTACK,
	HEAVY_ATTACK,
	JUMPINPUT,
	RUNINPUT,
	DEFEND,
	TAUNTINPUT,
	PARRYINPUT,
	AB_1,
	AB_2,
	AB_3
};

enum CharStateEnum
{
	IDLE,
	JUMP,
	WALK,
	RUN,
	ATTACK_LIGHT,
	ATTACK_HEAVY,
	STOP,
	HIT,
	KNOKED,
	DEATH,
	PROTECT,
	PARRY,
	PARRIED,
	TAUNT,
	AD_ACTION,
};

struct State
{
	CharStateEnum state;
	Animation anim;

};

struct Attack
{
	uint tag;
	int damage = 0;
	LIST(Attack*) childs {};
	CharInput input;
	Animation anim;
	bool air;
	bool ability;

	Attack(uint _tag, CharInput _input, std::string animationName, std::string fileName, int _damage = 0, bool _air = false, bool ab = false)
	{
		tag = _tag;
		input = _input;
		damage = _damage;
		anim.LoadAnimationsfromXML(animationName, fileName);
		air = _air;
		ability = ab;
	}

	void AddChild(Attack* _child)
	{
		childs.push_back(_child);
	}

	bool CheckChild(Attack* _child)
	{
		if (!childs.empty()) {
			for (std::list<Attack*>::const_iterator item = childs.begin(); item != childs.end(); item++) {
				if (*item == _child)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool CheckChildInput(CharInput input)
	{
		if (!childs.empty())
		{
			for (std::list<Attack*>::const_iterator item = childs.begin(); item != childs.end(); item++) {
				if ((*item)->input == input)
				{
					return true;
				}
			}
		}
		return false;
	}

	Attack* GetChildInput(CharInput input)
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

struct Ability
{
	Attack* atk = nullptr;
	Timer timer;
	float cooldown = 0;
	bool active = false;

	Ability(Attack* _atk, float cd = 0)
	{
		atk = _atk;
		cooldown = cd;
		active = false;
	}

	void Activate()
	{
		timer.Start();
		active = false;
	}

};

struct EventState
{
	bool active = 0;
	Timer timer;
	float time_active = 0;
	EntityStats stats;

	EventState(float time, int atk, int def, int spd)
	{
		time_active = time;
		stats.atk = atk;
		stats.def = def;
		stats.spd = spd;
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



	uint GetMaxLives() const;
	uint GetCurrentLives() const;

	void SetCombo();

	void Respawn();

	void GetHP(int& curr, int& max);

	virtual void OnCollisionEnter(Collider* _this, Collider* _other);
	Timer time_attack;
	Team team;

	void AdBuff(float time = 0, float spd = 0, float atk = 0, float def = 0);
	void SetAnimations();
	void SetCharType(CharacterTypes type);


protected:


	std::list<CharInput> GetControllerInputs()const;
	std::list<CharInput> FirstPlayerConfig();

	std::list<CharInput> RequestInputs()const;
	void RequestState() ;
	void UpdateMainStates();
	void UpdateCurState(float dt);
	void UpdateAnimation();
	bool StateisAtk(CharStateEnum State);
	Attack* GetAtk(uint atk);
	Ability* GetAbAtk(uint atk);
	void GetCollidersFromAnimation();
	void UpdateCollidersPosition();
	void LoadState(CharStateEnum _state, std::string animationName);
	void LoadBasicStates();
	void UpdateTag(uint& t);
	void UpdateAbilities();
	void AdAbility(Ability ab);
	void UpdateEventStates();


	virtual bool HeroStart() { return true; };
	virtual bool HeroUpdate(float dt) { return true; };
	virtual void UpdateSpecStates() {};
	
	Collider	*collFeet = nullptr,
				*collHitBox = nullptr,
				*collAtk = nullptr,
				*collDef = nullptr,
				*collParry = nullptr;




	CharStateEnum currentState;
	CharStateEnum wantedState;
	uint last_attack;

	LIST(State*) states;
	LIST(EventState*) eventstates;


	iPoint initialpos;
	int initialLife = 0;
	uint lives = 0, maxLives = 3;
	int hit_dir = 0;
	bool hit_bool = 0;
	bool parried = 0;
	bool jumping = 0;
	bool sound_avaliable = true;
	uint currentTag = 0;
	uint wantedTag = 0;
	std::string animations_name;



	LIST(Attack*) attacks;
	LIST(Ability) abilities;

};

#endif

