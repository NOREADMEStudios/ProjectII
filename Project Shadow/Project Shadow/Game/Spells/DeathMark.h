#ifndef __DEATHMARK_H__
#define __DEATHMARK_H__

#include "..\..\Engine\Spells.h"
class DeathMark :
	public Spells
{
public:
	DeathMark();
	~DeathMark();


	bool Start()override;

	bool PreUpdate()override { return true; }

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; }
	bool Save(pugi::xml_node&) const override { return true; }
	void SetPath(std::string _name);
	void SetBack() { z = -1; };
	void SetLifeTime(uint msec) { lifetime = msec; };

	bool cl;
protected:
	void Dead();
	bool stop = false;
	int z = 0;



};

#endif