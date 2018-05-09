#ifndef __LIGHTNING_H__
#define __LIGHTNING_H__
#include "../..\Engine\Spells.h"
class Lightning :
	public Spells
{
public:
	Lightning();
	~Lightning();


	bool Start()override;

	bool PreUpdate()override { return true; }

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; }
	bool Save(pugi::xml_node&) const override { return true; }

protected:
	void Dead();
	Timer ticks;
	bool dealingDmg = false;

	void OnCollisionEnter(Collider* _this, Collider* _other) override;
	void OnCollisionStay(Collider* _this, Collider* _other) override;
	void OnCollisionExit(Collider* _this, Collider* _other) override;

};

#endif