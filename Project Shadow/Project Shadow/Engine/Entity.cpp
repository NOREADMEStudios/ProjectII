#include "Entity.h"



Entity::Entity() {}


Entity::~Entity() {}



void Entity::Move(float delta_time) {
	position.x += speedVector.x * delta_time;
	position.y += speedVector.y * delta_time;
}

// HardCoded Valors Cough Cough -___-

void Entity::Break(float delta_time) {
	speedVector.x = Interpolate(speedVector.x, 0.0f, 10 * stats.spd * delta_time);
	speedVector.y = Interpolate(speedVector.y, 0.0f, 10 * stats.spd * delta_time);
}

void Entity::Accelerate(float x, float y, float delta_time) {
	speedVector.x += x * 10 * stats.spd * delta_time;
	speedVector.y += y * 10 * stats.spd * delta_time;

	speedVector.x = CLAMP(speedVector.x, -stats.spd, stats.spd);
	speedVector.y = CLAMP(speedVector.y, -stats.spd, stats.spd);
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