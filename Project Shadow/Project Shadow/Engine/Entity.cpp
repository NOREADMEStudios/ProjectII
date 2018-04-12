#include "App.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Entity.h"
#include "ModuleCollision.h"
#include "Log.h"

#define SHADOW_PATH "Characters/Shadow.png"
#define SHADOW_RECT {0,0,36,9}

Entity::Entity(EntityTypes type) {}

Entity::Entity() {}

Entity::~Entity() {}

void Entity::Draw(float dt) {
	AnimationFrame frame = currentAnimation->GetCurrentFrame(dt);
	if (shadowed) {
		DrawShadow(frame);
	}
	App->render->Blit(sprites, position.x, position.y, &frame.GetRectSDL(), 1.0f, 0.0, frame.pivot.x, frame.pivot.y);

}

void Entity::Move(float delta_time) {
	position.x += speedVector.x * delta_time;
	position.y += speedVector.y * delta_time;
}
void Entity::LoadShadow() {
	shadowSprites = App->textures->Load(SHADOW_PATH);
	shadowed = true;
}
void Entity::UnloadShadow() {
	App->textures->UnLoad(shadowSprites);
	shadowed = false;
}
void Entity::DrawShadow(AnimationFrame frame) {

	iRect rect = SHADOW_RECT;
	int x = position.x + rect.w/3;// need to fix this values
	int y = position.y+frame.rect.h-5;// need to fix this values
	iPoint fram = frame.pivot;
	
	App->render->Blit(shadowSprites, x, y, &rect.toSDL_Rect());
}
// HardCoded Valors Cough Cough -___-

void Entity::Break(float delta_time) {
	speedVector.x = Utils::Interpolate(speedVector.x, 0.0f, 10 * stats.spd * delta_time);
	speedVector.y = Utils::Interpolate(speedVector.y, 0.0f, 10 * stats.spd * delta_time);

	zVect = Utils::Interpolate(zVect, 0.0f, 10 * stats.spd * delta_time);


}

void Entity::Accelerate(float x, float y, float delta_time) {
	speedVector.x += x * 10 * stats.spd * delta_time;
	speedVector.y += y * 10 * stats.spd * delta_time;

	speedVector.x = CLAMP(speedVector.x, -stats.spd, stats.spd);
	speedVector.y = CLAMP(speedVector.y, -stats.spd, stats.spd);
}


//EntityState Entity::GetState()
//{
//	return state;
//}

void Entity::OnCollisionEnter(Collider * _this, Collider * _other)
{
}

void Entity::OnCollisionStay(Collider * _this, Collider * _other)
{
}

void Entity::OnCollisionExit(Collider * _this, Collider * _other)
{
}

//EntityState Entity::GetState()
//{
//	return state;
//}


iPoint Entity::GetPos()
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

void Entity::SetPos(int x, int y)
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