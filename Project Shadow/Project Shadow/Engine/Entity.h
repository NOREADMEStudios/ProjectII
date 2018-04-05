#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "ModuleSceneManager.h"

enum EntityTypes;
struct Collider;

class Entity
	: public Module
{
public:
	Entity();
	virtual ~Entity();

	virtual void OnCollisionEnter(Collider* _this, Collider* _other);

	virtual void OnCollisionStay(Collider* _this, Collider* _other);

	virtual void OnCollisionExit(Collider* _this, Collider* _other);

	EntityTypes type;
};
#endif



