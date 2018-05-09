#include "Spells.h"
#include "ModuleTextures.h"


Spells::Spells(SpellsType spellType): Entity(EntityTypes::SPELLS)
{
}


Spells::~Spells()
{
}

void Spells::UpdateCollidersPosition() {

	spellColl->collider.x = position.x;
	spellColl->collider.y = position.y;

}

bool Spells::CheckLifetime() const{

	if (lifeTime.Read() >= lifetime) {
		
		return true;
	}
	return false;
}


void Spells::LoadSprites() {

	sprites = App->textures->Load("Spells/spells.png");
}

void Spells::UnLoadSprites() {

	App->textures->UnLoad(sprites);
}

void Spells::GetColliderFromAnimation(){

	int z = GetGamePos().z;
	int d = GetCharDepth();
	iPoint pivot_pos = PivotPos();
	spellColl->collider = currentAnimation->CurrentFrame().rect;
	spellColl->collider.z = z;
	spellColl->collider.d = d;

	spellColl->collider.x += pivot_pos.x;
	spellColl->collider.y += pivot_pos.y;
	
}