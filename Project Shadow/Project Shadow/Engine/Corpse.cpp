#include "Corpse.h"
#include "ModuleTextures.h"


Corpse::Corpse(CharacterTypes _type) : Character(CharacterTypes::CORPSE)
{
	corpseType = _type;
	type = CHARACTER;
	charType = CORPSE;
}


Corpse::~Corpse()
{
}

bool Corpse::HeroStart() {
	
	LoadShadow();

	switch (corpseType) {
	case CLERIC:			
		animRect = {75,567,105,32};
		break;
	case WIZARD:		
		animRect = {880,2571,126,25};
		break;
	case ROGUE:		
		animRect = {319,464,96,33};
		break;
	case WARRIOR:		
		animRect = {662,996,114,32};
		break;
	}
	
	return true;
}

bool Corpse::CleanUp(pugi::xml_node&) {

	App->textures->UnLoad(sprites);
	sprites = nullptr;

	UnloadShadow();
	to_delete = true;
	return true;
}

bool Corpse::Update(float dt) {

	DrawCorpse();

	return true;
}