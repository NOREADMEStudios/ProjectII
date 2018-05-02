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

	bool RogueStart();

	bool PreUpdate()override;

	bool RogueUpdate(float dt);

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void UpdateSpecStates();


private:
	bool buffed = false;

};
#endif
