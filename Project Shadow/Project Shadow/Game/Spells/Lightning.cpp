#include "Lightning.h"


#define MS_LIFETIME	3000

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

	stats.atk = 8;

	char_depth = 20;

	lifeTime.Start();
	lifetime = MS_LIFETIME;

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

void Lightning::OnCollisionEnter(Collider* _this, Collider* _other) {}