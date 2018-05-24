#include "Dagger.h"
#include "../../Engine/ModuleTextures.h"
#include "../../Engine/ModuleMap.h"

#define MS_LIFETIME	5000


Dagger::Dagger() : Spells(SpellsType::DAGGER)
{
}


Dagger::~Dagger()
{
}

bool Dagger::Start() {
	LoadSprites();
	App->entities->entities.push_back(this);

	spellColl = App->collision->CreateCollider({}, "Spell", Collider::SPELL);
	App->collision->AddCollider(spellColl, this);


	spellAnim.speed = 20;

	//collider = { 0,0,45,65 };

	stats.atk = 8;
	stats.spd = 10;

	char_depth = 20;
	max_speed = 1000;

	lifeTime.Start();
	lifetime = MS_LIFETIME;

	return true;
}

bool Dagger::CleanUp(pugi::xml_node&)
{
	//UnLoadSprites();
	bool ret = App->collision->RemoveCollider(spellColl);

	return ret;
}

bool Dagger::Update(float dt) {

	if (paused) {
		return PausedUpdate();
	}

	priority = gamepos.z;

	CalcRealPos();
	GetColliderFromAnimation();

	if (dir.x == -1)
	{
		flip = true;
	}
	else
	{
		flip = false;
	}

	Accelerate(100 * dir.x, 0, 0, dt);
	//UpdateCollidersPosition();
	Move(dt);
	Break(dt);

	App->render->FillQueue(this);//prints the spell

	return true;
}

bool Dagger::PostUpdate() {
	if (CheckLifetime()) {

		to_delete = true;
	}
	return true;
}

void Dagger::Dead() {

	App->entities->DestroyEntity(this);
}

void Dagger::OnCollisionEnter(Collider* _this, Collider* _other) {

	//if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;
	if (_other->entity == parent) return;

	int hit_dir;

	if (_this->collider.x - _other->collider.x > 0)

	{
		hit_dir = 1;
	}
	else
	{
		hit_dir = -1;
	}

	if (_other->type == Collider::HITBOX)
	{
		to_delete = true;
		if (_this->entity->team != _other->entity->team)
		_other->entity->stats.life -= stats.atk - _other->entity->stats.def;

		parent->SetPos(gamepos.x + ((_other->collider.w - 100) * hit_dir), 1, gamepos.z);
		parent->SetState(JUMP);

	}
}

