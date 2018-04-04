#include "Entity.h"
#include "ModuleCollision.h"
#include "Log.h"



Entity::Entity() {}


Entity::~Entity() {}

void Entity::OnCollisionEnter(Collider * _this, Collider * _other)
{
	LOG("ENTER");
}

void Entity::OnCollisionStay(Collider * _this, Collider * _other)
{
	LOG("STAY");
}

void Entity::OnCollisionExit(Collider * _this, Collider * _other)
{
	LOG("EXIT");
}

