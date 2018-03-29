#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Module.h"
#include "Defs.h"
#include "Rect.h"

#define MAX_TAGS 16

class Entity;

struct Collider {
	enum Type {
		TRIGGER,
		PHYSIC,
		Number
	} type;
	iRect collider;
	Entity* callback = nullptr;
	String	tag;
};

class ModuleCollision
	: public Module
{
public:
	ModuleCollision();
	virtual ~ModuleCollision() {};

	bool Awake(xmlNode& config) override;

	bool Start() override;
	
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;

	bool CleanUp(xmlNode& config) override;

private:

	LIST(Collider*) colliders;
	ARRAY(String) tagList;
	bool interactionTable [MAX_TAGS][MAX_TAGS];

};

#endif