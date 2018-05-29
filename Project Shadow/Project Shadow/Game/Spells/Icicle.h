#ifndef __ICICLE_H__
#define __ICICLE_H__
#include "../../Engine/Spells.h"

#define ICICLE_MS_LIFETIME	3000
#define ICICLE_SPEED 8.0f
#define COOL_DURATION 4
#define COOL_EFFECT 0.35 

class Icicle :
	public Spells
{
public:
	Icicle();
	~Icicle();

	bool Start()override;

	bool PreUpdate()override { return true; }

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; }
	bool Save(pugi::xml_node&) const override { return true; }


protected:
	void Dead();
	void OnCollisionEnter(Collider* _this, Collider* _other) override;

	std::vector<Entity*> entitiesHitted;
};

#endif