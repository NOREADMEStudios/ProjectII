#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "Entity.h"
#include "ModuleEntityManager.h"
#include "Animation.h"



class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();


	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override;

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&) override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

		
	void LoadAnimations();

	Animation idle;
	Animation walking;
};
#endif
