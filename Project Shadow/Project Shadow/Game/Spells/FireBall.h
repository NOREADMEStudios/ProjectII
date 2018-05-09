#ifndef __FIREBALL_H__
#define __FIREBALL_H__
#include "../../Engine/Spells.h"
class FireBall : public Spells
{
public:
	FireBall();
	~FireBall();

	bool Start()override;

	bool PreUpdate()override { return true; }

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; }
	bool Save(pugi::xml_node&) const override { return true; }


	protected:
	void Dead();
	bool stop = false;

	void OnCollisionEnter(Collider* _this, Collider* _other) override;

};

#endif