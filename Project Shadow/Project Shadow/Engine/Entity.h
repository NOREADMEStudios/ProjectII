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
enum CharacterTypes;


enum Team {
	NOTEAM = 0,
	RED,
	BLUE
};

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
	int cdr = 0;
	int ccr = 0;
	bool hpRecover = false;

	EntityStats operator -(const EntityStats &v) const {
	EntityStats r;

	r.life = life - v.life;
	r.atk = atk - v.atk;
	r.def = def - v.def;
	r.spd = spd - v.spd;
	r.mgk = mgk - v.mgk;
	r.cdr = cdr - v.cdr;
	r.ccr = ccr - v.ccr;

	return(r);
	}

	EntityStats operator +(const EntityStats &v) const {
		EntityStats r;
		r.life = life + v.life;
		r.atk = atk + v.atk;
		r.def = def + v.def;
		r.spd = spd + v.spd;
		r.mgk = mgk + v.mgk;
		r.cdr = cdr + v.cdr;
		r.ccr = ccr + v.ccr;
		r.hpRecover = hpRecover + v.hpRecover;
		
		return(r);
	}
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

struct EventState
{
	bool active = 0;
	Timer timer;
	float time_active = 0;
	EntityStats stats;

	EventState(float time, int atk, int def, int spd)
	{
		time_active = time;
		stats.atk = atk;
		stats.def = def;
		stats.spd = spd;
	}
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


	void SetPos(int x,int y, int z);
	void SetActive(bool sactive);

	void Draw(float dt);
	virtual void LoadShadow();
	virtual void DrawShadow(AnimationFrame frame);
	virtual void UnloadShadow();
	virtual void Move(float delta_time);
	virtual void Break(float delta_time);
	void Accelerate(float x, float y,float z, float delta_time);
	void Impulsate(float x, float y, float z);
	void AdBuff(float time = 0, float spd = 0, float atk = 0, float def = 0);
	void SetFlip(bool f) { flip = f; };

	void CalcRealPos();



	virtual void OnCollisionEnter(Collider* _this, Collider* _other);

	virtual void OnCollisionStay(Collider* _this, Collider* _other);

	virtual void OnCollisionExit(Collider* _this, Collider* _other);


	bool PausedUpdate();
	iPoint PivotPos();

	EntityTypes type;
	EntityStats stats;
	CharacterTypes charType;

	bool noMove = false;
	bool paused = false;
	bool resume = false;

	uint heroNum = 0;
	bool breaking = false;
	bool active = true;
	float max_speed = 0;
	float max_speed_y = 0;
	bool cleric_ab = 0;

	bool to_delete = false;
	Team team = NOTEAM;

protected:
	
	uint priority = 0;
	iPoint position{ 0,0 };
	fPoint speedVector{ 0,0 };
	float zVect = 0;

	int char_depth = 0;
	LIST(EventState*) eventstates;

	// Collider has to be a struct Collider instead of an iRect
	iRect collider{ 0,0,0,0 };
	Animation* currentAnimation = nullptr;
	Point3D gamepos;
	SDL_Rect animRect;

	SDL_Texture* sprites;

	SDL_Texture* shadowSprites;

	bool shadowed = false;

	Directions directions;
	bool flip = 0;
	InvBlit invencible;
	
	



	// Should be same numeration as the states
	std::list<Animation> animations;

};
#endif



