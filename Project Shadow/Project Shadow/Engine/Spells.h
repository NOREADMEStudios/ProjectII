#ifndef __SPELLS_H__
#define __SPELLS_H__
#include "Entity.h"
#include "Animation.h"
#include "ModuleEntityManager.h"
#include "ModuleCollision.h"
#include "Timer.h"


#define SPELLS_ANIMS_ROOT "Assets/Animations/Spells/Spells.tmx"

enum SpellsType;

class Spells : 	public Entity
{
public:
	Spells(SpellsType spellType);
	~Spells();

	bool Awake(pugi::xml_node&) override { return true; }

	bool Start()override { return true; }

	bool PreUpdate()override { return true; }

	bool Update(float dt)override { return true; }

	bool PostUpdate()override { return true; }

	bool CleanUp(pugi::xml_node&)override { return true; }

	bool Load(pugi::xml_node&) override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

protected:

	void UpdateCollidersPosition();
	bool CheckLifetime() const;
	void DestroySpell();
	void LoadSprites();
	void UnLoadSprites();

	SpellsType spellType;
	Animation spellAnim;
	Collider* spellColl = nullptr;
	Timer lifeTime;
	uint lifetime;


	void GetColliderFromAnimation();
};

#endif