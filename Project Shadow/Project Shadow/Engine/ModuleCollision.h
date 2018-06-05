#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Module.h"
#include "Defs.h"
#include "Rect.h"
#include "Entity.h"
#include "Cube.h"

#define MAX_TAGS 16

class Entity;
struct Collision;

struct Collider {

public:

	enum Type {
		FEET, 
		HITBOX,
		ATK,
		DEF,
		PARRY,
		SPELL,
		TRIGGER,
		PHYSIC,
		Amount
	} type;

	iCube				collider;
	Entity*				entity = nullptr;
	String				sTag;
	uint				tag;
	LIST(Collision*)	collisions;

//private:
	Collider() {}
	~Collider() {}
	void CleanUp();
	friend class ModuleCollision;
};

struct Collision {

	Collision(Collider* _c1, Collider* _c2) {
		c1 = _c1;
		c2 = _c2;
		state = ON_ENTER;
	}
	~Collision() {}

	void CallOnStay() {
		c1->entity->OnCollisionStay(c1, c2);
		c2->entity->OnCollisionStay(c2, c1);
	}

	void CallOnEnter() {
		c1->entity->OnCollisionEnter(c1, c2);
		c2->entity->OnCollisionEnter(c2, c1);
	}

	void CallOnExit() {
		c1->entity->OnCollisionExit(c1, c2);
		c2->entity->OnCollisionExit(c2, c1);
	}

	void CleanUp(){
		Utils::RemoveFromList(this, c1->collisions);
		Utils::RemoveFromList(this, c2->collisions);
	}

	enum State {
		ON_ENTER,
		ON_STAY,
		ON_EXIT
	} state;

	Collider			*c1 = nullptr,
						*c2 = nullptr;

	iCube				collisionArea;

	bool				updated = false;
};

class ModuleCollision
	: public Module
{
public:
	ModuleCollision();
	virtual ~ModuleCollision() {};

	bool Awake(xmlNode& config) override;

	bool Start() override;
	
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;

	bool CleanUp(xmlNode& config) override;

	Collider* CreateCollider(iCube dims, String tag, Collider::Type type = Collider::TRIGGER);
	void AddCollider(Collider*, Entity*);
	bool RemoveCollider(Collider**);

	ARRAY(String) GetTags();
	String GetTag(uint tag);
	String GetTag(Collider* c);
	String GetTag(const Collider& c);

private:

	ARRAY(Collider*) colliders;
	ARRAY(String) tagList;
	bool interactionTable [MAX_TAGS][MAX_TAGS];

};

#endif