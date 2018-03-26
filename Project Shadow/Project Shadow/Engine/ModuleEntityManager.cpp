#include "ModuleEntityManager.h"

ModuleEntityManager::ModuleEntityManager()
{
	name = "entities";
}


ModuleEntityManager::~ModuleEntityManager(){}

bool ModuleEntityManager::Awake(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Awake(n);
	}
	return true;
}

bool ModuleEntityManager::Start() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Start();
	}
	return true;
}

bool ModuleEntityManager::PreUpdate() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->PreUpdate();
	}
	return true;
}

bool ModuleEntityManager::Update(float dt) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Update(dt);
	}
	return true;
}

bool ModuleEntityManager::PostUpdate() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->PostUpdate();
	}
	return true;
}

bool ModuleEntityManager::CleanUp(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->CleanUp(n);
	}
	return true;
}

bool ModuleEntityManager::Load(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Load(n);
	}
	return true;
}

bool ModuleEntityManager::Save(pugi::xml_node& n)const {

	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Save(n);
	}
	return true;
}

Entity* ModuleEntityManager::CreateEntity(EntityInfo entityInfo) {

	Entity* ret = nullptr;

	if (entityInfo.type == CHARACTER)
	{
		ret = new Character();
	}
	else
	{
		ret = new Entity();
	}

	ret->type = entityInfo.type;
	ret->SetPos(entityInfo.pos.x, entityInfo.pos.y);

	entities.push_back(ret);

	return ret;
}


void ModuleEntityManager::DestroyEntity(Entity* entity) {

	
}
