#ifndef __CLERIC_H__
#define __CLERIC_H__

#include "Module.h"
#include "Character.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Timer.h"




class Cleric : public Character
{
public:
	Cleric();
	~Cleric();

	bool Awake(pugi::xml_node&) override;

	bool HeroStart();

	bool HeroUpdate(float dt);

	bool PostUpdate()override;


	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void UpdateSpecStates();
	virtual void OnCollisionEnter(Collider* _this, Collider* _other);
	void CreateSounds();
	Character* GetPartner() { return partner; };

private:
	bool ab_3_active = false;
	bool ab_1_active = false;
	bool ab_2_active = false;
	Character* partner;

	Spells* knock;
	Spells* area;
	Spells* ulti;

	Ability* fire;
	Ability* thunder;
	Ability* ulti_ab;

};
#endif
