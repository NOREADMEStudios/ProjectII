#ifndef __ICICLE_H__
#define __ICICLE_H__
#include "../../Engine/Spells.h"
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
};

#endif