#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"
#include "Character.h"
#include "Module.h"

#include "App.h"
#include "ModuleRender.h"

#include <list>

enum Team;
class Character;
class Spells;
struct SDL_Texture;


enum SpellsType {
	NOSPELL,
	FIREBALL,
	LIGHTNING_AURA,
	LIGHTING,
	ICICLE,
	FIRE_DEMON,
	DAGGER,
	DEATH_MARK,
	CLERIC_STUN,
	AREA

};

enum CharacterTypes {
	WARRIOR,
	WIZARD,
	ROGUE,
	CLERIC,
	NONE,
	ENEMY,
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
	Team chTeam;
};

struct SpellsInfo {

	SpellsType spType = NOSPELL;

	Team chTeam;
	Point3D pos{ 0,0,0 };
	iPoint dir{ 0,0 };

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

	Character* CreateCharacter(CharacterInfo charInfo);
	Spells* CreateSpell(SpellsInfo spellsInfo);
	void DestroyEntity(Entity* entity);


	void CheckMidPos(float& min_x, float& max_x);
	void CheckMidPosY(float& min_y, float& max_y);
	void StartItems();

	uint numofplayers = 0;

	void PauseEntities(bool pause);
	Entity* GetSameTeam(Entity* c);

	Entity* GetEntity(uint num);
	Team GetWinnerTeam();
	EntityStats items[4];
	bool finish = false,
		locked = false;
	uint winner;
	Team winnerTeam;
	SDL_Texture* spellsTex;
	std::list<Entity*> entities;
	Spells* stuned;
	Spells* slowed;

private:



	
};

#endif // !__ENTITYMANAGER_H__
