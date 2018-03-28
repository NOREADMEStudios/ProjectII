#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "ModuleSceneManager.h"

enum EntityTypes;

class Entity : public Module
{
public:
	Entity();
	~Entity();


	bool Awake(pugi::xml_node&) override { return true; }

	bool Start()override { return true; }

	bool PreUpdate()override { return true; }

	bool Update(float dt)override { return true; }

	bool PostUpdate()override { return true; }

	bool CleanUp(pugi::xml_node&)override { return true; }

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };

	EntityTypes type;
};
#endif



