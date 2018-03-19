#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"
#include "Module.h"

#include <list>

enum EntityTypes {
	NOTYPE,
	SPELLS,
	ITEMS,
	CHARACTER
};

struct EntityInfo {

	EntityTypes type;
	
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


private:

	std::list<Entity*> entities;
	
};

#endif // !__ENTITYMANAGER_H__
