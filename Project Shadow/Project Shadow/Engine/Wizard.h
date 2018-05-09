#ifndef __WIZARD_H__
#define __WIZARD_H__

#include "Module.h"
#include "Character.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Timer.h"




class Wizard : public Character
{
public:
	Wizard();
	~Wizard();

	bool Awake(pugi::xml_node&) override;

	bool HeroStart();

	bool PreUpdate()override;

	bool HeroUpdate(float dt);

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void UpdateSpecStates();
	virtual void OnCollisionEnter(Collider* _this, Collider* _other);

private:
	bool ab_1_active = false;

};
#endif
