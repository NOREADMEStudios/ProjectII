#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Module.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Entity.h"
#include "ModuleCollision.h"



#define COMBO_MARGIN 0.5
#define HERO_SPRITE_ROOT "Assets/Animations/Characters/Fighter_Animations.tmx"




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

	Attack(uint _tag, CharInput _input, std::string animationName, int _damage = 0, bool _air = false)
	{
		tag = _tag;
		input = _input;
		damage = _damage;
		anim.LoadAnimationsfromXML(animationName, HERO_SPRITE_ROOT);
		air = _air;
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

	
protected:

	void RequestState();
	void UpdateMainStates();
	void UpdateCurState(float dt);
	void UpdateAnimation();
	bool StateisAtk(CharStateEnum State);
	Attack* GetAtk(uint atk);
	void GetCollidersFromAnimation();
	void UpdateCollidersPosition();
	void LoadState(CharStateEnum _state, std::string animationName);
	void LoadBasicStates();
	void UpdateTag(uint& t);

	virtual bool HeroStart() { return true; };
	virtual bool HeroUpdate(float dt) { return true; };
	virtual void UpdateSpecStates() {};
	
	Collider	*collFeet = nullptr,
				*collHitBox = nullptr,
				*collAtk = nullptr,
				*collDef = nullptr,
				*collParry = nullptr;


	CharacterTypes charType;

	CharStateEnum currentState;
	CharStateEnum wantedState;
	uint last_attack;

	LIST(State*) states;

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

	LIST(Attack*) attacks;

};
#endif


