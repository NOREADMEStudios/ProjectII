#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "ModuleSceneManager.h"
#include "Point.h"
#include "Animation.h"
#include "Timer.h"

#include "Defs.h"

struct SDL_Texture;
enum EntityTypes;
struct Collider;

struct InvBlit
{
	Timer duration;
	float dur;
	float fr;
	float cur_fr;
	bool active;
	bool on;

	InvBlit(float _dur = 0, float _fr = 0) {
		dur = _dur;
		fr = _fr;
		on = true;
		active = false;
	}

	void StartInv()
	{
		active = true;
		cur_fr = 0;
		duration.Start();
	}

	void UpdateInv()
	{
		if (duration.ReadSec() >= cur_fr)
		{
			cur_fr += fr;
			on = !on;
		}
		if (duration.Count(dur))
		{
			active = false;
		}

	}
};

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
struct Point3D
{
	float x = 0;
	float y = 0;
	float z = 0;
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
	Point3D GetGamePos();
	int GetCharDepth();


	void SetPos(int x, int z);
	void SetActive(bool sactive);

	void Draw(float dt);

	virtual void Move(float delta_time);
	virtual void Break(float delta_time);
	void Accelerate(float x, float y,float z, float delta_time);

	void CalcRealPos();


	virtual void OnCollisionEnter(Collider* _this, Collider* _other);

	virtual void OnCollisionStay(Collider* _this, Collider* _other);

	virtual void OnCollisionExit(Collider* _this, Collider* _other);

	bool PausedUpdate();
	iPoint PivotPos();

	EntityTypes type;
	EntityStats stats;
	bool paused = false;
	uint hero_num = 0;
	bool to_delete = false;
protected:

	bool active = true;
	
	uint priority = 0;
	iPoint position{ 0,0 };
	fPoint speedVector{ 0,0 };
	float zVect = 0;
	float max_speed = 0;
	int char_depth = 0;
	

	// Collider has to be a struct Collider instead of an iRect
	iRect collider{ 0,0,0,0 };
	Animation* currentAnimation = nullptr;
	Point3D gamepos;

	SDL_Texture* sprites;
	Directions directions;
	bool flip = 0;
	InvBlit invencible;

	// Should be same numeration as the states
	std::list<Animation> animations;
	std::list<Items> items;
};
#endif



