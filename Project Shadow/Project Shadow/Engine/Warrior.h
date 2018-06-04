#ifndef __WARRIOR_H__
#define __WARRIOR_H__

#include "Module.h"
#include "Character.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Timer.h"




class Warrior : public Character
{
public:
	Warrior();
	~Warrior();

	bool Awake(pugi::xml_node&) override;

	bool HeroStart();

	bool HeroUpdate(float dt);

	bool PostUpdate()override;


	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void UpdateSpecStates();
	virtual void OnCollisionEnter(Collider* _this, Collider* _other);
	void CreateSounds();


private:
	bool buffed = false;
	bool ab_1_bool = false;
	bool ab_2_bool = false;
	Character* partner;

	Spells* ulti_sp;
	Spells* ulti_indicator;
	Ability* Aulti;

};
#endif
