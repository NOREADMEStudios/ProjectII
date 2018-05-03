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

	bool ClericStart();

	bool PreUpdate()override;

	bool ClericUpdate(float dt);

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void UpdateSpecStates();


private:
	bool buffed = false;

};
#endif
