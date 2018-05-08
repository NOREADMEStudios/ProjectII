#include "Spells.h"
#include "ModuleTextures.h"


Spells::Spells(SpellsType spellType): Entity(EntityTypes::SPELLS)
{
}


Spells::~Spells()
{
}

void Spells::UpdateCollidersPosition() {

	collider.x = position.x;
	collider.y = position.y;

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