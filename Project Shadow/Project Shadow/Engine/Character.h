#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Module.h"
#include "Entity.h"



class Character : public Entity
{
public:
	Character();
	~Character();


	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override;

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&) override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void ModifyStats(int attack, int defense = 0, int speed = 0, int magic = 0);
	
	void UpdateInputs(float dt);
};
#endif


