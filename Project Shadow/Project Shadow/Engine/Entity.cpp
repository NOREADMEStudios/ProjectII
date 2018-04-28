#include "App.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Entity.h"
#include "ModuleCollision.h"
#include "Log.h"

#define SHADOW_PATH "Characters/Shadow.png"
#define SHADOW_RECT {0,0,36,9}
#define SHADOW_HEIGHT 5

Entity::Entity(EntityTypes type) {}

Entity::Entity() {}

Entity::~Entity() {}

void Entity::Draw(float dt) {
	AnimationFrame frame = currentAnimation->GetCurrentFrame(dt);

	if (shadowed) {
		DrawShadow(frame);
	}
	


	iPoint pivot_pos = PivotPos();

	App->render->Blit(sprites, pivot_pos.x, pivot_pos.y, &frame.GetRectSDL(),1.0f, 0.0, flip);

}

void Entity::Move(float delta_time) {
	gamepos.x += speedVector.x * delta_time;
	gamepos.y += speedVector.y * delta_time;
	gamepos.z += zVect * delta_time;
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
	iPoint pivot_pos = PivotPos();

	int x = position.x - (rect.w/2)  ;// need to fix this values
	int y = gamepos.z;
	
	App->render->Blit(shadowSprites, x, y, &rect.toSDL_Rect());
}
// HardCoded Valors Cough Cough -___-

void Entity::Break(float delta_time) {
	speedVector.x = Utils::Interpolate(speedVector.x, 0.0f, 10 * stats.spd * delta_time);
	speedVector.y = Utils::Interpolate(speedVector.y, 0.0f, 10 * stats.spd * delta_time);
	zVect = Utils::Interpolate(zVect, 0.0f, 10 * stats.spd * delta_time);
}

void Entity::Accelerate(float x, float y, float z, float delta_time) {
	speedVector.x += x * 10 * stats.spd * delta_time;
	speedVector.y += y * 10 * stats.spd * delta_time;
	zVect += z * 10 * stats.spd * delta_time;

	speedVector.x = CLAMP(speedVector.x, -max_speed, max_speed);
	speedVector.y = CLAMP(speedVector.y, -max_speed, max_speed);
	zVect = CLAMP(zVect, -max_speed, max_speed);
}
void Entity::Impulsate(float x, float y, float z)
{
	speedVector.x += x * 10 * stats.spd;
	speedVector.y += y * 10 * stats.spd;
	zVect += z * 10 * stats.spd;
}

bool Entity::PausedUpdate() {
	Draw(0);
	return true;
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

void Entity::SetPos(int x, int z)
{
	gamepos.x = x;
	gamepos.z = z;
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

void Entity::CalcRealPos()
{
	iPoint maplimits = { 10 , 500 };

	position.x = gamepos.x;
	position.y =  ((int)gamepos.z - (int)gamepos.y);
}

Point3D Entity::GetGamePos()
{
	return gamepos;
}

int Entity::GetCharDepth()
{
	return char_depth;
}

iPoint Entity::PivotPos()
{
	AnimationFrame frame = currentAnimation->CurrentFrame();
	iPoint pivot = {frame.pivot.x - frame.rect.x, frame.pivot.y - frame.rect.y};

	if (flip)
	{
		pivot.x = frame.rect.w - pivot.x;
	}


	iPoint pos = { position.x - pivot.x , position.y - pivot.y };


return pos;

}

