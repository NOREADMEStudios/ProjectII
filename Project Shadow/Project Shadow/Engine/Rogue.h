#ifndef __ROGUE_H__
#define __ROGUE_H__

#include "Module.h"
#include "Character.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Timer.h"




class Rogue : public Character
{
public:
	Rogue();
	~Rogue();

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
	bool ab_3_active = false;
	bool ab_1_active = false;
	bool ab_2_active = false;
	Spells* dagger;
	Spells* death_m;

	Ability* parry;
	Ability* behindU;
	Ability* ulti;

};
#endif
