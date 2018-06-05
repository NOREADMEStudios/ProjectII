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
	App->entities->entities.push_back(this);

	spellColl = App->collision->CreateCollider({}, "Spell", Collider::SPELL);
	App->collision->AddCollider(spellColl, this);



	//collider = { 0,0,45,65 };

	spellAnim.speed = 10;
	stats.atk = 10;

	char_depth = 20;

	lifeTime.Start();
	lifetime = MS_LIFETIME;



	return true;
}

bool FireDemon::CleanUp(pugi::xml_node&)
{
	//UnLoadSprites();
	currentAnimation->Reset();
	bool ret = App->collision->RemoveCollider(&spellColl);

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