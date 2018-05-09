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
	LoadSprites();
	spellAnim.LoadAnimationsfromXML("Icicle", SPELLS_ANIMS_ROOT);

	currentAnimation = &spellAnim;

	spellColl = App->collision->CreateCollider({}, "Icicle_Spell", Collider::SPELL);
	
	App->collision->AddCollider(spellColl, this);
	
	stats.atk = 8;

	char_depth = 20;

	lifeTime.Start();
	lifetime = MS_LIFETIME;

	return true;
}

bool Icicle::CleanUp(pugi::xml_node&)
{
	UnLoadSprites();
	bool ret = App->collision->RemoveCollider(spellColl);

	return ret;
}

bool Icicle::Update(float dt) {

	if (paused) {
		return PausedUpdate();
	}

	priority = gamepos.z;	

	CalcRealPos();
	GetColliderFromAnimation();

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
