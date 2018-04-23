#include "FireBall.h"
#include "../../Engine/ModuleTextures.h"



FireBall::FireBall() : Spells(SpellsType::FIREBALL)
{
}


FireBall::~FireBall()
{
}

bool FireBall::Start() {
	sprites = App->textures->Load("Spells/Fireball.png");
	spellAnim.PushBack({0,0,45,65});
	spellAnim.PushBack({ 50,0,45,65 });
	currentAnimation = &spellAnim;
	spellColl = App->collision->CreateCollider({}, "FireBall_Spell", Collider::ATK);

	return true;
}

bool FireBall::CleanUp(pugi::xml_node&)
{
	App->textures->UnLoad(sprites);
	App->collision->RemoveCollider(spellColl);

	return true;
}