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
	std::string red = "_red.png";
	std::string blue = "_blue.png";

	std::string spritePath;

	switch (corpseType) {
	case CLERIC:	
		spritePath = CLERIC_SPRITE_ROOT;
		animRect = {75,567,105,32};
		break;
	case WIZARD:
		spritePath = MAGE_SPRITE_ROOT;
		animRect = {880,2571,126,25};
		break;
	case ROGUE:
		spritePath = ELF_SPRITE_ROOT;
		animRect = {319,464,96,33};
		break;
	case WARRIOR:
		spritePath = MAGE_SPRITE_ROOT;
		animRect = {662,996,114,32};
		break;
	}

	if (team == Team::BLUE) {
		spritePath += blue;
	}
	else if (team == Team::RED) {
		spritePath += red;
	}

	sprites = App->textures->Load(spritePath.c_str());
	return true;
}