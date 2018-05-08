#include "Icicle.h"
#include "../../Engine/ModuleTextures.h"

#define MS_LIFETIME	3000

Icicle::Icicle() : Spells(SpellsType::ICICLE)
{
}


Icicle::~Icicle()
{
}

bool Icicle::Start() {
	sprites = App->textures->Load("Spells/Fireball.png");
	spellAnim.PushBack({ 0,0,45,65 });
	spellAnim.PushBack({ 50,0,45,65 });
	spellAnim.PushBack({ 101,0,45,65 });
	currentAnimation = &spellAnim;

	spellColl = App->collision->CreateCollider({}, "Icicle_Spell", Collider::SPELL);
	spellColl->collider = { 0,0,45,65 };
	App->collision->AddCollider(spellColl, this);
	
	stats.atk = 8;

	char_depth = 20;

	lifeTime.Start();
	lifetime = MS_LIFETIME;

	return true;
}

bool Icicle::CleanUp(pugi::xml_node&)
{
	App->textures->UnLoad(sprites);
	bool ret = App->collision->RemoveCollider(spellColl);

	return ret;
}

bool Icicle::Update(float dt) {

	if (paused) {
		return PausedUpdate();
	}

	priority = gamepos.z;
	spellColl->collider.x = gamepos.x;
	spellColl->collider.y = gamepos.z;

	CalcRealPos();

	//UpdateCollidersPosition();

	App->render->FillQueue(this);//prints the spell

	return true;
}

bool Icicle::PostUpdate() {
	if (CheckLifetime()) {
		to_delete = true;
	}
	return true;
}

void Icicle::Dead() {
	App->entities->DestroyEntity(this);
}
