#include "Entity.h"



Entity::Entity() {}


Entity::~Entity() {}



void Entity::Move(float delta_time) {
	position.x += speedVector.x * delta_time;
	position.y += speedVector.y * delta_time;
}

// This comented section should change 

void Entity::Break(float delta_time) {
	speedVector.x = Interpolate(speedVector.x, 0.0f, /*DECELERATION*/ 1 * delta_time);
}

void Entity::Accelerate(float x, float y, float delta_time) {
	speedVector.x += x * /*ACCELERATION*/ 1 * delta_time;
	speedVector.y += y * /*ACCELERATION*/ 1* delta_time;

	speedVector.x = CLAMP(speedVector.x, 1,1/*-max_speed.x, max_speed.x*/);
	speedVector.y = CLAMP(speedVector.y, 1,1/*-max_speed.y, max_speed.y*/);
}

EntityState Entity::GetState()
{
	return state;
}

fPoint Entity::GetPos()
{
	return position;
}

bool Entity::IsActive()
{
	return active;
}

float Entity::GetPosX()
{
	return position.x;
}

float Entity::GetPosY()
{
	return position.y;
}

fPoint Entity::GetSpeed()
{
	return speedVector;
}

EntityTypes Entity::GetType()
{
	return type;
}

void Entity::SetPos(float x, float y)
{
	position = { x,y };
}

void Entity::SetActive(bool sactive)
{
	active = sactive;
}