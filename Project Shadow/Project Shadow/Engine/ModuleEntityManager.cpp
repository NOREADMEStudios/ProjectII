#include "ModuleEntityManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Hero.h"
#include "ModuleAudio.h"


ModuleEntityManager::ModuleEntityManager()
{
	name = "entities";
}


ModuleEntityManager::~ModuleEntityManager(){}

bool ModuleEntityManager::Awake(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Awake(n);
	}

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
				sounds_list.push_back(iter2.text().as_string());
				App->audio->LoadFx(iter2.text().as_string());
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
	return true;
}

bool ModuleEntityManager::PreUpdate() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->PreUpdate();
	}
	return true;
}

bool ModuleEntityManager::Update(float dt) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Update(dt);
	}

	return true;
}

bool ModuleEntityManager::PostUpdate() {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->PostUpdate();

	}
	return true;
}

bool ModuleEntityManager::CleanUp(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->CleanUp(n);
	}
	return true;
}

bool ModuleEntityManager::Load(pugi::xml_node& n) {

	for (std::list<Entity*>::iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Load(n);
	}
	return true;
}

bool ModuleEntityManager::Save(pugi::xml_node& n)const {

	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->Save(n);
	}
	return true;
}

Entity* ModuleEntityManager::CreateCharacter(CharacterInfo charInfo) {

	Entity* ret = nullptr;

	if (charInfo.chType == CharacterTypes::ENEMY)
	{
		ret = new Enemy();
		numofplayers++;
	}
	else if (charInfo.chType == CharacterTypes::HERO)
	{
		ret = new Hero();
		numofplayers++;
		ret->hero_num = numofplayers;
	}
	else
	{
		//ret = new Entity();
		return nullptr;//
	}

	ret->type = CHARACTER;
	ret->SetPos(charInfo.pos.x, charInfo.pos.y);
	
	entities.push_back(ret);

	return ret;
}


void ModuleEntityManager::DestroyEntity(Entity* entity) {
	
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
				else if ((*item)->GetPosX() > max_x)
				{
					max_x = (*item)->GetPosX();
				}
			}

			if (current_players == numofplayers)
			{
				break;
			}
		}
	}
}

void ModuleEntityManager::PauseEntities(bool pause) {
	for (std::list<Entity*>::const_iterator item = entities.begin(); item != entities.end(); item++) {
		(*item)->paused = pause;
	}
}