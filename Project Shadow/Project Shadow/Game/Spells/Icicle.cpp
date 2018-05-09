#include "Icicle.h"
#include "../../Engine/ModuleTextures.h"



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
	
	stats.atk = 10;

	char_depth = 20;

	lifeTime.Start();
	lifetime = ICICLE_MS_LIFETIME;

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

void Icicle::OnCollisionEnter(Collider* _this, Collider* _other) {

	if ((_this->entity->team != NOTEAM) && (_other->entity->team != NOTEAM) && (_this->entity->team == _other->entity->team)) return;
	for (int i = 0; i < entitiesHitted.size(); i++) {
		if (entitiesHitted[i] == _other->entity) {
			return;		
		}	
	}
	if (_other->type == Collider::HITBOX)
	{
		_other->entity->AdBuff(COOL_DURATION, -COOL_EFFECT * _other->entity->stats.spd);
		_other->entity->stats.life -= stats.atk - _other->entity->stats.def;
		entitiesHitted.push_back(_other->entity);
	}
}