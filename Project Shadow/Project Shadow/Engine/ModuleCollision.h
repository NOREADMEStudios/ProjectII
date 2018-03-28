#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Module.h"
#include "Defs.h"
#include <list>
#include "Rect.h"

struct Collider {
	enum Type {
		TRIGGER,
		PHYSIC
	} type;
	iRect collider;
	Callback callback = nullptr;
};

class ModuleCollision
	: public Module
{
public:
	ModuleCollision() {};
	virtual ~ModuleCollision() {};

	bool Awake(xmlNode& config) override;

	bool Start() override;
	
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;

	bool CleanUp(xmlNode& config) override;

private:

	LIST(Collider*) colliders;

};

#endif