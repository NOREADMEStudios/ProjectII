#ifndef __CORPSE_H__
#define __CORPSE_H__


#include "Module.h"
#include "Character.h"

class Corpse : public Character
{
public:
	Corpse(CharacterTypes _type);
	~Corpse();

	bool Awake(pugi::xml_node&) override { return true; }

	bool HeroStart();

	bool HeroUpdate(float dt) { return true; }

	bool PostUpdate()override { return true; }

	bool CleanUp(pugi::xml_node&)override { return true; }

	CharacterTypes corpseType;
	
};

#endif
