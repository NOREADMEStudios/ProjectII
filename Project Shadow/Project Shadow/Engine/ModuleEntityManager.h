#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"
#include "Module.h"
#include "Character.h"
#include "App.h"
#include "ModuleRender.h"

#include <list>
#include <queue>

enum EntityTypes {
	NOTYPE,
	SPELLS,
	ITEMS,
	CHARACTER
};

struct EntityInfo {

	EntityTypes type = NOTYPE;
	iPoint pos{ 0, 0 };
	
};

struct OrderCrit
{
	bool operator()(const Entity* entity_1, const Entity* entity_2)const
	{
		return entity_1->GetPriority() > entity_2->GetPriority();
	}
};

class ModuleEntityManager : public Module
{
public:
	ModuleEntityManager();
	~ModuleEntityManager();

	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override;

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	Entity* CreateEntity(EntityInfo entityInfo);
	void DestroyEntity(Entity* entity);

	std::priority_queue<Entity*, std::vector<Entity*>, OrderCrit> SpriteOrderer;

	void FillQueue(Entity* entity);
	void PrintFromQueue(std::priority_queue<Entity*, std::vector<Entity*>, OrderCrit> queue, float dt);

private:

	std::list<Entity*> entities;
	
};

#endif // !__ENTITYMANAGER_H__
