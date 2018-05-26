#include "DeathMark.h"

#define MS_LIFETIME	3000

DeathMark::DeathMark() : Spells(SpellsType::DEATH_MARK)
{
}


DeathMark::~DeathMark()
{
}

bool DeathMark::Start() {
	LoadSprites();
	App->entities->entities.push_back(this);

	currentAnimation = &spellAnim;
	spellAnim.speed = 10;

	if (cl)
		spellAnim.speed = 15;



	lifeTime.Start();
	lifetime = MS_LIFETIME;

	return true;
}

bool DeathMark::CleanUp(pugi::xml_node&)
{
	currentAnimation->Reset();
	//UnLoadSprites();

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
	if (cl)
		gamepos.y = gp.y;
	else
	gamepos.y = gp.y + 75;

	gamepos.z = gp.z;
	if (cl && currentAnimation->Finished())
		to_delete = true;

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