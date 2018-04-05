#include "App.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Entity.h"
#include "ModuleCollision.h"
#include "Log.h"


Entity::Entity(EntityTypes type) {}

Entity::Entity() {}

Entity::~Entity() {}

void Entity::Draw(float dt) {
	AnimationFrame frame = currentAnimation->GetCurrentFrame(dt);
	App->render->Blit(sprites, position.x, position.y, &frame.GetRectSDL(), 1.0f, 0.0, frame.pivot.x, frame.pivot.y);
}

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

void Entity::OnCollisionEnter(Collider * _this, Collider * _other)
{
}

void Entity::OnCollisionStay(Collider * _this, Collider * _other)
{
}

void Entity::OnCollisionExit(Collider * _this, Collider * _other)
{
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
	return position.x + (collider.w  / 2);
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

uint Entity::GetPriority() const
{
	return priority;
}

iRect Entity::GetCollider() const
{
	return collider;
}