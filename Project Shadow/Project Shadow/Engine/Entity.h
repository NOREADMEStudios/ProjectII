#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "ModuleSceneManager.h"
#include "Point.h"
#include "Animation.h"

#include "Defs.h"

struct SDL_Texture;
enum EntityTypes;

enum EntityState
{
	IDLE,
	JUMP,
	WALK,
	RUN,
	DASH,
};

struct EntityStats
{
	int atk = 0;
	int def = 0;
	int spd = 0;
	int mgk = 0;
};

struct Items
{
	EntityStats stats;
};

class Entity : public Module
{
public:
	Entity(EntityTypes type);
	~Entity();


	bool Awake(pugi::xml_node&) override { return true; }

	bool Start()override { return true; }

	bool PreUpdate()override { return true; }

	bool Update(float dt)override { return true; }

	bool PostUpdate()override { return true; }

	bool CleanUp(pugi::xml_node&)override { return true; }

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };

	EntityState GetState();
	fPoint GetPos();
	bool IsActive();
	float GetPosX();
	float GetPosY();
	fPoint GetSpeed();
	EntityTypes GetType();


	void SetPos(float x, float y);
	void SetActive(bool sactive);

	void Draw(float dt);


	virtual void Move(float delta_time);
	virtual void Break(float delta_time);
	void Accelerate(float x, float y, float delta_time);

	EntityTypes type;

protected:

	bool active = true;
	fPoint position{ 0,0 };
	fPoint speedVector{ 0,0 };

	// Collider has to be a struct Collider instead of an iRect
	iRect collider{ 0,0,0,0 };
	EntityState state = IDLE;
	Animation* currentAnimation = nullptr;
	EntityStats stats;
	SDL_Texture* sprites;


	// Should be same numeration as the states
	std::list<Animation> animations;
	std::list<Items> items;




};
#endif



