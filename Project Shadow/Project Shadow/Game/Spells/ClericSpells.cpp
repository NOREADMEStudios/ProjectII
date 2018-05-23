#include "ClericSpells.h"
#include "../../Engine/ModuleTextures.h"
#include "../../Engine/ModuleMap.h"
#include "DeathMark.h"

#define MS_LIFETIME	5000


Stun::Stun() : Spells(SpellsType::CLERIC_STUN)
{
}


Stun::~Stun()
{
}

bool Stun::Start() {
	LoadSprites();
	App->entities->entities.push_back(this);

	spellColl = App->collision->CreateCollider({}, "Spell", Collider::SPELL);
	App->collision->AddCollider(spellColl, this);


	currentAnimation->speed = 15;
	currentAnimation->loop = false;

	//collider = { 0,0,45,65 };

	stats.atk = 8;
	stats.spd = 8;

	char_depth = 20;
	max_speed = 300;

	lifeTime.Start();
	lifetime = MS_LIFETIME;

	return true;
}

bool Stun::CleanUp(pugi::xml_node&)
{
	//UnLoadSprites();
	currentAnimation->Reset();
	bool ret = App->collision->RemoveCollider(spellColl);

	return ret;
}

bool Stun::Update(float dt) {

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

	Move(dt);
	Break(dt);

	App->render->FillQueue(this);//prints the spell

	if (currentAnimation->Finished())
		to_delete = true;

	return true;
}

bool Stun::PostUpdate() {
	if (CheckLifetime()) {

		to_delete = true;
	}
	return true;
}

void Stun::Dead() {

	App->entities->DestroyEntity(this);
}

void Stun::OnCollisionEnter(Collider* _this, Collider* _other) {

	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;
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
		_other->entity->AdBuff(3, -_other->entity->stats.spd);
		DeathMark* stun = new DeathMark{ *(DeathMark*)App->entities->stuned };
		stun->SetPos(gamepos.x, gamepos.y + 20, gamepos.z);
		stun->SetParent((Character*)_other->entity);
		stun->Start();


	}
}

