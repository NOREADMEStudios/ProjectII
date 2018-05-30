#ifndef __WIZARD_H__
#define __WIZARD_H__

#include "Module.h"
#include "Character.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Timer.h"

class Lightning;


class Wizard : public Character
{
public:
	Wizard();
	~Wizard();

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
	bool ab_1_active = false;
	bool ab_2_bool = false;
	bool ab_3_bool = false;
	Spells* icicle;
	Lightning* lighting;
	Spells* fireDemon;
	Spells* fireball;
	Spells* light_aura;

	Ability* icicle_ab;
	Ability* thunder;
	Ability* ulti;
	
	Timer pose_timer;
	float pose_wait;
	bool pose_bool;
	bool start_pose;
	float prevAnimSpeed;
};
#endif
