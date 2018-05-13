#include "ModuleEntityManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Warrior.h"
#include "Cleric.h"
#include "Rogue.h"
#include "Wizard.h"
#include "../Game/Spells/FireBall.h"
#include "../Game/Spells/Icicle.h"
#include "../Game/Spells/Lightning.h"
#include "../Game/Spells/FireDemon.h"
#include "../Game/Spells/Dagger.h"
#include "../Game/Spells/DeathMark.h"
#include "ModuleAudio.h"
#include "ModuleSceneManager.h"


ModuleEntityManager::ModuleEntityManager()
{
	name = "entities";
}


ModuleEntityManager::~ModuleEntityManager(){}

bool ModuleEntityManager::Awake(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Awake(n);
	}

	// Load SFX from XML
	xmlDocument audio_xml_file;
	xmlNode audio_xml;
	xmlNode iter;
	xmlNode iter2;

	pugi::xml_parse_result result = audio_xml_file.load_file("Assets/Audio/audio_xml.xml");
	if (result != NULL)
	{
		audio_xml = audio_xml_file.child("audio");
	}
	
	std::list<std::string> sounds_list;

	// SFX names finding loop
	for (iter = audio_xml.first_child(); iter; iter = iter.next_sibling())
	{
		for (iter2 = iter.first_child(); iter2; iter2 = iter2.next_sibling())
		{
			if (iter2.text().as_string() != "")
			{
				if (!IsSFXRepeated(sounds_list, iter2.text().as_string()))
				{
					sounds_list.push_back(iter2.text().as_string());
					App->audio->LoadFx(iter2.text().as_string());
				}
			}
		}
	}
	sounds_list.clear();

	return true;
}

bool ModuleEntityManager::Start() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Start();
	}

	justIn = true;
	StartItems();
	return true;
}

bool ModuleEntityManager::PreUpdate() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->PreUpdate();
	}

	VECTOR(LIST_ITERATOR(Entity*)) ents;
	for (LIST_ITERATOR(Entity*) ent = entities.begin(); ent != entities.end(); ent++) {
		if ((*ent)->to_delete) {
			ents.push_back(ent);
		}
	}

	for (size_t c = 0; c < ents.size(); c++) {
		Utils::Release(*ents[c]);
		entities.erase(ents[c]);
		//(*col)->c2->collisions.erase(cols[c]);
	}

	return true;
}

bool ModuleEntityManager::Update(float dt) {
	uint aliveCharacters = 0;
	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		if ((*item)->active)
		{
			(*item)->Update(dt);
			winner = (*item)->heroNum;
			winnerTeam = (*item)->team;
			aliveCharacters++;
		}
	}

	if (aliveCharacters == 1 && !justIn)
	{
		finish = true;
	}
	justIn = false;
	return true;
}

bool ModuleEntityManager::PostUpdate() {
	ARRAY(Entity*)toDestroy;
	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->PostUpdate();
		if ((*item)->to_delete)
			toDestroy.push_back(*item);
	}

	for (Entity* e : toDestroy) {
		DestroyEntity(e);
	}
	return true;
}

bool ModuleEntityManager::CleanUp(pugi::xml_node& n) {
	justIn = true;
	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->CleanUp(n);
	}
	StartItems();
	return true;
}

bool ModuleEntityManager::Load(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Load(n);
	}
	return true;
}

bool ModuleEntityManager::Save(pugi::xml_node& n) const {

	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Save(n);
	}
	return true;
}

Character* ModuleEntityManager::CreateCharacter(CharacterInfo charInfo) {

	Character* ret = nullptr;

	if(charInfo.chType == CharacterTypes::WARRIOR)
	{
		ret = new Warrior();
		
	}
	else if (charInfo.chType == CharacterTypes::CLERIC)
	{
		ret = new Cleric();		

	}
	else if (charInfo.chType == CharacterTypes::ROGUE)
	{
		ret = new Rogue();

	}
	else if (charInfo.chType == CharacterTypes::WIZARD)
	{
		ret = new Wizard();

	}

	else {
		return ret;
	}
	numofplayers++;

	ret->charType = charInfo.chType;
	ret->team = charInfo.chTeam;

	ret->heroNum = numofplayers;
	ret->type = CHARACTER;
	ret->SetPos(charInfo.pos.x, charInfo.pos.y, charInfo.pos.z);


	entities.push_back(ret);
	ret->Start();

	return ret;
}

Spells* ModuleEntityManager::CreateSpell(SpellsInfo spellsInfo) {

	Spells* ret = nullptr;

	if (spellsInfo.spType == SpellsType::FIREBALL)
	{
		ret = new FireBall();		
	}
	else if (spellsInfo.spType == SpellsType::ICICLE)
	{
		ret = new Icicle();
	}
	else if (spellsInfo.spType == SpellsType::LIGHTING)
	{
		ret = new Lightning();
	}
	else if (spellsInfo.spType == SpellsType::LIGHTNING_AURA)
	{
		ret = new Aura();
	}

	else if (spellsInfo.spType == SpellsType::FIRE_DEMON)
	{
		ret = new FireDemon();
	}
	else if (spellsInfo.spType == SpellsType::DAGGER)
	{
		ret = new Dagger();
	}
	else if (spellsInfo.spType == SpellsType::DEATH_MARK)
	{
		ret = new DeathMark();
	}

	else
	{
		return nullptr;
	}

	ret->type = SPELLS;

	ret->team = spellsInfo.chTeam;
	ret->SetPos(spellsInfo.pos.x, spellsInfo.pos.y, spellsInfo.pos.z);
	ret->SetDir(spellsInfo.dir.x, spellsInfo.dir.y);

	entities.push_back(ret);
	ret->Start();

	return ret;
}


void ModuleEntityManager::DestroyEntity(Entity* entity) {
	pugi::xml_node n;
	if (entity->heroNum != 0) {
		numofplayers--;
	}
	entity->CleanUp(n);
	entities.remove(entity);
	entity = nullptr;
}

void ModuleEntityManager::CheckMidPos(float &min_x, float &max_x)
{
	uint current_players = 0;
	if (entities.size() > 0) {
		min_x = entities.front()->GetPosX();
		max_x = entities.front()->GetPosX();

		for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
			if ((*item)->GetType() == CHARACTER)
			{
				current_players++;
				if ((*item)->GetPosX() < min_x)
				{
					min_x = (*item)->GetPosX();
				}
				
				if ((*item)->GetPosX() + (*item)->GetCollider().w/2 > max_x)
				{
					max_x = (*item)->GetPosX() + (*item)->GetCollider().w/2;
				}
			}

			if (current_players == numofplayers)
			{
				break;
			}
		}
	}
}

void ModuleEntityManager::CheckMidPosY(float &min_y, float &max_y)
{
	uint current_players = 0;
	if (entities.size() > 0) {
		min_y = entities.front()->GetPosY();
		max_y = entities.front()->GetPosY();

		for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
			if ((*item)->GetType() == CHARACTER)
			{
				current_players++;
				float posL = (*item)->GetPosY(), posR = (*item)->GetCollider().h;
				if (posL < min_y)
				{
					min_y = posL;
				}

				if (posR > max_y)
				{
					max_y = posL + posR;
				}
			}
		}
	}
}

void ModuleEntityManager::PauseEntities(bool pause) {
	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->paused = pause;
	}
}

bool ModuleEntityManager::IsSFXRepeated(std::list<std::string> list, std::string string) const
{
	for (std::list<std::string> newList = list; !newList.empty();)
	{
		if (newList.front() == string)
			return true;
		newList.pop_front();
	}
	return false;
}

Entity* ModuleEntityManager::GetEntity(uint num)
{
	Entity* ret = nullptr;

	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		if ((*item)->heroNum == num)
		{
			ret = (*item);
		}
	}
	return ret;
}

uint ModuleEntityManager::GetWinnerTeam()
{
	return winnerTeam;
}

void ModuleEntityManager::StartItems()
{
	for (int i = 0; i < 3; i++)
	{
		items[i].atk = 0;
		items[i].def = 0;
		items[i].spd = 0;
		items[i].life = 0;
		items[i].mgk = 0;
	}
}

Entity* ModuleEntityManager::GetSameTeam(Entity* c)
{
	Entity* ret = nullptr;

	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		if ((*item)->team == c->team && (*item) != c)
		{
			ret = (*item);
		}
	}
	return ret;
}