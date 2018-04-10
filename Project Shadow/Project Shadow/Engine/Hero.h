#ifndef __HERO_H__
#define __HERO_H__

#include "Module.h"
#include "Character.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Timer.h"

#define COMBO_MARGIN 0.2



class Hero : public Character
{
public:
	Hero();
	~Hero();


	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override;

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };



	void LoadAnimations();

	void RequestState();
	void UpdateState();
	void UpdateCurState(float dt);
	void UpdateAnimation();
	void CalculateAtk();
	bool StateisAtk(CharStateEnum State);
	Attack* GetAtk(CharStateEnum atk);
	void SetCombo();

	void Respawn();

	void GetHP(int& curr, int& max);

	virtual void OnCollisionEnter(Collider* _this, Collider* _other);

	Timer time_attack;

	Animation jump, stop, run, jumpAtk, jumpProt, kick, attack, hit, death, attack_l2, attack_l3;


	int currHP = 100;

protected:

	
	iPoint initialpos;
	int initialLife = 0;
	uint lives = 0;
	uint hit_dir = 0;
	bool hit_bool = 0;


};
#endif
