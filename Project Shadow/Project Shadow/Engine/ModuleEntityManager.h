#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"
#include "Module.h"

#include "App.h"
#include "ModuleRender.h"

#include <list>

enum CharacterTypes {
	NONE,
	ENEMY,
	HERO
};
enum EntityTypes {
	NOTYPE,
	SPELLS,
	ITEMS,
	CHARACTER
};

struct CharacterInfo {

	CharacterTypes chType = NONE;
	Point3D pos{ 0, 0 };
	
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

	bool IsSFXRepeated(std::list<std::string> list, std::string string) const;

	Entity* CreateCharacter(CharacterInfo charInfo);
	void DestroyEntity(Entity* entity);

	void CheckMidPos(float& min_x, float& max_x);
	void CheckMidPosY(float& min_y, float& max_y);
	void StartItems();

	uint numofplayers = 0;

	void PauseEntities(bool pause);

	Entity* GetEntity(uint num);
	EntityStats items[4];
	bool finish = false;

private:

	std::list<Entity*> entities;

	
};

#endif // !__ENTITYMANAGER_H__
