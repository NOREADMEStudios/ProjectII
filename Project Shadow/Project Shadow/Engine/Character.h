#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Module.h"
#include "Entity.h"


class Character : public Entity
{
public:
	Character();
	~Character();


	bool Awake(pugi::xml_node&) override { return true; }

	bool Start()override { return true; }

	bool PreUpdate()override { return true; }

	bool Update(float dt)override { return true; }

	bool PostUpdate()override { return true; }

	bool CleanUp(pugi::xml_node&)override { return true; }

	bool Load(pugi::xml_node&) override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	
};
#endif


