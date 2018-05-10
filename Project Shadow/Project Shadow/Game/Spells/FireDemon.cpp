#include "FireDemon.h"

#define MS_LIFETIME	700

FireDemon::FireDemon() : Spells(SpellsType::FIRE_DEMON)
{
}


FireDemon::~FireDemon()
{
}

bool FireDemon::Start() {
	LoadSprites();

	spellAnim.LoadAnimationsfromXML("fire_demon", SPELLS_ANIMS_ROOT);
	currentAnimation = &spellAnim;
	spellAnim.speed = 10;

	spellColl = App->collision->CreateCollider({}, "FireDemon_Spell", Collider::SPELL);
	App->collision->AddCollider(spellColl, this);
	//collider = { 0,0,45,65 };

	stats.atk = 8;

	char_depth = 20;

	lifeTime.Start();
	lifetime = MS_LIFETIME;

	return true;
}

bool FireDemon::CleanUp(pugi::xml_node&)
{
	UnLoadSprites();
	bool ret = App->collision->RemoveCollider(spellColl);

	return ret;
}

bool FireDemon::Update(float dt) {

	if (paused) {
		return PausedUpdate();
	}

	priority = gamepos.z;

	CalcRealPos();
	GetColliderFromAnimation();


	App->render->FillQueue(this);//prints the spell

	return true;
}

bool FireDemon::PostUpdate() {
	if (CheckLifetime()) {

		to_delete = true;
	}
	return true;
}

void FireDemon::Dead() {

	App->entities->DestroyEntity(this);
}