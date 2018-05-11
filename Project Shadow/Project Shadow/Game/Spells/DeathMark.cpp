#include "DeathMark.h"

#define MS_LIFETIME	5000

DeathMark::DeathMark() : Spells(SpellsType::FIRE_DEMON)
{
}


DeathMark::~DeathMark()
{
}

bool DeathMark::Start() {
	LoadSprites();


	currentAnimation = &spellAnim;
	spellAnim.speed = 10;

	//spellColl = App->collision->CreateCollider({}, "DeathMark_Spell", Collider::SPELL);
	//App->collision->AddCollider(spellColl, this);
	//collider = { 0,0,45,65 };

	stats.atk = 8;

	char_depth = 20;

	lifeTime.Start();
	lifetime = MS_LIFETIME;

	return true;
}

bool DeathMark::CleanUp(pugi::xml_node&)
{
	UnLoadSprites();


	return true;
}

bool DeathMark::Update(float dt) {

	if (paused) {
		return PausedUpdate();
	}

	priority = gamepos.z;

	CalcRealPos();
	//GetColliderFromAnimation();
	Point3D gp = parent->GetGamePos();
	gamepos.x = gp.x;
	gamepos.y = gp.y + 100;
	gamepos.z = gp.z;


	App->render->FillQueue(this);//prints the spell

	return true;
}

bool DeathMark::PostUpdate() {
	if (CheckLifetime()) {

		to_delete = true;
	}
	return true;
}

void DeathMark::Dead() {

	App->entities->DestroyEntity(this);
}

void DeathMark::SetPath(std::string _name)
{
	spellAnim.LoadAnimationsfromXML(_name, SPELLS_ANIMS_ROOT);
	currentAnimation = &spellAnim;
}