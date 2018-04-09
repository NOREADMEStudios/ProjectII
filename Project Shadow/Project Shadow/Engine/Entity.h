#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "ModuleSceneManager.h"
#include "Point.h"
#include "Animation.h"

#include "Defs.h"

struct SDL_Texture;
enum EntityTypes;
struct Collider;



struct EntityStats
{
	int life = 0;
	int atk = 0;
	int def = 0;
	int spd = 0;
	int mgk = 0;
};
struct Directions
{
	bool left, down, right, up = false;
};

struct Items
{
	EntityStats stats;
};

class Entity : public Module
{
public:
	Entity(EntityTypes type);
	Entity();
	virtual ~Entity();


	iPoint GetPos();
	bool IsActive();
	float GetPosX();
	float GetPosY();
	fPoint GetSpeed();
	EntityTypes GetType();
	uint GetPriority() const;
	iRect GetCollider() const;


	void SetPos(int x, int y);
	void SetActive(bool sactive);

	void Draw(float dt);

	virtual void Move(float delta_time);
	virtual void Break(float delta_time);
	void Accelerate(float x, float y, float delta_time);


	virtual void OnCollisionEnter(Collider* _this, Collider* _other);

	virtual void OnCollisionStay(Collider* _this, Collider* _other);

	virtual void OnCollisionExit(Collider* _this, Collider* _other);

	EntityTypes type;
	EntityStats stats;

	uint hero_num = 0;
protected:

	bool active = true;
	uint priority = 0;
	iPoint position{ 0,0 };
	fPoint speedVector{ 0,0 };

	// Collider has to be a struct Collider instead of an iRect
	iRect collider{ 0,0,0,0 };
	Animation* currentAnimation = nullptr;

	SDL_Texture* sprites;
	Directions directions;
	bool flip = 0;

	// Should be same numeration as the states
	std::list<Animation> animations;
	std::list<Items> items;
};
#endif



