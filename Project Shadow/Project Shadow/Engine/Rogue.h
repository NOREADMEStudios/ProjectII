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

	bool PreUpdate()override;

	bool HeroUpdate(float dt);

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void UpdateSpecStates();
	virtual void OnCollisionEnter(Collider* _this, Collider* _other);


private:
	bool buffed = false;

};
#endif
