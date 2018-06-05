#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "App.h"
#include "Character.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "Corpse.h"

#include "ModuleMap.h"


class Warrior;


Character::Character(CharacterTypes charType) : Entity(EntityTypes::CHARACTER)
{
}


Character::~Character()
{}

bool Character::Awake(pugi::xml_node&)
{
	return true;
}

bool Character::Start()
{ 
	SetAnimations();
	LoadBasicStates();

	gamepos.y = 0;
	CalcRealPos();
	collider.x = position.x;
	collider.y = position.y;


	currentState = IDLE;
	UpdateAnimation();
	currentTag = 0;
	wantedTag = 0;
	cleric_ab = false;

	collAtk = App->collision->CreateCollider({}, "enemy_attack", Collider::ATK);
	collHitBox = App->collision->CreateCollider({}, "player_hitbox", Collider::HITBOX);
	collFeet = App->collision->CreateCollider({}, "player_feet", Collider::FEET);
	collDef = App->collision->CreateCollider({}, "player_shield", Collider::DEF);
	collParry = App->collision->CreateCollider({}, "player_parry", Collider::PARRY);
	UpdateCollidersPosition();

	App->collision->AddCollider(collAtk, this);
	App->collision->AddCollider(collHitBox, this);
	App->collision->AddCollider(collFeet, this);
	App->collision->AddCollider(collDef, this);
	App->collision->AddCollider(collParry, this);

	invencible.dur = 3;
	invencible.fr = 0.2f;
	char_depth = 20;


	initialpos.x = gamepos.x;
	initialpos.y = gamepos.z;
	lives = maxLives;

	LoadShadow();

	HeroStart();
	noMove.SetZero();

	max_speed = stats.spd;
	max_speed_y = stats.spd;
	initialLife = stats.life;

	collider = currentAnimation->CurrentFrame().rect;

	active = true;
	return true; 
}

bool Character::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true; 
}

bool Character::Update(float dt)
{ 

	if (paused) {
		resume = true;
		return PausedUpdate();
	}

	if (resume) {
		currentAnimation->ResumeFrame();
		resume = false;
	}
	

	if (stats.hpRecover && hpRecTimer.Count(5) && stats.life < initialLife)
	{
		AdHp(5);
		hpRecTimer.Start();
	}

	currentAnimation = &states.front()->anim;


	if (stats.life > 0 && noMove.IsZero())
		RequestState();
	else if (stats.life <= 0)
		currentState = DEATH;

	if (!eventstates.empty())
		UpdateEventStates();

	Move(dt);
	Break(dt);

	UpdateAbilities();
	UpdateAnimation();
	UpdateMainStates();
	UpdateSpecStates();
	UpdateCurState(dt);
	HeroUpdate(dt);



	priority = gamepos.z;
	collider.x = position.x;
	collider.y = position.y;

	CalcRealPos();
	UpdateCollidersPosition();

	if (!invencible.active)
		App->render->FillQueue(this);

	else if (invencible.active)
	{
		invencible.UpdateInv();

		if (invencible.on)
			App->render->FillQueue(this);
	}

	// Map boundaries for the Heroes
	if (gamepos.x < App->map->GetMapBorders_X())
		gamepos.x = App->map->GetMapBorders_X();
	if (gamepos.z < App->map->GetMapBorders_Z())
		gamepos.z = App->map->GetMapBorders_Z();
	if (gamepos.x > App->map->GetMapBorders_X() + App->map->GetMapBorders_W())
		gamepos.x = App->map->GetMapBorders_X() + App->map->GetMapBorders_W();
	if (gamepos.z > App->map->GetMapBorders_Z() + App->map->GetMapBorders_H())
		gamepos.z = App->map->GetMapBorders_Z() + App->map->GetMapBorders_H();

	return true; 
}

bool Character::PostUpdate()
{
	return true; 
}

bool Character::CleanUp(pugi::xml_node&)
{ 
	App->textures->UnLoad(sprites);

	UnloadShadow();

	/*if (collAtk) {
		App->collision->RemoveCollider(&collAtk);
	}
	if (collDef) {
		App->collision->RemoveCollider(&collDef);
	}
	if (collParry) {
		App->collision->RemoveCollider(&collParry);
	}*/

	Utils::ClearList(attacks);


	/*App->collision->RemoveCollider(&collHitBox);
	App->collision->RemoveCollider(&collFeet);*/

	if (heroCorpse) {
		pugi::xml_node n;
		heroCorpse->CleanUp(n);
	}
	
	return true;
}


void Character::ModifyStats(int attack, int defense, int speed, int magic)
{
	//If one new stat = 0 then isn't modified

	if (attack != 0)
	{
		stats.atk = attack;
		
	}
	if (defense != 0)
	{
		stats.def = defense;

	}
	if (speed != 0)
	{
		stats.spd = speed;

	}

	if (magic != 0)
	{
		stats.mgk = magic;

	}

}

void Character::UpdateCollidersPosition() {
	GetCollidersFromAnimation();
	iPoint pivot_pos = PivotPos();
	collFeet->collider.x += pivot_pos.x;//pivot
	collFeet->collider.y += pivot_pos.y;//pivot
	collHitBox->collider.x += pivot_pos.x;
	collHitBox->collider.y += pivot_pos.y;
	if (!(collAtk->collider.w == 0)) {
		collAtk->collider.x += pivot_pos.x;
		collAtk->collider.y += pivot_pos.y;
	}
	if (!(collDef->collider.w == 0)) {
		collDef->collider.y += pivot_pos.y;
		collDef->collider.x += pivot_pos.x;
	}
	if (!(collParry->collider.w == 0)) {
		collParry->collider.y += pivot_pos.y;
		collParry->collider.x += pivot_pos.x;
	}
}

void Character::GetCollidersFromAnimation() {
	int z = GetGamePos().z;
	int d = GetCharDepth();
	collFeet->collider = currentAnimation->GetFeetColliderFromFrame();
	collFeet->collider.z = z;
	collFeet->collider.d = d;
	collHitBox->collider = currentAnimation->GetHitBoxColliderFromFrame();
	collHitBox->collider.z = z;
	collHitBox->collider.d = d;
	collAtk->collider = currentAnimation->GetAtkColliderFromFrame();
	collAtk->collider.z = z;
	collAtk->collider.d = d;
	collDef->collider = currentAnimation->GetDefColliderFromFrame();
	collDef->collider.z = z;
	collDef->collider.d = d;
	collParry->collider = currentAnimation->GetParryColliderFromFrame();
	collParry->collider.z = z;
	collParry->collider.d = d;

	if (flip)
	{
		collFeet->collider.x = currentAnimation->CurrentFrame().rect.w - (collFeet->collider.x + collFeet->collider.w);
		collHitBox->collider.x = currentAnimation->CurrentFrame().rect.w - (collHitBox->collider.x + collHitBox->collider.w);
		if (!(collAtk->collider.w==0))
		collAtk->collider.x = currentAnimation->CurrentFrame().rect.w - (collAtk->collider.x + collAtk->collider.w);
		if (!(collDef->collider.w == 0))
		collDef->collider.x = currentAnimation->CurrentFrame().rect.w - (collDef->collider.x + collDef->collider.w);
		if (!(collParry->collider.w == 0))
		collParry->collider.x = currentAnimation->CurrentFrame().rect.w - (collParry->collider.x + collParry->collider.w);
	}
}

std::list<CharInput> Character::RequestInputs() const {
	
		std::list<Input> inputs;
		std::list<CharInput> charInputs;
		inputs = App->input->GetInputListFromController(heroNum);
		for (std::list<Input>::iterator item = inputs.begin(); item != inputs.end(); item++) {
			Input input = *item;
			switch (input) {
			case NONEINPUT:
				break;
			case UP:
				charInputs.push_back(CharInput::CH_UP);
				break;
			case DOWN:
				charInputs.push_back(CharInput::CH_DOWN);
				break;
			case RIGHT:
				charInputs.push_back(CharInput::CH_RIGHT);
				break;
			case LEFT:
				charInputs.push_back(CharInput::CH_LEFT);
				break;
			case BUTTON_A:
				charInputs.push_back(CharInput::JUMPINPUT);
				break;
			case BUTTON_B:
				charInputs.push_back(CharInput::AB_1);
				break;
			case BUTTON_X:
				charInputs.push_back(CharInput::LIGHT_ATTACK);
				break;
			case BUTTON_Y:
				charInputs.push_back(CharInput::HEAVY_ATTACK);
				break;
			case L_SHOULDER:
				charInputs.push_back(CharInput::DEFEND);
				break;
			case R_SHOULDER:
				charInputs.push_back(CharInput::RUNINPUT);
				break;
			case BUTTON_SELECT:
				charInputs.push_back(CharInput::TAUNTINPUT);
				break;
			case R2:
				charInputs.push_back(CharInput::AB_3);
				break;
			case L2:
				charInputs.push_back(CharInput::AB_2);
				break;
			default:
				break;
			}
		}

		return charInputs;
	}

void Character::RequestState() {
	std::list<CharInput> inputs;
	int NumControllers = App->input->GetNumControllers();
	if (heroNum <= NumControllers) {
		inputs = RequestInputs();
	}
	else {
		if (heroNum - NumControllers == 1) {
			inputs = FirstPlayerConfig();
		}

		/*else if (heroNum - NumControllers == 2)
			inputs = SecondPlayerConfig();*/
	}

	bool run = false,
		block = false;
	directions.down = false;
	directions.up = false;
	directions.left = false;
	directions.right = false;

	wantedState = IDLE;
	wantedTag = 0;

	for (std::list<CharInput>::iterator item = inputs.begin(); item != inputs.end(); item++) {
		CharInput input = *item;
		switch (input)
		{
		case NONECHARINPUT:
			break;
		case CH_UP:
			directions.up = true;
			break;
		case CH_DOWN:
			directions.down = true;
			break;
		case CH_RIGHT:
			directions.right = true;
			break;
		case CH_LEFT:
			directions.left = true;
			break;
		case LIGHT_ATTACK:
			wantedState = ATTACK_LIGHT;
			wantedTag = 1;
			break;
		case HEAVY_ATTACK:
			wantedState = ATTACK_HEAVY;
			wantedTag = 2;
			break;
		case JUMPINPUT:
			wantedState = JUMP;
			wantedTag = 3;
			break;
		case RUNINPUT:
			run = true;
			break;
		case DEFEND:
			wantedState = PROTECT;
			break;
		case TAUNTINPUT:
			wantedState = TAUNT;
			break;
		case PARRYINPUT:
			wantedState = PARRY;
			break;
		case AB_1:
			wantedState = AD_ACTION;
			wantedTag = 11;
			break;
		case AB_2:
			wantedState = AD_ACTION;
			wantedTag = 12;
			break;
		case AB_3:
			wantedState = AD_ACTION;
			wantedTag = 13;
			break;
		default:
			break;
		}
	}

	if (wantedState == IDLE && (directions.down == true || directions.up == true || directions.left == true || directions.right == true))
	{
		wantedState = WALK;
		if (run)
			wantedState = RUN;
	}

}

void Character::UpdateMainStates()
{
	if (wantedTag != 0 && GetAtk(wantedTag)->ability)
	{
		if (!GetAbAtk(wantedTag)->active || (currentState == JUMP || (currentState == AD_ACTION && currentTag != 0 && GetAtk(currentTag)->air)))
		{
			wantedTag = currentTag;
			if (!StateisAtk(currentState))
				wantedState = currentState;
		}
		else 
			GetAbAtk(wantedTag)->Activate();

	}

	if (currentState == WALK || currentState == IDLE)
	{
		currentState = wantedState;
		if (wantedTag != 0)
		{
			if (last_attack != 0)
			{
				SetCombo();
			}
			else
			{
				currentTag = wantedTag;
				last_attack = currentTag;
			}
			time_attack.Start();
		}
		else
		{
			currentTag = 0;
		}
	}
	
	
	if (currentState == JUMP || (currentState == AD_ACTION && currentTag != 0 &&  GetAtk(currentTag)->air))
	{
		last_attack = 3;
		if (StateisAtk(wantedState))
		{
			SetCombo();
			time_attack.Start();
		}
		if ((currentState == AD_ACTION && currentTag != 0 && GetAtk(currentTag)->air) && currentAnimation->Finished())
		{
			currentAnimation->Reset();
			currentState = JUMP;
		}
		if (gamepos.y <= 0)
		{
			jumping = false;
			speedVector.y = 0;
			currentAnimation->Reset();
			currentState = wantedState;
		}
	}
	else if (currentState == KNOKED)
	{
		if (currentAnimation->Finished() && gamepos.y <= 0)
		{
			currentAnimation->Reset();
			currentState = IDLE;
		}

	}
	else if (currentAnimation->Finished())
	{
		if (currentState == DEATH)
		{
			active = false;
			LeaveCorpse();
		}
		
		if (StateisAtk(currentState))
			time_attack.Start();

		currentState = wantedState;

		if (noMove.IsZero())
		currentAnimation->Reset();

		if (wantedTag != 0)
		{
			if (last_attack != 0)
			{
				SetCombo();
			}
			else
			{
				currentTag = wantedTag;
				last_attack = currentTag;
			}
			time_attack.Start();
		}
		else
		{
			currentTag = 0;
		}
	}
	if (currentState == AD_ACTION && currentTag != 0)
	{
		char_depth = GetAtk(currentTag)->depth;
	}
	else
	{
		char_depth = 20;
	}


	//if (stats.spd == 0 && currentState != HIT)
	//{
	//	currentState = STUNED;
	//	currentTag = 0;
	//}

	if (time_attack.Count(COMBO_MARGIN))
	{
		last_attack = 0;
	}
}

void Character::UpdateCurState(float dt)
{
	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;


	if (gamepos.y > 0)
	{
		if (currentState == AD_ACTION && currentTag != 0 && GetAtk(currentTag)->air)
		Accelerate(x_dir, -1, z_dir, dt);

		else 
			Accelerate(x_dir, -2, z_dir, dt);
	
	}
	else if (gamepos.y < 0)
	{
		gamepos.y = 0;
	}

	switch (currentState)
	{
		case WALK:
		{
			max_speed = stats.spd;
			Accelerate(x_dir * stats.spd, 0, z_dir * stats.spd, dt);
			break;
		}
		case HIT:
		{
			if (hit_bool)
			{
				Accelerate(hit_dir, 0, 0, dt);
				hit_bool = false;
			}
			if (gamepos.y > 0)
			{
				currentState = KNOKED;
				currentTag = 0;
			}
			break;
		}
		case JUMP:
		{
			if (!jumping)
			{
				jumping = true;
				max_speed_y = 1000;
				Accelerate(x_dir, 500, z_dir, dt);
			}
			break;
		}

	}
}

void Character::UpdateAnimation()
{
	if (!StateisAtk(currentState))
	{
		for (std::list<State*>::iterator item = states.begin(); item != states.end(); item++)
		{
			if ((*item)->state == currentState)
			{
				currentAnimation = &(*item)->anim;
				break;
			}
		}
	}
	else
	{
		for (std::list<Attack*>::iterator item = attacks.begin(); item != attacks.end(); item++)
		{
			if ((*item)->tag == currentTag)
			{
				currentAnimation = &(*item)->anim;
				break;
			}
		}
	}

}

void Character::Respawn()
{
	gamepos.x = initialpos.x;
	gamepos.z = initialpos.y;

	stats.life = initialLife;
	invencible.StartInv();
}

void Character::GetHP(int& curr, int& max)
{
	curr = stats.life;
	max = initialLife;
}



bool Character::IsAbCooldown(uint abNum) const { 
	if (abilities.size()>abNum && abilities[abNum].active) {
		return false;
	}
	else 
		return true;
}

bool Character::StateisAtk(CharStateEnum state)
{
	return (state == ATTACK_LIGHT || state == ATTACK_HEAVY || state == AD_ACTION);
}

Attack* Character::GetAtk(uint atk)
{
	Attack* ret = nullptr;

	for (std::list<Attack*>::iterator item = attacks.begin(); item != attacks.end(); item++) {
		if ((**item).tag == atk)
		{
			ret = (*item);
		}
	}
	return ret;
}

Ability* Character::GetAbAtk(uint atk)
{

	Ability* ret = nullptr;
	for (std::vector<Ability>::iterator item = abilities.begin(); item != abilities.end(); item++) 
	{

		if (item->atk->tag == atk)
		{
			ret = &(*item);
		}

	}
	return ret;
}

void Character::LeaveCorpse() {

	heroCorpse = new Corpse(this->charType);
	heroCorpse->gamepos = this->gamepos;
	heroCorpse->team = this->team;
	heroCorpse->sprites = sprites;
	heroCorpse->flip = flip;
	App->entities->entities.push_back(heroCorpse);
	heroCorpse->HeroStart();

}

uint Character::GetMaxLives() const
{
	return maxLives;
}

uint Character::GetCurrentLives() const
{
	return lives;
}

void Character::SetCombo()
{
	Attack* wanted_atk = GetAtk(wantedTag);
	Attack* current_atk = GetAtk(last_attack);

	if (current_atk != nullptr && wanted_atk != nullptr && current_atk->CheckChildInput(wanted_atk->input))
	{
		current_atk = current_atk->GetChildInput(wanted_atk->input);
		currentState = AD_ACTION;
		last_attack = current_atk->tag;
		currentTag = last_attack;
	}
	else
	{
		currentTag = wantedTag;
		last_attack = currentTag;
	}

}

void Character::LoadState(CharStateEnum _state, std::string animationName)
{
	State* st = new State;
	st->state = _state;

	st->anim.LoadAnimationsfromXML(animationName, animations_name);
	states.push_back(st);
}

void Character::LoadBasicStates()
{
	LoadState(IDLE, "idle");
	LoadState(WALK, "walking");
	LoadState(JUMP, "jump");
	LoadState(STOP, "stop");
	LoadState(HIT, "hit");
	LoadState(DEATH, "death");
	LoadState(TAUNT, "win");
	LoadState(KNOKED, "death");
	LoadState(STUNED, "stuned");

}

void Character::UpdateTag(uint& t)
{
	if (currentState == ATTACK_LIGHT)
		t = 1;
	else if (currentState == ATTACK_HEAVY)
		t = 2;
	else
		t = 0;
}

void Character::UpdateAbilities()
{
	for (std::vector<Ability>::iterator item = abilities.begin(); item != abilities.end(); item++)
	{
		if (item->timer.Count(item->cooldown))
		{
			item->active = true;
		}
		else
			item->active = false;
	}
}

void Character::AdAbility(Ability ab)
{
	abilities.push_back(ab);
}


void Character::UpdateEventStates()
{
	std::list<EventState*>::iterator item = eventstates.begin(); 

	while (item != eventstates.end())
	{
		if ((*item)->timer.Count((*item)->time_active))
		{
			stats = stats - (*item)->stats;
			item = eventstates.erase(item);
		}
		else
		{
			item++;
		}
	}

}


void Character::SetAnimations()
{
	std::string red = "_red.png";
	std::string blue = "_blue.png";
	bool cleric = false;

	std::string spritePath;
	switch (charType)
	{
		case WARRIOR:
		{
			animations_name = FIGHTER_ANIM_ROOT;
			spritePath = FIGHTER_SPRITE_ROOT;
			break;
		}	
		case ROGUE:
		{
			animations_name = ELF_ANIM_ROOT;
			spritePath = ELF_SPRITE_ROOT;
			break;
		}
		case WIZARD:
		{
			animations_name = MAGE_ANIM_ROOT;
			spritePath = MAGE_SPRITE_ROOT;
			break;
		}
		case CLERIC:
		{
			animations_name = CLERIC_ANIM_ROOT;
			spritePath = CLERIC_SPRITE_ROOT;
			cleric = true;
			break;
		}
		
	}
	if (!cleric)
	{
		if (team == Team::BLUE) {
			spritePath += blue;
		}
		else if (team == Team::RED) {
			spritePath += red;
		}
	}


	sprites = App->textures->Load(spritePath.c_str());
}

void Character::SetCharType(CharacterTypes type)
{
	charType = type;
}

std::list<CharInput> Character::FirstPlayerConfig()
{

	std::list<CharInput> ret;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		ret.push_back(CharInput::CH_LEFT);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		ret.push_back(CharInput::CH_RIGHT);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		ret.push_back(CharInput::CH_UP);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		ret.push_back(CharInput::CH_DOWN);

	if (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
		ret.push_back(CharInput::RUNINPUT);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		ret.push_back(CharInput::JUMPINPUT);

	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
		ret.push_back(CharInput::LIGHT_ATTACK);

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		ret.push_back(CharInput::HEAVY_ATTACK);

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
		ret.push_back(CharInput::DEFEND);

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
		ret.push_back(CharInput::PARRYINPUT);

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		ret.push_back(CharInput::TAUNTINPUT);

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		ret.push_back(CharInput::AB_3);

	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		ret.push_back(CharInput::AB_1);
	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
		ret.push_back(CharInput::AB_2);

	return ret;

}

void Character::AdHp(int hp)
{
	stats.life += hp;

	if (stats.life > initialLife)
	{
		stats.life = initialLife;
	}
}

void Character::DrawCorpse() {


	priority = gamepos.z;	
	App->render->FillQueue(this);
	
}