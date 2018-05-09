#include "Lightning.h"




Lightning::Lightning() : Spells(SpellsType::LIGHTING)
{
}


Lightning::~Lightning()
{
}

bool Lightning::Start() {
	LoadSprites();
	spellAnim.LoadAnimationsfromXML("Lightning", SPELLS_ANIMS_ROOT);
	currentAnimation = &spellAnim;

	spellColl = App->collision->CreateCollider({}, "Lightning_Spell", Collider::SPELL);
	
	App->collision->AddCollider(spellColl, this);
	//collider = { 0,0,45,65 };

	stats.atk = 7;

	char_depth = 20;

	lifeTime.Start();
	lifetime = LIGHTNING_MS_LIFETIME;

	return true;
}

bool Lightning::CleanUp(pugi::xml_node&)
{
	UnLoadSprites();
	bool ret = App->collision->RemoveCollider(spellColl);

	return ret;
}

bool Lightning::Update(float dt) {

	if (paused) {
		return PausedUpdate();
	}

	priority = gamepos.z;

	CalcRealPos();
	GetColliderFromAnimation();
	
	App->render->FillQueue(this);//prints the spell

	return true;
}

bool Lightning::PostUpdate() {
	if (CheckLifetime()) {
		to_delete = true;
	}
	return true;
}

void Lightning::Dead() {
	App->entities->DestroyEntity(this);
}

void Lightning::OnCollisionEnter(Collider* _this, Collider* _other) {

	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;
	
	if (_other->type == Collider::HITBOX &&  !dealingDmg)
	{
		ticks.Start();
		dealingDmg = true;
		_other->entity->stats.life -= stats.atk - _other->entity->stats.def;
		
	}
}

void Lightning::OnCollisionStay(Collider* _this, Collider* _other) {
	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;

	if (_other->type == Collider::HITBOX) {
		if (ticks.Read() > TICKS_PER_DMG) {
			ticks.Start();
			if (stats.atk - _other->entity->stats.def <= 0)
				_other->entity->stats.life -= 1;
			else
				_other->entity->stats.life -= stats.atk - _other->entity->stats.def;
		}
	}
}
void Lightning::OnCollisionExit(Collider* _this, Collider* _other) {

	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;

	if (_other->type == Collider::HITBOX) {
		dealingDmg = false;
		
	}
}