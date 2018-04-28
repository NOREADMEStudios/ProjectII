#include "Hero.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "ModuleMap.h"
#include "App.h"

#define HERO_SPRITE_ROOT "Assets/Animations/Characters/Fighter_Animations.tmx"


Hero::Hero() : Character(CharacterTypes::HERO)
{
}

Hero::~Hero()
{
}


bool Hero::Awake(pugi::xml_node&)
{
	return true;
}

bool Hero::HeroStart()
{
	switch (hero_num) {
	case 1: 
		sprites = App->textures->Load("Characters/Fighter_sprites_red.png");
		break;
	case 2:
		sprites = App->textures->Load("Characters/Fighter_sprites_green.png");
		break;
	case 3:
		sprites = App->textures->Load("Characters/Fighter_sprites_blue.png");
		break;
	case 4:
		sprites = App->textures->Load("Characters/Fighter_sprites_grey.png");
		break;
	}




	invencible.dur = 3;
	invencible.fr = 0.2f;
	//collider = { 50 , 50 , 50, 50 };

	char_depth = 20;
	gamepos.y = 0;

	stats.atk += App->entities->items[hero_num-1].atk;
	stats.def += App->entities->items[hero_num-1].def;
	stats.spd += App->entities->items[hero_num-1].spd;
	stats.life += App->entities->items[hero_num-1].life;




	Attack* light_1 = new Attack(1, LIGHT_ATTACK, "attack", 2);
	Attack* heavy_1 = new Attack(2, HEAVY_ATTACK, "kick", 2, false, true);
	Attack* jump_a = new Attack(3, JUMPINPUT, "jump", 0, true);
	Attack* light_2 = new Attack(4, LIGHT_ATTACK, "attack_knee", 2);
	Attack* heavy_2 = new Attack(5, HEAVY_ATTACK, "strong_attack", 2);
	Attack* light_3 = new Attack(6, LIGHT_ATTACK, "attack_2", 5);
	Attack* jump_a2 = new Attack(7, LIGHT_ATTACK,"jump_attack", 2, true);
	Attack* jump_a3 = new Attack(8, HEAVY_ATTACK, "windwhirl", 5, true);

	attacks.push_back(light_1);
	attacks.push_back(light_2);
	attacks.push_back(light_3);
	attacks.push_back(heavy_1);
	attacks.push_back(heavy_2);
	attacks.push_back(jump_a);
	attacks.push_back(jump_a2);
	attacks.push_back(jump_a3);

	light_1->AddChild(light_2);
	light_2->AddChild(light_3);
	//heavy_1->AddChild(light_3);
	light_2->AddChild(heavy_2);
	//heavy_1->AddChild(heavy_2);
	jump_a->AddChild(jump_a2);
	jump_a->AddChild(jump_a3);

	Ability* kick = new Ability(heavy_1, 3);
	AdAbility(*kick);

	
	return true;
}

bool Hero::PreUpdate()
{
	//collider = currentAnimation->CurrentFrame().rect;

	return true;
}

bool Hero::HeroUpdate(float dt)
{

	int z_dir = directions.down - directions.up;
	int x_dir = directions.right - directions.left;

	switch (currentState)
	{
		case PROTECT:
		{
			max_speed = stats.spd * 0.5f;
			Accelerate((x_dir * stats.spd), 0, (z_dir * stats.spd), dt);
			break;
		}
	}


	if (currentState != PROTECT && !StateisAtk(currentState)) {
		if (directions.right - directions.left == 1)
		{
			flip = false;
		}
		else if (directions.right - directions.left == -1)
		{
			flip = true;
		}
	}




	priority = gamepos.z;
	//collider.x = position.x;
	//collider.y = position.y;



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

bool Hero::PostUpdate()
{
	return true;
}

bool Hero::CleanUp(pugi::xml_node&)
{
	

	App->textures->UnLoad(sprites);

	UnloadShadow();

	/*if (collAtk) {
		App->collision->RemoveCollider(collAtk);
	}
	if (collDef) {
		App->collision->RemoveCollider(collDef);
	}
	if (collParry) {
		App->collision->RemoveCollider(collParry);
	}
	App->collision->RemoveCollider(collHitBox);
	App->collision->RemoveCollider(collFeet);*/

	Utils::ClearList(attacks);

	return true;
}

void Hero::UpdateSpecStates()
{
	if (currentState == PARRY)
	{
		if (parried)
		{
			currentAnimation->Reset();
			currentState = wantedState;
			parried = false;
		}
		else if (currentAnimation->Finished())
		{
			currentAnimation->Reset();
			currentState = wantedState;
		}
	}
	else if (currentState == PROTECT && wantedState != PROTECT)
	{
		currentState = wantedState;
		currentAnimation->Reset();

	}
}


void Hero::UpdateAnimation()
{
	if (currentState == WALK)
	{
		currentAnimation = &walking;
	}
	else if (currentState == IDLE)
	{
		currentAnimation = &idle;

		if (!sound_avaliable)
			sound_avaliable = true;
	}
	else if (currentState == ATTACK_LIGHT)
	{
		currentAnimation = &attack;

		if (sound_avaliable)
		{
			App->audio->PlayFx(1);
			sound_avaliable = false;
		}
	}
	else if (currentState == ATTACK_L2)
	{
		currentAnimation = &attack_l2;

		if (sound_avaliable)
		{
			App->audio->PlayFx(1);
			sound_avaliable = false;
		}
	}
	else if (currentState == HIT)
	{
		currentAnimation = &hit;

		if (sound_avaliable)
			{
				App->audio->PlayFx(3);
				sound_avaliable = false;
			}
	}
	else if (currentState == DEATH)
	{
		currentAnimation = &death;
		if (sound_avaliable)
		{
			App->audio->PlayFx(8);
			sound_avaliable = false;
		}
	}
	else if (currentState == JUMP)
	{
		currentAnimation = &jump;

		if (sound_avaliable)
		{
			App->audio->PlayFx(5);
			sound_avaliable = false;
		}
	}
	else if (currentState == RUN)
	{
		currentAnimation = &run;
	}
	else if (currentState == ATTACK_HEAVY)
	{
		currentAnimation = &kick;

		if (sound_avaliable)
		{
			App->audio->PlayFx(11);
			sound_avaliable = false;
		}
	}
	else if (currentState == ATTACK_L3)
	{
		currentAnimation = &attack_l3;
		if (sound_avaliable)
		{
			App->audio->PlayFx(1);
			sound_avaliable = false;
		}
	}
	else if (currentState == STOP)
	{
		currentAnimation = &stop;
	}
	else if (currentState == PROTECT)
	{
		currentAnimation = &protect;
	}
	else if (currentState == TAUNT)
	{
		currentAnimation = &taunt;

		if (sound_avaliable)
		{
			App->audio->PlayFx(13);
			sound_avaliable = false;
		}
	}
	else if (currentState == ATTACK_H2)
	{
		currentAnimation = &attack_s2;

		if (sound_avaliable)
		{
			App->audio->PlayFx(12);
			sound_avaliable = false;
		}
	}
	else if (currentState == PARRY)
	{
		currentAnimation = &parry;

		if (sound_avaliable)
		{
			App->audio->PlayFx(14);
			sound_avaliable = false;
		}
	}
	else if (currentState == ATTACK_J1)
	{
		currentAnimation = &jumpAtk;
	}
	else if (currentState == ATTACK_J2)
	{
		currentAnimation = &attack_j2;
	}
}

void Hero::Respawn()
{
	gamepos.x = initialpos.x;
	gamepos.z = initialpos.y;

	stats.life = initialLife;
	invencible.StartInv();
}

void Hero::GetHP(int& curr, int& max)
{
	curr = stats.life;
	max = initialLife;
}

void Hero::OnCollisionEnter(Collider* _this, Collider* _other)
{
	if (_this->entity == _other->entity) return;

	int z1 = _this->entity->GetGamePos().z;
	int d1 = _this->entity->GetCharDepth();

	int z2 = _other->entity->GetGamePos().z;
	int d2 = _other->entity->GetCharDepth();

	int p11 = z1 - (d1 / 2);
	int p12 = z1 + (d1 / 2);
	int p21 = z2 - (d2 / 2);
	int p22 = z2 + (d2 / 2);

	if ((p11 <= p21 && p21 <= p12) || (p11 <= p22 && p22 <= p12) || (p21 <= p11 && p11 <= p22) || (p21 <= p12 && p12 <= p22))
	{
		if (_this->collider.x - _other->collider.x > 0)
		{
			hit_dir = 1;
		}
		else
		{
			hit_dir = -1;
		}

		if (_this->sTag == "player_shield" && _other->sTag == "enemy_attack")
		{
			if (_other->entity->breaking)
			{
				currentState = HIT;
				stats.life -= _other->entity->stats.atk;
				hit_bool = true;

				
			}
			else
			{
				max_speed = 600;
				_this->entity->Accelerate(hit_dir * 1000, 0, 0, 1);
			}
			App->audio->PlayFx(10);
		}
		else if (_this->sTag == "enemy_attack" && _other->sTag == "player_shield")
		{
			max_speed = 400;
			_this->entity->Accelerate(hit_dir * 800, 0, 0, 1);
		}
		else if (_this->sTag == "player_parry" && _other->sTag == "enemy_attack")
		{
			currentState = IDLE;
			parried = true;
		}
		else if (_this->sTag == "enemy_attack" && _other->sTag == "player_parry")
		{
			currentState = HIT;
		}
		else if (_this->sTag == "player_hitbox" && _other->sTag == "enemy_attack")
		{
			currentState = HIT;
			hit_bool = true;


			if (_this->collider.x - _other->collider.x > 0)
			{
				hit_dir = 1 * _other->entity->stats.atk;
			}
			else
			{
				hit_dir = -1 * _other->entity->stats.atk;
			}
						
		}
		else if (_this->sTag == "enemy_attack" && _other->sTag == "player_hitbox" && StateisAtk(currentState))
		{
			Attack * atk = GetAtk(currentState);
			if (atk != nullptr)
				_other->entity->stats.life -= _this->entity->stats.atk + atk->damage - _other->entity->stats.def;
		}

	}
}



bool Hero::StateisAtk(CharStateEnum state)
{
	return ((state != WALK &&
		state != RUN && state != IDLE &&
		state != JUMP && state != DEATH &&
		state != DEFEND && state != HIT &&
		state != PROTECT && state != TAUNT &&
		state != STOP && state != PARRY) || state == ATTACK_H2);
}

Attack* Hero::GetAtk(CharStateEnum atk)
{
	Attack* ret = nullptr;

	for (std::list<Attack*>::iterator item = attacks.begin(); item != attacks.end(); item++) {
		if ((**item).state == atk)
		{
			ret = (*item);
		}
	}
	return ret;
}

uint Hero::GetMaxLives() const
{
	return maxLives;
}

uint Hero::GetCurrentLives() const
{
	return lives;
}

void Hero::SetCombo()
{
	Attack* wanted_atk = GetAtk(wantedState);
	Attack* current_atk = GetAtk(last_attack);

	if (current_atk != nullptr && wanted_atk != nullptr && current_atk->CheckChildInput(wanted_atk->input))
	{
		currentState = current_atk->GetChildInput(wanted_atk->input)->state;
	}
	else
	{
		currentState = wantedState;
	}
}


std::list<CharInput> Hero::FirstPlayerConfig()
{

	std::list<CharInput> ret;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->debug = !App->debug;

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

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
		ret.push_back(CharInput::HEAVY_ATTACK);
		App->entities->CreateSpell({ FIREBALL, {(float) position.x, (float)position.y } });
	}
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
		ret.push_back(CharInput::DEFEND);

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
		ret.push_back(CharInput::PARRYINPUT);

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		ret.push_back(CharInput::TAUNTINPUT);

	return ret;
}

std::list<CharInput> Hero::SecondPlayerConfig()
{
	std::list<CharInput> ret;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		ret.push_back(CharInput::CH_LEFT);

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		ret.push_back(CharInput::CH_RIGHT);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		ret.push_back(CharInput::CH_UP);

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		ret.push_back(CharInput::CH_DOWN);

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		ret.push_back(CharInput::JUMPINPUT);

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT)
		ret.push_back(CharInput::DEFEND);

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT)
		ret.push_back(CharInput::PARRYINPUT);

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		ret.push_back(CharInput::LIGHT_ATTACK);

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		ret.push_back(CharInput::TAUNTINPUT);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		ret.push_back(CharInput::LIGHT_ATTACK);

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		ret.push_back(CharInput::HEAVY_ATTACK);

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		ret.push_back(CharInput::RUNINPUT);
	return ret;
}

std::list<CharInput> Hero::GetControllerInputs() const {

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
			charInputs.push_back(CharInput::PARRYINPUT);
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
		default:
			break;
		}
	}
	return charInputs;
}

