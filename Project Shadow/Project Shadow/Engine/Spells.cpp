#include "Spells.h"
#include "ModuleTextures.h"
#include "ModuleEntityManager.h"


Spells::Spells(SpellsType spellType): Entity(EntityTypes::SPELLS)
{
}


Spells::~Spells()
{
}


bool Spells::CheckLifetime() const{

	if (lifeTime.Read() >= lifetime) {
		
		return true;
	}
	return false;
}


void Spells::LoadSprites() {

	sprites = App->entities->spellsTex;
}

void Spells::UnLoadSprites() {

	App->textures->UnLoad(sprites);
}

void Spells::GetColliderFromAnimation(){

	int z = GetGamePos().z;
	int d = GetCharDepth();
	iPoint pivot_pos = PivotPos();

	spellColl->collider = currentAnimation->GetSpellColliderFromFrame();

	spellColl->collider.z = z;
	spellColl->collider.d = d;

	spellColl->collider.x += pivot_pos.x;
	spellColl->collider.y += pivot_pos.y;
	
}

Spells* Spells::LoadSpell(SpellsType type)
{
	std::string name;
	bool collides = true;
	bool is_mark = false;
	switch (type)
	{
	case FIREBALL:
		name = "fireball";
		break;
	case LIGHTING:
		name = "Lightning";
		break;
	case ICICLE:
		name = "Icicle";
		break;
	case FIRE_DEMON:
		name = "fire_demon";
		break;
	case DAGGER:
		name = "dagger";
		break;
	case LIGHTNING_AURA:
		name = "Aura";
		break;
	case DEATH_MARK:
		collides = false;
		is_mark = true;
		break;
	}
	if (!is_mark)
	{
		spellAnim.LoadAnimationsfromXML(name, SPELLS_ANIMS_ROOT);
		currentAnimation = &spellAnim;
	}


	return this;
}