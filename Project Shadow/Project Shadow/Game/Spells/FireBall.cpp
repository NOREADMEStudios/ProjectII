#include "FireBall.h"
#include "../../Engine/ModuleTextures.h"
#include "../../Engine/ModuleMap.h"

#define MS_LIFETIME	500


FireBall::FireBall() : Spells(SpellsType::FIREBALL)
{
}


FireBall::~FireBall()
{
}

bool FireBall::Start() {
	LoadSprites();


	spellAnim.speed = 20;
	App->entities->entities.push_back(this);

	spellColl = App->collision->CreateCollider({}, "Spell", Collider::SPELL);
	App->collision->AddCollider(spellColl, this);



	//collider = { 0,0,45,65 };

	stats.atk = 15;

	char_depth = 20;

	lifeTime.Start();
	lifetime = MS_LIFETIME;

	return true;
}

bool FireBall::CleanUp(pugi::xml_node&)
{
	UnLoadSprites();
	bool ret = App->collision->RemoveCollider(spellColl);

	return ret;
}

bool FireBall::Update(float dt) {

	if (paused) {
		return PausedUpdate();
	}

	priority = gamepos.z;	

	CalcRealPos();
	GetColliderFromAnimation();



	if (gamepos.x < App->map->GetMapBorders_X() || gamepos.z < App->map->GetMapBorders_Z() || gamepos.x > App->map->GetMapBorders_X() + App->map->GetMapBorders_W() || gamepos.z > App->map->GetMapBorders_Z() + App->map->GetMapBorders_H())
		stop = true;

	//UpdateCollidersPosition();

	App->render->FillQueue(this);//prints the spell

	return true;
}

bool FireBall::PostUpdate() {
	if (CheckLifetime()) {
		if (!stop)
		App->entities->CreateSpell({ FIREBALL,team,{ gamepos.x + (50 * dir.x), gamepos.y, gamepos.z + (30 * dir.y) }, dir });

		to_delete = true;
	}
	return true;
}

void FireBall::Dead() {

	App->entities->DestroyEntity(this);
}

void FireBall::OnCollisionEnter(Collider* _this, Collider* _other) {

	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;

	if (_other->type == Collider::HITBOX)
	{
		to_delete = true;
		_other->entity->stats.life -= stats.atk - _other->entity->stats.def;

	}

	if (_other->type == Collider::PARRY)
	{
		to_delete;
	}
}